#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "ir_uart.h"

#include <stdint.h>
#include <stdlib.h>

void send_signal(char c[]);

char recv_signal(void);

#endif