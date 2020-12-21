#ifndef GATES_H_
#define GATES_H_

#include <stdint.h>

/* Task 1 - Bit by Bit */
uint8_t get_bit(uint64_t nr, uint8_t i);
uint64_t flip_bit(uint64_t nr, uint8_t i);
uint64_t activate_bit(uint64_t nr, uint8_t i);
uint64_t clear_bit(uint64_t nr, uint8_t i);

/* Task 2 - One Gate to Rule Them All */
uint8_t nand_gate(uint8_t a, uint8_t b); /* Already implemented */

uint8_t not_gate(uint8_t a);
uint8_t or_gate(uint8_t a, uint8_t b);
uint8_t and_gate(uint8_t a, uint8_t b);
uint8_t xor_gate(uint8_t a, uint8_t b);

/* Task 3 - Just Carry the Bit */
uint8_t full_adder(uint8_t a, uint8_t b, uint8_t c);
uint64_t ripple_carry_adder(uint64_t a, uint64_t b);

#endif
