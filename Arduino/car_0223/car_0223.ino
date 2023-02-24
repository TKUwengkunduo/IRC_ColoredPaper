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
 *  |2|-0-----車頭-----1-|0|
 *   |                   |
 *   |                   |
 *   |                   |
 *   |                   |
 *   5                   2
 *   |                   |
 *   |                   |
 *   |                   |
 *   |                   |
 *  |3|-4-------------3-|1|
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
void Forward(int reference, int compare, int target, int spd);
void Backward(int reference, int compare, int target, int spd);
void Leftward(int reference, int compare, int target, int spd);
void Rightward(int reference, int compare, int target, int spd);
void Stop();
void Reset_angle(int reference, int spd);
void Reset_dis(int reference, int target, int spd);

// 感應器
int Select_sensor(int reference);
unsigned long sr04(int num);



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
  
  while(Serial.read()!='1'){} //串口監視器輸入1以開始

  // 後退(A點至B點)
  Serial.println("Backward Start");
  Backward(1, 1, 25, 255); // 參考牆(後), 距離小於25停止, 速度255
  Serial.println("Backward End");
  delay(2000);

  // 調整角度
  Serial.println("Reset Angle Start");
  Reset_angle(150);
  Serial.println("Reset Angle End");
  delay(1000);

  // 前進(至AB中點)
  Serial.println("Forward Start");
  Forward(1, 0, 130, 255);   // 參考牆(後), 距離大於130停止, 速度255
  Serial.println("Forward End");
  delay(2000);

  // 向左(至中間牆)
  Serial.println("Leftward Start");
  Leftward(2, 1, 25, 255);     // 參考牆(左), 距離小於25停止, 速度255
  Serial.println("Leftward End");
  delay(2000);

  // 後退(至C點右側)
  Serial.println("Backward Start");
  Backward(1, 1, 50, 255);  // 參考牆(後), 距離小於40停止, 速度255
  Serial.println("Backward End");
  delay(2000);

  // 調整角度
  Serial.println("Reset Angle Start");
  Reset_angle(150);
  Serial.println("Reset Angle End");
  delay(1000);

  // 向左(至C點)
  Serial.println("Leftward Start");
  Leftward(3, 0, 160, 200);    // 參考牆(右), 距離大於160停止, 速度255
  Serial.println("Leftward End");
  delay(2000);

  // 調整角度
  Serial.println("Reset Angle Start");
  Reset_angle(150);
  Serial.println("Reset Angle End");
  delay(1000);

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
    while(dis >= target || cnt < 3){
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
    while(dis >= target || cnt < 3){
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
    while(dis >= target || cnt < 3){
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
    while(dis >= target || cnt < 3){
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
}


//========================= Stop =========================//
void Stop(){
  Stop_IN();
  SetSpeed_EN(0);
}


//========================= Reset Angle =========================//
void Reset_angle(int spd){
  float dis3 = sr04(3);
  float dis4 = sr04(4);
  float min = -0.5;
  float max = 0.5;
  while((dis3-dis4) < min || (dis3-dis4) > max){
    dis3 = sr04(3);
    dis4 = sr04(4);
    Serial.print(dis3);
    Serial.print(",");
    Serial.println(dis4);
    if ((dis3-dis4) < min){         //超聲波4比較近
      Counterclockwise_IN();        //順時針轉
      SetSpeed_EN(spd);
    }else if((dis3-dis4) > max){    //超聲波3比較近
      Clockwise_IN();               //逆時針轉
      SetSpeed_EN(spd);
    }
  }
  Stop();
}


void Reset_dis(int reference, int target, int spd){
  
}



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
  digitalWrite(MIN[0][0],HIGH);
  digitalWrite(MIN[0][1],LOW);
  digitalWrite(MIN[1][0],LOW);
  digitalWrite(MIN[1][1],HIGH);
  digitalWrite(MIN[2][0],LOW);
  digitalWrite(MIN[2][1],HIGH);
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
