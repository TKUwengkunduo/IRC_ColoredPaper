import YOLO_Detect
import cv2

cap = cv2.VideoCapture(0)


if __name__=='__main__':
    while True:
        img = cv2.imread('/home/ickab/IRC_ColoredPaper/test_img/Blue_0002.jpg')
        reslut = YOLO_Detect.detect(img)
        print(reslut)
