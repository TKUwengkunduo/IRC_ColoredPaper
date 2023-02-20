import serial
from time import sleep
import sys

COM_PORT = '/dev/ttyACM0'  # 請自行修改序列埠名稱
BAUD_RATES = 9600
ser = serial.Serial(COM_PORT, BAUD_RATES)

try:
    while True:
        # 接收用戶的輸入值並轉成小寫
        choice = input('按1~5開燈、按0關燈、按e關閉程式  ').lower()

        if choice == '1':
            print('傳送開燈指令')
            ser.write(b'A_ON\n')  # 訊息必須是位元組類型
            sleep(0.5)              # 暫停0.5秒，再執行底下接收回應訊息的迴圈
        elif choice == '2':
            print('傳送開燈指令')
            ser.write(b'B_ON\n')
            sleep(0.5)
        elif choice == '3':
            print('傳送開燈指令')
            ser.write(b'C_ON\n')
            sleep(0.5)
        elif choice == '4':
            print('傳送開燈指令')
            ser.write(b'D_ON\n')
            sleep(0.5)
        elif choice == '5':
            print('傳送開燈指令')
            ser.write(b'E_ON\n')
            sleep(0.5)
        elif choice == '0':
            print('傳送關燈指令')
            ser.write(b'OFF\n')
            sleep(0.5)
        elif choice == 'e':
            ser.close()
            print('再見！')
            sys.exit()
        else:
            print('指令錯誤…')

        while ser.in_waiting:
            mcu_feedback = ser.readline().decode()  # 接收回應訊息並解碼
            print('控制板回應：', mcu_feedback)
            
except KeyboardInterrupt:
    ser.close()
    print('再見！')



















# import serial
# import time

# COM_PORT = '/dev/ttyACM0'    # 指定通訊埠名稱
# BAUD_RATES = 9600    # 設定傳輸速率
# ser = serial.Serial(COM_PORT, BAUD_RATES,timeout=0.001)   # 初始化序列通訊埠

# try:
#     while True:
#         ser.write(str('2').encode("gbk"))
#         while ser.in_waiting:          # 若收到序列資料…
#             data = ser.read().decode("utf-8")  # 讀取一行
#             print('接收到的資料：', data)
#             time.sleep(5)

# except KeyboardInterrupt:
#     ser.close()    # 清除序列通訊物件
#     print('再見！')