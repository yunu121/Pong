/** @file   game.c
    @author Yunu Cho, Owen Zhou
    @date   10 October 2023
    @brief  Implementation of a ping pong game.
*/

#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "draw.h"

#include <stdlib.h>

/** Defining macros  */
#define PACER_RATE 500
#define MESSAGE_RATE 10
#define MIN_ROUNDS 1
#define MAX_ROUNDS 9

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) < (y) ? (y) : (x))

static uint8_t score = 0;

/** Function implementations  */

/** Initialises display related modules.  */
void draw_init(void)
{
    tinygl_init(PACER_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(MESSAGE_RATE);
}

/** Return the received character.  */
char recv_signal(void)
{
    char c = NULL;
    if (ir_uart_read_ready_p()) {
        c = ir_uart_getc();
    }
    return c;
}

/** Sends the given character.
    @param c character that is being sent.  */
void send_signal(char c)
{
    ir_uart_putc(c);
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
    Paddle_t paddle = paddle_init();
    paddle_set_pos(&paddle, PADDLE_X, PADDLE_Y);

    Ball_t ball = ball_init();
    ball_set_pos(&ball, 3, 3);

    int64_t tick = 0;
    while(1) {
        pacer_wait();
        tinygl_update();
        navswitch_update();

        if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
            paddle_set_pos(&paddle, paddle.x, max(1, paddle.y-1));
        }
        if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
            paddle_set_pos(&paddle, paddle.x, min(5, paddle.y+1));
        }
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            if (ball.vx == 0 && ball.vy == 0 && paddle.y > 1 && paddle.y < 5) {
                ball.vx--;
            }
        }

        tick++;
        if (tick > PACER_RATE) {
            tick = 0;
            ball_set_pos(&ball, ball.x+ball.vx, ball.y+ball.vy);
        }

        tinygl_clear();
        display_paddle(&paddle);
        display_ball(&ball);
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
        // display score for 1s
        score += play_round();
    }
    // send game end signal to break while loop in other funkit
    // display win or lose

    // test code
    while(1) {
        pacer_wait();
        tinygl_update();
        display_character('W');
    }
}
