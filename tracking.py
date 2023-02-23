import cv2
import numpy as np
import math

def get_line_position(org_img):

    # 匯入圖
    # org_img = cv2.imread("C:/Users/40850/Downloads/IRC_ws/Line/WIN_20230223_16_55_25_Pro.jpg")

    # 裁切區域
    org_img = cv2.resize(org_img, (720,405))
    org_img = org_img[150:405, 0:720]

    # gray_img = cv2.cvtColor(org_img, cv2.COLOR_BGR2GRAY)

    # 雙向濾波
    fillter_img = cv2.bilateralFilter(org_img, 10, 50, 300)

    # 中值濾波
    fillter_img = cv2.medianBlur(fillter_img, 37)

    # 提取邊緣
    canny_img = cv2.Canny(fillter_img, 10, 20, None, 3)

    # 膨脹
    kernel = np.ones((5,5), np.uint8)
    canny_img = cv2.dilate(canny_img, kernel, 1)

    # 霍夫找線
    lines = cv2.HoughLinesP(canny_img,1,np.pi/180, 100, 1000, 50)

    # 計算線
    weights = 0
    for line in lines:  
        for x1,y1,x2,y2 in line:
            angle = math.degrees(math.atan2((y2-y1),(x2-x1)))
            if ((angle>30 and angle<150) or (angle<-30 and angle>-150)):
                weights += (((x1+x2)-720)*6)*((y1/345)*4)
                # weights += ((x1+x2)-720)*(y1/345)*100
                cv2.putText(org_img, str(weights), (x1,y1), cv2.FONT_HERSHEY_SIMPLEX, .5, (0, 255, 255), 1, cv2.LINE_AA)
                cv2.line(org_img, (x1,y1), (x2,y2), (0,0,255), 2)
        weights = weights/len(lines)
    # cv2.imshow('org_img', org_img)
    # cv2.waitKey(1)

    return weights

# print('weights=', weights)

# cv2.imshow('org_img', org_img)
# cv2.imshow('fillter_img', fillter_img)
# cv2.imshow('canny_img', canny_img)
# cv2.waitKey(0)