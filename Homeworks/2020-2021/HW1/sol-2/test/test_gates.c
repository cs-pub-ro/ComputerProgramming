#include <stdio.h>
#include <math.h>
#include <string.h>

#include "test.h"
#include "gates.h"


/* Task 1 - Bit by Bit */
uint8_t test_get_bit_all_one(void)
{
    const uint64_t test_value = UINT64_MAX;

    for (int i = 0; i < 64; i++) {
        const uint8_t bit_state = get_bit(test_value, i);
        if (bit_state != 1) {
            DEBUG_LOG("bit %d value 0, expected 1", i);
            return 1;
        }
    }

    return 0;
}

uint8_t test_get_bit_all_zero(void)
{
    const uint64_t test_value = 0;

    for (int i = 0; i < 64; i++) {
        const uint8_t bit_state = get_bit(test_value, i);
        if (bit_state != 0) {
            DEBUG_LOG("bit %d value 1, expected 0", i);
            return 1;
        }
    }

    return 0;
}


uint8_t test_get_bit_custom(void)
{
    const uint64_t test_value = 0xDEADBEEFDEADBEEF;
    uint64_t reconstructed_value = 0;

    for (int i = 0; i < 64; i++) {
        const uint8_t bit_state = get_bit(test_value, i);
        reconstructed_value += (uint64_t)powl(2, i) * bit_state;
    }

    if (reconstructed_value != test_value) {
        DEBUG_LOG("value %lu, expected %lu", reconstructed_value, test_value);
        return 1;
    }

    return 0;
}


/* flip_bit tests */
uint8_t test_flip_bit_all(void)
{
    uint64_t test_value = UINT64_MAX;

    for (uint8_t i = 0; i < 64; i++) {
        test_value = flip_bit(test_value, i);
    }

    if (test_value != 0) {
        DEBUG_LOG("value %lu, expected 0", test_value);
        return 1;
    }

    return 0;
}


uint8_t test_flip_bit_half(void)
{
    const uint64_t test_value = 0xAABBCCDDEEFF0011;
    uint64_t tmp = test_value;

    for (int i = 0; i < 32; i++) {
        tmp = flip_bit(tmp, i);
        const uint8_t exp_value = ((uint64_t)powl(2, i) & test_value) != 0;
        const uint8_t test_value = ((uint64_t)powl(2, i) & tmp) != 0;
        if (exp_value == test_value) {
            DEBUG_LOG("bit %u value %u, expected %u",
                      i, test_value, 1 - test_value);
            return 1;
        }
    }

    return 0;
}


uint8_t test_flip_bit_all_bits(void)
{
    const uint64_t test_value = 0xDEADBEEFFFFFFFFF;
    uint64_t tmp = test_value;

    for (uint32_t i = 0; i < 64; i++) {
        tmp = flip_bit(tmp, i);
        const uint8_t exp_value = ((uint64_t)powl(2, i) & test_value) != 0;
        const uint8_t test_value = ((uint64_t)powl(2, i) & tmp) != 0;
        if (exp_value == test_value) {
            DEBUG_LOG("bit %u value %u, expected %u", i, test_value, 1 - test_value);
            return 1;
        }
    }

    return 0;
}


/* activate_bit tests */
uint8_t test_activate_bit_all(void)
{
    uint64_t test_value = 0;

    for (uint8_t i = 0; i < 64; i++) {
        test_value = activate_bit(test_value, i);
    }

    if (test_value != UINT64_MAX) {
        DEBUG_LOG("value %lu, expected %lu", test_value, UINT64_MAX);
        return 1;
    }

    return 0;
}


uint8_t test_activate_bit_half(void)
{
    uint64_t test_value = 0xFFFFFFFF;

    for (int i = 32; i < 64; i++) {
        test_value = activate_bit(test_value, i);
    }

    if (test_value != UINT64_MAX) {
        DEBUG_LOG("value %lu, expected %lu", test_value, UINT64_MAX);
        return 1;
    }

    return 0;
}


uint8_t test_activate_bit_custom(void)
{
    const uint8_t bits_to_activate[] = {57, 23, 56, 10, 61, 5, 50, 15};
    const uint64_t exp_value[] = {
        0xDEADBEEFFFF000FF, 0xDEADBEEFFFF000FF, 0xDFADBEEFFFF000FF,
        0xDEADBEEFFFF004FF, 0xFEADBEEFFFF000FF, 0xDEADBEEFFFF000FF,
        0xDEADBEEFFFF000FF, 0xDEADBEEFFFF080FF,
    };
    const uint64_t test_value = 0xDEADBEEFFFF000FF;

    for (uint32_t i = 0; i < ARRAY_SIZE(bits_to_activate); i++) {
        const uint64_t value = activate_bit(test_value, bits_to_activate[i]);
        if (value != exp_value[i]) {
            DEBUG_LOG("activate bit %u value %lx, expected %lx",
                      bits_to_activate[i], value, exp_value[i]);
            return 1;
        }
    }

    return 0;
}


/* clear_bit tests */
uint8_t test_clear_bit_all(void)
{
    uint64_t test_value = UINT64_MAX;

    for (uint8_t i = 0; i < 64; i++) {
        test_value = clear_bit(test_value, i);
    }

    if (test_value != 0) {
        return 1;
    }

    return 0;
}


uint8_t test_clear_bit_half(void)
{
    uint64_t test_value = 0xFFFFFFFF0000;

    for (int i = 16; i < 48; i++) {
        test_value = clear_bit(test_value, i);
    }

    if (test_value != 0) {
        DEBUG_LOG("value %lu, expected 0", test_value);
        return 1;
    }

    return 0;
}


