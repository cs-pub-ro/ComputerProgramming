#include "gates.h"

#include <stdio.h>
#include <assert.h>

/* Task 1 - Bit by Bit */

uint8_t get_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);
    return (nr >> (uint64_t)i) & 1;
}

uint64_t flip_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);
    return (nr ^ ((uint64_t)1 << i));
}

uint64_t activate_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);
    return (nr | (((uint64_t)1) << i));
}


uint64_t clear_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);
    return (nr & ~(((uint64_t)1) << i));
}


/* Task 2 - One Gate to Rule Them All */

uint8_t nand_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    return !(a & b);
}


uint8_t and_gate(uint8_t a, uint8_t b)
{
    uint8_t res = nand_gate(a, b);
    return nand_gate(res, res);
}


uint8_t not_gate(uint8_t a)
{
    return nand_gate(a, a);
}


uint8_t or_gate(uint8_t a, uint8_t b)
{
    return nand_gate(not_gate(a), not_gate(b));
}


uint8_t xor_gate(uint8_t a, uint8_t b)
{
    uint8_t res = nand_gate(a, b);
    return nand_gate(nand_gate(a, res), nand_gate(b, res));
}


/* Task 3 - Just Carry the Bit */

uint8_t full_adder(uint8_t a, uint8_t b, uint8_t c)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);
    assert (c == 0 || c == 1);

    uint8_t xor = xor_gate(a, b);
    uint8_t sum = xor_gate(xor, c);
    uint8_t carry = or_gate(and_gate(a, b), and_gate(xor, c));

    return (sum << 4) | carry;
}


uint64_t ripple_carry_adder(uint64_t a, uint64_t b)
{
    uint8_t r = full_adder(a & 1, b & 1, 0);
    uint8_t s = r >> 4;
    uint8_t c = r & 1;
    uint64_t res = s;

    for (int i = 1; i < 64; i++) {
        r = full_adder((a >> i) & 1, (b >> i) & 1, c);
        s = r >> 4;
        c = r & 0x1;

        res =  (s << i) | res;
    }

    if (c) {
	    return 0;
    }

    return res;
}
