#include <ESP32Servo.h>

Servo myservo1;          // Servoオブジェクトの宣言
Servo myservo2;
const int SV_PIN1 = 25;   // サーボモーターをデジタルピン設定
const int SV_PIN2 = 26;
int servo_angle1 = 0; //サーボ角度用変数（電源ON時にゼロ度に初期化）
int servo_angle2 = 0;
const int motorPin1 = 32;
const int motorPin2 = 33;
const int motorPin3 = 18;
const int motorPin4 = 19;

void setup() {
  Serial.begin(115200);
  myservo1.attach(SV_PIN1, 500, 2400);  // サーボの割当(パルス幅500~2400msに指定)
  myservo1.write(servo_angle1);//サーボ角度を0度にリセット
  myservo2.attach(SV_PIN2, 500, 2400);
  myservo2.write(servo_angle2);

  // モーターピンを出力として設定
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  //pinMode(enablePin, OUTPUT);

  // モーターを停止状態に設定
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 0);
  //analogWrite(enablePin, 0);  // モーターのスピードを0に設定
  delay(100);
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    int firstDelimiterIndex = data.indexOf(':');

    if (firstDelimiterIndex > 0) {
      String command = data.substring(0, firstDelimiterIndex);
      int secondDelimiterIndex = data.indexOf(':', firstDelimiterIndex + 1);

      if (secondDelimiterIndex > 0) {
        int firstValue = data.substring(firstDelimiterIndex + 1, secondDelimiterIndex).toInt();
        int secondValue = data.substring(secondDelimiterIndex + 1).toInt();

        if (command == "redcap"){
          if (firstValue >300 && firstValue <340 && secondValue >220 && secondValue < 260){
            myservo1.write(round(180));
            delay(100);
          } else {
            myservo1.write(round(0));
            delay(100);
          }  
        }
        if (command == "LYRX") {
          int LSY_V = firstValue;
          int RSX_V = secondValue;
          if (LSY_V > 10){
            if (RSX_V > 10){
              if ((LSY_V - RSX_V) > 0){
                analogWrite(motorPin1,(LSY_V - RSX_V));
                analogWrite(motorPin2,0);
                analogWrite(motorPin3,(LSY_V));
                analogWrite(motorPin4,0);
              } else {
                analogWrite(motorPin1,0);
                analogWrite(motorPin2,0);
                analogWrite(motorPin3,(LSY_V));
                analogWrite(motorPin4,0);
              }
            } else if (RSX_V < -10){
              if ((LSY_V + RSX_V) > 0){
                analogWrite(motorPin1,(LSY_V));
                analogWrite(motorPin2,0);
                analogWrite(motorPin3,(LSY_V + RSX_V));
                analogWrite(motorPin4,0);
              } else {
                analogWrite(motorPin1,(LSY_V));
                analogWrite(motorPin2,0);
                analogWrite(motorPin3,0);
                analogWrite(motorPin4,0);
              }
            } else {
              analogWrite(motorPin1,(LSY_V));
              analogWrite(motorPin2,0);
              analogWrite(motorPin3,(LSY_V));
              analogWrite(motorPin4,0);
            }
          } else if (LSY_V < -10){
            if (RSX_V > 10){
              if ((LSY_V + RSX_V) < 0){
                analogWrite(motorPin1,0);
                analogWrite(motorPin2,abs(LSY_V + RSX_V));
                analogWrite(motorPin3,0);
                analogWrite(motorPin4,abs(LSY_V));
              } else {
                analogWrite(motorPin1,0);
                analogWrite(motorPin2,0);
                analogWrite(motorPin3,0);
                analogWrite(motorPin4,abs(LSY_V));
              }
            } else if (RSX_V < -10){
              if ((LSY_V - RSX_V) < 0){
                analogWrite(motorPin1,0);
                analogWrite(motorPin2,abs(LSY_V));
                analogWrite(motorPin3,0);
                analogWrite(motorPin4,abs(LSY_V - RSX_V));
              } else {
                analogWrite(motorPin1,0);
                analogWrite(motorPin2,abs(LSY_V));
                analogWrite(motorPin3,0);
                analogWrite(motorPin4,0);
              }
            } else {
              analogWrite(motorPin1,0);
              analogWrite(motorPin2,abs(LSY_V));
              analogWrite(motorPin3,0);
              analogWrite(motorPin4,abs(LSY_V));
            }
          } else {
            analogWrite(motorPin1,0);
            analogWrite(motorPin2,0);
            analogWrite(motorPin3,0);
            analogWrite(motorPin4,0);
          } 
        } 
      } else {
        int firstValue = data.substring(firstDelimiterIndex + 1).toInt();
          
        if (command == "XB") {
          int XB_V = firstValue;
          if (XB_V == 1){
            myservo1.write(round(180));
          }else if (XB_V == 0){
            myservo1.write(round(0));
          }
        } 
        if (command == "CB"){
          int CB_V = firstValue;
          if (CB_V == 1){
            myservo2.write(round(180));
          }else if (CB_V == 0){
            myservo2.write(round(0));
          }
        }
        if (command == "R2A") {
          if (firstValue > 10){
            analogWrite(motorPin1,firstValue);
            analogWrite(motorPin2,0);
            analogWrite(motorPin3,0);
            analogWrite(motorPin4,firstValue);
          } else {
            analogWrite(motorPin1,0);
            analogWrite(motorPin2,0);
            analogWrite(motorPin3,0);
            analogWrite(motorPin4,0);
          }
        } 
        if (command == "L2A") {
          if (firstValue > 10){
            analogWrite(motorPin1,0);
            analogWrite(motorPin2,firstValue);
            analogWrite(motorPin3,firstValue);
            analogWrite(motorPin4,0);
          } else {
            analogWrite(motorPin1,0);
            analogWrite(motorPin2,0);
            analogWrite(motorPin3,0);
            analogWrite(motorPin4,0);
          }
        }
      }
    }
  }
}

