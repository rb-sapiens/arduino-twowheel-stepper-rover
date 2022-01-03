/*
  rover move
  This example code is in the public domain.
  rb-station.com
*/

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
}
 
void loop() 
{
  // 前進
  setMotor(1,1,1000);
  delay(1000);

  // 後退
  setMotor(-1,-1,1000);
  delay(1000);

  // 右に曲がる
  setMotor(1,0,1000);
  delay(1000);

  // 左に曲がる
  setMotor(0,1,1000);
  delay(1000);

  // その場で左に回転
  setMotor(-1,1,1000);
  delay(1000);

  // その場で右に回転
  setMotor(1,-1,1000);
  delay(1000);

}

void setMotor(int left, int right, int duration){
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
  int pulseNum = duration * 1000 / WAITTIME / 2;
  for(int i=0;i<pulseNum;i++)
  {
    if(left != 0){ digitalWrite(LEFT_STEPPIN, HIGH);}
    if(right != 0){digitalWrite(RIGHT_STEPPIN, HIGH);}
    delayMicroseconds(WAITTIME);
    if(left != 0){ digitalWrite(LEFT_STEPPIN, LOW);}
    if(right != 0){digitalWrite(RIGHT_STEPPIN, LOW);}
    delayMicroseconds(WAITTIME);
  }
}
