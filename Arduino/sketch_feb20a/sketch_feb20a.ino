/* 
 * 場佈圖
 * | --------------------------| 
 * |  |---|    |---|    |---|  |
 * |    藍       綠       紅    |
 * |  |---|    |---|    |---|  |
 * |                           |
 * |    |---|        |---|     |
 * |    | C |\      /| D |    ||
 * |    |---| \____/ |---|     |
 * |                           |
 * |       ------------        |
 * |                           |
 * |_______             _______|
 * |                           |
 * |  |---|            |---|   |
 * |  | B |            | A |   |
 * |  |---|            |---|   |
 * |---------------------------|
 * 
 * 車子構造
 *  |3|-0-----車頭-----1-|0|
 *   |                   |
 *   |                   |
 *   |                   |
 *   |                   |
 *   5                   2
 *   |                   |
 *   |                   |
 *   |                   |
 *   |                   |
 *  |2|-4-------------3-|1|
 *  
 *                            ENA  IN1  IN2  IN3  IN4  ENB
 *  右 L298N (0號馬達、1號馬達):   9   13   12   11   10    8
 *  左 L298N (2號馬達、3號馬達):   3    7    6    5    4    2
 */


//========================= 馬達腳位定義 =========================//
//// IN:方向 EN:速度
const int MEN[4] = {9, 8, 3, 2};
const int MIN[4][2] = {{13,12},{11,10},{7,6},{5,4}};


//========================= 超音波腳位定義 =========================//
const byte trigPin[6] = {29,33,37,41,45,49};
const byte echoPin[6] = {31,35,39,43,47,51};


//========================= 函式定義 =========================//
// 基本動作
// reference=參考牆(0=前,1=後,2=左,3=右)
// compare=比較方法(0=大於,1=小於)
// target=目標距離(數值)
// spd=速度(0~255)
String YOLODetect();
void FollowLine(float spd);
void Forward(int reference, int compare, int target, int spd);
void Backward(int reference, int compare, int target, int spd);
void Leftward(int reference, int compare, int target, int spd);
void Rightward(int reference, int compare, int target, int spd);
void Stop();
void Reset_angle(int spd, int sensorA, int sensorB);
void Go_Red();
void Go_Green();
void Go_Blue();

// 感應器
int Select_sensor(int reference);
unsigned long sr04(int num);

// IN和EN設定
void Forward_IN();
void Backward_IN();
void Leftward_IN();
void Rightward_IN();
void LeftLine_IN();
void RighttLine_IN();
void Stop_IN();
void Clockwise_IN();
void Counterclockwise_IN();
void SetSpeed_EN(float spd);

//========================= setup =========================//
void setup(){
  // 設定通訊鮑率
  Serial.begin(9600);

  // 設定馬達控制板腳位
  for(int i=0; i<4; i++){
    pinMode(MEN[i],    OUTPUT);   
    pinMode(MIN[i][0], OUTPUT);
    pinMode(MIN[i][1], OUTPUT);
  } 

  // 設定超音波感應器腳位
  for(int i=0; i<6; i++){
    pinMode(trigPin[i], OUTPUT);
    pinMode(echoPin[i], INPUT);
  }
}


