/** @file   communications.c
    @author Yunu Cho, Owen Zhou
    @date   14 October 2023
    @brief  Functions related to communication systems
*/

#include "communications.h"

/** Sends the given character.
    @param c character that is being sent.  */
void send_signal(char c)
{
    ir_uart_putc(c);
}

/** Receives the received character.
    @return the received character.  */
char recv_signal(void)
{
    char c;
    
    if (ir_uart_read_ready_p()) {
        c = ir_uart_getc();
    }
    
    return c;
}
