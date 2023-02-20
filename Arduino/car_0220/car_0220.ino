
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
 *  |3|------|1|
 *   |        |
 *   |        |
 *   |        |
 *  |4|------|2|
 *  
 *                            ENA   ENB   IN1   IN2   IN3   IN4 
 *  L298N[1](1號馬達、2號馬達):  2     3     4     5     6     7
 *  L298N[2](3號馬達、4號馬達):  8     9    10    11    12    13
 */


const int In1[2] = {4,10};
const int In2[2] = {5,11};
const int In3[2] = {6,12};      
const int In4[2] = {7,13};  
const int EVA[2] = {2,8};
const int EVB[2] = {3,9};    

const byte trigPin[6] = {29,33,37,41,45,49};
const byte echoPin[6] = {31,35,39,43,47,51};


void setup(){
  for(int i=0;i<2;i++){
    pinMode(In1[i], OUTPUT);
    pinMode(In2[i], OUTPUT);
    pinMode(In3[i], OUTPUT);
    pinMode(In4[i], OUTPUT);    
    pinMode(EVA[i], OUTPUT);
    pinMode(EVB[i], OUTPUT);
  } 
  int i=0;
  for(i=0;i<6;i++){
    pinMode(trigPin[i], OUTPUT);
    pinMode(echoPin[i], INPUT);
  }
  Serial.begin(9600);
}
void loop(){
  int FLAG=0;
  int i=0;
  for(i=0;i<6;i++){
    float d = sr04(i) * 0.017; 
    Serial.print(i+1);
    Serial.print(": ");
    Serial.print(d,0);
    Serial.print("cm ");
    delay(1);
  }

//前進
  float d = sr04(4) * 0.017;
  while(d>=20){
    d = sr04(4) * 0.017;
    Serial.println(d);
    delay(1);
    Front(d);
  }
  Serial.println(" ");
  Stop();
  delay(30000);

  int count = 3000;
  int target = 50;
  int sensor = d;
  int speed;
  for(int i=0;i<count;i++){
    speed = (sensor - target)*20;
    delay(1);
  }

//後退
    d = sr04(4) * 0.017;
    while(d<=130){
      d = sr04(4) * 0.017;
      Serial.println(d);
      delay(1);
      Back();
    }
    Serial.println(" ");
    Stop();
    delay(30000);
sensor = d;
for(int i=0;i<count;i++){
    speed = (sensor - target)*20;
    delay(1);
  }
//右邊
    d = sr04(5) * 0.017;
    while(d>=30){
      d = sr04(5) * 0.017;
      Serial.println(d);
      delay(1);
      Right();
    }
    Serial.println(" ");
    Stop();
    delay(2000);
    sensor = d;
  for(int i=0;i<count;i++){
    speed = (sensor - target)*20;
    delay(1);
    
  }
}

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


void Front(int d){
  if(d>50){
    analogWrite(EVA[0],255);
    analogWrite(EVB[0],255);
    analogWrite(EVA[1],255);
    analogWrite(EVB[1],255);
  }
  if(d<=50){
    analogWrite(EVA[0],150);
    analogWrite(EVB[0],150);
    analogWrite(EVA[1],150);
    analogWrite(EVB[1],150);
  }
  digitalWrite(In1[0],LOW);
  digitalWrite(In2[0],HIGH);
  digitalWrite(In3[0],HIGH);
  digitalWrite(In4[0],LOW);
  digitalWrite(In1[1],HIGH);
  digitalWrite(In2[1],LOW);
  digitalWrite(In3[1],LOW);
  digitalWrite(In4[1],HIGH);
  
}

void Back(){
  analogWrite(EVA[0],255);
  analogWrite(EVB[0],225);
  analogWrite(EVA[1],225);
  analogWrite(EVB[1],225);
  
  digitalWrite(In1[0],HIGH);
  digitalWrite(In2[0],LOW);
  digitalWrite(In3[0],LOW);
  digitalWrite(In4[0],HIGH);
  digitalWrite(In1[1],LOW);
  digitalWrite(In2[1],HIGH);
  digitalWrite(In3[1],HIGH);
  digitalWrite(In4[1],LOW);
  
}

void Right(){
    analogWrite(EVA[0],255);
    analogWrite(EVB[0],255);
    analogWrite(EVA[1],255);
    analogWrite(EVB[1],255);
    
  digitalWrite(In1[0],LOW);
  digitalWrite(In2[0],HIGH);
  digitalWrite(In3[0],LOW);
  digitalWrite(In4[0],HIGH);
  digitalWrite(In1[1],HIGH);
  digitalWrite(In2[1],LOW);
  digitalWrite(In3[1],HIGH);
  digitalWrite(In4[1],LOW);
  
}
unsigned long sr04(int num) { 
  digitalWrite(trigPin[num], HIGH);
  delayMicroseconds(10);  
  digitalWrite(trigPin[num], LOW);
  return pulseIn(echoPin[num], HIGH);
}
