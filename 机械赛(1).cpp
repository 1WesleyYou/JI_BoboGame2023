#include<SoftwareSerial.h>
#include "Dadie.h"
//左前轮（LF）引脚
#define LF1 6
#define LF2 7
#define E1A_F 20
#define E1B_F 34
//右前轮（RF）引脚
#define RF1 2
#define RF2 3
#define E2A_F 21
#define E2B_F 38
//右后轮（RB）引脚
#define RB1 9
#define RB2 8
#define E1A_B 18
#define E1B_B 26
//左后轮（LB）引脚
#define LB1 4
#define LB2 5
#define E2A_B 19
#define E2B_B 30
float maxLinearSpeed = 200;
int origin, x, y;
Chassis chassis;
int comd;

void Chassis::Reset() {
    FL.Reset(FL_WHEEL);
    BL.Reset(BL_WHEEL);
    BR.Reset(BR_WHEEL);
    FR.Reset(FR_WHEEL);
}

void Chassis::Handle() {
    FL.Handle();
    FR.Handle();
    BL.Handle();
    BR.Handle();
}


void Chassis::AttitudeEncoding(float speedLF, float speedLB, float speedRF, float speedRB) {
    FL.Handle();
    FR.Handle();
    BL.Handle();
    BR.Handle();
    Serial.print("speedLF=");
    Serial.println(speedLF);
    Serial.print("speedLB=");
    Serial.println(speedLB);
    Serial.print("speedRF=");
    Serial.println(speedRF);
    Serial.print("speedRB=");
    Serial.println(speedRB);
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

}

void loop() {
    command(mode);

}

void command(int mode) {
    switch (mode) {
        case 1:
            Mecanumrun(-100, 40, -3);
            delay(2100);
            Mecanumrun(0, 0, 0);
            delay(3000);
            Mecanumrun(100, -40, 2);
            delay(2100);
            Mecanumrun(0, 0, 0);
            delay(3000);
        case 2:
            Mecanumrun(-150, 30, 0);
            delay(1500);
            Mecanumrun(0, 0, 0);
            delay(3000);
            Mecanumrun(150, -30, 0);
            delay(1500);
            Mecanumrun(0, 0, 0);
            delay(3000);
        case 3:
            Mecanumrun(-75, 60, 0);
            delay(1500);
            Mecanumrun(0, 0, 0);
            delay(3000);
            Mecanumrun(75, -60, 0);
            delay(1500);
            Mecanumrun(0, 0, 0);
            delay(3000);
    }
}

void Chassis::MecanumRun(float FBSpeed, float LRSpeed, float RTSpeed) {//整个底盘运动的速度解算
    float speedLF = LRSpeed - FBSpeed + RTSpeed;
    float speedRF = LRSpeed + FBSpeed - RTSpeed;
    float speedRB = LRSpeed - FBSpeed - RTSpeed;
    float speedLB = LRSpeed + FBSpeed + RTSpeed;

    float Max = speedLF;
    if (Max < speedLB) Max = speedLB;
    if (Max < speedRF) Max = speedRF;
    if (Max < speedRB) Max = speedRB;

    if (Max > maxLinearSpeed) {
        speedLF = speedLF / Max * maxLinearSpeed;
        speedLB = speedLB / Max * maxLinearSpeed;
        speedRF = speedRF / Max * maxLinearSpeed;
        speedRB = speedRB / Max * maxLinearSpeed;
    }

    AttitudeEncoding(speedLF, speedLB, speedRF, speedRB);
}

void Motor::Handle() {
    switch (id) {
        case FL_WHEEL:
            if (pwm > 0) {
                analogWrite(LF1, 255);
                analogWrite(LF2, 255 - pwm);
            } else {
                analogWrite(LF2, 255);
                analogWrite(LF1, 255 + pwm);
            }
            break;
        case BL_WHEEL:
            if (pwm > 0) {
                analogWrite(LB1, 255);
                analogWrite(LB2, 255 - pwm);
            } else {
                analogWrite(LB2, 255);
                analogWrite(LB1, 255 + pwm);
            }
            break;
        case BR_WHEEL:
            if (pwm > 0) {
                analogWrite(RB1, 255);
                analogWrite(RB2, 255 - pwm);
            } else {
                analogWrite(RB2, 255);
                analogWrite(RB1, 255 + pwm);
            }
        case FR_WHEEL:
            if (pwm > 0) {
                analogWrite(RF1, 255);
                analogWrite(RF2, 255 - pwm);
            } else {
                analogWrite(RF2, 255);
                analogWrite(RF1, 255 + pwm);
            }
    }
}