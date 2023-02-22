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


//=========================== 馬達腳位定義 ===========================//
//// IN:方向 EN:速度
const int MEN[4] = {9, 8, 3, 2};
const int MIN[4][2] = {{13,12},{11,10},{7,6},{5,4}};


//=========================== 超音波腳位定義 ===========================//
const byte trigPin[6] = {29,33,37,41,45,49};
const byte echoPin[6] = {31,35,39,43,47,51};


//=========================== 函式定義 ===========================//
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



//=========================== setup ===========================//
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


//=========================== loop ===========================//
void loop(){
  int FLAG=0;
  int i=0;
  
  while(Serial.read()!='1'){} //串口監視器輸入1以開始
/*
  // 後退(A點至B點)
  Serial.println("Backward Start");
  Backward(1, 1, 25, 255); // 參考牆(後), 距離小於25停止, 速度255
  Serial.println("Backward end");
  delay(2000);

  // 前進(至AB中點)
  Serial.println("Forward Start");
  Forward(1, 0, 130, 255);   // 參考牆(後), 距離大於130停止, 速度255
  Serial.println("Forward end");
  delay(2000);

  // 向左(至中間牆)
  Serial.println("Leftward Start");
  Leftward(2, 1, 25, 255);     // 參考牆(左), 距離小於25停止, 速度255
  Serial.println("Leftward end");
  delay(2000);

  // 後退(至C點右側)
  Serial.println("Backward Start");
  Backward(1, 1, 50, 255);  // 參考牆(後), 距離小於40停止, 速度255
  Serial.println("Backward end");
  delay(2000);
*/
  // 向左(至C點)
  Serial.println("Leftward Start");
  Leftward(3, 0, 160, 200);    // 參考牆(右), 距離大於160停止, 速度255
  Serial.println("Leftward end");
  delay(2000);
}



//=========================== Forward ===========================//
void Forward(int reference, int compare, int target, int spd){

  // 設定比較對象(使用哪顆感應器)
  int sensor_num = Select_sensor(reference);
  float dis = sr04(sensor_num);


  // set motor dir (01:CW, 10:CCW)
  digitalWrite(MIN[0][0],LOW);
  digitalWrite(MIN[0][1],HIGH);
  digitalWrite(MIN[1][0],LOW);
  digitalWrite(MIN[1][1],HIGH);
  digitalWrite(MIN[2][0],HIGH);
  digitalWrite(MIN[2][1],LOW);
  digitalWrite(MIN[3][0],HIGH);
  digitalWrite(MIN[3][1],LOW);

  // 比較方法(大於或小於)
  if(compare==0){
    while(dis <= target){
      dis = sr04(sensor_num);
      if(dis < target-30){
        analogWrite(MEN[0],spd);
        analogWrite(MEN[1],spd);
        analogWrite(MEN[2],spd);
        analogWrite(MEN[3],spd);
      }else{
        analogWrite(MEN[0],spd*0.5);
        analogWrite(MEN[1],spd*0.5);
        analogWrite(MEN[2],spd*0.5);
        analogWrite(MEN[3],spd*0.5);
      }
    }
  }else{
    while(dis >= target){
      dis = sr04(sensor_num);
      if(dis>target+30){
        analogWrite(MEN[0],spd);
        analogWrite(MEN[1],spd);
        analogWrite(MEN[2],spd);
        analogWrite(MEN[3],spd);
      }else{
        analogWrite(MEN[0],spd*0.5);
        analogWrite(MEN[1],spd*0.5);
        analogWrite(MEN[2],spd*0.5);
        analogWrite(MEN[3],spd*0.5);
      }
    }
  }
  Stop();
}


