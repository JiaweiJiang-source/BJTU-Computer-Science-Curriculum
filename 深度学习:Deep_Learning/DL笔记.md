## 第一章 AI导论

“人工智能” 概念诞生-----1956年美国达特茅斯会议

AI使一部机器像人一样进行**感知、认知、决策、执行**的人工程序或系统

**麦卡锡**——第一个提出“人工智能”一词的人，也被奉为**<u>人工智能之父</u>**

联结主义：**感知机**/**神经网络**

符号主义：代表方法**决策树**和**关联规则**

统计学习：SVM(支持向量机)

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222120841061.png" alt="image-20241222120841061" style="zoom:33%;" />

第一层面是运算智能

第二层面是是感知智能

第三层面是认知智能：能理解，会思考/但当前的人工智能是**能感知，会识别**

- 人工智能的第1次兴起：感知器的发明

- 人工智能的第2次兴起：Hopfield网络和BP算法的发明

- 人工智能的第3次兴起：深度学习

简单表示学习：简单进行特征映射

深度学习：获取**更抽象**的特征

1. **一切的开始：感知机**（Rosenblatt vs. Minsky ）
   + 第一：单层神经网络无法解决线性不可分问题，如异或门->二次准则函数H-K(识别和解决线性不可分问题)

   + 计算量大

   + 使用的是阶跃函数    

2. **BP Algorithm（**Rumelhart & Hinton）
   + 反向传播
   + 通过在神经网络中增加hidden layer，解决了**XOR问题**
3. **CNN**(Yann LeCun)
4. **Deep Learning** （Hinton）
5. **ImageNet**(Feifei Li)
6. **Relu**(Yoshua Bengio)
7. **LSTM** (Schmidhube)



## 第二讲 基础知识

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222123445538.png" alt="image-20241222123445538" style="zoom:33%;" />

### 2.1 线性模型

**线性回归**是一种典型的线性模型：输出的标签是连续值

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222125806232.png" alt="image-20241222125806232" style="zoom:33%;" />

+ **logistic回归：**常见的二分类问题（分类决策问题看作条件概率估计问题，给定样本的特征x， 预测标签y是哪个类别的后验概率）
  + <img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222130150846.png" alt="image-20241222130150846" style="zoom:33%;" />
  + 熵：自信息的期望，熵越高，则随机变量的信息越多；熵越低，则随机变量的信息越少 。
  + **逻辑回归使用交叉熵作为损失函数**$$L = -\frac{1}{N} \sum_{n=1}^{N} \left( y^{(n)} \log \hat{y}^{(n)} + \left( 1 - y^{(n)} \right) \log \left( 1 - \hat{y}^{(n)} \right) \right)$$
  + **逻辑回归激活函数sigmoid**$$\sigma(z) = \frac{1}{1 + e^{-z}}$$
+ **Softmax回归**:多分类问题
  + softmax:$$\sigma(z_i) = \frac{e^{z_i}}{\sum_{j=1}^k e^{z_j}}, \quad \text{for } i = 1, 2, \dots, k$$
+ **感知机**:一种广泛使用的线性分类器, 有一点需要注意的是，他的输出是+1和-1，来分别代表两个类别，用的是这样一个决策函数。这样的激活函数是不连续的，因此优化起来是非常困难的。
  + <img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222131523376.png" alt="image-20241222131523376" style="zoom:33%;" />

线性模型：但是线性模型的值域是实数，因此线性模型不能直接做分类；因此，我们需要在线性模型的基础上，再加上一个非线性的决策函数，组合起来使用，来做分类。

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222125941392.png" alt="image-20241222125941392" style="zoom:25%;" />

### 2.2 梯度下降

+ 批量梯度下降：更新的梯度方向是由**全部**训练样本计算得到的——计算复杂度高
+ 随机梯度下降：每次迭代**随机使用一个**样本——计算复杂度低，引入了随机噪声，所以容易逃离局部最优点，但可能收敛较慢
+ 小批量梯度下降：批量和随机之间取一个平衡

### 2.3 过拟合方法

+ **提前停止（Early Stop）**也是防止模型过拟合的一类方法，使用一个**验证集**来进行模型选择
  + <img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222125237392.png" alt="image-20241222125237392" style="zoom:33%;" />
+ **正则化（Regularization）**是一类通过**限制模型复杂度**，从而避免**过拟合**，提高泛化能力的方法，比如引入约束、提前停止等

## 第三讲 前馈神经网络

人工神经网络的**三大要素:节点、连边、连接方式**

从最简单的网络开始讲起，最简单的人工神经网络，只有一个神经元，采用符号函数（sgnx）作为激活函数，即感知器

感知器无法解决非线性的异或问题，由一层变成两层，得到双层感知器，即为最简单的前馈神经网络,继续增加层次，就得到深度前馈神经网络

+ **单隐层网络**可以近似任何函数，但其**规模可能巨大**
+ 随着**深度的增加**，网络的**表示能力呈<u>指数</u>增加**
+ 更深层的网络具有更好的**泛化能力**
+ **参数数量的增加未必一定会带来模型效果的提升**



+ **前馈网络：**每一层中的神经元接收来自前一层神经元的输出，并输出给下一层神经元
  + 包括<u>全连接前馈神经网络</u>和<u>卷积神经网络</u>
+ **记忆网络：**神经元不但可以接收其他神经元的信息，也可以接收自己的<u>历史信息</u>
  + 包括<u>循环神经网络、Hopfield网络、玻尔兹曼机、受限玻尔兹曼机</u>

记忆网络不包括CNN，但是权重共享CNN和RNN LSTM GRU都有

前馈神经网络的搭建，直接看b站视频

**前馈神经网络**（FNN/MLP）:第0层为输入层，最后一层为输出层，其他中间层称为隐藏层,信号从输入层向输出层单向传播，整个网络中无反馈，可用一个有向无环图表示

激活函数要求：

+ **<u>连续可导</u>**&**<u>非线性</u>**
+ 激活函数和导函数尽可能简单
+ 激活函数的**<u>导函数</u>**值域要在合理区间



## 第四讲 反向传播

### 4.1 反向传播算法

### 4.2 自动梯度计算

### 4.3 神经网络参数优化的主要问题

也是可以直接看b站视频

然后就可以看到有数值的推算

简单点来说呢

就是通过求得的最后的结果
反向往后求到最后的真实值和预测值之间的损失函数，从而输出损失函数
然后再将损失函数对每一个线上的参数进行求导
从而可以不断更新参数（w=w-a*[loss对于w的偏导]））

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222141014996.png" alt="image-20241222141014996" style="zoom:33%;" />

梯度消失和梯度爆炸的原因，以及解决方案？

https://blog.csdn.net/a8039974/article/details/122381069?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522cb3b147694daabd7a96cd402880f4a68%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=cb3b147694daabd7a96cd402880f4a68&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-122381069-null-null.142^v100^pc_search_result_base5&utm_term=%E6%A2%AF%E5%BA%A6%E7%88%86%E7%82%B8%E5%92%8C%E6%A2%AF%E5%BA%A6%E6%B6%88%E5%A4%B1&spm=1018.2226.3001.4187

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241219191547806.png" alt="image-20241219191547806" style="zoom:33%;" />

## 第五讲 网络优化I

### **高维空间中大部分驻点都是鞍点**

二阶导数的所有特征值都为正，则是局部最小点，但是概率非常小，大部分都是鞍点

当batch-size比较大时，需要比较大的学习率。但开始训练的参数是随机初始化的，梯度往往也比较大，加上比较大的初始学习率，会使得训练不稳定，因此预热过程中，学习率逐步加大。学习率预热尤其用于复杂模型。

### **AdaGrad算法：Adaptive Gradient Algorithm**

该算法借鉴了L_2正则化的思想，每次迭代时，自适应地调整每个参数的学习率。

在AdaGrad 算法中，如果某个参数的偏导数累积比较大，就让其学习率相对较小；

相反，如果其偏导数累积较小，就让其学习率相对较大．

整体上随着迭代次数的增加，学习率逐渐缩小。

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222142158928.png" alt="image-20241222142158928" style="zoom:33%;" />

### **RMSprop算法**

AdaGrad局限：在经过一定次数的迭代依然没有找到最优点时，由于这时的学习率已经非常小，很难再继续找到最优点。

**AdaGrad的学习率不断衰减，而RMSProp学习率可能变大也可能变小**（衰减率β一般取值0.9）

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222142331261.png" alt="image-20241222142331261" style="zoom:33%;" />

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222142320089.png" alt="image-20241222142320089" style="zoom:33%;" />

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241218110619903.png" alt="image-20241218110619903" style="zoom:50%;" />

https://www.bilibili.com/video/BV1NZ421s75D/?spm_id_from=333.337.search-card.all.click&vd_source=d8d1cb7c2fbd392ee1bd6c2f57af5b4e

### **梯度修正**

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222142544724.png" alt="image-20241222142544724" style="zoom:33%;" />

### **动量法**---帮助模型跳出局部极值点

一般而言，在迭代初期，梯度方向都比较一致，动量法会起到加速作用，可以更快地到达最优点。在迭代后期，梯度方向会不一致，在收敛值附近震荡，动量法会起到**减速**作用，增加稳定性。

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222142715543.png" alt="image-20241222142715543" style="zoom:33%;" />

### **Adam算法**：自适应学习率（RMSprop）+梯度方向优化（动量法）

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222142848422.png" alt="image-20241222142848422" style="zoom:33%;" />

+ **一阶矩** —— 对梯度方向的修正（动量法）

+ **二阶矩** —— 对学习率的调整（RMSprop）

梯度爆炸---梯度截断

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222143058221.png" alt="image-20241222143058221" style="zoom:33%;" />

如果一个神经元的输入连接很多，它的每个输入连接上的权重就应该小一些以避免神经元的输出过大（eg. ReLU）或过饱和（eg. sigmoid）初始化一个深度网络时，为了缓解梯度消失或者爆炸问题，**须尽可能保持每个神经元的输入和输出的期望始终为0，方差保持一致**

### Xavier初始化？？

虽然在 Xavier 初始化中我们假设激活函数为恒等函数，**但是 Xavier 初始化也适用于 Logistic 函数和 Tanh 函数（神经元参数和输入都比较小，处于激活函数的线性区间）**

### He 初始化

只针对神经元使用**ReLU** 激活函数时，通常有一半的神经元输出为0，因此其分布的方差也近似为使用恒等函数时的一半

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222150553335.png" alt="image-20241222150553335" style="zoom:33%;" />

### 白化--主要处理输入数据的归一化

降低输入的冗余性，降低特征之间的相关性,使得所有的特征具有相同的方差

PCA白化保证数据各维度的方差为1, ZCA白化保证数据各维度的方差相同

PCA白化可以用于降维也可以去相关性，而ZCA白化主要用于去相关性，且尽量使白化后的数据接近原始输入数据

标准化，均值为0，方差为1的高斯分布；

### 归一化

批量归一化：对于每一个神经元做归一化--适合大数据的批量

+ 小批量样本的数量**不能太小**

+ **不能处理**神经元的净输入的分布在神经网络中是**动态变化**(RNN)

层归一化：对于一个层做归一化--适合小数据样本量

### 超参数优化

+ 网格搜索：是一种通过尝试所有超参数的组合来寻址合适一组超参数配置的方法。

+ 随机搜索：一种改进方法是对超参数进行随机组合，然后选取一个性能最好的配置，这就是随机搜索。

网格搜索和随机搜索都没有利用不同超参数组合之间的**相关性**，即如果模型的超参数组合比较类似，其模型性能也是比较接近的．因此这两种搜索方式一般都比较低效．下面我们介绍两种自适应的超参数优化方法：贝叶斯优化和动态资源分配

+ 贝叶斯优化：根据当前已经试验的超参数组合，来预测下一个可能带来最大收益的组合．
+ 动态资源分配：动态资源分配的关键是将有限的资源分配给更有可能带来收益的超参数
+ 神经架构搜索

## 第六讲 卷积神经网络

### 稀疏交互/局部感受野

回想一下传统的全连接(full-connected) DNN，每一层的神经元都和上一层以及下一层的所有神经元建立了连接。

但是CNN不这样做，我们以图像为例，CNN只会将隐藏层(CNN的hidden layer就是一层滤波器层/卷积层)和图像上相同大小的一个区域进行连接。这个输入图像的区域被称为隐藏神经元的局部感受野

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222152241074.png" alt="image-20241222152241074" style="zoom:33%;" />

卷积网络还具有平移不变性：**平移不变性**意味着系统产生完全相同的响应（输出），不管它的输入是如何平移的。（卷积+池化实现平移不变性）



### 转置卷积/反卷积

我们将低维特征映射到高维特征的卷积操作称为**转置卷积（Transposed Convolution），也称为反卷积（Deconvolution）。**

**仿射变换（Affine Transformation）**可以实现高维到低维的转换，也可以实现低维到高维的转换：

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222154035236.png" alt="image-20241222154035236" style="zoom:33%;" />



### 空洞卷积

增加输出单元的感受野：

① 增加卷积核的大小；

② 增加卷积层数；比如两层3 × 3 的卷积可以近似一层5 × 5 卷积的效果；

③ 加入池化操作；

l空洞卷积（Atrous Convolution）是一种**不增加参数数量**，同时**增加输出单元感受野**的方法

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222154229601.png" alt="image-20241222154229601" style="zoom:33%;" />

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241218185513356.png" alt="image-20241218185513356" style="zoom:50%;" />

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241218185845242.png" alt="image-20241218185845242" style="zoom:50%;" />

https://www.bilibili.com/video/BV1RJ4m177hi/?spm_id_from=333.337.search-card.all.click&vd_source=d8d1cb7c2fbd392ee1bd6c2f57af5b4e

### LeNet

**LeNet提供了利用卷积层堆叠进行特征提取的框架，开启了深度卷积神经网络的发展。**

卷积，池化，非线性

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241218190435214.png" alt="image-20241218190435214" style="zoom:50%;" />

### AlexNet

+ 大数据训练：百万级ImageNet图像数据
+ 防止过拟合：**Dropout**, **数据增强**
+ 分Group实现双**GPU**并行，局部响应归一化（LRN）层
+ 5个卷积层、3个汇聚层和3个全连接层
+ 非线性激活函数：**ReLU**（首次采用）
+ 数据增强

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241218193034456.png" alt="image-20241218193034456" style="zoom:50%;" />



AlexNet在ILSVRC2012图像分类竞赛中将top-5 错误率降至16.4%，掀起了深度卷积神经网络在各个领域的研究热潮。

### Inception--多尺度、多层次滤波

Inception module 包含四个分支：

+ Shortcut连接：将前一层输入通过1×1卷积
+ 多尺度滤波：输入通过1×1卷积之后分别连接卷积核大小为3和5的卷积
+ 池化分支：相继连接3×3 pooling和1×1卷积

主要引入了1x1的卷积（filters），从而减少了学习参数的数值，减少网络开销

因此，1×1卷积的作用（1x1卷积的主要目的是为了减少维度，还用于修正线性激活（ReLU）。）

+ 是通过降维减少网络开销
+ 是对低层滤波结果进行有效的组合

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241218193233172.png" alt="image-20241218193233172" style="zoom:50%;" />

#### InceptionV3

+ 用多层的小卷积核来替换大的卷积核，以减少计算量和参数量。
+ 使用连续的nx1和1xn来替换nxn的卷积（Inception v2）
+ 使用两层3x3的卷积来替换v1中的5x5的卷积

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241218193954059.png" alt="image-20241218193954059" style="zoom:50%;" />

### ResNet-解决深度神经网络中的退化问题

通过添加**恒等映射**不能提高网络准确率

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241218194117626.png" alt="image-20241218194117626" style="zoom:33%;" />

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241218194540042.png" alt="image-20241218194540042" style="zoom:33%;" />

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241218191617257.png" alt="image-20241218191617257" style="zoom:50%;" />

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241218191633749.png" alt="image-20241218191633749" style="zoom:50%;" />

**网络参数的计算！！！！**

$$(5*5*3+1)*6=456$$

max pooling参数不变

FC3：$$5 * 5 * 16 * 120 + 120(bias) = 48120$$

CNN是前馈网络，不是记忆网络

## 第八讲 循环神经网络 I 

one-to-many:生成任务

Many-to-one:分类任务

many-to-many(同步)：输入一句话，输出每个单词对应的词性------标注

many-to-many(异步)：输入输出的序列长度不同

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222155844201.png" alt="image-20241222155844201" style="zoom:33%;" />

双向循环网络

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241222155955328.png" alt="image-20241222155955328" style="zoom:33%;" />

双向循环神经网络由两层并列的循环神经网络组成，它们的**输入相同**，只是信息传递的方向不同。最终的输出取决于正向和反向计算的**加和**。从上面三个公式得到，正向计算和反向计算**不共享权重**，也就是说*U* 和*U’*、*W* 和*W’*、*V* 和*V’*都是不同的**权重矩阵**。

RNN具有**长程依赖问题**，容易出现梯度消失和梯度爆炸问题（RNN总的梯度是不会消失的。其梯度消失的真正含义是：梯度被近距离梯度主导，导致模型难以学习远距离的依赖关系，这也是原始RNN无法处理长距离依赖的原因。）

https://space.bilibili.com/34230158

RNN+LSTM+GRU⬆️

### LSTM--遗忘门、输入门、输出门

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20241219165525810.png" alt="image-20241219165525810" style="zoom:50%;" />

### GRU--重置门、更新门