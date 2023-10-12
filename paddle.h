/* @file   paddle.h
 * @author Yunu Cho, Owen Zhou
 * @date   10 October 2023
 * @brief  Declaration and definitions of the ping pong paddle
 */

#ifndef PADDLE_H
#define PADDLE_H

#include <stdint.h>

#define CENTRE_POS 3
#define PADDLE_SIZE 3

typedef struct {
    uint8_t x; 
    uint8_t y;
    uint8_t face[3];
} Paddle_t;

/* Paddle initialisation and returns paddle object */
Paddle_t paddle_init(void);

/* Change the xy position of the paddle */
void paddle_set_pos(Paddle_t paddle, uint8_t x, uint8_t y);

#endif