/** @file   communications.h
    @author Yunu Cho, Owen Zhou
    @date   14 October 2023
    @brief  Declarations of functions related to communication systems
*/

#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "ir_uart.h"

#include <stdint.h>
#include <stdlib.h>

void send_signal(char c);

char recv_signal(void);

void synchronise_boards(uint8_t code);

#endif