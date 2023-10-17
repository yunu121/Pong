/** @file   ball.h
    @author Yunu Cho, Owen Zhou
    @date   9 October 2023
    @brief  Declaration and definitions of the ping pong ball
*/

#ifndef BALL_H
#define BALL_H

#include <stdint.h>

#define BALL_X 3
#define BALL_Y 3
#define BALL_SPEED 2

typedef struct {
    int8_t x;
    int8_t y;
    int8_t vx;
    int8_t vy;
} Ball_t;

Ball_t ball_init(void);

void ball_set_pos(Ball_t* ball, int8_t x, int8_t y);

void ball_set_dir(Ball_t* ball, int8_t vx, int8_t vy);

#endif
