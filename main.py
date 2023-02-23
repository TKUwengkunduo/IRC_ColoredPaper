import YOLO_Detect
import tracking
import cv2

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1920)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1080)


if __name__=='__main__':
    while True:
        # img = cv2.imread('/home/ickab/IRC_ColoredPaper/test_img/Green_0057.jpg')
        # reslut = YOLO_Detect.detect(img)
        # print(reslut)

        ret, img = cap.read()
        weights = tracking.get_line_position(img)
        print(weights)

