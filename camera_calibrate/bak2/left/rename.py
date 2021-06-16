import glob
import os
    
img_lsts = glob.glob(os.getcwd() + "/*.jpg")
for i , name in enumerate(img_lsts):
	new_name = "left-" + (i + 1) + ".jpg"
        os.system("mv " + name + " " + os.path.join(os.getcwd() , new_name))
