import YOLO_Detect
import tracking
import cv2
import serial
import sys
import time

COM_PORT = '/dev/ttyACM0'  # 請自行修改序列埠名稱
BAUD_RATES = 9600
ser = serial.Serial(COM_PORT, BAUD_RATES, timeout=0.1)

cap = cv2.VideoCapture(2)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1920)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1080)

cap2 = cv2.VideoCapture(0)
cap2.set(cv2.CAP_PROP_FRAME_WIDTH, 1920)
cap2.set(cv2.CAP_PROP_FRAME_HEIGHT, 1080)


if __name__=='__main__':
    
    while True:
        while ser.in_waiting:
            mcu_feedback = ser.readline().decode()  # 接收回應訊息並解碼
            print('get',mcu_feedback)
        
            if(mcu_feedback[:-2] == "TT"):
                now_time = time.time()
                while True:
                    # mcu_feedback = ser.readline().decode()
                    print('get', mcu_feedback)
                    ret, img = cap.read()
                    weights = tracking.get_line_position(img)
                    if(weights > 0):
                        ser.write(b'p\n')  # 訊息必須是位元組類型
                        print("return " , weights)
                    if(weights < 0):
                        ser.write(b'n\n')  # 訊息必須是位元組類型
                        print("return " , weights)
                    if(weights == 0):
                        ser.write(b'o\n')  # 訊息必須是位元組類型
                        print("return " , weights)

                    
                    # if(mcu_feedback[:-2] == "SS" or ((time.time()-now_time)>6)):
                    if ((time.time()-now_time)>6):
                        print('end')
                        break
                while True:
                    ret, img = cap2.read()
                    reslut = YOLO_Detect.detect(img)
                    if(reslut == '0'):
                        ser.write(b'N\n')
                        print('NoColor')
                    elif(reslut == '1'):
                        ser.write(b'R\n')
                        print('Red')
                    elif(reslut == '2'):
                        ser.write(b'G\n')
                        print('Green')
                    elif(reslut == '3'):
                        ser.write(b'B\n')
                        print('Blue')
                    else:
                        ser.write(b'N\n')
                        print('else')
                    time.sleep(300)
            
            # if(mcu_feedback[:-2] == "YY"):
                
                    
        
        # img = cv2.imread('/home/ickab/IRC_ColoredPaper/test_img/Green_0057.jpg')
        # reslut = YOLO_Detect.detect(img)
        # print(reslut)

        

        