//=========================== Backward ===========================//
void Backward(int reference, int compare, int target, int spd){

  // 設定比較對象(使用哪顆感應器)
  int sensor_num = Select_sensor(reference);
  float dis = sr04(sensor_num);

  // set motor dir (01:CW, 10:CCW)
  digitalWrite(MIN[0][0],HIGH);
  digitalWrite(MIN[0][1],LOW);
  digitalWrite(MIN[1][0],HIGH);
  digitalWrite(MIN[1][1],LOW);
  digitalWrite(MIN[2][0],LOW);
  digitalWrite(MIN[2][1],HIGH);
  digitalWrite(MIN[3][0],LOW);
  digitalWrite(MIN[3][1],HIGH);

  // 比較方法(大於或小於)
  if(compare==0){
    while(dis <= target){
      dis = sr04(sensor_num);
      if(dis < target-30){
        analogWrite(MEN[0],spd);
        analogWrite(MEN[1],spd);
        analogWrite(MEN[2],spd);
        analogWrite(MEN[3],spd);
      }else{
        analogWrite(MEN[0],spd*0.5);
        analogWrite(MEN[1],spd*0.5);
        analogWrite(MEN[2],spd*0.5);
        analogWrite(MEN[3],spd*0.5);
      }
    }
  }else{
    while(dis >= target){
      dis = sr04(sensor_num);
      if(dis>target+30){
        analogWrite(MEN[0],spd);
        analogWrite(MEN[1],spd);
        analogWrite(MEN[2],spd);
        analogWrite(MEN[3],spd);
      }else{
        analogWrite(MEN[0],spd*0.5);
        analogWrite(MEN[1],spd*0.5);
        analogWrite(MEN[2],spd*0.5);
        analogWrite(MEN[3],spd*0.5);
      }
    }
  }
  Stop();
}


//=========================== Leftward ===========================//
void Leftward(int reference, int compare, int target, int spd){
  // 設定比較對象(使用哪顆感應器)
  int sensor_num = Select_sensor(reference);
  float dis = sr04(sensor_num);

  // set motor dir (01:CW, 10:CCW)
  digitalWrite(MIN[0][0],LOW);
  digitalWrite(MIN[0][1],HIGH);
  digitalWrite(MIN[1][0],HIGH);
  digitalWrite(MIN[1][1],LOW);
  digitalWrite(MIN[2][0],HIGH);
  digitalWrite(MIN[2][1],LOW);
  digitalWrite(MIN[3][0],LOW);
  digitalWrite(MIN[3][1],HIGH);

  // 比較方法(大於或小於)
  if(compare==0){
    while(dis <= target){
      dis = sr04(sensor_num);
      if(dis < target-30){
        analogWrite(MEN[0],spd);
        analogWrite(MEN[1],spd);
        analogWrite(MEN[2],spd);
        analogWrite(MEN[3],spd);
      }else{
        analogWrite(MEN[0],spd*0.5);
        analogWrite(MEN[1],spd*0.5);
        analogWrite(MEN[2],spd*0.5);
        analogWrite(MEN[3],spd*0.5);
      }
    }
  }else{
    while(dis >= target){
      dis = sr04(sensor_num);
      if(dis>target+30){
        analogWrite(MEN[0],spd);
        analogWrite(MEN[1],spd);
        analogWrite(MEN[2],spd);
        analogWrite(MEN[3],spd);
      }else{
        analogWrite(MEN[0],spd*0.5);
        analogWrite(MEN[1],spd*0.5);
        analogWrite(MEN[2],spd*0.5);
        analogWrite(MEN[3],spd*0.5);
      }
    }
  }
  Stop();
}


