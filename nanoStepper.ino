const int outputPin = 9;
const int dirPin = 8;
const int enaPin = 7;
const int MS1 = 3;
const int MS2 = 2;

const int VCC = A0; 
const int speedIn = A1; 
const int GND = A2;

#define prescaler 8 // 分频系数8

/* 脉搏波数据缓冲区 */
#define data_len 412

float outdata[data_len]={0,1,1,1,1,1,1,1,2,2,2,2,2,3,3,4,4,5,6,7,8,8,9,10,11,12,13,14,15,16,17,17,18,
19,19,20,21,21,21,22,22,22,23,23,23,23,23,23,23,23,23,23,22,22,22,22,22,21,21,21,21,20,20,20,19,19,19,
18,18,18,17,17,17,16,16,16,16,15,15,15,15,14,14,14,14,14,14,13,13,13,13,13,12,12,12,12,12,12,12,11,11,
11,11,11,11,10,10,10,10,10,10,9,9,9,9,9,9,8,8,8,8,8,8,8,7,7,7,7,7,7,7,6,6,6,6,6,6,6,6,6,6,6,5,5,5,5,5,
5,5,5,5,5,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0};

float constVal = 0;

int i = 0;  // 循环遍历数组数据

void setup() {
  Serial.begin(9600);

  pinMode(outputPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enaPin, OUTPUT);

  //方向
  digitalWrite(dirPin, HIGH);

  // 细分  
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  //MS2, MS1: 00: ¼, 01: ⅛, 10: 1/16, 11: 1/32
  digitalWrite(MS2, HIGH);
  digitalWrite(MS1, LOW);
  
  //调速
  pinMode(VCC, OUTPUT);
  pinMode(GND, OUTPUT);
  digitalWrite(VCC, HIGH);
  digitalWrite(GND, LOW);
  float speed = analogRead(speedIn) / 1023.0 * 2 +0.1 ;
  if(speed>0.2)
    // LOW使能
    digitalWrite(enaPin, LOW);
  else
    digitalWrite(enaPin, HIGH);

  // 时钟8分频
  TCCR1A = _BV(WGM11) | _BV(WGM10) | _BV(COM1A0) | _BV(COM1B1);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11);

  // 转换脉搏数据
  for(int j = 0;j<data_len;j++)
  {
    if(outdata[j]!=0)
      outdata[j] = F_CPU / prescaler / 1000.0 / (outdata[j]+constVal);  //1000 此数字用于等比例修改速度，数值和速度成反比
    else
      outdata[j] = F_CPU / prescaler /1000.0 / (0.01+constVal);
  }

  OCR1A = outdata[i]/speed -1;
  OCR1B = (OCR1A+1)/2; //占空比50%
}

void loop() {
  float speed = analogRead(speedIn) / 1023.0 * 2 + 0.1; 
  if(speed>0.2)
    // LOW使能
    digitalWrite(enaPin, LOW);
  else
    digitalWrite(enaPin, HIGH);

  // delay(2);
  delayMicroseconds(1800);

  i++;
  if(i==data_len) i=0;

  OCR1A = outdata[i]/speed -1;
  OCR1B = (OCR1A+1)/2; //占空比50%

}