//========================= loop =========================//
void loop(){
  int FLAG=0;
  int i=0;
  String color = "Red";

  while(Serial.read()!='1'){} //串口監視器輸入1以開始
/*
  // 後退(A點至B點)
  Backward(1, 1, 25, 255); // 參考牆(後), 距離小於25停止, 速度255
  //delay(100);

  // 調整角度
  Reset_angle(100);
  delay(3300);

  // 前進(至AB中點)
  Forward(1, 0, 130, 255);   // 參考牆(後), 距離大於130停止, 速度255
  //delay(100);

  // 向左(至中間牆)
  Leftward(2, 1, 25, 255);     // 參考牆(左), 距離小於25停止, 速度255
  //delay(100);

  // 後退(至C點右側)
  Backward(1, 1, 55, 255);  // 參考牆(後), 距離小於55停止, 速度255
  //delay(100);

  // 調整角度
  Reset_angle(100, 3, 4);   // 調整速度, 感測器A, 感測器B
  //delay(100);

  // 向左(至C點)
  Leftward(3, 0, 160, 255);    // 參考牆(右), 距離大於160停止, 速度255
  //delay(3000);

  // 調整角度
  Reset_angle(100, 3, 4);   // 調整速度, 感測器A, 感測器B
  //delay(3300);

  // 前進(至C點內)
  Forward(1, 0, 60, 255);   // 參考牆(後), 距離大於130停止, 速度255
  //delay(100);

  // 調整角度
  Reset_angle(100, 3, 4);   // 調整速度, 感測器A, 感測器B
  delay(3300);
  */

  // 循線
  Serial.println("Follow Line Start");
  FollowLine(150);    //循線速度
  Serial.println("Follow Line End");
  // delay(3300);

/*
  // 前進(至D點內)
  Forward(1, 0, 60, 255);   // 參考牆(後), 距離大於130停止, 速度255
  // delay(3300);

  // 調整角度
  Reset_angle(100, 0, 1);   // 調整速度, 感測器A, 感測器B
  delay(3300);

  // YOLO辨識
  color = YOLODetect();   // 調整速度, 感測器A, 感測器B
  // delay(3300);

  // 向左(至顏色區)
  Leftward(2, 1, 25, 255);    // 參考牆(左), 距離小於60停止, 速度255

  // 調整角度
  Reset_angle(100, 0, 1);   // 調整速度, 感測器A, 感測器B

  if(color == "Red"){
    Go_Red();
    Leftward(2, 1, 25, 255);    // 參考牆(左), 距離小於60停止, 速度255
    Reset_angle(100, 0, 1);   // 調整速度, 感測器A, 感測器B
  }

  if(color == "Green"){
    Go_Green();
    Leftward(2, 1, 25, 255);    // 參考牆(左), 距離小於60停止, 速度255
    Reset_angle(100, 3, 4);   // 調整速度, 感測器A, 感測器B
  }

  if(color == "Blue"){
    Go_Green();   
    Reset_angle(100, 3, 4);   // 調整速度, 感測器A, 感測器B
    Go_Blue();
    Leftward(2, 1, 25, 255);    // 參考牆(左), 距離小於60停止, 速度255
    Reset_angle(100, 3, 4);   // 調整速度, 感測器A, 感測器B
  }
*/
}


//========================= YOLO Detect =========================//

String YOLODetect(){
  Serial.println("YOLO Detect Start");
  String color;
  Serial.println("TT");
  Serial.println("YOLO_Detect");
  // color = Serial.readStringUntil('\n');
  int cnt = 0;
  while(cnt < 5){
    if(Serial.available()){
      color = Serial.readStringUntil('\n');
      if(color == "Red"){
        return color;
        break;
      }else if(color == "Green"){
        return color;
        break;
      }else if(color == "Blue"){
        return color;
        break;
      }else{
        Serial.println("YOLO_Detect");
        cnt++;
      }
    }
  Serial.println("YOLO Detect End");
  }
}

//========================= Follow Line =========================//
void FollowLine(float spd){
  int sensor_num = Select_sensor(0);
  float dis = sr04(sensor_num);
  String str;
  Serial.println("TT");
  Serial.println("Trace_Start");
  while(dis > 160){
    dis = sr04(sensor_num);
    if (Serial.available()){
      str = Serial.readStringUntil('\n');
      if (str == "p"){
        RightLine_IN();
        SetSpeed_EN(spd);
        Serial.println("Get p");
      }
      if (str == "n"){
        LeftLine_IN();
        SetSpeed_EN(spd);
        Serial.println("Get n");
      }
      if (str == "o"){
        Forward_IN();
        SetSpeed_EN(spd);
        Serial.println("Get o");
      }
    }
  }
  Serial.println("Trace_Done");
}

