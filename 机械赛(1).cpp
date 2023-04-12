#include<SoftwareSerial.h>
#include <PS2X_lib.h>  //for v1.6
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
RemoteControl remotecontrol;
int comd;
PS2X ps2x;

void Chassis::Reset() {//底盘初始化赋值，每个轮子id设置
    FL.Reset(FL_WHEEL);
    BL.Reset(BL_WHEEL);
    BR.Reset(BR_WHEEL);
    FR.Reset(FR_WHEEL);
}

void Chassis::Handle() {//底盘最终输出，每个轮子输出
    FL.Handle();
    FR.Handle();
    BL.Handle();
    BR.Handle();
}

void RemoteControl::Reset() {
    Mode=STOP;
}

void RemoteControl::Input(){
    ps2x.read_gamepad();          //read controller

    if(ps2x.ButtonPressed(GREEN_FRET)){//按下：进入自旋模式
        Serial.println("Green Fret Pressed");
        Mode=ROTATE;
    }
    if(ps2x.ButtonPressed(RED_FRET)){//按下：进入直行模式
        Serial.println("Red Fret Pressed");
        Mode=MOVE;
    }
    if(ps2x.ButtonPressed(YELLOW_FRET)){//按下：进入底盘锁死模式
        Serial.println("Yellow Fret Pressed");
        Mode=LOCK;
    }
   /*
    if(ps2x.ButtonPressed(BLUE_FRET))
        Serial.println("Blue Fret Pressed");
    if(ps2x.ButtonPressed(ORANGE_FRET))
        Serial.println("Orange Fret Pressed");
*///TODO:这几个按钮不知道要干嘛
    if(ps2x.ButtonPressed(STAR_POWER))
        Serial.println("Star Power Command");

    if(ps2x.Button(UP_STRUM))  {
        Serial.println("Up Strum");

    }        //will be TRUE as long as button is pressed

    if(ps2x.Button(DOWN_STRUM))
        Serial.println("DOWN Strum");

    if(ps2x.Button(PSB_START)){//will be TRUE as long as button is pressed
        Serial.println("Start is being held");
    }

    if(ps2x.Button(PSB_SELECT))
        Serial.println("Select is being held");
}

void Chassis::AttitudeEncoding(int speedLF, int speedLB, int speedRF, int speedRB) {//底盘运动编码
    FL.SetPWM(speedLF);
    FR.SetPWM(speedRF);
    BL.SetPWM(speedLB);
    BR.SetPWM(speedRB);
    Serial.print("speedLF=");
    Serial.println(speedLF);
    Serial.print("speedLB=");
    Serial.println(speedLB);
    Serial.print("speedRF=");
    Serial.println(speedRF);
    Serial.print("speedRB=");
    Serial.println(speedRB);
}

void Motor::SetPWM(int _pwm) {
    pwm=_pwm;
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    chassis.Reset();
    remotecontrol.Reset();
}

void loop() {
    remotecontrol.Command();
    chassis.Handle();
}

void command(int mode) {
    switch (mode){
        case 1:
            chassis.MecanumRun(-100, 40, -3);
            delay(2100);
            chassis.MecanumRun(0, 0, 0);
            delay(3000);
            chassis.MecanumRun(100, -40, 2);
            delay(2100);
            chassis.MecanumRun(0, 0, 0);
            delay(3000);
        case 2:
            chassis.MecanumRun(-150, 30, 0);
            delay(1500);
            chassis.MecanumRun(0, 0, 0);
            delay(3000);
            chassis.MecanumRun(150, -30, 0);
            delay(1500);
            chassis.MecanumRun(0, 0, 0);
            delay(3000);
        case 3:
            chassis.MecanumRun(-75, 60, 0);
            delay(1500);
            chassis.MecanumRun(0, 0, 0);
            delay(3000);
            chassis.MecanumRun(75, -60, 0);
            delay(1500);
            chassis.MecanumRun(0, 0, 0);
            delay(3000);
    }
}

void Chassis::MecanumRun(int FBSpeed, int LRSpeed, int RTSpeed) {//整个底盘运动的速度解算
    int speedLF = LRSpeed - FBSpeed + RTSpeed;
    int speedRF = LRSpeed + FBSpeed - RTSpeed;
    int speedRB = LRSpeed - FBSpeed - RTSpeed;
    int speedLB = LRSpeed + FBSpeed + RTSpeed;

    int Max = speedLF;
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

void Motor::Handle() {//给每个轮子加上pwm波进行控制（最终输出函数）
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