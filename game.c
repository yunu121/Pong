/** @file   game.c
    @author Yunu Cho, Owen Zhou
    @date   10 October 2023
    @brief  Implementation of a ping pong game.
*/

#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "communications.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "draw.h"

#include <stdlib.h>

/** Defining macros  */
#define PACER_RATE 500
#define MESSAGE_RATE 10
#define MIN_ROUNDS 1
#define MAX_ROUNDS 9
#define X_BOUNDARY 4
#define Y_BOUNDARY 6

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) < (y) ? (y) : (x))

static uint8_t score = 0;
static uint8_t host = 1;

/** Function implementations  */

/** Initialises display related modules.  */
void draw_init(void)
{
    tinygl_init(PACER_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(MESSAGE_RATE);
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

        if ((ch = recv_signal()) && ch != NULL) {
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
    uint8_t in_motion = 0;
    int16_t tick = 0;

    Paddle_t paddle = paddle_init();
    paddle_set_pos(&paddle, PADDLE_X, PADDLE_Y);

    Ball_t ball = ball_init();
    ball_set_pos(&ball, BALL_X, BALL_Y);

    char ch;
    uint8_t num;

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
            if (!in_motion && host) {
                ball_set_dir(&ball, -1, ball.vy);
                in_motion = 1;
            }
        }

        if (in_motion && host) {
            if (ball.x < 0) {
                // send ball to opponent and disable ball display
                // ball_set_dir(&ball, 1, ball.vy);
                host = 0;
                num = ball.y << 4;
                num += (-ball.vx << 2);
                num += ball.vy;
                send_signal(num);
            }
            if (ball.x == paddle.x-1 && ball.y >= paddle.y-1 && ball.y <= paddle.y+1) {
                // ball is next to paddle
                ball_set_dir(&ball, -1, ball.vy);
            }
            if (ball.x > 4) {
                // lost round and send end round signal to opponent
                return 0;
            }
        } else if (!host) {
            ch = recv_signal();
            if (ch != NULL) {
                host = 1;
                ball.x = 0;
                ball.y = ch >> 4;
                ball.vx = (ch << 4) >> 6;
                ball.vy = (ch << 6) >> 6;
            }
        }



        tick++;
        if (tick > PACER_RATE) {
            tick = 0;
            ball_set_pos(&ball, ball.x+ball.vx, ball.y+ball.vy);
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
        display_text("YOU WIN!");
    } else {
        display_text("YOU LOSE!");
    }
}

int main(void)
{
    system_init();
    ir_uart_init();
    navswitch_init();
    timer_init();
    draw_init();
    pacer_init(PACER_RATE);
    tinygl_init(PACER_RATE);

    uint8_t rounds = select_rounds();

    while(score != rounds) {
        show_score();
        score += play_round();
    }
    // send game end signal to break while loop in other funkit
    // display win or lose

    // test code
    while(1) {
        pacer_wait();
        tinygl_update();
        display_character(score + '0');
    }
}
