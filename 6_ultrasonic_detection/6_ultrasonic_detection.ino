/*
  This example code is in the public domain.
  rb-station.com
*/
// 超音波センサで計測をして正面に障害物があった場合に回転して方向を転換して再度前進するサンプル

#define LEFT_STEPPIN 5
#define LEFT_DIRPIN 2
#define RIGHT_STEPPIN 6
#define RIGHT_DIRPIN 3

#define TRIG 12
#define ECHO 13

double usDuration = 0;
double usDistance = 0;
double speedOfSound = 331.5 + 0.6 * 25; // 25℃の気温の想定
long nextTrigPulse = 0;
long currentMicros = 0;

int phase = 0; // 0:前進状態 1:障害物があった際に回転している状態

void setup()
{
  pinMode(LEFT_STEPPIN, OUTPUT);
  pinMode(LEFT_DIRPIN, OUTPUT);
  pinMode(RIGHT_STEPPIN, OUTPUT);
  pinMode(RIGHT_DIRPIN, OUTPUT);

  pinMode(ECHO, INPUT );
  pinMode(TRIG, OUTPUT );

  Serial.begin( 9600 );

  setMotor(220, 220, 50000); //前進
}
 
void loop() 
{
}


// left/right: -255〜255の間の数値を指定すると、数値の大きさによって回転スピードが変わる
//             leftとrightは絶対値が同じか、0の場合のみ対応（left=100, right=200などは不可）
// duration:   継続時間[ミリ秒]
void setMotor(int left, int right, long duration){
  if(left == 0 && right == 0){
    return;
  }
  
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
  long waittime = max(256 - abs(left), 256 - abs(right)) * 10 + 1000;
  if(left == 0){
    waittime = (256 - abs(right)) * 10 + 1000;
  }
  if(right == 0){
    waittime = (256 - abs(left)) * 10 + 1000;
  }

  long pulseNum = duration * 1000 / waittime / 2;
  for(int i=0;i<pulseNum;i++)
  {
    if(left != 0){ digitalWrite(LEFT_STEPPIN, HIGH);}
    if(right != 0){digitalWrite(RIGHT_STEPPIN, HIGH);}
    for(int j=0;j<waittime;j+=10){
      delayMicroseconds(10);

      // pulseIn()を使うと、その間に処理が停止してしまうので、ステッピングモータへのパルス入力時に超音波センサの値の取得も同時に行う
      int dist = getDistance();
      if(dist > 0){
        // 障害物があったときにphaseを1にして、回転をする
        if(phase == 0 && dist < 50){
          phase = 1;
          setMotor(160, -160, 50000);
          return;
        }
        // 障害物がなくなったときにphaseを0にして、前進をする
        if(phase == 1 && dist >= 50){
          phase = 0;
          setMotor(220, 220, 50000);
          return;
        }
      }
    }

    if(left != 0){ digitalWrite(LEFT_STEPPIN, LOW);}
    if(right != 0){digitalWrite(RIGHT_STEPPIN, LOW);}
    for(int j=0;j<waittime;j+=10){
      delayMicroseconds(10);
      int dist = getDistance();
      if(dist > 0){
        if(phase == 0 && dist < 50){
          phase = 1;
          setMotor(160, -160, 50000);
          return;
        }
        if(phase == 1 && dist >= 50){
          phase = 0;
          setMotor(220, 220, 50000);
          return;
        }
      }
    }

    currentMicros = micros();
    // 一定時間ごとに超音波センサのTrigをコールする
    if(currentMicros > nextTrigPulse){
      digitalWrite(TRIG, LOW); 
      delayMicroseconds(2);
      digitalWrite(TRIG, HIGH); 
      delayMicroseconds(10);
      digitalWrite( TRIG, LOW );
      nextTrigPulse = currentMicros + 400000;     
    }
    
  }
}

// 超音波センサから距離を取得
int getDistance(){
  int duration = read_pulse(ECHO);
  if(duration > 0){
    int distance = (duration / 2) * speedOfSound * 100 / 1000000;
    Serial.print("Distance:");
    Serial.print(distance);
    Serial.println(" cm");

    return distance;
  }else{
    return -1;
  }
}

// pulseIn()を使うと、その間に処理が停止してしまうのでステッピングモータが一時的に停止してしまうのを防ぐためにノンブロッキングでの実装を用いる
unsigned long read_pulse(int pin)
{
    static unsigned long rising_time;  // time of the rising edge
    static int last_state;
    int state = digitalRead(pin);      // current pin state
    unsigned long pulse_length = 0;    // default return value

    // On rising edge: record current time.
    if (last_state == LOW && state == HIGH) {
        rising_time = micros();
    }

    // On falling edge: report pulse length.
    if (last_state == HIGH && state == LOW) {
        unsigned long falling_time = micros();
        pulse_length = falling_time - rising_time;
    }

    last_state = state;
    //Serial.println(pulse_length);
    return pulse_length;
}