//========================= Forward =========================//
void Forward(int reference, int compare, int target, int spd){

  // 設定比較對象(使用哪顆感應器)
  int sensor_num = Select_sensor(reference);
  float dis = sr04(sensor_num);

  Serial.println("Forward Start");
  // set motor dir (01:CW, 10:CCW)
  Forward_IN();

  // 比較方法(大於或小於)
  int cnt = 0;
  if(compare==0){
    while(dis <= target || cnt < 10){
      dis = sr04(sensor_num);
      if(dis < target - 30){
        Forward_IN();
        SetSpeed_EN(spd);
      }else if(dis > target && dis < target*2){
        
        Backward_IN();
        SetSpeed_EN(spd * 0.5);
      }else{
        cnt++;
        Serial.print("cnt : ");
        Serial.println(cnt);
        Serial.print("dis : ");
        Serial.println(dis);
        Forward_IN();
        SetSpeed_EN(spd * 0.5);
      }
    }
  }else{
    while(dis >= target || cnt < 5){
      dis = sr04(sensor_num);
      if(dis > target + 30){
        Forward_IN();
        SetSpeed_EN(spd);
      }else if(dis < target){
        
        Backward_IN();
        SetSpeed_EN(spd * 0.5);
      }else{
        cnt++;
        Serial.print("cnt : ");
        Serial.println(cnt);
        Serial.print("dis : ");
        Serial.println(dis);
        Forward_IN();
        SetSpeed_EN(spd * 0.5);
      }
    }
  }
  Stop();
  Serial.println(cnt);
  Serial.println("Forward End");
}


//========================= Backward =========================//
void Backward(int reference, int compare, int target, int spd){

  // 設定比較對象(使用哪顆感應器)
  int sensor_num = Select_sensor(reference);
  float dis = sr04(sensor_num);
  
  Serial.println("Backward Start");
  // set motor dir (01:CW, 10:CCW)
  Backward_IN();

  // 比較方法(大於或小於)
  int cnt = 0;
  if(compare==0){
    while(dis <= target || cnt < 10){
      dis = sr04(sensor_num);
      if(dis < target - 30){
        Backward_IN();
        SetSpeed_EN(spd);
      }else if(dis > target){
        cnt++;
        Serial.println(cnt);
        Forward_IN();
        SetSpeed_EN(spd * 0.5);
      }else{
        Backward_IN();
        SetSpeed_EN(spd * 0.5);
      }
    }
  }else{
    while(dis >= target || cnt < 5){
      dis = sr04(sensor_num);
      if(dis > target + 30){
        Backward_IN();
        SetSpeed_EN(spd);
      }else if(dis < target){
        cnt++;
        Serial.println(cnt);
        Forward_IN();
        SetSpeed_EN(spd * 0.5);
      }else{
        Backward_IN();
        SetSpeed_EN(spd * 0.5);
      }
    }
  }
  Stop();
  Serial.println(cnt);
  Serial.println("Backward End");
}


//========================= Leftward =========================//
void Leftward(int reference, int compare, int target, int spd){
  // 設定比較對象(使用哪顆感應器)
  int sensor_num = Select_sensor(reference);
  float dis = sr04(sensor_num);

  Serial.println("Leftward Start");
  // set motor dir (01:CW, 10:CCW)
  Leftward_IN();

  // 比較方法(大於或小於)
  int cnt = 0;
  if(compare==0){
    while(dis <= target || cnt < 10){
      dis = sr04(sensor_num);
      if(dis < target - 30){
        Leftward_IN();
        SetSpeed_EN(spd);
      }else if(dis > target){
        cnt++;
        Serial.println(cnt);
        Rightward_IN();
        SetSpeed_EN(spd * 0.5);
      }else{
        Leftward_IN();
        SetSpeed_EN(spd * 0.5);
      }
    }
  }else{
    while(dis >= target || cnt < 5){
      dis = sr04(sensor_num);
      if(dis > target + 30){
        Leftward_IN();
        SetSpeed_EN(spd);
      }else if(dis < target){
        cnt++;
        Serial.println(cnt);
        Rightward_IN();
        SetSpeed_EN(spd * 0.5);
      }else{
        Leftward_IN();
        SetSpeed_EN(spd * 0.5);
      }
    }
  }
  Stop();
  Serial.println(cnt);
  Serial.println("Leftward End");
}


