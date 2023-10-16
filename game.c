/** @file   game.c
    @author Yunu Cho, Owen Zhou
    @date   10 October 2023
    @brief  Implementation of a ping pong game.
*/

#include "system.h"
#include "pacer.h"
#include "timer.h"
#include "button.h"
#include "navswitch.h"
#include "communications.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "draw.h"

#include <stdlib.h>

/** Defining macros  */
#define MIN_ROUNDS 1
#define MAX_ROUNDS 9
#define X_BOUNDARY 4
#define Y_BOUNDARY 6
#define END_ROUND 126
#define END_GAME 127
#define BALL_SPEED 2

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) < (y) ? (y) : (x))

static uint8_t score = 0;
static uint8_t host = 1;

/** Function implementations  */

/** Initialises all the drivers used for the game.  */
void drivers_init(void)
{
    system_init();
    ir_uart_init();
    button_init();
    navswitch_init();
    timer_init();
    draw_init();
    pacer_init(PACER_RATE);
    tinygl_init(PACER_RATE);
}

/** Start screen of the game.
    @return 1 when the game is ready to start.  */
uint8_t start_game(void)
{
    display_text("Pong - Press Pushbutton to start");

    while ((button_push_event_p(0)) != 1) {
        pacer_wait();
        tinygl_update();
        button_update();
    }
    
    return 1;
}

/** Selects the number of rounds using the navswitch + pushbutton.
    @return number of rounds to be played.  */
uint8_t select_rounds(void)
{
    char ch;
    uint8_t rounds = MIN_ROUNDS;

    while(1) {
        pacer_wait();
        tinygl_update();
        navswitch_update();

        if ((ch = recv_signal()) /*&& ch != NULL*/ && ch >= MIN_ROUNDS+'0' && ch <= MAX_ROUNDS+'0') {
            host = 0;
            return ch - '0';
        }

        if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
            rounds = min(MAX_ROUNDS, rounds + 1);
        }
        
        if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
            rounds = max(MIN_ROUNDS, rounds - 1);
        }
        
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            send_signal(rounds + '0');
            return rounds;
        }

        display_character(rounds + '0');
    }
}

/** Plays a round of pong.
    @return 1 if the player wins, else 0.  */
uint8_t play_round(void)
{
    char ch;
    uint8_t in_motion = 0;
    int16_t tick = 0;

    Paddle_t paddle = paddle_init();
    paddle_set_pos(&paddle, PADDLE_X, PADDLE_Y);

    Ball_t ball = ball_init();
    ball_set_pos(&ball, BALL_X, BALL_Y);

    while(1) {
        pacer_wait();
        tinygl_update();
        navswitch_update();

        if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
            paddle_set_pos(&paddle, paddle.x, max(1, paddle.y - 1));
        }
        
        if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
            paddle_set_pos(&paddle, paddle.x, min(5, paddle.y + 1));
        }
        
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            
            if (!in_motion && host && ball.x == paddle.x - 1 && ball.y >= paddle.y - 1 && ball.y <= paddle.y + 1) {
                ball_set_dir(&ball, -1, ball.vy);
                in_motion = 1;
            }
        
        }

        if (in_motion && host) {
            
            if (ball.x < 0) {
                // send ball to opponent and disable ball display
                host = 0;
                in_motion = 0;
                send_signal((ball.y << 4) + ((ball.vx+1) << 2) + ball.vy+1);
            }
            
            if (ball.x == paddle.x-1 && ball.y >= paddle.y-1 && ball.y <= paddle.y+1) {
                // ball is next to paddle
                if (ball.y == paddle.y-1) {
                    ball_set_dir(&ball, -1, -1);
                } else if (ball.y == paddle.y+1) {
                    ball_set_dir(&ball, -1, 1);
                } else {
                    ball_set_dir(&ball, -1, 0);
                }
            }
            
            if (ball.x > X_BOUNDARY) {
                // lost round and send end round signal to opponent
                send_signal(END_ROUND);
                return 0;
            }
            
            // ball bounces off wall
            if (ball.y >= Y_BOUNDARY) {
                ball_set_pos(&ball, ball.x, Y_BOUNDARY);
                ball_set_dir(&ball, ball.vx, ball.vy * -1);
            }
            
            if (ball.y <= 0) {
                ball_set_pos(&ball, ball.x, 0);
                ball_set_dir(&ball, ball.vx, ball.vy * -1);
            }
        }

        if ((ch = recv_signal())) {
            
            if (host && ch == END_GAME) {
                return 2;
            } else if (!host && ch == END_ROUND) {
                return 1;
            } else if (!host) {
                host = 1;
                in_motion = 1;
                ball_set_pos(&ball, 0, Y_BOUNDARY - (ch >> 4));
                ball_set_dir(&ball, (-(((ch << 4) & 0xFF) >> 6) + 1), (-(((ch << 6) & 0xFF) >> 6)) + 1);
            }
        }

        tick++;
        
        if (tick > PACER_RATE / BALL_SPEED) {
            tick = 0;
            ball_set_pos(&ball, ball.x + ball.vx, ball.y + ball.vy);
        }

        tinygl_clear();
        display_paddle(&paddle);
        
        if (host) {
            display_ball(&ball);
        }   
    }
}

/** Shows the score on the LED matrix.  */
void show_score(void)
{
    int16_t tick = 0;
    
    while (tick < 2 * PACER_RATE) {
        pacer_wait();
        tinygl_update();
        display_character(score + '0');
        tick++;
    }
}

/** Evaluates the winner by comparing the number of rounds with the player's score.
    @param rounds the number of rounds to be played.  */
void evaluate_winner(uint8_t rounds)
{
    if (score == rounds) {
        display_text("WIN");
    } else {
        display_text("LOSE");
    }
    
    while(1) {
        pacer_wait();
        tinygl_update();
    }
}

int main(void)
{
    drivers_init();

    if ((start_game())) {
        uint8_t rounds = select_rounds();
        uint8_t pts;

        while (score != rounds) {
            pts = play_round();
            if (pts == 2) {
                break;
            } else {
                score += pts;
            }
            show_score();
        }
        // send game end signal to break while loop in other funkit
        if (score == rounds) {
            send_signal(END_GAME);
        }
        
        evaluate_winner(rounds);
        
        return 0;
    }
}
