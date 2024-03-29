/** @file   paddle.h
    @author Yunu Cho, Owen Zhou
    @date   10 October 2023
    @brief  Declaration and definitions of the ping pong paddle
*/

#ifndef PADDLE_H
#define PADDLE_H

#include <stdint.h>

#define PADDLE_X 4
#define PADDLE_Y 3

typedef struct {
    int8_t x;
    int8_t y;
} Paddle_t;

Paddle_t paddle_init(void);

void paddle_set_pos(Paddle_t* paddle, uint8_t x, uint8_t y);

#endif
