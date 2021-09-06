import numpy as np

if __name__ == '__main__':
    ir_points = np.array([[18 , 9] , [262 , 29]  , [8 , 161] , [256 , 172]])
    rgb_points = np.array([[178 , 149] , [408 , 149] , [181 , 283]  , [410 , 270]])
    scale_rgb_points = np.array([[178 , 149] , [408 , 149] , [181 , 283] , [410 , 270]])


    # print(ir_points)
    # print("------------------")
    # print(rgb_points)
    # print("******************")

    # for i in range(4):
    #     one_index = i
    #     two_index = (i + 1)%4
    #     print(f"({one_index} , {two_index})")

        
    #     ir = (ir_points[two_index] - ir_points[one_index])
    #     rgb = (rgb_points[two_index] - rgb_points[one_index])
    #     print(ir)
    #     print("------------------")
    #     print(rgb)
    #     print("********************")
    #     print(ir/rgb)

    #     print("************************************")

    #     # print(scaleFactor)

    a = scale_rgb_points - ir_points
    print(np.mean(a , 0))


    