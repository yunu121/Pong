/** @file   communications.c
    @author Yunu Cho, Owen Zhou
    @date   14 October 2023
    @brief  Functions related to communication systems
*/

#include "communications.h"

/** Sends the given character.
    @param c character that is being sent.  */
void send_signal(char ch)
{
    ir_uart_putc(ch);
}

/** Receives the received character.
    @return the received character.  */
char recv_signal(void)
{
    char ch = '\0';

    if (ir_uart_read_ready_p()) {
        ch = ir_uart_getc();
    }

    return ch;
}
