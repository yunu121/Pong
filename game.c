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

#define PACER_RATE 500
#define MIN_ROUNDS 1
#define MAX_ROUNDS 9

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) < (y) ? (y) : (x))

static uint8_t score = 0;
static uint8_t host = 1;

/** Initialises all the drivers used for the game.  */
void drivers_init(void)
{
    system_init();
    ir_uart_init();
    button_init();
    navswitch_init();
    timer_init();
    pacer_init(PACER_RATE);
    tinygl_init(PACER_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(MESSAGE_RATE);
}

/** Start screen of the game.
    @return 1 when the game is ready to start.  */
uint8_t start_game(void)
{
    display_text("Pong - Pushbutton to start");

    while ((button_push_event_p(0)) != 1) {
        pacer_wait();
        tinygl_update();
        button_update();
    }

    return 1;
}

/** Select number of rounds to win using the navswitch.
    @return number of rounds. */
uint8_t select_rounds(void)
{
    char ch;
    uint8_t rounds = MIN_ROUNDS;

    while(1) {
        pacer_wait();
        tinygl_update();
        navswitch_update();

        /* receive number of rounds from opponent */
        if ((ch = recv_signal()) && ch >= MIN_ROUNDS && ch <= MAX_ROUNDS) {
            host = 0;

            return ch;
        }

        if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
            rounds = min(MAX_ROUNDS, rounds + 1);
        }

        if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
            rounds = max(MIN_ROUNDS, rounds - 1);
        }

        /* send number of rounds to opponent */
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            send_signal(rounds);

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
    Ball_t ball = ball_init();

    while(1) {
        pacer_wait();
        tinygl_update();
        navswitch_update();

        /* constrained paddle movement */
        if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
            paddle_set_pos(&paddle, paddle.x, max(paddle.y - 1, 1));
        }

        if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
            paddle_set_pos(&paddle, paddle.x, min(paddle.y + 1, TINYGL_HEIGHT - 2));
        }

        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {

            if (!in_motion && host) {

                if (ball.x == paddle.x - 1 && ball.y >= paddle.y - 1 && ball.y <= paddle.y + 1) {
                    ball_set_dir(&ball, -1, ball.vy);
                    in_motion = 1;
                }

            }

        }

        if (in_motion && host) {

            /* send ball to opponent */
            if (ball.x < 0) {
                host = 0;
                in_motion = 0;
                send_signal(10 * (ball.vy + 1) + ball.y);
            }

            /* ball redirection via paddle */
            if (ball.x == paddle.x - 1) {

                if (ball.y == paddle.y - 1) {
                    ball_set_dir(&ball, -1, -1);
                } else if (ball.y == paddle.y) {
                    ball_set_dir(&ball, -1, 0);
                } else if (ball.y == paddle.y + 1) {
                    ball_set_dir(&ball, -1, 1);
                }

            }

            /* send end round signal to opponent */
            if (ball.x >= TINYGL_WIDTH) {
                send_signal(END_ROUND);
                return 0;
            }

            /* ball bounces off edge */
            if (ball.y >= TINYGL_HEIGHT - 1) {
                ball_set_pos(&ball, ball.x, TINYGL_HEIGHT - 1);
                ball_set_dir(&ball, ball.vx, -1);
            }

            /* ball bounces off other edge */
            if (ball.y <= 0) {
                ball_set_pos(&ball, ball.x, 0);
                ball_set_dir(&ball, ball.vx, 1);
            }

        }

        if ((ch = recv_signal())) {

            if (host && ch == END_GAME) {
                return 2;
            } else if (!host && ch == END_ROUND) {
                return 1;
            } else if (!host && ch >= 0 && ch <= 26) {
                /* receive ball from opponent */
                host = 1;
                in_motion = 1;
                ball_set_pos(&ball, 0, (TINYGL_HEIGHT - 1) - (ch % 10));
                ball_set_dir(&ball, 1, 1 - (ch / 10));
            }

        }

        tick++;

        if (tick > PACER_RATE / BALL_SPEED) {
            tick = 0;
            /* update ball position based on timer */
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
    @param rounds the number of rounds to win.  */
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

        /* send signal to break loop in opponent's funkit */
        if (score == rounds) {
            send_signal(END_GAME);
        }

        evaluate_winner(rounds);

        return 0;
    }
}
