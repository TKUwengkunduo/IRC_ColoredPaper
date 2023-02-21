
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
 *  |3|-0-------車頭-------1-|1|
 *   |                        |
 *   |                        |
 *   |                        |
 *   |                        |
 *   5                        2
 *   |                        |
 *   |                        |
 *   |                        |
 *   |                        |
 *  |4|--4-----------------3-|2|
 *  
 *                            ENA  IN1  IN2  IN3  IN4  ENB
 *  L298N[1](1號馬達、2號馬達):   9   13   12   11   10    8
 *  L298N[2](3號馬達、4號馬達):   3    7    6    5    4    2
 */


//=========================== 馬達腳位定義 ===========================//
const int In1[2] = {13,7};
const int In2[2] = {12,6};
const int In3[2] = {11,5};      
const int In4[2] = {10,4};  
const int EVA[2] = {9,3};
const int EVB[2] = {8,2};    

//=========================== 超音波腳位定義 ===========================//
const byte trigPin[6] = {29,33,37,41,45,49};
const byte echoPin[6] = {31,35,39,43,47,51};







//=========================== 函式定義 ===========================//
// 基本動作
// reference=參考牆(0=前,1=後,2=左,3=右)
// compare=比較方法(0=大於,1=小於)
// target=目標距離(數值)
// s=速度(0~255)
void Forward(int reference, int compare, int target, int s);
void Back(int reference, int compare, int target, int s);
void Left(int reference, int compare, int target, int s);
void Right(int reference, int compare, int target, int s);
void Stop();
void Reset_angle(int reference, int s);
void Reset_distance(int reference, int target, int s);

// 感應器
int Select_sensor(int reference);
unsigned long sr04(int num);









//=========================== setup ===========================//
void setup(){
  // 設定通訊鮑率
  Serial.begin(9600);

  // 設定馬達控制板腳位
  for(int i=0;i<2;i++){
    pinMode(In1[i], OUTPUT);
    pinMode(In2[i], OUTPUT);
    pinMode(In3[i], OUTPUT);
    pinMode(In4[i], OUTPUT);    
    pinMode(EVA[i], OUTPUT);
    pinMode(EVB[i], OUTPUT);
  } 

  // 設定超音波感應器腳位
  for(int i=0;i<6;i++){
    pinMode(trigPin[i], OUTPUT);
    pinMode(echoPin[i], INPUT);
  }
}








//=========================== loop ===========================//
void loop(){
  int FLAG=0;
  int i=0;
  

  while(Serial.read()!='1'){}

  // 前進
  Serial.println("Forward Start");
  Forward(1, 1, 25, 255); // 參考牆(後), 距離小於, 25, 速度200
  Serial.println("Forward end");
  delay(2000);

  // 後退
  Serial.println("Back Start");
  Back(1, 0, 130, 255); // 參考牆(後), 距離大於, 20, 速度200
  Serial.println("Back end");
  delay(2000);

  // 向左
  Serial.println("Left Start");
  Left(2, 1, 25, 255); // 參考牆(左), 距離小於, 25, 速度200
  Serial.println("Left end");
  delay(2000);

  // 前進
  Serial.println("Forward Start");
  Forward(1, 1, 40, 255); // 參考牆(後), 距離小於, 40, 速度200
  Serial.println("Forward end");
  delay(2000);

  // 向左
  Serial.println("Left Start");
  Left(3, 1, 160, 255); // 參考牆(右), 距離大於, 160, 速度200
  Serial.println("Left end");
  delay(2000);
  
//前進
//   float d = sr04(4) * 0.017;
//   while(d>=20){
//     d = sr04(4) * 0.017;
//     Serial.println(d);
//     delay(1);
//     Forward(d);
//   }
//   Serial.println(" ");
//   Stop();
//   delay(3000);

//   int count = 3000;
//   int target = 50;
//   int sensor = d;
//   int speed;
//   for(int i=0;i<count;i++){
//     speed = (sensor - target)*20;
//     delay(1);
//   }

// //後退
//     d = sr04(4) * 0.017;
//     while(d<=130){
//       d = sr04(4) * 0.017;
//       Serial.println(d);
//       delay(1);
//       Back();
//     }
//     Serial.println(" ");
//     Stop();
// //    delay(3000);
// sensor = d;
// for(int i=0;i<count;i++){
//     speed = (sensor - target)*20;
//     delay(1);
//   }
// //右邊
//     d = sr04(5) * 0.017;
//     while(d>=20){
//       d = sr04(5) * 0.017;
//       Serial.println(d);
//       delay(1);
//       Right(d);
//     }
//     Serial.println(" ");
//     Stop();
// //    delay(3000);
//     sensor = d;
//   for(int i=0;i<count;i++){
//     speed = (sensor - target)*20;
//     delay(1);
    
//   }
//   //前進
//   d = sr04(4) * 0.017;
//   while(d>=65){
//     d = sr04(4) * 0.017;
//     Serial.println(d);
//     delay(1);
//     Forward(d);
//   }
//   Serial.println(" ");
//   Stop();
//  // delay(3000);

//   sensor = d;
//   for(int i=0;i<count;i++){
//     speed = (sensor - target)*20;
//     delay(1);
//   }
//   //右邊
//     d = sr04(2) * 0.017;
//     while(d<=160){
//       d = sr04(2) * 0.017;
//       Serial.println(d);
//       delay(1);
//       Right(d);
//     }
//     Serial.println(" ");
//     Stop();
//     delay(3000);
//     sensor = d;
//   for(int i=0;i<count;i++){
//     speed = (sensor - target)*20;
//     delay(1);
    
//   }
}






