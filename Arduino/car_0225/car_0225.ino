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
 *                              ENA  IN1  IN2  IN3  IN4  ENB
 *  右 L298N (0號馬達、1號馬達):   9   13   12   11   10    8
 *  左 L298N (2號馬達、3號馬達):   3    7    6    5    4    2
 */


//========================= 馬達腳位定義 =========================//
//// IN:方向 EN:速度
const int MEN[4] = {9, 8, 3, 2};
// const int MIN[4][2] = {{13,12},{11,10},{7,6},{5,4}};
const int MIN[4][2] = {{12,13},{10,11},{6,7},{4,5}};

//========================= 超音波腳位定義 =========================//
const byte trigPin[6] = {43,23,27,31,51,47};
const byte echoPin[6] = {45,25,29,33,53,49};

//========================= 函式定義 =========================//
// 基本動作
// reference=參考牆(0=前,1=後,2=左,3=右)
// compare=比較方法(0=大於,1=小於)
// target=目標距離(數值)
// spd=速度(0~255)
int YOLODetect();
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
  int color=1;

  //Backward_IN();
  //SetSpeed_EN(255);

  while(Serial.read()!='1'){} //串口監視器輸入1以開始
  
  
  // 後退(A點至B點)
  Backward(1, 1, 25, 255); // 參考牆(後), 距離小於25停止, 速度255
  //delay(100);

  // 調整角度
  Reset_angle(80, 3, 4);   // 調整角度(速度, 感測器A, 感測器B)
  delay(3150);

  // 向左(至A點)
  // Leftward(2, 1, 45, 255);     // 參考牆(左), 距離小於25停止, 速度255  

  // 前進(至AB中點)
  Forward(1, 0, 110, 255);   // 參考牆(後), 距離大於135停止, 速度255
  //delay(100);

  // 調整角度
  Reset_angle(80, 3, 4);   // 調整角度(速度, 感測器A, 感測器B)

  // 向左(至中間牆)
  Leftward(2, 1, 20, 255);     // 參考牆(左), 距離小於25停止, 速度255
  //delay(100);

  // 調整角度
  Reset_angle(80, 3, 4);   // 調整角度(速度, 感測器A, 感測器B)

  // 後退(至C點右側)
  Backward(1, 1, 40, 255);  // 參考牆(後), 距離小於55停止, 速度255
  //delay(100);

  // 調整角度
  Reset_angle(80, 3, 4);   // 調整角度(速度, 感測器A, 感測器B)
  //delay(100);

  // 向左(至C點)
  Leftward(3, 0, 150, 255);    // 參考牆(右), 距離大於150停止, 速度255
  // Leftward(2, 1, 125, 255);    // 參考牆(右), 距離大於150停止, 速度255
  //delay(3000);

  // 調整角度
  Reset_angle(80, 3, 4);   // 調整角度(速度, 感測器A, 感測器B)
  //delay(3300);

  // 前進+左移(至C點內)
  Forward(1, 0, 60, 255);   // 參考牆(後), 距離大於65停止, 速度255
  Leftward(2, 1, 135, 255);    // 參考牆(左), 距離小於150停止, 速度255
  //delay(100)

  // 調整角度
  Reset_angle(80, 3, 4);   // 調整角度(速度, 感測器A, 感測器B)
  delay(2000);  //c點停留3秒
  Reset_angle(80, 3, 4);

  // 前進+左移(至C點內)
  Forward(1, 0, 60, 255);   // 參考牆(後), 距離大於65停止, 速度255
  Leftward(2, 1, 145, 255);    // 參考牆(左), 距離小於150停止, 速度255

  
  // 調整角度
  Reset_angle(80, 3, 4);   // 調整角度(速度, 感測器A, 感測器B)

  Leftward(2, 1, 150, 255);    // 參考牆(左), 距離小於150停止, 速度255

  // line
  FollowLine(200);
  
  // 前進+左移(至D點內)
  Forward(0, 1, 60, 200);   // 參考牆(前), 距離小於60停止, 速度255
  Leftward(2, 1, 130, 255);    // 參考牆(左), 距離小於150停止, 速度255
  
  // 調整角度
  Reset_angle(80, 0, 1);   // 調整角度(速度, 感測器A, 感測器B)

  // 前進+左移(至D點內)
  Forward(0, 1, 60, 200);   // 參考牆(前), 距離小於60停止, 速度255
  Leftward(2, 1, 130, 255);    // 參考牆(左), 距離小於150停止, 速度255
  
  // 調整角度
  Reset_angle(80, 0, 1);   // 調整角度(速度, 感測器A, 感測器B)
  
  delay(3150);

  // YOLO辨識
  color = YOLODetect();   // 調整速度, 感測器A, 感測器B
