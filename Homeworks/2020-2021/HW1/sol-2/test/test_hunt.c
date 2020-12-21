#include "test.h"
#include "hunt.h"

#include <string.h>

/* Task 1 - Axii */

uint8_t test_find_spell(void)
{
    const uint64_t memory[] = {
        2866642101, 753050972331,
        439647, 168135073280488787
    };
    const uint16_t exp_spell[] = {21946, 43690, 13738, 10922};


    for (uint32_t i = 0; i < ARRAY_SIZE(memory); i++) {
        const uint16_t spell = find_spell(memory[i]);
        if (exp_spell[i] != spell) {
            DEBUG_LOG("Index %u expected %u, value %u", i, exp_spell[i], spell);
            return 1;
        }
    }

    return 0;
}


uint8_t test_find_key(void)
{
    const uint64_t memory[] = {
        1833391462, 129010438066947,
        438317376683, 39830094785365
    };
    const uint16_t exp_key[] = {21862, 21849, 402, 13652};


    for (uint32_t i = 0; i < ARRAY_SIZE(memory); i++) {
        const uint16_t key = find_key(memory[i]);
        if (exp_key[i] != key) {
            DEBUG_LOG("Index %u expected %u, value %u", i, exp_key[i], key);
            return 1;
        }
    }

    return 0;
}


uint8_t test_decrypt_spell(void)
{
    const uint16_t key[] = {32321, 13543, 53534, 12312};
    const uint16_t spell_enc[] = {35433, 21432, 25312, 21323};
    const uint16_t exp_spell[] = {62504, 26463, 46078, 25427};

    for (uint32_t i = 0; i < ARRAY_SIZE(key); i++) {
        const uint16_t spell_dec = decrypt_spell(spell_enc[i], key[i]);
        if (exp_spell[i] != spell_dec) {
            DEBUG_LOG("Index %u expected %u, value %u", i, exp_spell[i], spell_dec);
            return 1;
        }
    }

    return 0;
}


/* Task 2 - Sword Play */

uint8_t test_choose_sword(void)
{
    const uint16_t enemy[] = {0xF0, 0xF1, 0x55, 0x36, 0x0, 0xFF, 0x34};
    const uint32_t exp_sword[] = {
        0x90000010, 0x6000FF0F, 0x90000004,
        0x90000002, 0x90000000, 0x90000002, 0x6000FFCC
    };
    const uint32_t mask = 0xF000FFFF;

    for (uint32_t i = 0; i < ARRAY_SIZE(enemy); i++) {
        const uint32_t sword = choose_sword(enemy[i]) & mask;
        if (exp_sword[i] != sword) {
            DEBUG_LOG("Index %u expected 0x%x, value 0x%x", i, exp_sword[i], sword);
            return 1;
        }
    }

    return 0;
}


/* Task 3 - The Witcher Trials */

uint8_t test_trial_of_the_grasses(void)
{
    uint16_t cocktail[] = {0x0, 0xFFFF, 0x3213, 0x5342, 0xABCD, 0xFE45, 0x32FB};
    uint32_t exp_antibodies[] = {
        0, 0xFFFF0000, 0x32130000, 0x53420000,
        0xABCD0000, 0xFE450000, 0x32FB0000,
    };

    for (uint32_t i = 0; i < ARRAY_SIZE(cocktail); i++) {
        const uint32_t antibodies = trial_of_the_grasses(cocktail[i]);
        if (exp_antibodies[i] != antibodies) {
            DEBUG_LOG("Index %u expected 0x%x, value 0x%x",
                      i, exp_antibodies[i], antibodies);
            return 1;
        }
    }


    return 0;
}

uint8_t test_trial_of_forrest_eyes(void)
{
    uint64_t map[] = {
        0x0, 0xFFFEAB032, 0x180000000, 0x3C3E1,
        0xF00F000000F001E0, 0x180000001, -1,
    };
    uint8_t exp_idx[] = {0, 3, 1, 3, 0, 3, 2};

    for (uint32_t i = 0; i < ARRAY_SIZE(map); i++) {
        const uint8_t idx = trial_of_forrest_eyes(map[i]);
        if (exp_idx[i] != idx) {
            DEBUG_LOG("Index %u expected 0x%x, value 0x%x", i, exp_idx[i], idx);
            return 1;
        }
    }


    return 0;
}


uint8_t test_trial_of_the_dreams(void)
{
    uint32_t map[] = {3, 5, 0x80000001, 0x1080, 0x20000020};
    uint8_t exp_dist[] = {1, 2, 31, 5, 24};

    for (uint32_t i = 0; i < ARRAY_SIZE(map); i++) {
        const uint8_t len = trial_of_the_dreams(map[i]);
        if (exp_dist[i] != len) {
            DEBUG_LOG("Index %u expected %u, value %u", i, exp_dist[i], len);
            return 1;
        }
    }


    return 0;
}


int main(void)
{
    testcase tests[] = {
        /* Task 1 - Axii */
        {"test_find_spell", test_find_spell, 4},
        {"test_find_key", test_find_key, 4},
        {"test_decrypt_spell", test_decrypt_spell, 2},

        /* Task 2 - Sword Play */
        {"test_choose_sword", test_choose_sword, 10},


        /* Task 3 - The Witcher Trials */
        {"test_trial_of_the_grasses", test_trial_of_the_grasses, 4},
        {"test_trial_of_forrest_eyes", test_trial_of_forrest_eyes, 4},
        {"test_trial_of_the_dreams", test_trial_of_the_dreams, 2},

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
