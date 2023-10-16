/** @file   ball.c
    @author Yunu Cho, Owen Zhou
    @date   10 October 2023
    @brief  Functions related to the initialisation and movement of the ping pong ball
*/

#include "ball.h"

/** Initialises ball.
    @return the initialised ball object.  */
Ball_t ball_init(void)
{
    Ball_t ball;
    ball.x = 0;
    ball.y = 0;
    ball.vx = 0;
    ball.vy = 0;
    
    return ball;
}

/** Set the xy position of the ball.
    @param ball pointer of the the ball object to be updated
    @param x the new x value of the ball
    @param y the new y value of the ball.  */
void ball_set_pos(Ball_t* ball, int8_t x, int8_t y)
{
    ball->x = x;
    ball->y = y;
}

/** Set the vxy direction of the ball.
    @param ball pointer of the the ball object to be updated
    @param vx the new vx value of the ball
    @param vy the new vy value of the ball.  */
void ball_set_dir(Ball_t* ball, int8_t vx, int8_t vy)
{
    ball->vx = vx;
    ball->vy = vy;
}
