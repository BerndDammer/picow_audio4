#ifndef HARDWARE_DRIVE_H_
#define HARDWARE_DRIVE_H_

enum WHEEL
{
    FRONT_LEFT,
    FRONT_RIGHT,
    REAR_LEFT,
    REAR_RIGHT
};

#define DRIVE_MAX 100
#define DRIVE_OFF 0
#define DRIVE_MIN -100

void motor_init(void);
void motor_set(enum WHEEL w, int pwm_val);

#endif
