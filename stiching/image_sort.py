#coding:utf-8
import os
from glob import glob
import numpy as np
import cv2

# 无序图像的自动排序
def sortImages(dir_path):
    img_lsts = os.listdir(dir_path)

    suffix = None
    if img_lsts[1].endswith("jpeg"):
        suffix = "*.jpeg"
    elif img_lsts[1].endswith("png"):
        suffix = "*.png"
    elif img_lsts[1].endswith("jpg"):
        suffix = "*.jpg"
    else:
        print('not support image format')
        exit(0)

    img_lsts = glob(os.path.join(dir_path , suffix))
    imgs = [cv2.pyrDown(cv2.imread(i)) for i in img_lsts]
    imgs = [cv2.pyrDown(i) for i in imgs]

    num_imgs = len(img_lsts)
    # 同名点矩阵
    match_point_matrix = np.zeros((num_imgs, num_imgs), dtype=np.int32)
    # 左右位移关系矩阵
    relation_matrix = np.zeros(((num_imgs, num_imgs)), dtype=np.int32)

    surf = cv2.xfeatures2d.SURF_create()
    key_des = []
    for img in imgs:
        k, d = surf.detectAndCompute(img, None)
        key_des.append((k, d))

    # Bruteforce matcher on the descriptors
    all_matches = []
    bf = cv2.BFMatcher()
    for i in range(num_imgs):
        for j in range(num_imgs):
            verified_matches = []
            if i != j:
                matches1 = bf.knnMatch(key_des[i][1], key_des[j][1], 2)
                # --------------------------
                # Make sure that the matches are good
                verify_ratio = 0.6
                verified_matches1 = []

                for m1, m2 in matches1:
                    if m1.distance < verify_ratio * m2.distance:
                        verified_matches1.append(m1)

                matches2 = bf.knnMatch(key_des[j][1], key_des[i][1], k=2)
                verified_matches2 = []
                for m1, m2 in matches2:
                    if m1.distance < verify_ratio * m2.distance:
                        verified_matches2.append(m1)

                # symmetry test
                for sym1 in verified_matches1:
                    for sym2 in verified_matches2:
                        if sym1.queryIdx == sym2.trainIdx and sym1.trainIdx == sym2.queryIdx:
                            verified_matches.append(sym1)


                match_point_matrix[i][j] = len(verified_matches)

                if len(verified_matches) < 8:
                    continue

                points_i = []
                points_j = []
                for match in verified_matches:
                    points_i.append(key_des[i][0][match.queryIdx].pt)
                    points_j.append(key_des[j][0][match.trainIdx].pt)

                points_i = np.array(points_i)
                points_j = np.array(points_j)

                if np.mean(points_i - points_j, 0)[0] > 0:
                    relation_matrix[i][j] = 1  # j在i的右侧
                else:
                    relation_matrix[i][j] = -1  # j在i的左侧

            all_matches.append(verified_matches)

    # print(match_point_matrix)
    # print(relation_matrix)

    left_index = np.where(relation_matrix == 1)
    right_index = np.where(relation_matrix == -1)

    head_index = 0
    tail_index = num_imgs - 1
    for i in range(num_imgs):
        if i not in left_index[1]:
            head_index = i
            continue
        if i not in right_index[1]:
            tail_index = i
            continue

    image_sequence = {}
    image_sequence[0] = img_lsts[head_index]
    image_sequence[num_imgs - 1] = img_lsts[tail_index]

    right = head_index
    count = 0
    while count < num_imgs - 2:
        match_points_col = match_point_matrix[:, right]
        relation_matrix_col = relation_matrix[:, right]

        less_index = np.argsort(-match_points_col)
        count += 1
        for i in less_index:
            if relation_matrix_col[i] == -1 and not image_sequence.has_key(count):
                right = i
                image_sequence[count] = img_lsts[i]
                break

    print(image_sequence)
    sorted_lst = []
    for i in xrange(num_imgs):
        sorted_lst.append(image_sequence[i])
    return sorted_lst


if __name__ == '__main__':
    dir_path = "/Users/han/PycharmProjects/stichImg"
    sorted_image = sortImages(dir_path)
    print(sorted_image)










