'''
班级：计科2204班
学号：22281188
姓名：江家玮
目前可运行的代码为视频Alan_.avi视频
更改相关注释即可运行tunnel.avi视频(更改注释98行、234行，以及更改266行相关路径即可）

2024-07-13
'''
import cv2
import numpy as np
import time
from sklearn.linear_model import RANSACRegressor

# 历史帧存储车道线信息
class LaneLines:
    def __init__(self):
        self.left_fit = None
        self.right_fit = None

    def add_fit(self, left_fit, right_fit):
        if left_fit is not None:
            self.left_fit = left_fit
        if right_fit is not None:
            self.right_fit = right_fit

    def get_fit(self):
        return self.left_fit, self.right_fit

# Canny算子或Sobel算子进行边缘检测
def canny_func(blur_gray, canny_lthreshold=150, canny_hthreshold=250):
    edges = cv2.Canny(blur_gray, canny_lthreshold, canny_hthreshold)
    return edges


# 设置ROI区域,定义一个和输入图像同样大小的全黑图像mask
def roi_mask(img, vertices):
    mask = np.zeros_like(img)
    # 检查输入图像是否有多个通道（例如彩色图像有3个通道）
    if len(img.shape) > 2:
        channel_count = img.shape[2]
        mask_color = (255,) * channel_count # 设置 mask 的颜色为白色，对应于每个通道值为255
    else:
        # 如果是单通道图像，设置 mask 的颜色为白色
        mask_color = 255
    # 在 mask 上填充多边形区域，区域内的颜色为白色（mask_color）
    cv2.fillPoly(mask, [vertices], mask_color)
    masked_img = cv2.bitwise_and(img, mask)
    return masked_img


# Hough变换函数，用于检测直线
def hough_func(roi_image, rho=1, theta=np.pi / 180, threshold=15, min_line_length=40, max_line_gap=20):
    # 使用概率霍夫变换检测直线
    line_img = cv2.HoughLinesP(
        roi_image,        # 输入的二值化图像（通常是边缘检测的结果）
        rho,              # 距离分辨率（像素）
        theta,            # 角度分辨率（弧度）
        threshold,        # 累加器阈值，只有获得足够的投票数的直线才会被检测出
        np.array([]),     # 空数组，作为占位符
        min_line_length,  # 能被检测为线段的最小长度
        max_line_gap      # 同一条直线上两点之间的最大间隔
    )
    # 返回检测到的线段
    return line_img


# RANSAC拟合
def ransac_fit(x, y):
    if len(x) == 0 or len(y) == 0:
        return None, None
    # 将 x 转换为 NumPy 数组并重塑为二维数组，形状为 (-1, 1)
    x = np.array(x).reshape(-1, 1)
    y = np.array(y)
    # 创建 RANSAC 回归模型实例
    ransac = RANSACRegressor()
    ransac.fit(x, y)
    # 获取拟合直线的斜率（系数）和截距
    line_k = ransac.estimator_.coef_[0]  # 斜率
    line_b = ransac.estimator_.intercept_  # 截距
    return line_k, line_b


# 计算左右车道线的直线方程并绘制
def draw_lines(img, lines, prev_lines, color=[0, 0, 255], thickness=2):
    left_lines_x = []
    left_lines_y = []
    right_lines_x = []
    right_lines_y = []

    # #Alan_1.avi的表示界限
    # 获取图像的高度，即y轴的最大值
    line_y_max = int(img.shape[0])
    # 设置y轴的最小值为图像高度的2/3处
    line_y_min = int(img.shape[0]*0.7)
    print(img.shape[0]) #360

    # # tunnel.avi的表示界限
    # line_y_max = int(img.shape[0] * 0.8)
    # line_y_min = int(img.shape[0] * 0.3)

    if lines is not None:
        try:
            for line in lines:
                for x1, y1, x2, y2 in line:
                    k = (y2 - y1) / (x2 - x1)
                    if k < -0.3:
                        left_lines_x.extend([x1, x2])
                        left_lines_y.extend([y1, y2])
                    elif k > 0.3:
                        right_lines_x.extend([x1, x2])
                        right_lines_y.extend([y1, y2])

            left_line_k, left_line_b = ransac_fit(left_lines_x, left_lines_y)
            right_line_k, right_line_b = ransac_fit(right_lines_x, right_lines_y)

            if left_line_k is not None and left_line_b is not None: # 如果左车道线拟合参数不为空，则绘制左车道线
                cv2.line(img,
                         (int((line_y_max - left_line_b) / left_line_k), line_y_max),
                         (int((line_y_min - left_line_b) / left_line_k), line_y_min), color, thickness)
                prev_lines['left'] = (left_line_k, left_line_b)
            elif 'left' in prev_lines: # 如果当前帧没有检测到左车道线，则使用前一帧的左车道线参数
                left_line_k, left_line_b = prev_lines['left']
                cv2.line(img,
                         (int((line_y_max - left_line_b) / left_line_k), line_y_max),
                         (int((line_y_min - left_line_b) / left_line_k), line_y_min), color, thickness)

            if right_line_k is not None and right_line_b is not None:
                cv2.line(img,
                         (int((line_y_max - right_line_b) / right_line_k), line_y_max),
                         (int((line_y_min - right_line_b) / right_line_k), line_y_min), color, thickness)
                prev_lines['right'] = (right_line_k, right_line_b)
            elif 'right' in prev_lines:
                right_line_k, right_line_b = prev_lines['right']
                cv2.line(img,
                         (int((line_y_max - right_line_b) / right_line_k), line_y_max),
                         (int((line_y_min - right_line_b) / right_line_k), line_y_min), color, thickness)

            # 创建一个与输入图像相同大小的全零图像
            zero_img = np.zeros((img.shape), dtype=np.uint8)
            # 如果成功拟合出左右车道线，则绘制车道线之间的多边形区域
            if left_line_k is not None and left_line_b is not None and right_line_k is not None and right_line_b is not None:
                polygon = np.array([
                    [int((line_y_max - left_line_b) / left_line_k), line_y_max],
                    [int((line_y_max - right_line_b) / right_line_k), line_y_max],
                    [int((line_y_min - right_line_b) / right_line_k), line_y_min],
                    [int((line_y_min - left_line_b) / left_line_k), line_y_min]
                ])
                # 使用绿色填充多边形区域
                cv2.fillConvexPoly(zero_img, polygon, color=(0, 255, 0))

                # 将填充的多边形区域叠加到原图像上
                img = cv2.addWeighted(img, 1, zero_img, 0.3, 0)

        # 异常处理
        except Exception as e:
            print(str(e))
            print("NO detection")
            if 'left' in prev_lines:    # 使用前一帧的左车道线参数绘制左车道线
                left_line_k, left_line_b = prev_lines['left']
                cv2.line(img,
                         (int((line_y_max - left_line_b) / left_line_k), line_y_max),
                         (int((line_y_min - left_line_b) / left_line_k), line_y_min), color, thickness)
            if 'right' in prev_lines:   # 使用前一帧的右车道线参数绘制右车道线
                right_line_k, right_line_b = prev_lines['right']
                cv2.line(img,
                         (int((line_y_max - right_line_b) / right_line_k), line_y_max),
                         (int((line_y_min - right_line_b) / right_line_k), line_y_min), color, thickness)

    # 如果当前帧没有检测到线段，则使用前一帧的拟合参数绘制车道线
    else:
        print("NO detection")
        if 'left' in prev_lines:
            left_line_k, left_line_b = prev_lines['left']
            cv2.line(img,
                     (int((line_y_max - left_line_b) / left_line_k), line_y_max),
                     (int((line_y_min - left_line_b) / left_line_k), line_y_min), color, thickness)
        if 'right' in prev_lines:
            right_line_k, right_line_b = prev_lines['right']
            cv2.line(img,
                     (int((line_y_max - right_line_b) / right_line_k), line_y_max),
                     (int((line_y_min - right_line_b) / right_line_k), line_y_min), color, thickness)

    return img

