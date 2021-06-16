##### 行人检测
- 启动服务 python3 webapp.py
- 开始测试 python3 test.py

本工程行人检测主要包括两种方法
- yolov3-tiny检测
- hog+svm检测
  
&#8194;&#8194;&#8194;如果需要尝试两种不同的方法，需要在webapp.py中detect_Person函数中进行修改。
通过测试发现，yolov3-tiny相比于hog+svm方法效果更好一些，但是相对于耗时是其5倍左右。由于用的都是公有模型，如果后续需要提升精度，需要重新训练模型。