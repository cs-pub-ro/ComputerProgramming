#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdint.h>

/* Task 1 - The Beginning */
void send_byte_message(void);
void recv_byte_message(void);
void comm_byte(void);

/* Task 2 - Waiting for the Message */
void send_message(void);
void recv_message(void);
void comm_message(void);

/* Task 3 - In the Zone */
void send_squanch2(uint8_t c1, uint8_t c2);
uint8_t decode_squanch2(uint8_t c);

#endif
