#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "test.h"
#include "util_comm.h"
#include "communication.h"


/* Task 1 - The Beginning */

uint8_t test_send_byte(void)
{
    const uint8_t exp_res[] = {18, 9, 3, 11};
    uint8_t c[4];

    /* Clear the communication channel */
    reset_communication();

    /* Morty sending the message */
    send_byte_message();

    c[0] = recv_squanch();
    c[1] = recv_squanch();
    c[2] = recv_squanch();
    c[3] = recv_squanch();

    for (uint32_t i = 0; i < ARRAY_SIZE(exp_res); i++) {
        if (c[i] != exp_res[i]) {
            DEBUG_LOG("Index %u value %u, expected %u", i, c[i], exp_res[i]);
            return 1;
        }
    }

    return 0;
}


uint8_t test_recv_byte(void)
{
    char *buff;
    size_t size;
    int ret = 0;
    FILE *stdout_ref = stdout;
    const uint8_t to_send_enc[] = {13, 15, 18, 20, 25};
    const char exp_res[]    = {'M', 'O', 'R', 'T', 'Y'};

    /* Clear the communication channel */
    reset_communication();

    FILE *stream = open_memstream(&buff, &size);
    if (stream == NULL) {
        fprintf(stderr, "Error allocating the memstream\n");
        return 1;
    }

    stdout = stream;

    /* Rick sends the message */
    for (uint32_t i = 0; i < ARRAY_SIZE(to_send_enc); i++) {
        send_squanch(to_send_enc[i]);
    }

    /* Morty receives the message */
    recv_byte_message();

    fflush(stdout);

    if (size != ARRAY_SIZE(to_send_enc)) {
        ret = 1;
        DEBUG_LOG("Morty printed %lu characters, but there were %lu expected",
                  size, ARRAY_SIZE(to_send_enc));
        goto cleanup;
    }

    for (uint32_t i = 0; i < ARRAY_SIZE(to_send_enc); i++) {
        if (buff[i] != exp_res[i]) {
            DEBUG_LOG("%uth value printed is %c, expected %c",
                      i, buff[i], exp_res[i]);
            ret = 1;
            goto cleanup;
        }
    }

cleanup:

    fclose(stream);
    free(buff);

    stdout = stdout_ref;

    return ret;
}


uint8_t test_comm_byte(void)
{
    uint8_t c1, c2;
    const uint8_t exp_res[] = {16, 18, 15, 7, 18, 1, 13, 1, 18, 5};

    /* Clear the communication channel */
    reset_communication();

    /* Rick sends the message */
    for (uint32_t i = 0; i < ARRAY_SIZE(exp_res); i++) {
        send_squanch(exp_res[i]);
    }

    /* Morty does his stuff */
    comm_byte();

    for (uint32_t i = 0; i < ARRAY_SIZE(exp_res); i++) {
        c1 = recv_squanch();
        c2 = recv_squanch();
        if (c1 != exp_res[i]) {
            DEBUG_LOG("Rick sent %u, but Morty sent back %u", exp_res[i], c1);
            return 1;
        }

        if (c1 != c2) {
            DEBUG_LOG("The received characters should be sent twice");
            DEBUG_LOG("Rick received %u %u", c1, c2);
            return 1;
        }
    }

    return 0;
}


/* Task 2 - Waiting for the Message */

uint8_t test_send_message(void)
{
    const uint8_t exp_res[] = {8, 5, 12, 12, 15, 20, 8, 5, 18, 5};

    /* Clear the communication channel */
    reset_communication();

    /* Morty sends the message */
    send_message();

    uint8_t len = recv_squanch();
    len = (~((~len & (0x0078 + 0x023C + 0x083C - 0x0AB4))
            >> (0x0004 + 0x0202 + 0x0802 - 0x0A06))) & 0xF;

    if (len != 10) {
        DEBUG_LOG("Morty sent %u as length, but expected 10", len);
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        const uint8_t c = recv_squanch();
        if (exp_res[i] != c) {
            DEBUG_LOG("Morty sent %u, but expected to see %u", c, exp_res[i]);
            return 1;
        }
    }

    return 0;
}


uint8_t test_recv_message(void)
{
    char *buff;
    size_t size;
    int ret = 0;
    FILE *stdout_ref = stdout;
    const uint8_t to_send_enc[] = {2, 9, 18, 4, 16, 5, 18, 19, 15, 14};
    const char exp_res[] = {
        'B', 'I', 'R', 'D',
        'P', 'E', 'R', 'S', 'O', 'N'
    };

    const uint8_t len_enc = 235;

    /* Clear the communication channel */
    reset_communication();

    FILE *stream = open_memstream(&buff, &size);
    if (stream == NULL) {
        fprintf(stderr, "Error allocating the memstream\n");
        return 1;
    }

    stdout = stream;

    /* Rick sends the message */

    /* First send the length */
    send_squanch(len_enc);

    /* Then the characters */
    for (int i = 0; i < 10; i++) {
        send_squanch(to_send_enc[i]);
    }

    /* Morty receives the message */
    recv_message();

    fflush(stdout);

    if (size != 12) {
        /*
         * In total there are 12 characters because the
         * message length is "10" <-- 2 chars + the message (10 characters)
         */
        ret = 1;
        DEBUG_LOG("Morty printed %lu characters, but there were %u expected",
                  size, 12);
        goto cleanup;
    }

    if (buff[0] != 49 || buff[1] != 48) {
        DEBUG_LOG("Expected message length %u and printed %c%c", 10, buff[0], buff[1]);
        ret = 1;
        goto cleanup;
    }

    for (uint32_t i = 0; i < 10; i++) {
        if (buff[i+2] != exp_res[i]) {
            DEBUG_LOG("%uth value printed is %c, expected %c\n",
                      i, buff[i+2], exp_res[i]);
            ret = 1;
            goto cleanup;
        }
    }

cleanup:

    fclose(stream);
    free(buff);

    stdout = stdout_ref;

    return ret;
}


