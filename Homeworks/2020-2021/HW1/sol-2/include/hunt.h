#ifndef HUNT_H_
#define HUNT_H_

#include <stdint.h>

/* Task 1 - Axii */
uint16_t find_spell(uint64_t memory);
uint16_t find_key(uint64_t memory);
uint16_t decrypt_spell(uint16_t spell, uint16_t key);

/* Task 2 - Sword Play */
uint32_t choose_sword(uint16_t enemy);

/* Task 3 - The Witcher Trials */
uint32_t trial_of_the_grasses(uint16_t cocktail);
uint8_t trial_of_forrest_eyes(uint64_t map);
uint8_t trial_of_the_dreams(uint32_t map);

#endif