//========================= Rightward =========================//
void Rightward(int reference, int compare, int target, int spd){
  // 設定比較對象(使用哪顆感應器)
  int sensor_num = Select_sensor(reference);
  float dis = sr04(sensor_num);

  Serial.println("Rightward Start");
  // set motor dir (01:CW, 10:CCW)
  Rightward_IN();

  // 比較方法(大於或小於)
  int cnt = 0;
  if(compare==0){
    while(dis <= target || cnt < 10){
      dis = sr04(sensor_num);
      if(dis < target - 30){
        Rightward_IN();
        SetSpeed_EN(spd);
      }else if(dis > target){
        cnt++;
        Serial.println(cnt);
        Leftward_IN();
        SetSpeed_EN(spd * 0.5);
      }else{
        Rightward_IN();
        SetSpeed_EN(spd * 0.5);
      }
    }
  }else{
    while(dis >= target || cnt < 5){
      dis = sr04(sensor_num);
      if(dis > target + 30){
        Rightward_IN();
        SetSpeed_EN(spd);
      }else if(dis < target){
        cnt++;
        Serial.println(cnt);
        Leftward_IN();
        SetSpeed_EN(spd * 0.5);
      }else{
        Rightward_IN();
        SetSpeed_EN(spd * 0.5);
      }
    }
  }
  Stop();
  Serial.println(cnt);
  Serial.println("Rightward End");
}


//========================= Stop =========================//
void Stop(){
  Stop_IN();
  SetSpeed_EN(0);
}


//========================= Reset Angle =========================//
void Reset_angle(int spd, int sensorA, int sensorB){
  Serial.println("Reset Angle Start");
  float disA = sr04(sensorA);
  float disB = sr04(sensorB);
  float min = -0.5;
  float max = 0.5;
  while((disA - disB) < min || (disA - disB) > max){
    disA = sr04(sensorA);
    disB = sr04(sensorB);
    Serial.print(disA);
    Serial.print(",");
    Serial.println(disB);
    if ((disA - disB) < min){         //超聲波4比較近
      Counterclockwise_IN();        //順時針轉
      SetSpeed_EN(spd);
    }else if((disA - disB) > max){    //超聲波3比較近
      Clockwise_IN();               //逆時針轉
      SetSpeed_EN(spd);
    }
  }
  Stop();
  Serial.println("Reset Angle End");
}

//========================= Go Red =========================//
void Go_Red(){
  Serial.println("Go Red Start");
  Forward(0, 1, 25, 255);   // 參考牆(前), 距離小於60停止, 速度255
  Serial.println("Go Red End");
}

//========================= Go Green =========================//
void Go_Green(){
  Serial.println("Go Green Start");
  Backward(1, 1, 145, 255);   // 參考牆(後), 距離小於145停止, 速度255
  Serial.println("Go Green End");
}

//========================= Go Blue =========================//
void Go_Blue(){
  Serial.println("Go Blue Start");
  Backward(1, 1, 25, 255);   // 參考牆(後), 距離小於25停止, 速度255
  Serial.println("Go Blue End");
}

//========================= Reference Wall Setting =========================//
int Select_sensor(int reference){
  // 輸入參考面 輸出感應器編號
  int sensor = 0;
  if(reference==0){
    return 1;
  }else if(reference==1){
    return 3;    
  }else if(reference==2){
    return 5;
  }else{
    return 2;    
  }
}


//========================= Sensor Setting =========================//
unsigned long sr04(int num) { 
  digitalWrite(trigPin[num], HIGH);
  delayMicroseconds(10);  
  digitalWrite(trigPin[num], LOW);

  float d = pulseIn(echoPin[num], HIGH) * 0.017;
  
  if(d < 500){
    Serial.print(num);
    Serial.print(": ");
    Serial.print(d,0);
    Serial.println("cm ");
    return d;
  }
}


//========================= IN Setting =========================//
void Forward_IN(){
  digitalWrite(MIN[0][0],LOW);
  digitalWrite(MIN[0][1],HIGH);
  digitalWrite(MIN[1][0],LOW);
  digitalWrite(MIN[1][1],HIGH);
  digitalWrite(MIN[2][0],HIGH);
  digitalWrite(MIN[2][1],LOW);
  digitalWrite(MIN[3][0],HIGH);
  digitalWrite(MIN[3][1],LOW);
}