//  Serial.println("color=");
//  Serial.println(color);
//  Serial.println("color=");
//  Serial.println(color);
//  Serial.println("color=");
//  Serial.println(color);
  // delay(3300);

  // 向左(至顏色區)
  Leftward(2, 1, 60, 255);    // 參考牆(左), 距離小於60停止, 速度255

  // 調整角度
  Reset_angle(80, 0, 1);   // 調整角度(速度, 感測器A, 感測器B)
  Reset_angle(80, 0, 1);   // 調整角度(速度, 感測器A, 感測器B)

//  Serial.println(color);
  if(color == 1){
    delay(500);
    Serial.println("aR");
    Reset_angle(90, 0, 1);   // 調整角度(速度, 感測器A, 感測器B) 
    Forward(0, 1, 20, 120);   // 參考牆(前), 距離小於25停止, 速度255
    Leftward(2, 1, 25, 200);    // 參考牆(左), 距離小於60停止, 速度255
    Reset_angle(80, 0, 1);   // 調整角度(速度, 感測器A, 感測器B)  
    Stop();
  }
  if(color == 2){
    Serial.println("aG");
    Forward(0, 1, 60, 200);
    Leftward(2, 1, 25, 255);    // 參考牆(左), 距離小於60停止, 速度255
    Reset_angle(80, 0, 1);   // 調整角度(速度, 感測器A, 感測器B) 
    Forward(0, 1, 120, 200);
  }
  if(color == 3){
    Serial.println("aB");
    Go_Blue();
    Leftward(2, 1, 25, 255);    // 參考牆(左), 距離小於60停止, 速度255
    Reset_angle(80, 3, 4);   // 調整角度(速度, 感測器A, 感測器B)  
  }
  
}


//========================= Forward =========================//
void Forward(int reference, int compare, int target, int spd){

  // 設定比較對象(使用哪顆感應器)
  int sensor_num = Select_sensor(reference);
  float dis = sr04(sensor_num);

  // set motor dir (01:CW, 10:CCW)
  Forward_IN();

  // 比較方法(大於或小於)
  int cnt = 0;
  if(compare==0){               // 大於
    while(dis < target || cnt < 10){
      dis = sr04(sensor_num);
      if(dis < target - 30){    // 離目標還有距離
        Forward_IN();
        SetSpeed_EN(spd);
      }else if(dis < target){   // 接近目標
        Forward_IN();
        SetSpeed_EN(spd * 0.5);
      }else{                    // 走過頭
        cnt++;
        Backward_IN();
        SetSpeed_EN(spd * 0.5);
      }
      //Serial.println(dis);
      delay(2);
    }
  }else{                        // 小於
    while(dis > target || cnt < 10){
      dis = sr04(sensor_num);
      if(dis > target + 30){    // 離目標還有距離
        Forward_IN();
        SetSpeed_EN(spd);
      }else if(dis > target){   // 接近目標
        Forward_IN();
        SetSpeed_EN(spd * 0.5);
      }else{                    // 走過頭
        cnt++;
        Backward_IN();
        SetSpeed_EN(spd * 0.5);
      }
      delay(2);
    }
  }
  Stop();
}


//========================= Backward =========================//
void Backward(int reference, int compare, int target, int spd){

  // 設定比較對象(使用哪顆感應器)
  int sensor_num = Select_sensor(reference);
  float dis = sr04(sensor_num);
  
  // set motor dir (01:CW, 10:CCW)
  Backward_IN();

  // 比較方法(大於或小於)
  int cnt = 0;
  if(compare==0){               // 大於
    while(dis < target || cnt < 10){
      dis = sr04(sensor_num);
      if(dis < target - 30){    // 離目標還有距離
        Backward_IN();
        SetSpeed_EN(spd);
      }else if(dis < target){   // 接近目標
        Backward_IN();
        SetSpeed_EN(spd * 0.5);
      }else{                    // 走過頭
        cnt++;
        Forward_IN();
        SetSpeed_EN(spd * 0.5);
      }
      delay(2);
    }
  }else{                        // 小於
    while(dis > target || cnt < 10){
      dis = sr04(sensor_num);
      if(dis > target + 30){    // 離目標還有距離
        Backward_IN();
        SetSpeed_EN(spd);
      }else if(dis > target){   // 接近目標
        Backward_IN();
        SetSpeed_EN(spd * 0.5);
      }else{                    // 走過頭
        cnt++;
        Forward_IN();
        SetSpeed_EN(spd * 0.5);
      }
//    Serial.println(dis);
      delay(2);
    }
  }
  Stop();
}


