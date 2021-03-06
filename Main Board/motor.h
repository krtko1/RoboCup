#ifndef MOTOR_H_
#define MOTOR_H_

#include "ch.h"
#include "hal.h"
#include "includes/chprintf.h"

#define NUM_OF_MOTORS 3
#define MOTOR_FORWARD 1
#define MOTOR_BACKWARD 2
#define MOTOR_BREAK 3
#define MOTOR_OFF 0
#define ENCODER_OFFSET 3
#define MOTOR_CONST 2/NUM_OF_MOTORS
#define PI 3.141

void set_motor_state(int8_t dir,int8_t num);
void move_motor(int8_t motor_number, int16_t speed);
void set_motor_off(int8_t motor_number);
void set_motors_off(void);
void encoder_pulse_captured(ICUDriver *icup);
void read_motors_speed(int16_t *buff);
void read_motors_period(int16_t *buff);
void motor_tick(void);
void motor_checker(int8_t motor_number);
void timer_init(void);

#endif