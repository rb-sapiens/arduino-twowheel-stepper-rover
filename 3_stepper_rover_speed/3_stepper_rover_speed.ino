/*
  This example code is in the public domain.
  rb-station.com
*/
// 速度指定のサンプル

#define LEFT_STEPPIN 5
#define LEFT_DIRPIN 2
#define RIGHT_STEPPIN 6
#define RIGHT_DIRPIN 3

// パルスを送る際の周波数。単位はマイクロ秒
#define WAITTIME 1500


void setup()
{
  pinMode(LEFT_STEPPIN, OUTPUT);
  pinMode(LEFT_DIRPIN, OUTPUT);
  pinMode(RIGHT_STEPPIN, OUTPUT);
  pinMode(RIGHT_DIRPIN, OUTPUT);

  Serial.begin( 9600 );

  setMotor(50, 50, 1000);
  setMotor(100, 100, 1000);
  setMotor(200, 200, 1000);
  setMotor(200, -200, 500);
  setMotor(100, -100, 500);
  setMotor(0, 0, 1000);

}
 
void loop() 
{

}

// left/right: -255〜255の間の数値を指定すると、数値の大きさによって回転スピードが変わる
//             leftとrightは絶対値が同じか、0の場合のみ対応（left=100, right=200などは不可）
// duration:   継続時間[ミリ秒]
void setMotor(int left, int right, long duration){
  // 回転方向の設定
  if(left < 0){
    digitalWrite(LEFT_DIRPIN, LOW);
  }else{
    digitalWrite(LEFT_DIRPIN, HIGH);
  }
  if(right < 0){
    digitalWrite(RIGHT_DIRPIN, HIGH);
  }else{
    digitalWrite(RIGHT_DIRPIN, LOW);
  }

  // パルスを送る回数を計算
  long waittime = max(256 - abs(left), 256 - abs(right)) * 10 + 1500;
  if(left == 0){
    waittime = (256 - abs(right)) * 10 + 1500;
  }
  if(right == 0){
    waittime = (256 - abs(left)) * 10 + 1500;
  }
  long pulseNum = duration * 1000 / waittime / 2;
  for(int i=0;i<pulseNum;i++)
  {
    if(left != 0){ digitalWrite(LEFT_STEPPIN, HIGH);}
    if(right != 0){digitalWrite(RIGHT_STEPPIN, HIGH);}
    delayMicroseconds(waittime);
    if(left != 0){ digitalWrite(LEFT_STEPPIN, LOW);}
    if(right != 0){digitalWrite(RIGHT_STEPPIN, LOW);}
    delayMicroseconds(waittime);
  }
}