//========================= Leftward =========================//
void Leftward(int reference, int compare, int target, int spd){
  // 設定比較對象(使用哪顆感應器)
  int sensor_num = Select_sensor(reference);
  float dis = sr04(sensor_num);

  // set motor dir (01:CW, 10:CCW)
  Leftward_IN();

  // 比較方法(大於或小於)
  int cnt = 0;
  if(compare==0){               // 大於
    while(dis < target || cnt < 10){
      dis = sr04(sensor_num);
      if(dis < target - 30){    // 離目標還有距離
        Leftward_IN();
        SetSpeed_EN(spd);
      }else if(dis < target){   // 接近目標
        Leftward_IN();
        SetSpeed_EN(spd * 0.5);
      }else{                    // 走過頭
        cnt++;
        Rightward_IN();
        SetSpeed_EN(spd * 0.5);
      }
      delay(5);
    }
  }else{                        // 小於
    while(dis > target || cnt < 10){
      dis = sr04(sensor_num);
      if(dis > target + 30){    // 離目標還有距離
        Leftward_IN();
        SetSpeed_EN(spd);
      }else if(dis > target){   // 接近目標
        Leftward_IN();
        SetSpeed_EN(spd * 0.5);
      }else{                    // 走過頭
        cnt++;
        Rightward_IN();
        SetSpeed_EN(spd * 0.5);
      }
      delay(5);
    }
  }
  Stop();
}


//========================= Rightward =========================//
void Rightward(int reference, int compare, int target, int spd){
  // 設定比較對象(使用哪顆感應器)
  int sensor_num = Select_sensor(reference);
  float dis = sr04(sensor_num);

  // set motor dir (01:CW, 10:CCW)
  Rightward_IN();

  // 比較方法(大於或小於)
  int cnt = 0;
  if(compare==0){               // 大於
    while(dis < target || cnt < 10){
      dis = sr04(sensor_num);
      if(dis < target - 30){    // 離目標還有距離
        Rightward_IN();
        SetSpeed_EN(spd);
      }else if(dis < target){   // 接近目標
        Rightward_IN();
        SetSpeed_EN(spd * 0.5);
      }else{                    // 走過頭
        cnt++;
        Leftward_IN();
        SetSpeed_EN(spd * 0.5);
      }
    }
  }else{                        // 小於
    while(dis > target || cnt < 10){
      dis = sr04(sensor_num);
      if(dis > target + 30){    // 離目標還有距離
        Rightward_IN();
        SetSpeed_EN(spd);
      }else if(dis > target){   // 接近目標
        Rightward_IN();
        SetSpeed_EN(spd * 0.5);
      }else{                    // 走過頭
        cnt++;
        Leftward_IN();
        SetSpeed_EN(spd * 0.5);
      }
    }
  }
  Stop();
}


//========================= Stop =========================//
void Stop(){
  Stop_IN();
  SetSpeed_EN(0);
}


//========================= Reset Angle =========================//
void Reset_angle(int spd, int sensorA, int sensorB){
  unsigned long myTime;
  unsigned long TimeLag = 0;
  myTime = millis();
  float disA = sr04(sensorA);
  float disB = sr04(sensorB);
  float min = -0.5;
  float max = 0.5;
  // min = 0, max = 0;
  while((disA - disB) < min || (disA - disB) > max){
    if ((disA - disB) < min){       //超聲波A比較近
      Counterclockwise_IN();        //順時針轉
      SetSpeed_EN(spd);
      // delay(50);
      // Stop_IN();
    }else if((disA - disB) > max){  //超聲波B比較近
      Clockwise_IN();               //逆時針轉
      SetSpeed_EN(spd);
      // delay(50);
      // Stop_IN();
    }
    disA = sr04(sensorA);   
    disB = sr04(sensorB);
    TimeLag = millis() - myTime;
    if(TimeLag>5000){
      break;
    }
  }
  Stop();
}


