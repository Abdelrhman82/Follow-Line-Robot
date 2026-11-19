// Motor
int LPWM = 9; //ENA
int RPWM = 10; //ENB
int IN1 = 8;
int IN2 = 11;
int IN3 = 12;
int IN4 = 13;

// Sensors
int irPins[5] = {A4, A3, A2, A1, A0};
int Weights[5] = {10, 20, 30, 40, 50};
int bitweight[5] = {1, 2, 4, 8, 16};
int sensorADC[5];
int sumOnSensor;
int sensorWeight;
int bit_sensor;

//PID
float line_position;
float center_position = 30;
float error;
float derivative;
float I;
float pervios_error = 0;
int speed = 50;
int Rspeed;
int Lspeed;
float KP = 1;
float KI = 0;
float KD = 1;

int readLine() {
  sensorWeight = 0;
  sumOnSensor = 0;
  bit_sensor = 0;

  for (int i = 0; i < 5; i++) {
    sensorADC[i] = analogRead(irPins[i]);
    if(sensorADC[i] > 150){
      sensorADC[i] = 1;
    }else{
      sensorADC[i] = 0;
    }

    sumOnSensor += sensorADC[i];
    sensorWeight += sensorADC[i] * Weights[i];
    bit_sensor += sensorADC[i] * bitweight[i];
  }
}

void Motor(int Lspeed, int Rspeed){
  if(Lspeed > 0){
    pinMode(IN1, LOW);
    pinMode(IN2, HIGH);
  }else{
    pinMode(IN1, HIGH);
    pinMode(IN2, LOW);
  }

  if(Rspeed > 0){
    pinMode(IN3, HIGH);
    pinMode(IN4, LOW);
  }else{
    pinMode(IN3, LOW);
    pinMode(IN4, HIGH);
  }

  Lspeed = constrain(Lspeed, -255, 255);
  Rspeed = constrain(Rspeed, -255, 255);

  analogWrite(LPWM , abs(Lspeed));
  analogWrite(RPWM , abs(Rspeed));
}

void PID_Controller(int base_speed, float kp, float ki, float kd){
  readLine();

  if(sumOnSensor > 0) line_position = sensorWeight / sumOnSensor;
  error = center_position - line_position; // error
  derivative = error - pervios_error; // derivative
  I += error * (.1);  // integral

  Rspeed = base_speed + (error * kp + ki * I + derivative * kd);
  Lspeed = base_speed - (error * kp + ki * I + derivative * kd);

  Motor(Lspeed, Rspeed);
}

void setup() {
  Serial.begin(9600);

  pinMode(LPWM, OUTPUT);
  pinMode(RPWM, OUTPUT);

  digitalWrite(IN1, OUTPUT);
  digitalWrite(IN2, OUTPUT);
  digitalWrite(IN3, OUTPUT);
  digitalWrite(IN4, OUTPUT);

  Serial.println("u,y");
}

void loop() {

  PID_Controller(speed, KP, KI, KD);
  Serial.print(String(Rspeed - Lspeed)+","+String(error));
  Serial.println("");
  delay(100);

}