void Backward_IN(){
  digitalWrite(MIN[0][0],HIGH);
  digitalWrite(MIN[0][1],LOW);
  digitalWrite(MIN[1][0],HIGH);
  digitalWrite(MIN[1][1],LOW);
  digitalWrite(MIN[2][0],LOW);
  digitalWrite(MIN[2][1],HIGH);
  digitalWrite(MIN[3][0],LOW);
  digitalWrite(MIN[3][1],HIGH);
}

void Leftward_IN(){
  digitalWrite(MIN[0][0],LOW);
  digitalWrite(MIN[0][1],HIGH);
  digitalWrite(MIN[1][0],HIGH);
  digitalWrite(MIN[1][1],LOW);
  digitalWrite(MIN[2][0],HIGH);
  digitalWrite(MIN[2][1],LOW);
  digitalWrite(MIN[3][0],LOW);
  digitalWrite(MIN[3][1],HIGH);
}

void Rightward_IN(){
  digitalWrite(MIN[0][0],LOW);
  digitalWrite(MIN[0][1],LOW);
  digitalWrite(MIN[1][0],LOW);
  digitalWrite(MIN[1][1],HIGH);
  digitalWrite(MIN[2][0],HIGH);
  digitalWrite(MIN[2][1],LOW);
  digitalWrite(MIN[3][0],LOW);
  digitalWrite(MIN[3][1],LOW);
}

void LeftLine_IN(){
  digitalWrite(MIN[0][0],LOW);
  digitalWrite(MIN[0][1],HIGH);
  digitalWrite(MIN[1][0],LOW);
  digitalWrite(MIN[1][1],LOW);
  digitalWrite(MIN[2][0],HIGH);
  digitalWrite(MIN[2][1],LOW);
  digitalWrite(MIN[3][0],LOW);
  digitalWrite(MIN[3][1],LOW);
}

void RightLine_IN(){
  digitalWrite(MIN[0][0],LOW);
  digitalWrite(MIN[0][1],LOW);
  digitalWrite(MIN[1][0],LOW);
  digitalWrite(MIN[1][1],HIGH);
  digitalWrite(MIN[2][0],LOW);
  digitalWrite(MIN[2][1],LOW);
  digitalWrite(MIN[3][0],HIGH);
  digitalWrite(MIN[3][1],LOW);
}

void Stop_IN(){
  digitalWrite(MIN[0][0],LOW);
  digitalWrite(MIN[0][1],LOW);
  digitalWrite(MIN[1][0],LOW);
  digitalWrite(MIN[1][1],LOW);
  digitalWrite(MIN[2][0],LOW);
  digitalWrite(MIN[2][1],LOW);
  digitalWrite(MIN[3][0],LOW);
  digitalWrite(MIN[3][1],LOW);
}

void Clockwise_IN(){
  digitalWrite(MIN[0][0],HIGH);
  digitalWrite(MIN[0][1],LOW);
  digitalWrite(MIN[1][0],HIGH);
  digitalWrite(MIN[1][1],LOW);
  digitalWrite(MIN[2][0],HIGH);
  digitalWrite(MIN[2][1],LOW);
  digitalWrite(MIN[3][0],HIGH);
  digitalWrite(MIN[3][1],LOW);
}

void Counterclockwise_IN(){
  digitalWrite(MIN[0][0],LOW);
  digitalWrite(MIN[0][1],HIGH);
  digitalWrite(MIN[1][0],LOW);
  digitalWrite(MIN[1][1],HIGH);
  digitalWrite(MIN[2][0],LOW);
  digitalWrite(MIN[2][1],HIGH);
  digitalWrite(MIN[3][0],LOW);
  digitalWrite(MIN[3][1],HIGH);
}


//========================= EN Setting =========================//
void SetSpeed_EN(float spd){
  analogWrite(MEN[0],spd);
  analogWrite(MEN[1],spd);
  analogWrite(MEN[2],spd);
  analogWrite(MEN[3],spd);
}
