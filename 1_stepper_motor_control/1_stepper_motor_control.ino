/*
  motor control
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
  spinLeftMotor(1,1000);
  delay(1000);

  spinLeftMotor(-1,1000);
  delay(1000);

  spinRightMotor(1,1000);
  delay(1000);

  spinRightMotor(-1,1000);
  delay(1000);

}

void spinLeftMotor(int dir, int duration){
  // 回転方向の設定
  if(dir < 0){
    digitalWrite(LEFT_DIRPIN, LOW);
  }else{
    digitalWrite(LEFT_DIRPIN, HIGH);
  }

  // パルスを送る回数を計算
  int pulseNum = duration * 1000 / WAITTIME / 2;
  for(int i=0;i<pulseNum;i++)
  {
    digitalWrite(LEFT_STEPPIN, HIGH);
    delayMicroseconds(WAITTIME);
    digitalWrite(LEFT_STEPPIN, LOW);
    delayMicroseconds(WAITTIME);
  }
}

void spinRightMotor(int dir, int duration){
  // 回転方向の設定（右側は左側と逆向きについているため、逆向きに設定）
  if(dir < 0){
    digitalWrite(RIGHT_DIRPIN, HIGH);
  }else{
    digitalWrite(RIGHT_DIRPIN, LOW);
  }

  // パルスを送る回数を計算
  int pulseNum = duration * 1000 / WAITTIME / 2;
  for(int i=0;i<pulseNum;i++)
  {
    digitalWrite(RIGHT_STEPPIN, HIGH);
    delayMicroseconds(WAITTIME);
    digitalWrite(RIGHT_STEPPIN, LOW);
    delayMicroseconds(WAITTIME);
  }
}
