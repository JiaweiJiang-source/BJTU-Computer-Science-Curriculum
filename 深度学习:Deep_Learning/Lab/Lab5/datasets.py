import numpy as np
import pandas as pd
import torch
import torch.utils.data as data
from torch.utils.data import DataLoader
import warnings
from sklearn.preprocessing import LabelEncoder
from datetime import datetime
from torch.utils.data import DataLoader, Dataset
from sklearn.preprocessing import MinMaxScaler

warnings.filterwarnings("ignore")

# 定义dataset
class my_Dataset(data.Dataset):
    def __init__(self, features, labels):
        self.X = features
        self.y = labels

    def __getitem__(self, index):
        return self.X[index], self.y[index]

    def __len__(self):
        return self.X.shape[0]


# 空气质量数据集
class KrakowDataset:
    def __init__(self, sensor=171, is_resample=True):
        # 选取几个月个月的数据
        self.month = ['april-2017', 'august-2017', 'december-2017', 'february-2017',
                      'january-2017', 'july-2017', 'june-2017', 'march-2017',
                      'may-2017', 'november-2017', 'october-2017', 'september-2017']
        raw_data = pd.concat([pd.read_csv(f'./dataset/Krakow-airquality/{month}.csv') for month in self.month])

        # 确定特征列
        features = ['temperature', 'humidity', 'pressure', 'pm1', 'pm25', 'pm10']
        self.sensor = sensor    # 选取探测器,并非每个探测器都有数据
        self.feature_col = ['UTC time'] + [f'{self.sensor}_{fea}' for fea in features]
        data_df = raw_data[[col for col in raw_data.columns if col in self.feature_col]]

        # 按时间戳排序
        data_df['UTC time'] = pd.to_datetime(data_df['UTC time'])
        data_df = data_df.set_index('UTC time').sort_index()

        # 重采样、插值
        if is_resample:
            self.start_time, self.end_time = data_df.index.min(), data_df.index.max()
            full_index = pd.date_range(self.start_time, self.end_time, freq='h')
            data_df = data_df.reindex(full_index)
            data_df = data_df.interpolate(method='linear')
        else:
            data_df = data_df.dropna()

        # 数据标准化
        self.min = data_df.min()
        self.max = data_df.max()
        self.data = (data_df - self.min) / (self.max - self.min)

    def denormalize(self, x):
        key = f'{self.sensor}_{self.target}'
        return x * (self.max[key] - self.min[key]) + self.min[key]

    def construct_set(self, train_por=0.6, test_por=0.2, window_size=12, target='pm25', batch_size=32):
        train_x = []
        train_y = []
        val_x = []
        val_y = []
        test_x = []
        test_y = []
        self.target = target

        # 确保 feature_col 中不包含 'UTC time' 或其他非数值列
        self.feature_col = [col for col in self.feature_col if self.data[col].dtype in [np.float64, np.int64]]

        # 获取数据的总长度
        total_len = self.data.shape[0]

        # 计算训练集、验证集、测试集的分割点
        len_train = int(total_len * train_por)
        len_val = int(total_len * (train_por + test_por))

        # 构造训练集
        train_seqs = self.data.iloc[:len_train]  # 使用 iloc 按位置索引
        for i in range(len_train - window_size):
            # 确保 train_x 中的数据是数值类型
            train_x.append(train_seqs.iloc[i:i + window_size - 1][self.feature_col].values)
            # 确保 train_y 中的数据是数值类型
            train_y.append(train_seqs.iloc[i + window_size][f'{self.sensor}_{target}'])

        # 构造验证集
        val_seqs = self.data.iloc[len_train:len_val]  # 使用 iloc 按位置索引
        for i in range(len(val_seqs) - window_size):
            # 确保 val_x 中的数据是数值类型
            val_x.append(val_seqs.iloc[i:i + window_size - 1][self.feature_col].values)
            # 确保 val_y 中的数据是数值类型
            val_y.append(val_seqs.iloc[i + window_size][f'{self.sensor}_{target}'])

        # 构造测试集
        test_seqs = self.data.iloc[len_val:]  # 使用 iloc 按位置索引
        for i in range(len(test_seqs) - window_size):
            # 确保 test_x 中的数据是数值类型
            test_x.append(test_seqs.iloc[i:i + window_size - 1][self.feature_col].values)
            # 确保 test_y 中的数据是数值类型
            test_y.append(test_seqs.iloc[i + window_size][f'{self.sensor}_{target}'])

        # 将数据转换为 torch.Tensor
        train_set = my_Dataset(torch.Tensor(train_x), torch.Tensor(train_y))
        val_set = my_Dataset(torch.Tensor(val_x), torch.Tensor(val_y))
        test_set = my_Dataset(torch.Tensor(test_x), torch.Tensor(test_y))

        # 使用 DataLoader 封装数据集并返回
        train_loader = DataLoader(train_set, batch_size=batch_size, shuffle=True)
        val_loader = DataLoader(val_set, batch_size=batch_size, shuffle=False)
        test_loader = DataLoader(test_set, batch_size=batch_size, shuffle=False)

        return train_loader, val_loader, test_loader

