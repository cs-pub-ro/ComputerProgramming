#include "communication.h"
#include "util_comm.h"

#include <stdio.h>


/* Task 1 - The Beginning */

void send_byte_message(void)
{
    /*
     * Send the encoding of the characters: R, I, C, K
     */
    send_squanch('R' - 'A' + 1);
    send_squanch('I' - 'A' + 1);
    send_squanch('C' - 'A' + 1);
    send_squanch('K' - 'A' + 1);
}


void recv_byte_message(void)
{
    /*
     * Receive 5 encoded characters, decode them and print
     * them to the standard output (as characters)
     */
    uint8_t c;

    c = recv_squanch() + 'A' - 1;
    printf("%c", c);

    c = recv_squanch() + 'A' - 1;
    printf("%c", c);

    c = recv_squanch() + 'A' - 1;
    printf("%c", c);

    c = recv_squanch() + 'A' - 1;
    printf("%c", c);

    c = recv_squanch() + 'A' - 1;
    printf("%c", c);
}


void comm_byte(void)
{
    /*
     * Receive 10 encoded characters and send each character (the character is
     * still encoded) 2 times
     */
    uint8_t c;

    for (int i = 0; i < 10; i++) {
        c = recv_squanch();
        send_squanch(c);
        send_squanch(c);
    }
}


/* Task 2 - Waiting for the Message */

void send_message(void)
{
    /*
     * Send the message: HELLOTHERE
     * - send the encoded length
     * - send each character encoded
     */

    char chars_to_send[] = "HELLOTHERE";
    uint8_t len = 10 << 2;

    send_squanch(len);

    for (int i = 0; i < 10; i++) {
        send_squanch(chars_to_send[i] - 'A' + 1);
    }
}


void recv_message(void)
{
    /*
     * Receive a message:
     * - the first value is the encoded length
     * - length x encoded characters
     * - print each decoded character
     */
    uint8_t len;

    len = recv_squanch();
    len = (len & (0xF << 2)) >> 2;

    printf("%u", len);
    for (int i = 0; i < len; i++) {
        uint8_t c = recv_squanch() + 'A' - 1;
        printf("%c", c);
    }
}


void comm_message(void)
{
    /*
     * Receive a message from Rick and do one of the following depending on the
     * last character from the message:
     * - 'P' - send back PICKLERICK
     * - anything else - send back VINDICATORS
     * You need to send the messages as you did at the previous tasks:
     * - encode the length and send it
     * - encode each character and send them
     */
    uint8_t len;
    uint8_t c;

    char *m1 = "PICKLERICK";
    char *m2 = "VINDICATORS";
    char *m;

    len = recv_squanch();
    len = (len & (0xF << 2)) >> 2;

    for (int i = 0; i < len; i++) {
        c = recv_squanch() + 'A' - 1;
    }

    if (c == 'P') {
        len = 10;
        m = m1;
    } else {
        len = 11;
        m = m2;
    }

    send_squanch(len << 2);
    for (int i = 0; i < len; i++) {
        send_squanch(m[i] - 'A' + 1);
    }
}


/* Task 3 - In the Zone */

void send_squanch2(uint8_t c1, uint8_t c2)
{
    /*
     * Step:
     * - "merge" the character encoded in c1 and the character encoded in c2
     * - use send_squanch to send the newly formed byte
     */

    uint8_t res = 0;

    for (int i = 0; i < 4; i++) {
        res |= ((c1 >> i) & 1) << (2 * i + 1);
    }

    for (int i = 0; i < 4; i++) {
        res |= ((c2 >> i) & 1) << (2 * i);
    }

    send_squanch(res);
}


uint8_t decode_squanch2(uint8_t c)
{
    /*
     * Decode the given byte:
     * - split the two characters as in the image from ocw.cs.pub.ro
     */

    uint8_t res = 0;


    for (int i = 0; i < 4; i++) {
        res |= ((c >> (2 * i + 1)) & 1) << i;
    }

    res = res << 4;
    for (int i = 0; i < 4; i++) {
        res |= ((c >> (2 * i)) & 1) << i;
    }

    return res;
}
