#include "util_comm.h"

#include <string.h>

static char s_message[100];
static uint8_t s_recv_idx;
static uint8_t s_send_idx;

void send_squanch(uint8_t c)
{
    s_message[s_send_idx++] = c;
}

uint8_t recv_squanch(void)
{
    return s_message[s_recv_idx++];
}

/* ATTENTION! Don't use this */
void reset_communication(void)
{
    memset(s_message, 0, sizeof s_message);
    s_recv_idx = s_send_idx = 0;
}