class TrafficDataset(Dataset):
    def __init__(self, data_path, window_size=12, train_por=0.6, test_por=0.2, target_col=0, sensor_num=5):
        # 读取数据
        self.raw_data = np.load(data_path)['data']
        
        # 检查数据的形状，确保数据是三维的 (时间步, 样本数, 特征)
        print("Raw data shape:", self.raw_data.shape)
        
        # 如果数据是三维的，我们需要对每个特征列进行标准化
        self.num_samples, num_time_steps, num_features = self.raw_data.shape
        
        # 将数据 reshape 为二维的 (样本数 * 时间步数, 特征数)
        data_reshaped = self.raw_data.reshape(-1, num_features)
        
        # 标准化每个特征
        self.scaler = MinMaxScaler()
        self.data = self.scaler.fit_transform(data_reshaped)
        
        # 将标准化后的数据 reshape 回原来的三维形状
        self.data = self.data.reshape(self.raw_data.shape)
        
        # 配置参数
        self.window_size = window_size
        self.target_col = target_col
        self.sensor_num = sensor_num

        # 划分数据集
        total_len = self.data.shape[0]
        len_train = int(total_len * train_por)
        len_val = int(total_len * (train_por + test_por))

        self.train_data = self.data[:len_train]
        self.val_data = self.data[len_train:len_val]
        self.test_data = self.data[len_val:]

    def __len__(self):
        return len(self.train_data) - self.window_size

    def __getitem__(self, index):
        # 获取滑动窗口中的数据
        x = self.train_data[index:index + self.window_size, self.sensor_num, :]  # 取特定传感器的数据
        y = self.train_data[index + self.window_size, self.sensor_num, self.target_col]  # 目标值为特定传感器的某一特征
        return torch.Tensor(x), torch.Tensor([y])

    def get_val_data(self):
        val_x, val_y = [], []
        for i in range(len(self.val_data) - self.window_size):
            val_x.append(self.val_data[i:i + self.window_size, self.sensor_num, :])  # 取特定传感器的数据
            val_y.append(self.val_data[i + self.window_size, self.sensor_num, self.target_col])  # 目标值
        return torch.Tensor(val_x), torch.Tensor(val_y)

    def get_test_data(self):
        test_x, test_y = [], []
        for i in range(len(self.test_data) - self.window_size):
            test_x.append(self.test_data[i:i + self.window_size, self.sensor_num, :])  # 取特定传感器的数据
            test_y.append(self.test_data[i + self.window_size, self.sensor_num, self.target_col])  # 目标值
        return torch.Tensor(test_x), torch.Tensor(test_y)




# 四方数据集

class FourSquareDataset:
    def __init__(self, data_path):
        # 读取数据
        self.raw_data = pd.read_csv(data_path)
        
        # 1. 转换 userId 和 venueId 为整数标签
        self.user_encoder = LabelEncoder()
        self.venue_encoder = LabelEncoder()
        self.raw_data['userId'] = self.user_encoder.fit_transform(self.raw_data['userId'])
        self.raw_data['venueId'] = self.venue_encoder.fit_transform(self.raw_data['venueId'])
        
        # 2. 将 utcTimestamp 转换为时间戳（秒数）
        self.raw_data['utcTimestamp'] = self.raw_data['utcTimestamp'].apply(
            lambda x: datetime.strptime(x, "%a %b %d %H:%M:%S +0000 %Y").timestamp()
        )

        # 3. 选择数值型列进行标准化
        self.num_columns = ['userId', 'venueId', 'utcTimestamp']

        # 数据标准化，只对数值型列进行标准化
        self.min = self.raw_data[self.num_columns].min()
        self.max = self.raw_data[self.num_columns].max()
        self.data = self.raw_data.copy()  # 复制原始数据，以免修改原数据
        self.data[self.num_columns] = (self.raw_data[self.num_columns] - self.min) / (self.max - self.min)

    def denormalize(self, x, feat):
        return x * (self.max[feat] - self.min[feat]) + self.min[feat]

    def construct_set(self, train_por=0.6, test_por=0.2, window_size=12, label=0, batch_size=32):
        train_x, train_y = [], []
        val_x, val_y = [], []
        test_x, test_y = [], []

        # 获取数据的总长度
        total_len = self.data.shape[0]

        # 切割训练集、验证集、测试集的起始索引
        len_train = int(total_len * train_por)
        len_val = int(total_len * (train_por + test_por))

        # 构造训练集
        train_data = self.data[:len_train]
        for i in range(len_train - window_size):
            train_x.append(train_data.iloc[i:i + window_size].values)  # 序列数据
            train_y.append(train_data.iloc[i + window_size, label])    # 预测值

        # 构造验证集
        val_data = self.data[len_train:len_val]
        for i in range(len_val - len_train - window_size):
            val_x.append(val_data.iloc[i:i + window_size].values)
            val_y.append(val_data.iloc[i + window_size, label])

        # 构造测试集
        test_data = self.data[len_val:]
        for i in range(len(test_data) - window_size):
            test_x.append(test_data.iloc[i:i + window_size].values)
            test_y.append(test_data.iloc[i + window_size, label])

        # 将数据转换为 torch.Tensor 并返回
        train_set = my_Dataset(torch.Tensor(train_x), torch.Tensor(train_y))
        val_set = my_Dataset(torch.Tensor(val_x), torch.Tensor(val_y))
        test_set = my_Dataset(torch.Tensor(test_x), torch.Tensor(test_y))

        # 使用 DataLoader 封装数据集并返回
        train_loader = DataLoader(train_set, batch_size=batch_size, shuffle=True)
        val_loader = DataLoader(val_set, batch_size=batch_size, shuffle=False)
        test_loader = DataLoader(test_set, batch_size=batch_size, shuffle=False)

        return train_loader, val_loader, test_loader