uint8_t test_clear_bit_custom(void)
{
    const uint8_t bits_to_clear[] = {57, 23, 56, 55, 61, 0, 60, 57};
    const uint64_t test_value = 0xDEADBEEFFFFFFFFF;
    const uint64_t exp_value[] = {
        0xDCADBEEFFFFFFFFF, 0xDEADBEEFFF7FFFFF, 0xDEADBEEFFFFFFFFF,
        0xDE2DBEEFFFFFFFFF, 0xDEADBEEFFFFFFFFF,
        0xDEADBEEFFFFFFFFE, 0xCEADBEEFFFFFFFFF, 0xDCADBEEFFFFFFFFF
    };

    for (uint32_t i = 0; i < ARRAY_SIZE(bits_to_clear); i++) {
        const uint64_t value = clear_bit(test_value, bits_to_clear[i]);
        if (exp_value[i] != value) {
            DEBUG_LOG("bit %u value 0x%lx, expected 0x%lx",
                      bits_to_clear[i], value, exp_value[i]);
            return 1;
        }
    }

    return 0;
}


/* gates tests */
uint8_t test_and_gate(void)
{
    uint8_t res = and_gate(0, 0);
    if (res >= 1) {
        DEBUG_LOG("val %u, expected 0", res);
        return 1;
    }

    res = and_gate(0, 1);
    if (res >= 1) {
        DEBUG_LOG("val %u, expected 0", res);
        return 1;
    }

    res = and_gate(1, 0);
    if (res >= 1) {
        DEBUG_LOG("val %u, expected 0", res);
        return 1;
    }

    res = and_gate(1, 1);
    if (res == 0 || res > 1) {
        DEBUG_LOG("val %u, expected 1", res);
        return 1;
    }

    return 0;
}


uint8_t test_not_gate(void)
{
    uint8_t res = not_gate(0);
    if (res == 0 || res > 1) {
        DEBUG_LOG("val %u, expected 1", res);
        return 1;
    }

    res = not_gate(1);
    if (res >= 1) {
        DEBUG_LOG("val %u, expected 0", res);
        return 1;
    }

    return 0;
}


uint8_t test_or_gate(void)
{
    uint8_t res = or_gate(0, 0);
    if (res >= 1) {
        DEBUG_LOG("val %u, expected 0", res);
        return 1;
    }

    res = or_gate(0, 1);
    if (res == 0 || res > 1) {
        DEBUG_LOG("val %u, expected 1", res);
        return 1;
    }

    res = or_gate(1, 0);
    if (res == 0 || res > 1) {
        DEBUG_LOG("val %u, expected 1", res);
        return 1;
    }

    res = or_gate(1, 1);
    if (res == 0 || res > 1) {
        DEBUG_LOG("val %u, expected 1", res);
        return 1;
    }

    return 0;
}


uint8_t test_xor_gate(void)
{
    uint8_t res = xor_gate(0, 0);
    if (res >= 1) {
        DEBUG_LOG("val %u, expected 0", res);
        return 1;
    }

    res = xor_gate(0, 1);
    if (res == 0 || res > 1) {
        DEBUG_LOG("val %u, expected 1", res);
        return 1;
    }

    res = xor_gate(1, 0);
    if (res == 0 || res > 1) {
        DEBUG_LOG("val %u, expected 1", res);
        return 1;
    }

    res = xor_gate(1, 1);
    if (res >= 1) {
        DEBUG_LOG("val %u, expected 0", res);
        return 1;
    }

    return 0;
}


/* ripple carry adder */
uint8_t test_ripple_carry_adder(void)
{
    const uint64_t op1[]    = {0, 532, 5432, 5613, 1, UINT64_MAX, UINT64_MAX};
    const uint64_t op2[]    = {0, 523, 3213, 231, 999, 1, UINT64_MAX};
    const uint64_t exp_res[] = {0, 1055, 8645, 5844, 1000, 0, 0};

    for (uint32_t i = 0; i < ARRAY_SIZE(op1); i++) {
        const uint64_t value = ripple_carry_adder(op1[i], op2[i]);
        if (value != exp_res[i]) {
            DEBUG_LOG("index %u value %lu, expected %lu", i, value, exp_res[i]);
            return 1;
        }
    }

    return 0;
}


int main(void)
{
    testcase tests[] = {
        /* Task 1 - Bit by Bit */

        /* Get Bit */
        {"test_get_bit_all_one", test_get_bit_all_one, 1},
        {"test_get_bit_all_zero", test_get_bit_all_zero, 1},
        {"test_get_bit_custom", test_get_bit_custom, 1},

        /* Flip Bit */
        {"test_flip_bit_all", test_flip_bit_all, 1},
        {"test_flip_bit_half", test_flip_bit_half, 1},
        {"test_flip_bit_custom", test_flip_bit_all_bits, 1},

        /* Activate Bit */
        {"test_activate_bit_all", test_activate_bit_all, 1},
        {"test_activate_bit_half", test_activate_bit_half, 1},
        {"test_activate_bit_custom", test_activate_bit_custom, 1},

        /* Negate Bit */
        {"test_clear_bit_all", test_clear_bit_all, 1},
        {"test_clear_bit_half", test_clear_bit_half, 1},
        {"test_clear_bit_custom", test_clear_bit_custom, 1},

        /* Task 2 - One gate to rull them all */
        {"test_and_gate", test_and_gate, 2},
        {"test_not_gate", test_not_gate, 2},
        {"test_or_gate", test_or_gate, 2},
        {"test_xor_gate", test_xor_gate, 2},

        /* Task 3 - Just Carry the Bit */
        {"test_ripple_carry_adder", test_ripple_carry_adder, 10},
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
