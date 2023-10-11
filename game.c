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

#include <stdlib.h>

/* macros */
#define PACER_RATE 500
#define MESSAGE_RATE 10

#define MIN_ROUNDS 1
#define MAX_ROUNDS 9

/* function implementations */

char recv_signal(void)
{
    char c;
    if (ir_uart_read_ready_p()) {
        c = ir_uart_getc();
    }
    return c;
}

void send_signal(char c)
{
    ir_uart_putc(c);
}

void display_character(char c)
{
    char buffer[2];

    buffer[0] = c;
    buffer[1] = '\0';
    tinygl_text(buffer);
}

uint8_t select_host(void)
{
    //uint8_t host;
    int8_t num = rand_r(timer_get()) % 10;
    //char ch = NULL;

    while(1) {
        pacer_wait();
        tinygl_update();
        display_character(num+'0');
        //ch = recv_signal();
        //send_signal(num+'0');
    }
    /*
    if (ch-'0' < num) {
        host = 1;
    } else {
        host = 0;
    }
    */
    /* or just do
    host = ch - '0' < num ? 1 : 0
    */
    return 1;
}

uint8_t pre_phase(void)
{
    uint8_t rounds = MIN_ROUNDS;
    while(1) {
        pacer_wait();
        tinygl_update();
        navswitch_update();

        if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
            rounds = (rounds == MAX_ROUNDS) ? MAX_ROUNDS : rounds+1;
        }
        if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
            rounds = (rounds == MIN_ROUNDS) ? MIN_ROUNDS : rounds-1;
        }
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            return rounds;
        }

        display_character(rounds+'0');
    }
}

int main (void)
{
    system_init();
    ir_uart_init ();
    navswitch_init();

    tinygl_init(PACER_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    pacer_init(PACER_RATE);

    uint8_t host = select_host();
    //uint8_t rounds = pre_phase();
    // round_phase();
    // post_phase();
}
