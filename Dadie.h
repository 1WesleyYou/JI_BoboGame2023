//
// Created by 25755 on 2023/4/11.
//

#ifndef JI_BOBOGAME2023_DADIE_H
#define JI_BOBOGAME2023_DADIE_H

#define FL_WHEEL 1
#define BL_WHEEL 2
#define BR_WHEEL 3
#define FR_WHEEL 4

#define PS2_DAT        13  //14
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        12  //17

enum {
    STOP,
    ROTATE,
    MOVE,
    LOCK
} Move_Mode_e;

class Motor {
public:
    void Reset(int _id) { id = _id; }

    void Handle();

    void SetPWM(int pwm);

private:
    int id;
    float pwm;
};

class Chassis {
public:
    void Reset();

    void Handle();

    void MecanumRun(int FBSeed, int LRSpeed, int RTSpeed);

    void AttitudeEncoding(int speedLF, int speedLB, int speedRF, int speedRB);

    inline void Forward() { MecanumRun(100, 0, 0); }

    inline void LeftMove() { MecanumRun(0, 100, 0); }

    inline void RightMove() { MecanumRun(0, -100, 0); }

    inline void LeftRotate() { MecanumRun(0, 0, 1); }

    inline void RightRotate() { MecanumRun(0, 0, -1); }

    inline void Back() { MecanumRun(-100, 0, 0); }

private:
    float FBVelocity, LRVelocity, RTVelocity;
    Motor FL, FR, BL, BR;
};

class RemoteControl {
public:
    void Reset();

    void Input();

    void Command();

private:
    int Mode;
    int FBVelocity, LRVelocity, RTVelocity;

};

#endif //JI_BOBOGAME2023_DADIE_H
