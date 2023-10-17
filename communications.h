/** @file   communications.h
    @author Yunu Cho, Owen Zhou
    @date   14 October 2023
    @brief  Declarations of functions related to communication systems
*/

#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "ir_uart.h"

#include <stdint.h>

#define END_ROUND -127
#define END_GAME -128

void send_signal(char ch);

char recv_signal(void);

#endif
