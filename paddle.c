/* @file   paddle.c
 * @author Yunu Cho, Owen Zhou
 * @date   10 October 2023
 * @brief  Declaration and definitions of the ping pong paddle
 */

#include "paddle.h"

/* Paddle initialisation and returns paddle object */
Paddle_t paddle_init(void)
{
    Paddle_t paddle;
    paddle.x = CENTRE_POS;
    paddle.y = 0;        
    paddle.face[0] = CENTRE_POS - 1;
    paddle.face[1] = CENTRE_POS;
    paddle.face[2] = CENTRE_POS + 1;
    return paddle;
}

/* Change the xy position of the paddle */
void paddle_set_pos(Paddle_t paddle, uint8_t x, uint8_t y)
{
    paddle.x = x;
    paddle.y = y;
}