# 开始检测
def main(video_file):
    cap = cv2.VideoCapture(video_file)
    if not cap.isOpened():
        print("没有正确打开视频文件")
        return
    # 获取视频的帧数、宽度、高度和帧率
    frame_count = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    width, height = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)), int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = int(cap.get(cv2.CAP_PROP_FPS))


    fourcc = cv2.VideoWriter_fourcc('m', 'p', '4', 'v')
    writer = cv2.VideoWriter("video_3_sol.mp4", fourcc, fps, (width, height), 1)

    lane_lines = LaneLines()
    prev_lines = {}

    # 循环读取视频帧
    while cap.isOpened():
        try:
            # 读取一帧图像
            ret, img = cap.read()
            if not ret:
                break

            # 开始计时，用于计算处理帧率
            start = time.time()
            gray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)     # 转换图像为灰度图像
            blur_gray = cv2.GaussianBlur(gray, (3, 3), 0)    # 使用高斯模糊平滑图像
            edges = canny_func(blur_gray)    # 使用Canny算子进行边缘检测

            # Alan_1.avi中的感兴趣的ROI设置，目前模拟较好
            rows, cols = edges.shape
            bottom_left = [cols * 0.1, rows]
            top_left = [cols * 0.333,rows * 0.67]
            bottom_right = [cols * 0.65, rows]
            top_right = [cols * 0.55, rows * 0.67]
            vertices = np.array([[bottom_left, top_left, top_right, bottom_right]], dtype=np.int32)

            # tunnel.avi中的感兴趣的ROI设置
            # rows, cols = edges.shape
            # bottom_left = [cols * 0.1, rows * 0.7]
            # top_left = [cols * 0.35,rows * 0.5]
            # bottom_right = [cols * 0.9, rows * 0.7]
            # top_right = [cols * 0.55, rows * 0.5]
            # vertices = np.array([[bottom_left, top_left, top_right, bottom_right]], dtype=np.int32)

            # # tunnel.avi中的感兴趣的ROI设置,这个坐标整体设置不错
            # rows, cols = edges.shape
            # bottom_left = [40, 239]
            # top_left = [200, 100]
            # bottom_right = [433, 239]
            # top_right = [433, 100]
            # vertices = np.array([[bottom_left, top_left, top_right, bottom_right]], dtype=np.int32)

            roi_image = roi_mask(edges, vertices)
            lines = hough_func(roi_image)

            img = draw_lines(img, lines, prev_lines)

            end = time.time()
            detect_fps = round(1.0 / (end - start + 0.00001), 2)

            font = cv2.FONT_HERSHEY_SIMPLEX

            writer.write(img)
            cv2.imshow('lane_detect', img)

            if cv2.waitKey(25) & 0xFF == ord('q'):
                break
        except Exception as e:
            print(str(e))
            break

    cap.release()
    writer.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    video_file = "C:\\Users\\37623\\Desktop\\LaneLineDet\\Alan_1.avi"
    # video_file = "C:\\Users\\37623\\Desktop\\LaneLineDet\\tunnel.avi"
    main(video_file)