//========================= Go Red =========================//
void Go_Red(){
  Forward(0, 1, 25, 255);   // 參考牆(前), 距離小於25停止, 速度255
  Forward(0, 1, 25, 255);   // 參考牆(前), 距離小於25停止, 速度255
//  Serial.print("Go_Red_GOGO");
}

//========================= Go Green =========================//
void Go_Green(){
  Backward(1, 1, 145, 255);   // 參考牆(後), 距離小於145停止, 速度255
}

//========================= Go Blue =========================//
void Go_Blue(){
  Backward(1, 1, 25, 255);   // 參考牆(後), 距離小於25停止, 速度255
}

//========================= Follow Line =========================//
void FollowLine(float spd){
  int sensor_num = Select_sensor(0);
  float dis = sr04(sensor_num);
  unsigned long StartTime;
  unsigned long TimeLag = 0;
  StartTime = millis();
  Serial.println("TT");
  Serial.println("TT");
  delay(200);
  Serial.println("TT");

  String str;
  

  int cnt=0;
  while(cnt<15){
    delay(3);
    dis = sr04(sensor_num);
    if(dis < 50 && ((millis()-StartTime)>7000)){
      cnt=cnt+1;
    }
    if (Serial.available()){
      str = Serial.readStringUntil('\n');
      if (str == "p"){
        Forward_IN();
        RightLineSpeed_EN(spd);
      }
      if (str == "n"){
        Forward_IN();
        LeftLineSpeed_EN(spd);
      }
      if (str == "o"){
        Forward_IN();
        SetSpeed_EN(spd);
      }
    }
  }
  Stop();
  Serial.println("SS");
  Serial.println("SS");
  delay(200);
  Serial.println("SS");
  Serial.println("SS");
  delay(200);
  Serial.println("SS");
  Serial.println("SS");
  delay(200);
  Serial.println("SS");
  Serial.println("SS");
  delay(500);
  Serial.println("SS");
  Serial.println("SS");
}


//========================= YOLO Detect =========================//
int YOLODetect(){
  String color;
  int color2=1;
  int cnt=0;
  Serial.println("YY");
  Serial.println("YY");
  delay(500);
  Serial.println("YY");
  Serial.println("YY");
  while(cnt<15){
  
    if (Serial.available()){
      color = Serial.readStringUntil('\n');
      if(color == "R"){
        color2=1;
        cnt=15;
      }else if(color == "G"){
        color2=2;
        cnt=15;
      }else if(color == "B"){
        color2=3;
        cnt=15;
      }else{
        color2=1;
        cnt++;
      }
    }
    delay(350);
  }

  if(color2== 1||color2== 2||color2== 3){
    return color2;
  }else{
    color=4;
    return color2;
  }
}

//========================= Reference Wall Setting =========================//
int Select_sensor(int reference){
  // 輸入參考面 輸出感應器編號
  // int sensor = 0;
  if(reference==0){
    return 1;
  }else if(reference==1){
    return 4;    
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
    // Serial.print("d = ");
    // Serial.println(d);
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

void Test_IN(){
  digitalWrite(MIN[0][0],LOW);
  digitalWrite(MIN[0][1],LOW);
  digitalWrite(MIN[1][0],LOW);
  digitalWrite(MIN[1][1],HIGH);
  digitalWrite(MIN[2][0],LOW);
  digitalWrite(MIN[2][1],LOW);
  digitalWrite(MIN[3][0],HIGH);
  digitalWrite(MIN[3][1],LOW);
}

//========================= EN Setting =========================//
void SetSpeed_EN(float spd){
  analogWrite(MEN[0],spd);
  analogWrite(MEN[1],spd*0.9);
  analogWrite(MEN[2],spd);
  analogWrite(MEN[3],spd*0.8);
}

void LeftLineSpeed_EN(float spd){
  analogWrite(MEN[0],spd);
  analogWrite(MEN[1],spd*0.15);
  analogWrite(MEN[2],spd);
  analogWrite(MEN[3],spd*0.15);
}

void RightLineSpeed_EN(float spd){
  analogWrite(MEN[0],spd*0.25);
  analogWrite(MEN[1],spd);
  analogWrite(MEN[2],spd*0.25);
  analogWrite(MEN[3],spd);
}
