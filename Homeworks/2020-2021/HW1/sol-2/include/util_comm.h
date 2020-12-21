#ifndef UTIL_COMM_H_
#define UTIL_COMM_H_

#include <stdint.h>

/* Rick functions */
void send_squanch(uint8_t c);
uint8_t recv_squanch(void);

/* ATTENTION! DON'T USE THIS! */
void reset_communication(void);

#endif
