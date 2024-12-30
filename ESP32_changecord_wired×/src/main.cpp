#include <ESP32Servo.h>

Servo myservo;          // Servoオブジェクトの宣言
const int SV_PIN = 25;   // サーボモーターをデジタルピン設定
int servo_angle = 0; //サーボ角度用変数（電源ON時にゼロ度に初期化）
const int motorPin1 = 32;
const int motorPin2 = 33;
const int motorPin3 = 18;
const int motorPin4 = 19;
byte data = 0;

void setup()
{
  myservo.attach(SV_PIN, 500, 2400);  // サーボの割当(パルス幅500~2400msに指定)
  myservo.write(servo_angle);//サーボ角度を0度にリセット

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
void loop(){
  String currentLine = "";//クライアントから得たデータを格納するための文字列を用意
  String orderCode = "";//命令文の種類を格納
  int orderAmount[2];//命令文の数値を格納するための要素数2の配列
  int ref = 0;//配列の要素数カウント用
  int r2Value = 0;
  int l2Value =0;
  int RSYValue = 0;
  int LSYValue = 0;

  while (true) {            
    if (Serial.available()) {             
      data = (byte)Serial.read();  
      if (data == '\n') {                    // 読み取った文字が改行コード(/n)だった場合
        if (currentLine == "exit"){
          Serial.println("Serial Disconnected.");
        }else{
          orderAmount[ref] = currentLine.toInt();;
          currentLine = "";//文字列の初期化
            
          if (orderCode == "XB"){
            //Serial.println("×ボタンが押されました");
            Serial.println(orderAmount[0]);
          }
          if (orderCode == "R2A"){
            //Serial.println("R2押し込み量");
            Serial.println(orderAmount[0]);
            //Serial.println("サーボ角度");　　サーボ設定例
            //Serial.print(round(0.9*(orderAmount[0]+100)));
            //Serial.print("度");
            //myservo.write(round(0.9*(orderAmount[0]+100)));
            //int r2Value = orderAmount[0]+100;  モーター設定例
              //if (r2Value > 0) {
                //analogWrite(motorPin1, r2Value);
                //analogWrite(motorPin2, 0);
              //} else {
                //analogWrite(motorPin1, 0);
                //analogWrite(motorPin2, 0);
              //}
          }
          if (orderCode == "L2A"){
            //Serial.println("L2押し込み量");
            Serial.println(orderAmount[0]);
          }
          if (orderCode == "RSY"){
            //Serial.println("右スティックy軸座標");
            Serial.println(orderAmount[0]);
            int RSYValue = orderAmount[0];
              if (RSYValue > 20){
                analogWrite(motorPin1,abs(RSYValue));
                analogWrite(motorPin2,0);
              } else if (RSYValue < -20){
                analogWrite(motorPin1,0);
                analogWrite(motorPin2,abs(RSYValue));
              } else {
                analogWrite(motorPin1,0);
                analogWrite(motorPin2,0);
              }
          }
          if (orderCode == "LSY"){
            //Serial.println("左スティックy軸座標");
            Serial.println(orderAmount[0]);
            int LSYValue = orderAmount[0];
              if (LSYValue > 20){
                analogWrite(motorPin3,abs(LSYValue));
                analogWrite(motorPin4,0);
              } else if (LSYValue < -20){
                analogWrite(motorPin3,0);
                analogWrite(motorPin4,abs(LSYValue));
              } else {
                analogWrite(motorPin3,0);
                analogWrite(motorPin4,0);
              }
          }
          ref = 0;//初期化
          orderCode = "";//初期化
          currentLine = "";//文字列の初期化
        }
      } else if (data == ':') {
        orderCode = currentLine;
        currentLine = "";//文字列の初期化
      } else if (data == ',') {
        orderAmount[ref] = currentLine.toInt();
        ref += 1;
        currentLine = "";//文字列の初期化
      } else if (data != '\r') {  // 受け取った文字が改行コードではない場合（普通の文字の場合）
        currentLine += data;      //変数currentLineに受け取った文字を追加する
      }
    }
  }
}
