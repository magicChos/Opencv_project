#### 利用bow+svm进行猫狗分类
利用opencv中的bow类和svm类
主要流程：
- 特征提取与描述子生成
- 生成词汇字典，通过k-means对描述子数据进行聚类分析，生成k个聚类，得到每个聚类中心的数据，根据每个描述子到这些聚类中心的距离，决定了它属于哪个聚类
- SVM分类训练与模型生成
- 使用模型预测
完整步骤借用一张图表示如下
![](https://mmbiz.qpic.cn/mmbiz_png/urgCdYOG5QcgoUhIPruxxa6Q56s8frLRAibwLNHNyTayWeVmXNdFhsqJ4WDicJD46XDhbcweHUqCaHjVDao9kzNA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)
生成视觉词汇字典和训练svm分类器
```
python3 train.py
```
测试
```
python3 test.py
```