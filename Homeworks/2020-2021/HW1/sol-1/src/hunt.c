#include "hunt.h"
#include <stdio.h>

/* Task 1 - Axii */

uint16_t find_spell(uint64_t memory)
{
    /*
     * TODO
     * The spell is 16 bits and they are in the memory exactly after 5
     * consecutive bits of 1:
     *
     *            high                                             low
     *                [][][x]...[x][x][x][1][1][1][1][1][]...[][][]
     *                    |----16bits---|
     *                          Spell
     *
     * There is only 1 group of 5 consecutive bits of 1 and the other groups
     * have less than consecutive bits of 1
     * Steps:
     * - find 5 consecutive bits of 1
     * - the NEXT 16 bits == spell
     */

    int nr = 0;
    uint64_t one = 1;

    for (int i = 0; i < 64; i++) {
        if (memory & (one << i)) {
            nr++;
        } else {
            nr = 0;
        }

        if (nr == 5) {
            return (memory >> (i + 1)) & 0xFFFF;
        }
    }

    return 0;
}


uint16_t find_key(uint64_t memory)
{
    /*
     * TODO
     * The key is 16 bits and they are in the memory exactly before 3
     * consecutive bits of 1:
     *
     *                high                                           low
     *                     [][][][1][1][1][x][x]...[x][x][]...[][][]
     *                                    |----16bits---|
     *                                           Key
     * Steps:
     * - find 3 consecutive bits of 1
     * - the PREVIOUS 16 bits == key
     */

    int nr = 0;
    uint64_t one = 1;

    for (int i = 0; i < 64; i++) {
        if (memory & (one << i)) {
            nr++;
        } else {
            nr = 0;
        }

        if (nr == 3) {
            return (memory >> (i - 18)) & 0xFFFF;
        }
    }

    return 0;
}


uint16_t decrypt_spell(uint16_t spell, uint16_t key)
{
    /*
     * TODO
     * Find the spell knowing that
     * spell_encrypted = spell_plaintext ^ key
     */

    return spell ^ key;
}


/* Task 2 - Sword Play */

uint32_t choose_sword(uint16_t enemy)
{
    /*
     * TODO
     * Help Geralt construct a sword that will give him an advantage in battle.
     * The steps we will follow:
     * - choose the corect sword
     * - brand the sword with the correct runes such that the equation
     * for that specific enemy type is true
     *
     * How does the sword look (Sword template):
     *  [][][][] [0][0][0]...[0][0][0] [][][][][][][][][][][][][][][][]
     *  -------- --------------------- --------------------------------
     *     Type    Does not matter      The "Value" such that the equation
     *                               from 2 holds depending on the enemy type
     *    4 bits      12 bits                     16 bits
     *
     * 1. Type of the sword:
     * - 1001 - Silver Sword - for monsters
     * - 0110 - Steel Sword - for humans
     *
     *  If the number of active bits from variable "enemy" is even == Monster
     *  else == "Human"
     *
     * 2.
     * Monster Equation:
     *    Value ^ (Monster & (1 - Monster)) = 0
     *
     * Human Equation:
     *    Value + Human = 0
     */

    uint32_t res = 0;


    int nr = 0;
    for (int i = 0; i < 16; i++) {
        if (enemy & (1 << i)) {
            nr++;
        }
    }

    if (nr % 2) {
        res = (6 << 28) | (0x0000FFFF & (0 - enemy));
    } else {
        res = (9 << 28) | (enemy & (1 - enemy));

    }

    return res;
}


/* Task 3 - The Witcher Trials */

uint32_t trial_of_the_grasses(uint16_t cocktail)
{
    /*
     * TODO
     * To become a witcher one must survive a cocktail of different
     * herbs. The body should generate the correct antibodies to
     * neutralize the given cocktail.
     *
     *
     * The antibodies need to respect the following properties:
     *   (antibodies_high & cocktail) ^ (antibodies_low | cocktail) = 0
     *   antibodies_low & antibodies_high = 0
     *
     * Where:
     *  [][][]...[][][] | [][][]...[][][]
     *  ---------------   ---------------
     *  antibodies_high    antibodies_low
     *      16 bits           16 bits
     *      -------------------------
     *              antibodies
     */

    uint32_t antibodies = 0;
    uint32_t antibodies_low = 0;
    uint64_t antibodies_high = cocktail;


    antibodies = (antibodies_high << 16) + antibodies_low;

    return antibodies;
}


uint8_t trial_of_forrest_eyes(uint64_t map)
{
    /* TODO
     * For the next trail, the candidate is tied up and blindfolded.
     * They are also taken to a forest and they should return until
     * the next morning.
     *
     * The candidate knows that there are 4 possibilities for the forest:
     * Brokilon, Hindar, Wolven Glade and Caed Dhu.
     *
     * The candidate also knows those forests by heart, but first you
     * need to identify the forest.
     *
     * The characteristics for 3 of the forests are as follows:
     * - Brokilon - has ONLY groups of 4 trees (or a forest without any tree)
     *   Ex: a group of 4 trees: "...00111100..."
     * - Hindar - has ONLY 2 trees in the MIDDLE
     * - Wolven Glade - FULL of trees
     *
     * The "map" variable encodes the following information:
     * - 1 tree
     * - 0 patch of grass
     *
     * You should put in the "res" variable the index for the identified
     * forrest.
     * - Brokilon - index 0
     * - Hindar - index 1
     * - Wolven Glade - index 2
     * - Caed Dhu - index 3
     */

    if ((map & UINT64_MAX) == UINT64_MAX) {
        return 2;
    }

    int i = 0;
    int count = 0;

    for (; i < 64; i++) {
        if (map & ((uint64_t)1 << i)) {
            count++;
            continue;
        }

        if (count != 0 && count != 4) {
            break;
        }

        count = 0;
    }

    if (i == 64 && (count == 0 || count == 4)) {
        return 0;
    }

    if (map == ((uint64_t)3 << 31)) {
        return 1;
    }

    return 3;
}


uint8_t trial_of_the_dreams(uint32_t map)
{
    /* TODO
     * For this final trial, the candidates go into a trance and when
     * they woke up they would be a fully fledged witcher.
     *
     * For this task, you need to help the witcher find the number
     * of steps that would take the candidate from the dream realm to
     * the real world.
     *
     * The "map" variable would have only 2 bits of 1:
     * - candidate location
     * - portal location
     *
     * You should put in the "res" variable the distance from the
     * candidate position to the portal.
     *
     *  Ex:
     *  [0][0][0][0][0][0]....[1][0][0][0][0][1][0]....[0][0][0]
     *
     *  res = 5
     */

    uint8_t res = 0;
    int8_t start = -1;
    int i;

    for (i = 0; i < 32; i++) {
        if (map & (1 << i)) {
            start = i;
            break;
        }
    }

    i++;

    for ( ; i < 32; i++) {
        if (map & (1 << i)) {
            return i - start;
        }
    }

    return res;
}
