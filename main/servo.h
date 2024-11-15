#ifndef SERVO_H
#define SERVO_H

#include <driver/mcpwm_prelude.h>

void servo_init(void);
void servo_control_task(void *arg);

#endif // SERVO_H
