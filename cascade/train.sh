#!/bin/bash

# 1通用参数
# -data  :目录名，用于存储训练好的分类器
# -vec   :包含正样本的vec文件，由opencv_createsamples程序生成
# -bg    :背景描述文件，也就是包含负样本文件名的那个文件
# -numPos:每级分类器训练时所用的正样本数目
# -numNeg:每级分类器训练时所用的负样本数目，可以大于-bg指定的图片数目
# -numState:训练的分类器的级数
# -precalcValBufSize:缓存大小，用于存储预先计算的特征值(feature values)，单位为MB
# -precalcIdxBufSize:缓存大小，用于存储预先计算的特征索引(feature indices)，单位为MB。内存越大，训练时间越短
# -baseFormatSave   :这个参数仅在使用Haar特征时有效。如果指定这个参数，那么级联分类器将以老的格式存储

# 2级联参数
# -stageType  :级别（stage）参数。目前只支持将BOOST分类器作为级别的类型
# -featureType:特征的类型： HAAR - 类Haar特征； LBP - 局部纹理模式特征
# -w
# -h    :训练样本的尺寸（单位为像素），必须跟训练样本创建时的尺寸保持一致

# 3分类器参数
# bt <{DAB, RAB, LB,GAB(default)}> Boosted分类器参数
# 

# 4Harr特征参数
# -mode <BASIC (default) |CORE | ALL>选择训练过程中使用的Haar特征的类型。 BASIC 只使用右上特征， ALL使用所有右上特征和45度旋转特征

# 5LBP特征参数
# LBP特征无参数
opencv_traincascade -data TrainCasecade -vec pos.vec -bg negtive.txt -numPos 320 -numNeg 1000 -numStages 20 -mem 200 -featureType LBP -w 64 -h 64 -minHitRate 0.9999 -maxFalseAlarmRate 0.5 -mode ALL
