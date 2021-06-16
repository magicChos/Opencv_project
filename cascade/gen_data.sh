#!bin/bash

# -info :描述物体所在图像以及大小位置的描述文件
# -vec  :输出文件，内含用于训练的正样本
# -img  :输入的图像文件名（例如一个公司的标志）
# -bg   :背景图像的描述文件，文件包含一系列的图像文件名，这些图像将被随机选做物体的背景
# -num  :生成的正样本数目
# -bgcolor:背景颜色（目前为灰度图）；背景颜色表示透明颜色。因为图像压缩可造成颜色偏差，颜色的容差可以由-bgthresh指定。所有处于bgcolor-bgthresh和bgcolor+bgthresh之间的像素都被设置为透明像素
# -bgthresh
# -inv  :如果指定该标志，前景图像的颜色将翻转
# -randinv:如果指定该标志。颜色将随机的翻转
# -maxidev:前景样本里像素的亮度梯度的最大值
# -maxxangle:X轴最大旋转角度，必须以弧度为单位
# -maxyangle:Y轴最大旋转角度，必须以弧度为单位
# -maxzangle:Z轴最大旋转角度，必须以弧度为单位
# -show  :很有用的调试选项。如果指定该选项，每个样本都将被显示。如果按下Esc键，程序将继续创建样本但不再显示
# -w     :输出样本的宽度
# -h     :输出样本的高度
opencv_createsamples -info positive.txt -vec pos.vec -num 464 -show True -w 64 -h 64
