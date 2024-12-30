//#ESP32-python(DUALSHOCK4使用)
//ライブラリのインクルード
#include <WiFi.h>
#include <ESP32Servo.h>      // Servoライブラリの読み込み
const char* ssid     = "mottyan laptop";//ルーターのSSID
const char* password = "kazuyan0624";//ルーターのパスワード
WiFiServer server(5000);//ポート番号5000でサーバーとして使用する
String message;
String oneLetter;
Servo myservo1;          // Servoオブジェクトの宣言
Servo myservo2;
const int SV_PIN1 = 25;   // サーボモーターをデジタルピン設定
const int SV_PIN2 = 26;
int servo_angle1 = 90; //サーボ角度用変数（電源ON時にゼロ度に初期化）
int servo_angle2 = 90;
const int motorPin1 = 32;
const int motorPin2 = 33;
const int motorPin3 = 18;
const int motorPin4 = 19;

void setup()
{
    Serial.begin(115200);//ビットレート115200でシリアル通信を開始
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);//アクセスポイントに接続
    while (WiFi.status() != WL_CONNECTED) {//接続が完了しない場合"・"を打って待機
        delay(500);
        Serial.print(".");
    }
//接続が完了したら以下の内容を表示
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());//自身のIPアドレスを表示
    
    server.begin();//サーバー開始
    
    myservo1.attach(SV_PIN1, 500, 2400);  // サーボの割当(パルス幅500~2400msに指定)
    myservo2.attach(SV_PIN2, 500, 2400);
    myservo1.write(servo_angle1);//サーボ角度を0度にリセット
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
void loop(){
 WiFiClient client = server.available();//サーバーに接続され、読み取り可能なデータがあるクライアントを取得
  if (client) {//クライアントの情報が取得出来た場合（クライアントに対しサーバーが開かれている場合）
    Serial.println("New Client.");
    if (client.connected()){
    message = "ready";
    client.print(message);//クライアントへメッセージを送信
    }
    String currentLine = "";//クライアントから得たデータを格納するための文字列を用意
    String orderCode = "";//命令文の種類を格納
    int orderAmount[2];//命令文の数値を格納するための要素数2の配列
    int ref = 0;//配列の要素数カウント用
    int r2Value = 0;
    int l2Value =0;
    int RSYValue = 0;
    int LSYValue = 0;
    while (client.connected()) {            // client.connected()は接続時True,切断時Falth
      if (client.available()) {             // client.abalableは読み込み可能なバイト数(接続先のサーバーによってクライアントに書き込まれたデータの量)を返す
        char c = client.read();  //クライアントから送信されたデータを1バイトだけ読み取る
        if (c == '\n') {                    // 読み取った文字が改行コード(/n)だった場合
          if (currentLine == "exit"){
            client.stop();//サーバーとの接続を切断
            Serial.println("Client Disconnected.");
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
                //if (RSYValue > 20){
                  //analogWrite(motorPin1,abs(RSYValue));
                  //analogWrite(motorPin2,0);
                //} else if (RSYValue < -20){
                  //analogWrite(motorPin1,0);
                  //analogWrite(motorPin2,abs(RSYValue));
                //} else {
                  //analogWrite(motorPin1,255);
                  //analogWrite(motorPin2,255);
                //}
                if (RSYValue < 85){
                  myservo1.write(RSYValue);
                } else if (RSYValue > 95){
                  myservo1.write(RSYValue);
                } else {
                  myservo1.write(90);
                }
            }
            if (orderCode == "LSY"){
              //Serial.println("左スティックy軸座標");
              Serial.println(orderAmount[0]);
              int LSYValue = orderAmount[0];
                //if (LSYValue > 20){
                  //analogWrite(motorPin3,abs(LSYValue));
                  //analogWrite(motorPin4,0);
                //} else if (LSYValue < -20){
                  //analogWrite(motorPin3,0);
                  //analogWrite(motorPin4,abs(LSYValue));
                //} else {
                  //analogWrite(motorPin3,255);
                  //analogWrite(motorPin4,255);
                //}
                if (LSYValue < 85){
                  myservo2.write(LSYValue);
                } else if (LSYValue > 95){
                  myservo2.write(LSYValue);
                } else {
                  myservo2.write(90);
                }
            }
            ref = 0;//初期化
            orderCode = "";//初期化
            currentLine = "";//文字列の初期化
          }
        } else if (c == ':') {
          orderCode = currentLine;
          currentLine = "";//文字列の初期化
        } else if (c == ',') {
          orderAmount[ref] = currentLine.toInt();
          ref += 1;
          currentLine = "";//文字列の初期化
        } else if (c != '\r') {  // 受け取った文字が改行コードではない場合（普通の文字の場合）
          currentLine += c;      //変数currentLineに受け取った文字を追加する
        }
      }
    }
  }
}