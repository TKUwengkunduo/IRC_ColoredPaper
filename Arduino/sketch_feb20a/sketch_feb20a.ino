#define LED1 9
#define LED2 10
#define LED3 11
#define LED4 12
#define LED5 13

String str;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    // 讀取傳入的字串直到"\n"結尾
    str = Serial.readStringUntil('\n');

    if (str == "A_ON") {           // 若字串值是 "LED_ON" 開燈
        digitalWrite(LED1, HIGH);     // 開燈
        Serial.println("LED1 is ON"); // 回應訊息給電腦
    } else if (str == "B_ON") {
        digitalWrite(LED2, HIGH);
        Serial.println("LED2 is ON");
    } else if (str == "C_ON") {
        digitalWrite(LED3, HIGH);
        Serial.println("LED3 is ON");
    } else if (str == "D_ON") {
        digitalWrite(LED4, HIGH);
        Serial.println("LED4 is ON");
    } else if (str == "E_ON") {
        digitalWrite(LED5, HIGH);
        Serial.println("LED5 is ON");
    } else if (str == "OFF") {
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
        Serial.println("All LED is OFF");
    }
  }
}
