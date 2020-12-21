#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdint.h>

#ifdef DEBUG
#define DEBUG_LOG(fmt, ...) \
    do { \
	    fprintf(stderr, "%s/%s [%d] " fmt "\n", \
                __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
    } while(0)
#else
#define DEBUG_LOG(fmt, ...)
#endif

#define PADDING "........................................"
#define TARGET_LENGTH 50

#define ARRAY_SIZE(x) (sizeof x / sizeof *x)

typedef struct {
    const char *test_name;
    uint8_t (*f)(void);
    uint32_t points;
} testcase;

#endif
