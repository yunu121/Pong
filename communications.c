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
    char c = NULL;
    if (ir_uart_read_ready_p()) {
        c = ir_uart_getc();
    }
    return c;
}

