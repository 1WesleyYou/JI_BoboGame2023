//
// Created by 25755 on 2023/4/11.
//

#ifndef JI_BOBOGAME2023_DADIE_H
#define JI_BOBOGAME2023_DADIE_H

#define FL_WHEEL 1
#define BL_WHEEL 2
#define BR_WHEEL 3
#define FR_WHEEL 4

enum{
    ROTATE,
    MOVE,
    STOP,
    LOCK
}Move_Mode_e;

class Motor{
public:
    void Reset(int _id){id=_id;}

    void Handle();
private:
   int id;
   float pwm;
};

class Chassis{
public:
    void Reset();
    void Handle();
    void MecanumRun(float FBSeed, float LRSpeed, float RTSpeed);
    void AttitudeEncoding(float speedLF, float speedLB,float speedRF,float speedRB);
private:
    float FBVelocity,LRVelocity,RTVelocity;
    Motor FL,FR,BL,BR;
};

#endif //JI_BOBOGAME2023_DADIE_H