//=========================== Forward ===========================//
void Forward(int reference, int compare, int target, int s){

  // 設定比較對象(使用哪顆感應器)
  int sensor_num = Select_sensor(reference);
  float distance = sr04(sensor_num);

  // set motor dir
  digitalWrite(In1[1],LOW);
  digitalWrite(In2[1],HIGH);
  digitalWrite(In3[1],HIGH);
  digitalWrite(In4[1],LOW);
  digitalWrite(In1[0],HIGH);
  digitalWrite(In2[0],LOW);
  digitalWrite(In3[0],LOW);
  digitalWrite(In4[0],HIGH);

  // 比較方法(大於或小於)
  if(compare==0){
    while(distance<=target){
      distance = sr04(sensor_num);
      if(distance<target-30){
        analogWrite(EVA[0],s);
        analogWrite(EVB[0],s);
        analogWrite(EVA[1],s);
        analogWrite(EVB[1],s);
      }else{
        analogWrite(EVA[0],s*0.5);
        analogWrite(EVB[0],s*0.5);
        analogWrite(EVA[1],s*0.5);
        analogWrite(EVB[1],s*0.5);
      }
    }
  }else{
    while(distance>=target){
      distance = sr04(sensor_num);
      if(distance>target+30){
        analogWrite(EVA[0],s);
        analogWrite(EVB[0],s);
        analogWrite(EVA[1],s);
        analogWrite(EVB[1],s);
      }else{
        analogWrite(EVA[0],s*0.5);
        analogWrite(EVB[0],s*0.5);
        analogWrite(EVA[1],s*0.5);
        analogWrite(EVB[1],s*0.5);
      }
    }
  }
  Stop();
}




