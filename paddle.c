/** @file   paddle.c
    @author Yunu Cho, Owen Zhou
    @date   10 October 2023
    @brief  Functions related to the paddle initialisation and movement
*/

#include "paddle.h"

/** Initialises paddle.
    @return the initialised paddle object.  */
Paddle_t paddle_init(void)
{
    Paddle_t paddle;
    paddle.x = PADDLE_X;
    paddle.y = PADDLE_Y;
    
    return paddle;
}

/** Set the xy position of the paddle.
    @param paddle pointer of the paddle object to be updated
    @param x the new x value of the paddle
    @param y the new y value of the paddle.  */
void paddle_set_pos(Paddle_t* paddle, uint8_t x, uint8_t y)
{
    paddle->x = x;
    paddle->y = y;
}
