import YOLO_Detect
import tracking
import cv2
import serial
import sys

COM_PORT = '/dev/ttyACM0'  # 請自行修改序列埠名稱
BAUD_RATES = 9600
ser = serial.Serial(COM_PORT, BAUD_RATES)

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
        if(weights > 0):
            ser.write(b'p\n')  # 訊息必須是位元組類型
        if(weights < 0):
            ser.write(b'n\n')  # 訊息必須是位元組類型
        if(weights == 0):
            ser.write(b'o\n')  # 訊息必須是位元組類型
        print(weights)

        while ser.in_waiting:
            mcu_feedback = ser.readline().decode()  # 接收回應訊息並解碼
            print('控制板回應：', mcu_feedback)


