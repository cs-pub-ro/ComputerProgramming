#include "communication.h"
#include "util_comm.h"

#include <stdio.h>


/* Task 1 - The Beginning */

void send_byte_message(void)
{
    /* TODO
     * Send the encoding of the characters: R, I, C, K
     */
    send_squanch('R' - 'A' + 1);
    send_squanch('I' - 'A' + 1);
    send_squanch('C' - 'A' + 1);
    send_squanch('K' - 'A' + 1);
}


void recv_byte_message(void)
{
    /* TODO
     * Receive 5 encoded characters, decode them and print
     * them to the standard output (as characters)
     *
     * ATTENTION!: Use fprintf(stdout, ...)
     */
    for (int i = 0; i < 5; i++) {
        uint8_t ch = recv_squanch();
        fprintf(stdout, "%c", ch + 'A' - 1);
    }
}


void comm_byte(void)
{
    /* TODO
     * Receive 10 encoded characters and send each character (the character is
     * still encoded) 2 times
     */
    for (int i = 0; i < 10; i++) {
        uint8_t ch = recv_squanch();
        send_squanch(ch);
        send_squanch(ch);
    }
}


/* Task 2 - Waiting for the Message */

void send_message(void)
{
    /* TODO
     * Send the message: HELLOTHERE
     * - send the encoded length
     * - send each character encoded
     */
    uint8_t length = (10 << 2);
    send_squanch(length);
    send_squanch('H' - 'A' + 1);
    send_squanch('E' - 'A' + 1);
    send_squanch('L' - 'A' + 1);
    send_squanch('L' - 'A' + 1);
    send_squanch('O' - 'A' + 1);
    send_squanch('T' - 'A' + 1);
    send_squanch('H' - 'A' + 1);
    send_squanch('E' - 'A' + 1);
    send_squanch('R' - 'A' + 1);
    send_squanch('E' - 'A' + 1);
}


void recv_message(void)
{
    /* TODO
     * Receive a message:
     * - the first value is the encoded length
     * - length x encoded characters
     * - print each decoded character
     * 
     * ATTENTION!: Use fprintf(stdout, ...)
     */
    uint8_t length = (recv_squanch() >> 2) & ((1 << 4) - 1);
    fprintf(stdout, "%d", length);
    for (int i = 0; i < length; i++) {
        uint8_t ch = recv_squanch();
        fprintf(stdout, "%c", ch + 'A' - 1);
    }
}


void comm_message(void)
{
    /* TODO
     * Receive a message from Rick and do one of the following depending on the
     * last character from the message:
     * - 'P' - send back PICKLERICK
     * - anything else - send back VINDICATORS
     * You need to send the messages as you did at the previous tasks:
     * - encode the length and send it
     * - encode each character and send them
     */
    uint8_t length = (recv_squanch() >> 2) & ((1 << 4) - 1);
    uint8_t ch;
    for (int i = 0; i < length; i++) {
        ch = recv_squanch();
    }

    if (ch + 'A' - 1 == 'P') {
        uint8_t length = (10 << 2);
        send_squanch(length);
        send_squanch('P' - 'A' + 1);
        send_squanch('I' - 'A' + 1);
        send_squanch('C' - 'A' + 1);
        send_squanch('K' - 'A' + 1);
        send_squanch('L' - 'A' + 1);
        send_squanch('E' - 'A' + 1);
        send_squanch('R' - 'A' + 1);
        send_squanch('I' - 'A' + 1);
        send_squanch('C' - 'A' + 1);
        send_squanch('K' - 'A' + 1);
    } else {
        uint8_t length = (11 << 2);
        send_squanch(length);
        send_squanch('V' - 'A' + 1);
        send_squanch('I' - 'A' + 1);
        send_squanch('N' - 'A' + 1);
        send_squanch('D' - 'A' + 1);
        send_squanch('I' - 'A' + 1);
        send_squanch('C' - 'A' + 1);
        send_squanch('A' - 'A' + 1);
        send_squanch('T' - 'A' + 1);
        send_squanch('O' - 'A' + 1);
        send_squanch('R' - 'A' + 1);
        send_squanch('S' - 'A' + 1);
    }
}


/* Task 3 - In the Zone */

void send_squanch2(uint8_t c1, uint8_t c2)
{
    /* TODO
     * Steps:
     * - "merge" the character encoded in c1 and the character encoded in c2
     * - use send_squanch to send the newly formed byte
     */
    uint8_t message = 0;
    c1 &= 15;
    c2 &= 15;
    for (int i = 0; i < 4; i++) {
        message += ((c2 & (1 << i)) << i)  + ((c1 & (1 << i)) << (i + 1));
    }

    send_squanch(message);
}


uint8_t decode_squanch2(uint8_t c)
{
    /*
     * Decode the given byte:
     * - split the two characters as in the image from ocw.cs.pub.ro
     */

    uint8_t res = -1;

    /* TODO */
    res = 0;
    uint8_t c1 = 0, c2 = 0;
    for (int i = 0; i < 4; i++) {
        c1 += (c & (1 << (2 * i + 1))) >> (i + 1);
        c2 += (c & (1 << (2 * i))) >> i;
    }

    res = (c1 << 4) + c2;

    return res;
}
