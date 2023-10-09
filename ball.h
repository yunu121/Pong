/* @file   ball.h
 * @author Yunu Cho, Owen Zhou
 * @date   9 October 2023
 * @brief  Declaration and definitions of the ping pong ball
 */

#ifndef BALL_H
#define BALL_H

typedef struct {
    uint8_t x; 
    uint8_t y;
    int8_t vx;
    int8_t vy;
} Ball_t;

/* Ball initialisation and returns ball object */
Ball_t ball_init(void);

/* Change the xy position of the ball */
void ball_set_pos(Ball_t ball, uint8_t x, uint8_t y);

/* Change the vxy direction of the ball*/
void ball_set_dir(Ball_t ball, int8_t vx, int8_t vy);

#endif