//=========================== Back ===========================//
void Back(int reference, int compare, int target, int s){

  // 設定比較對象(使用哪顆感應器)
  int sensor_num = Select_sensor(reference);
  float distance = sr04(sensor_num);

  // set motor dir
  digitalWrite(In1[1],HIGH);
  digitalWrite(In2[1],LOW);
  digitalWrite(In3[1],LOW);
  digitalWrite(In4[1],HIGH);
  digitalWrite(In1[0],LOW);
  digitalWrite(In2[0],HIGH);
  digitalWrite(In3[0],HIGH);
  digitalWrite(In4[0],LOW);

  // 比較方法(大於或小於)
  if(compare==0){
    while(distance<=target){
      distance = sr04(sensor_num);
      if(distance<target-30){
        analogWrite(EVA[0],s);
        analogWrite(EVB[0],s);
        analogWrite(EVA[1],s);
        analogWrite(EVB[1],s);
      }else{
        analogWrite(EVA[0],s*0.5);
        analogWrite(EVB[0],s*0.5);
        analogWrite(EVA[1],s*0.5);
        analogWrite(EVB[1],s*0.5);
      }
    }
  }else{
    while(distance>=target){
      distance = sr04(sensor_num);
      if(distance>target+30){
        analogWrite(EVA[0],s);
        analogWrite(EVB[0],s);
        analogWrite(EVA[1],s);
        analogWrite(EVB[1],s);
      }else{
        analogWrite(EVA[0],s*0.5);
        analogWrite(EVB[0],s*0.5);
        analogWrite(EVA[1],s*0.5);
        analogWrite(EVB[1],s*0.5);
      }
    }
  }
  Stop();
}


//=========================== Left ===========================//
void Left(int reference, int compare, int target, int s){
  // 設定比較對象(使用哪顆感應器)
  int sensor_num = Select_sensor(reference);
  float distance = sr04(sensor_num);

  // set motor dir
  digitalWrite(In1[1],HIGH);
  digitalWrite(In2[1],LOW);
  digitalWrite(In3[1],HIGH);
  digitalWrite(In4[1],LOW);
  digitalWrite(In1[0],LOW);
  digitalWrite(In2[0],HIGH);
  digitalWrite(In3[0],LOW);
  digitalWrite(In4[0],HIGH);

  // 比較方法(大於或小於)
  if(compare==0){
    while(distance<=target){
      distance = sr04(sensor_num);
      if(distance<target-30){
        analogWrite(EVA[0],s);
        analogWrite(EVB[0],s);
        analogWrite(EVA[1],s);
        analogWrite(EVB[1],s);
      }else{
        analogWrite(EVA[0],s*0.5);
        analogWrite(EVB[0],s*0.5);
        analogWrite(EVA[1],s*0.5);
        analogWrite(EVB[1],s*0.5);
      }
    }
  }else{
    while(distance>=target){
      distance = sr04(sensor_num);
      if(distance>target+30){
        analogWrite(EVA[0],s);
        analogWrite(EVB[0],s);
        analogWrite(EVA[1],s);
        analogWrite(EVB[1],s);
      }else{
        analogWrite(EVA[0],s*0.5);
        analogWrite(EVB[0],s*0.5);
        analogWrite(EVA[1],s*0.5);
        analogWrite(EVB[1],s*0.5);
      }
    }
  }
  Stop();
}


//=========================== Right ===========================//
void Right(int reference, int compare, int target, int s){
  // 設定比較對象(使用哪顆感應器)
  int sensor_num = Select_sensor(reference);
  float distance = sr04(sensor_num);

  // set motor dir
  digitalWrite(In1[1],LOW);
  digitalWrite(In2[1],HIGH);
  digitalWrite(In3[1],LOW);
  digitalWrite(In4[1],HIGH);
  digitalWrite(In1[0],HIGH);
  digitalWrite(In2[0],LOW);
  digitalWrite(In3[0],HIGH);
  digitalWrite(In4[0],LOW);

  // 比較方法(大於或小於)
  if(compare==0){
    while(distance<=target){
      distance = sr04(sensor_num);
      if(distance<target-30){
        analogWrite(EVA[0],s);
        analogWrite(EVB[0],s);
        analogWrite(EVA[1],s);
        analogWrite(EVB[1],s);
      }else{
        analogWrite(EVA[0],s*0.5);
        analogWrite(EVB[0],s*0.5);
        analogWrite(EVA[1],s*0.5);
        analogWrite(EVB[1],s*0.5);
      }
    }
  }else{
    while(distance>=target){
      distance = sr04(sensor_num);
      if(distance>target+30){
        analogWrite(EVA[0],s);
        analogWrite(EVB[0],s);
        analogWrite(EVA[1],s);
        analogWrite(EVB[1],s);
      }else{
        analogWrite(EVA[0],s*0.5);
        analogWrite(EVB[0],s*0.5);
        analogWrite(EVA[1],s*0.5);
        analogWrite(EVB[1],s*0.5);
      }
    }
  }
  Stop();
}


