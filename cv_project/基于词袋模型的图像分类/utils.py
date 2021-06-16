
import os
import csv
import imutils
from imutils.paths import list_images 
from random import shuffle

map_label = {"dog" : 1 , "cat" : -1}

# data_dir: 数据集的根目录
# csv_file_name: 将数据集的名字标签保存到该csv文件中
# name_map_label: 记录类别名映射表
def create_csv(data_dir , csv_file_name , name_map_label , train_flag = True):
    dataset_dir = os.path.join(data_dir , 'train') if train_flag else os.path.join(data_dir , 'test')
    if not os.path.exists(os.path.join(data_dir , csv_file_name)):
        images = []

        img_lsts = list(list_images(dataset_dir))
        shuffle(img_lsts)

        with open(os.path.join(data_dir , csv_file_name) , 'w' , newline = '') as f:
            writer = csv.writer(f)
            for imgName in img_lsts:
                label = imgName.split('/')[-1].split('.')[0]
                print('label: ' , label)
                label = map_label[label]

                writer.writerow([imgName , label])

    # read from csv file
    images , labels = [] , []
    with open(os.path.join(data_dir , csv_file_name)) as f:
        reader = csv.reader(f)
        for row in reader:
            img , label = row
            label = int(label)

            images.append(img)
            labels.append(label)

    assert len(images) == len(labels)
    return images , labels

if __name__ == "__main__":
    dataset_dir = "/Users/han/Opencv_Project/opencv_opengl/基于词袋模型的图像分类/data/猫狗训练集"
    
    train_csv_file = "train.csv"
    create_csv(dataset_dir, train_csv_file , True)

    test_csv_file = "test.csv"
    create_csv(dataset_dir , test_csv_file , False)





    