uint8_t test_comm_message(void)
{
    /* Rick prepares 4 messages to send */
    const uint8_t to_send_enc[][100] = {
        {16},
        {13, 1, 24, 24, 5},
        {2, 5, 5, 5, 3, 16},
        {1,1,1}
    };

    const uint8_t msg_len[] = {1, 5, 6, 3};
    const uint8_t msg_len_enc[] = {199, 212, 216, 206};

    const uint8_t exp_len[] = {10, 11, 10, 11};
    const uint8_t exp_res[][100] = {
        {16, 9, 3, 11, 12, 5, 18, 9, 3, 11},
        {22, 9, 14, 4, 9, 3, 1, 20, 15, 18, 19},
        {16, 9, 3, 11, 12, 5, 18, 9, 3, 11},
        {22, 9, 14, 4, 9, 3, 1, 20, 15, 18, 19}
    };

    for (int i = 0; i < 4; i++) {
        /* Clear the communication channel */
        reset_communication();

        send_squanch(msg_len_enc[i]);
        for (int j = 0; j < msg_len[i]; j++) {
            send_squanch(to_send_enc[i][j]);
        }

        /* Morty receives the message and sends something else */
        comm_message();

        uint8_t len = recv_squanch();

        len = (~((~len & (0x0078 + 0x023C + 0x083C - 0x0AB4))
                >> (0x0004 + 0x0202 + 0x0802 - 0x0A06))) & 0xF;

        if (len != exp_len[i]) {
            DEBUG_LOG("Index %d expected length %u, but received %u",
                      i, exp_len[i], len);
            return 1;
        }

        for (int j = 0; j < len; j++) {
            uint8_t c = recv_squanch();
            if (exp_res[i][j] != c) {
                DEBUG_LOG("Idx msg %d, idx char %d, expected %u, received %u",
                          i, j, exp_res[i][j], c);
                return 1;
            }
        }
    }

    return 0;
}


/* Task 3 - In the Zone */

uint8_t test_send_squanch2(void)
{
    const uint8_t to_send_enc[][2] = {
        {13, 12}, {10, 6}, {0, 3}, {4, 5}, {7, 14}
    };
    const uint8_t exp_recv[] = {242, 156, 5, 49, 126};

    reset_communication();

    /* Send all the chars */
    for (uint32_t i = 0; i < ARRAY_SIZE(to_send_enc); i++) {
        send_squanch2(to_send_enc[i][0], to_send_enc[i][1]);
    }

    /* Check the result */
    for (uint32_t i = 0; i < ARRAY_SIZE(to_send_enc); i++) {
        uint8_t c = recv_squanch();
        if (exp_recv[i] != c) {
            DEBUG_LOG("Idx %d, expected %u, received %u", i, exp_recv[i], c);
            return 1;
        }
    }

    return 0;
}


uint8_t test_decode_squanch2(void)
{
    const uint8_t to_dec[] = {242, 156, 5, 49, 126};
    const uint8_t chars_enc[][2] = {{13, 12}, {10, 6}, {0, 3}, {4, 5}, {7, 14}};

    for (uint32_t i = 0; i < ARRAY_SIZE(to_dec); i++) {
        uint8_t c = decode_squanch2(to_dec[i]);
        uint8_t exp_dec = (chars_enc[i][0] << 4) + chars_enc[i][1];
        if (exp_dec != c) {
            DEBUG_LOG("Idx %d, expected %u, received %u", i, exp_dec, c);
            return 1;
        }
    }

    return 0;
}


int main(void)
{
    testcase tests[] = {
        /* Task 1 - The Beginning */
        {"test_send_byte", test_send_byte, 4},
        {"test_recv_byte", test_recv_byte, 4},
        {"test_comm_byte", test_comm_byte, 2},

        /* Task 2 - Waiting for the Message */
        {"test_send_message", test_send_message, 4},
        {"test_recv_message", test_recv_message, 4},
        {"test_comm_message", test_comm_message, 2},

        /* Task 3 - In the Zone */
        {"test_send_squanch2", test_send_squanch2, 6},
        {"test_decode_squanch2", test_decode_squanch2, 4}
    };

    for (uint32_t i = 0; i < ARRAY_SIZE(tests); i++) {
        const uint32_t length_test_name = strlen(tests[i].test_name);
        const uint32_t padding_len = TARGET_LENGTH - length_test_name;
        if (tests[i].f() == 0) {
            printf("%s%*.*s%d\n", tests[i].test_name, padding_len,
                   padding_len, PADDING, tests[i].points);
        } else {
            printf("%s%*.*s0\n", tests[i].test_name, padding_len,
                   padding_len, PADDING);
        }
    }
    return 0;
}
