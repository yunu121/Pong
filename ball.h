/* @file   ball.h
 * @author Yunu Cho, Owen Zhou
 * @date   9 October 2023
 * @brief  Declaration and definitions of the ping pong ball
 */

#ifndef BALL_H
#define BALL_H

typedef enum {
    NORTH,
    NORTH_EAST,
    EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    WEST,
    NORTH_WEST
} Direction_t;

typedef struct {
    uint8_t x; 
    uint8_t y;
    Direction dir;
} Ball_t;

/*

Ball_t ball_init(...);            initialise ball object
void ball_set_dir(Ball_t ball);   change the ball direction
void ball_set_pos(Ball_t ball);   change ball x y position

*/

#endif