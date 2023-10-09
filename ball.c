/* @file   ball.c
 * @author Yunu Cho, Owen Zhou
 * @date   10 October 2023
 * @brief  Declaration and definitions of the ping pong ball
 */

#include "ball.h"

/* Ball initialisation and returns ball object */
Ball_t ball_init(void)
{
    Ball_t ball;
    ball.x = 0;
    ball.y = 0;
    ball.vx = 0;
    ball.vy = 0;
    return ball;
}

/* Change the xy position of the ball */
void ball_set_pos(Ball_t ball, uint8_t x, uint8_t y)
{
    ball.x = x;
    ball.y = y;
}

/* Change the vxy direction of the ball*/
void ball_set_dir(Ball_t ball, int8_t vx, int8_t vy)
{
    ball.vx = vx;
    ball.vy = vy;
}