//=========================== Rightward ===========================//
void Rightward(int reference, int compare, int target, int spd){
  // 設定比較對象(使用哪顆感應器)
  int sensor_num = Select_sensor(reference);
  float dis = sr04(sensor_num);

  // set motor dir (01:CW, 10:CCW)
  digitalWrite(MIN[0][0],HIGH);
  digitalWrite(MIN[0][1],LOW);
  digitalWrite(MIN[1][0],LOW);
  digitalWrite(MIN[1][1],HIGH);
  digitalWrite(MIN[2][0],LOW);
  digitalWrite(MIN[2][1],HIGH);
  digitalWrite(MIN[3][0],HIGH);
  digitalWrite(MIN[3][1],LOW);

  // 比較方法(大於或小於)
  if(compare==0){
    while(dis <= target){
      dis = sr04(sensor_num);
      if(dis < target-30){
        analogWrite(MEN[0],spd);
        analogWrite(MEN[1],spd);
        analogWrite(MEN[2],spd);
        analogWrite(MEN[3],spd);
      }else{
        analogWrite(MEN[0],spd*0.5);
        analogWrite(MEN[1],spd*0.5);
        analogWrite(MEN[2],spd*0.5);
        analogWrite(MEN[3],spd*0.5);
      }
    }
  }else{
    while(dis >= target){
      dis = sr04(sensor_num);
      if(dis>target+30){
        analogWrite(MEN[0],spd);
        analogWrite(MEN[1],spd);
        analogWrite(MEN[2],spd);
        analogWrite(MEN[3],spd);
      }else{
        analogWrite(MEN[0],spd*0.5);
        analogWrite(MEN[1],spd*0.5);
        analogWrite(MEN[2],spd*0.5);
        analogWrite(MEN[3],spd*0.5);
      }
    }
  }
  Stop();
}


//=========================== Stop ===========================//
void Stop(){
  digitalWrite(MIN[0][0],LOW);
  digitalWrite(MIN[0][1],LOW);
  digitalWrite(MIN[1][0],LOW);
  digitalWrite(MIN[1][1],LOW);
  digitalWrite(MIN[2][0],LOW);
  digitalWrite(MIN[2][1],LOW);
  digitalWrite(MIN[3][0],LOW);
  digitalWrite(MIN[3][1],LOW);
  analogWrite(MEN[0],0);
  analogWrite(MEN[1],0);
  analogWrite(MEN[2],0);
  analogWrite(MEN[3],0);
}


//=========================== Reset Angle ===========================//
void Reset_angle(int reference, int spd_turn){
  float dis3 = sr04(3);
  float dis4 = sr04(4);
  int min = -10;
  int max = 10;
  while((dis3-dis4) < min || (dis3-dis4) > max){
    if ((dis3-dis4) < min){         //超聲波4比較近
      digitalWrite(MIN[0][0],HIGH);  //順時針轉
      digitalWrite(MIN[0][1],LOW);
      digitalWrite(MIN[1][0],HIGH);
      digitalWrite(MIN[1][1],LOW);
      digitalWrite(MIN[2][0],HIGH);
      digitalWrite(MIN[2][1],LOW);
      digitalWrite(MIN[3][0],HIGH);
      digitalWrite(MIN[3][1],LOW);
      analogWrite(MEN[0],spd_turn);
      analogWrite(MEN[1],spd_turn);
      analogWrite(MEN[2],spd_turn);
      analogWrite(MEN[3],spd_turn);
    }else if((dis3-dis4) > max){      //超聲波3比較近
      digitalWrite(MIN[0][0],LOW);    //逆時針轉
      digitalWrite(MIN[0][1],HIGH);
      digitalWrite(MIN[1][0],LOW);
      digitalWrite(MIN[1][1],HIGH);
      digitalWrite(MIN[2][0],LOW);
      digitalWrite(MIN[2][1],HIGH);
      digitalWrite(MIN[3][0],LOW);
      digitalWrite(MIN[3][1],HIGH);
      analogWrite(MEN[0],spd_turn);
      analogWrite(MEN[1],spd_turn);
      analogWrite(MEN[2],spd_turn);
      analogWrite(MEN[3],spd_turn);
    }
  }
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

  Serial.print(num);
  Serial.print(": ");
  Serial.print(d,0);
  Serial.println("cm ");
  
  return d;
}