//=========================== Stop ===========================//
void Stop(){
  digitalWrite(In1[0],HIGH);
  digitalWrite(In2[0],HIGH);
  digitalWrite(In3[0],HIGH);
  digitalWrite(In4[0],HIGH);
  digitalWrite(In1[1],HIGH);
  digitalWrite(In2[1],HIGH);
  digitalWrite(In3[1],HIGH);
  digitalWrite(In4[1],HIGH);
  analogWrite(EVA[0],0);
  analogWrite(EVB[0],0);
  analogWrite(EVA[1],0);
  analogWrite(EVB[1],0);
}


void Reset_angle(int reference, int s){
  
}

void Reset_distance(int reference, int target, int s){
  
}



int Select_sensor(int reference){
  // 輸入參考面 輸出感應器編號
  int sensor = 0;
  if(reference==0){
    return 1;
  }else if(reference==1){
    return 3;    
  }else if(reference==1){
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










//void Stop(){
//  digitalWrite(In1[0],HIGH);
//  digitalWrite(In2[0],HIGH);
//  digitalWrite(In3[0],HIGH);
//  digitalWrite(In4[0],HIGH);
//  digitalWrite(In1[1],HIGH);
//  digitalWrite(In2[1],HIGH);
//  digitalWrite(In3[1],HIGH);
//  digitalWrite(In4[1],HIGH);
//  analogWrite(EVA[0],0);
//  analogWrite(EVB[0],0);
//  analogWrite(EVA[1],0);
//  analogWrite(EVB[1],0);
//}
//
//
//void Front(int d){
//  if(d>70){
//    analogWrite(EVA[0],255);
//    analogWrite(EVB[0],255);
//    analogWrite(EVA[1],255);
//    analogWrite(EVB[1],255);
//  }
//  if(d<=70){
//    analogWrite(EVA[0],150);
//    analogWrite(EVB[0],150);
//    analogWrite(EVA[1],150);
//    analogWrite(EVB[1],150);
//  }
//  digitalWrite(In1[0],LOW);
//  digitalWrite(In2[0],HIGH);
//  digitalWrite(In3[0],HIGH);
//  digitalWrite(In4[0],LOW);
//  digitalWrite(In1[1],HIGH);
//  digitalWrite(In2[1],LOW);
//  digitalWrite(In3[1],LOW);
//  digitalWrite(In4[1],HIGH);
//  
//}
//
//void Back(){
//  analogWrite(EVA[0],255);
//  analogWrite(EVB[0],255);
//  analogWrite(EVA[1],255);
//  analogWrite(EVB[1],255);
//  
//  digitalWrite(In1[0],HIGH);
//  digitalWrite(In2[0],LOW);
//  digitalWrite(In3[0],LOW);
//  digitalWrite(In4[0],HIGH);
//  digitalWrite(In1[1],LOW);
//  digitalWrite(In2[1],HIGH);
//  digitalWrite(In3[1],HIGH);
//  digitalWrite(In4[1],LOW);
//  
//}
//
//void Right(int d){
//  if(d>50){
//    analogWrite(EVA[0],255);
//    analogWrite(EVB[0],255);
//    analogWrite(EVA[1],255);
//    analogWrite(EVB[1],255);
//  }
//  if(d<=50){
//    analogWrite(EVA[0],150);
//    analogWrite(EVB[0],150);
//    analogWrite(EVA[1],150);
//    analogWrite(EVB[1],150);
//  }
//  digitalWrite(In1[0],LOW);
//  digitalWrite(In2[0],HIGH);
//  digitalWrite(In3[0],LOW);
//  digitalWrite(In4[0],HIGH);
//  digitalWrite(In1[1],HIGH);
//  digitalWrite(In2[1],LOW);
//  digitalWrite(In3[1],HIGH);
//  digitalWrite(In4[1],LOW);
//  
//}
