<img src="D:\学习\交大高清标示--透明背景（校徽等）\交大高清标示--透明背景-03.png" style="zoom:50%;" />


<h1 align = "center" style="font-size:40px">北京交通大学</h1>

<center style="font-size:18px">		课程名称：计算机网络原理</left>
<center style="font-size:18px">实验题目     ：Lab2_SMTP</center>
<center style="font-size:18px">学号    ：22281188</center>
<center style="font-size:18px">姓名 ： 江家玮</center>
<center style="font-size:18px">班级 ： 计科2204班</center>
<center style="font-size:18px">指导老师 ： 张志飞老师</center>
<center style="font-size:18px">报告日期 ：2025-03-18</center>



------

[TOC]




 <div style="page-break-after: always;"></div>

# 计网仿真Lab2

## 一、实验时间

实验时间：

## 二、实验地点



## 三、实验目的



## 四、实验内容

## 4.1 作业目录

>

## 4.2 实验过程

### 4.2.1 实验背景知识



## 五、实验结果



## 六、实验心得











#### 分配次启动PC并配置

`PC_1`配置：

|  配置项  |      值       |
| :------: | :-----------: |
| IP 地址  |  192.168.1.2  |
| 子网掩码 | 255.255.255.0 |
| 默认网关 |  192.168.1.1  |

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20250515172338295.png" alt="image-20250515172338295" style="zoom:33%;" />

`PC_2`配置：

|  配置项  |      值       |
| :------: | :-----------: |
| IP 地址  |  192.168.1.3  |
| 子网掩码 | 255.255.255.0 |
| 默认网关 |  192.168.1.1  |

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20250515172552670.png" alt="image-20250515172552670" style="zoom:33%;" />

`PC_3`配置：

|  配置项  |      值       |
| :------: | :-----------: |
| IP 地址  |  192.168.1.4  |
| 子网掩码 | 255.255.255.0 |
| 默认网关 |  192.168.1.1  |

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20250515173215082.png" alt="image-20250515173215082" style="zoom:33%;" />

`PC_4`配置：

|  配置项  |      值       |
| :------: | :-----------: |
| IP 地址  |  192.168.1.5  |
| 子网掩码 | 255.255.255.0 |
| 默认网关 |  192.168.1.1  |

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20250515173301245.png" alt="image-20250515173301245" style="zoom:33%;" />

#### Switch 1

```python
sys # 进入系统视图
vlan 2 # 创建 VLAN 2
interface g1/0/1 # 进入接口
port access vlan 2 # 将 GE_0/1 接口加入 VLAN2
exit
vlan 3 # 创建 VLAN 3
interface g1/0/2 # 进入接口
port access vlan 3 # 将 GE_0/2 接口加入 VLAN3
exit
interface g1/0/3 # 进入接口
port link-type trunk # Switch 互联接口类型设置为 trunk
port trunk permit vlan 2 3
exit
```

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20250515174424204.png" alt="image-20250515174424204" style="zoom:50%;" />

#### Switch 2

```python
sys
vlan 2 # 创建 VLAN 2
interface g1/0/1 # 进入接口
port access vlan 2 # 将 GE_0/1 接口加入 VLAN2
exit
vlan 3 # 创建 VLAN 3
interface g1/0/2 # 进入接口
port access vlan 3 # 将 GE_0/2 接口加入 VLAN3
exit
interface g1/0/3 # 进入接口
port link-type trunk # Switch 互联接口类型设置为 trunk
port trunk permit vlan 2 3
exit
```

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20250515180401137.png" alt="image-20250515180401137" style="zoom:53%;" />

#### 分析与连线

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20250515174847041.png" alt="image-20250515174847041" style="zoom:50%;" />

```markdown
- Switch_1 port GE_0/3 <------> Switch_2 port GE_0/1
- Switch_1 port GE_0/1 <------> PC_1
- Switch_1 port GE_0/2 <------> PC_2
- Switch_2 port GE_0/2 <------> PC_3
- Switch_2 port GE_0/3 <------> PC_4
```

#### 测试

```
display vlan brief # 查看vlan及接口
PC1 ping PC2 timeout
PC1 ping PC3 access
```

##### PC1 ping PC2 timeout

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20250515181314627.png" alt="image-20250515181314627" style="zoom:50%;" />

##### PC1 ping PC3 access

这是成功ping：

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20250515182855736.png" alt="image-20250515182855736" style="zoom:33%;" />























