/* @file   game.c
 * @author Yunu Cho, Owen Zhou
 * @date   10 October 2023
 * @brief  Implementation of a ping pong game.
 */

#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "draw.h"

#include <stdlib.h>

/* macros */
#define PACER_RATE 500
#define MESSAGE_RATE 10

#define MIN_ROUNDS 1
#define MAX_ROUNDS 9

static uint8_t score = 0;

/* function implementations */

void draw_init(void)
{
    tinygl_init(PACER_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
}

char recv_signal(void)
{
    char c = NULL;
    if (ir_uart_read_ready_p()) {
        c = ir_uart_getc();
    }
    return c;
}

void send_signal(char c)
{
    ir_uart_putc(c);
}

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
            rounds = (rounds == MAX_ROUNDS) ? MAX_ROUNDS : rounds + 1;
        }
        if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
            rounds = (rounds == MIN_ROUNDS) ? MIN_ROUNDS : rounds - 1;
        }
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            send_signal(rounds + '0');
            return rounds;
        }

        display_character(rounds + '0');
    }
}

uint8_t play_round(void)
{
    return 1;
}

// void evaluate_winner(void)
// {
//     if (score == rounds) {
//         display_text("YOU WIN!");
//     } else {
//         display_text("YOU LOSE!");
//     }
// }

int main(void)
{
    system_init();
    ir_uart_init();
    navswitch_init();
    timer_init();
    draw_init();
    pacer_init(PACER_RATE);

    uint8_t rounds = select_rounds();


    while(1) {
        pacer_wait();
        tinygl_update();
        display_character(rounds+'0');
    }

    // while (score != rounds) {
    //     // print current score standings for a brief moment
    //     score += play_round();
    //     // if recv game end signal then break loop (print your score)
    //     // assume opponent score is equal to rounds
    // }


    // conclude with win and lose etc
}
