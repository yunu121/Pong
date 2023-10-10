/* @file   game.c
 * @author Yunu Cho, Owen Zhou
 * @date   10 October 2023
 * @brief  Implementation of a ping pong game.
 */

#include "system.h"
#include "ir_uart.h"
#include "ball.h"
#include "paddle.h"

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

int main (void)
{
    system_init ();
    ir_uart_init ();

    while (1)
    {



    }
}
