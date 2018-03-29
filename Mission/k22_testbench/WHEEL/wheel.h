/*
 * wheel.h
 *
 *  Created on: Mar 22, 2018
 *      Author: quinn
 */

#ifndef WHEEL_WHEEL_H_
#define WHEEL_WHEEL_H_

#define MOTOR_CCW 0
#define MOTOR_CW 1

void FTM3_IRQHandler();

void motor_init();

void motor_setdir(int dir);

void motor_setspeed(float dcycle);

void motor_stop();

void motor_rotate(int deg);

void motor_testcase();

#endif /* WHEEL_WHEEL_H_ */
