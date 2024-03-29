/** @file   ball.h
    @author Yunu Cho, Owen Zhou
    @date   13 October 2023
    @brief  Declaration and definitions of the drawing module
*/

#ifndef DRAW_H
#define DRAW_H

#include "ball.h"
#include "paddle.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"

#define MESSAGE_RATE 20

void display_ball(Ball_t* ball);

void display_paddle(Paddle_t* paddle);

void display_character(char c);

void display_text(char text[]);

#endif
