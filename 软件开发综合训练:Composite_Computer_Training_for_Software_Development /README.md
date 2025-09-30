# README

## Requirements - 必要条件

- python 3.x
- numpy
- opencv-python
- Scikit-learn

```python
pip install opencv-python
```

```python
pip install numpy
```

```python
pip install scikit-learn
```



## 运行ALan_.avi视频

位于代码91行，取消91至96行注释，并且保证98至100行已被注释

```python
# Alan_1.avi的表示界限
# 获取图像的高度，即y轴的最大值
line_y_max = int(img.shape[0])
# 设置y轴的最小值为图像高度的2/3处
line_y_min = int(img.shape[0]*0.7)
```

位于代码218行，取消218至224行注释，并且保证226至240行已被注释

```python
# Alan_1.avi中的感兴趣的ROI设置，目前模拟较好
rows, cols = edges.shape
bottom_left = [cols * 0.1, rows]
top_left = [cols * 0.333,rows * 0.67]
bottom_right = [cols * 0.65, rows]
top_right = [cols * 0.55, rows * 0.67]
vertices = np.array([[bottom_left, top_left, top_right, bottom_right]], dtype=np.int32)
```

位于代码266行，放入`Alan_.avi`路径

```python
video_file = "C:\\Users\\37623\\Desktop\\LaneLineDet\\Alan_1.avi"
```



## 运行tunnel.avi视频

位于代码98行，取消98至100行注释，并且保证91至96行已被注释

```python
# tunnel.avi的表示界限
line_y_max = int(img.shape[0] * 0.8)
line_y_min = int(img.shape[0] * 0.3)
```

位于代码234行，取消234至240行注释，并且保证218至231行已被注释

```python
# tunnel.avi中的感兴趣的ROI设置,这个坐标整体设置不错
rows, cols = edges.shape
bottom_left = [40, 239]
top_left = [200, 100]
bottom_right = [433, 239]
top_right = [433, 100]
vertices = np.array([[bottom_left, top_left, top_right, bottom_right]], dtype=np.int32)
```

位于代码266行，放入`tunnel.avi`路径

```python
video_file = "C:\\Users\\37623\\Desktop\\LaneLineDet\\tunnel.avi"
```

