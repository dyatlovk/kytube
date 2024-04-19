#pragma once

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[37m"
#define ANSI_COLOR_GRAY    "\x1b[90m"
#define ANSI_COLOR_BRIGHT  "\x1b[91m"

#define TEST_CASE(name, ...) \
void name() { \
    __VA_ARGS__ \
    printf(ANSI_COLOR_GREEN "=== %s PASSED === " ANSI_COLOR_RESET, #name); \
    printf("\n" ANSI_COLOR_RESET); \
    printf("\n" ANSI_COLOR_RESET); \
}

inline void ASSERT_EQ_INT(int actual, int expected, const char *message) {
    if (expected != actual) {
        printf(ANSI_COLOR_RED "Assertion failed: %s\n" ANSI_COLOR_RESET, message);
        return;
    }

    printf(ANSI_COLOR_WHITE "Assertion passed: %s\n" ANSI_COLOR_RESET, message);
}

inline void ASSERT_EQ_FLOAT(float actual, float expected, const char *message) {
    if (expected != actual) {
        printf(ANSI_COLOR_RED "Assertion failed: %s\n" ANSI_COLOR_RESET, message);
        return;
    }

    printf(ANSI_COLOR_WHITE "Assertion passed: %s\n" ANSI_COLOR_RESET, message);
}

inline void ASSERT_EQ_CHAR(const char *actual, const char *expected, const char *message) {
    if (strcmp(expected, actual) != 0) {
        printf(ANSI_COLOR_RED "Assertion failed: %s\n" ANSI_COLOR_RESET, message);
        return;
    }

    printf(ANSI_COLOR_WHITE "Assertion passed: %s\n" ANSI_COLOR_RESET, message);
}

inline void ASSERT_TRUE(bool actual, const char *message) {
    if (actual != true) {
        printf(ANSI_COLOR_RED "Assertion failed: %s\n" ANSI_COLOR_RESET, message);
        return;
    }

    printf(ANSI_COLOR_WHITE "Assertion passed: %s\n" ANSI_COLOR_RESET, message);
}

inline void ASSERT_FALSE(bool actual, const char *message) {
    if (actual != false) {
        printf(ANSI_COLOR_RED "Assertion failed: %s\n" ANSI_COLOR_RESET, message);
        return;
    }

    printf(ANSI_COLOR_WHITE "Assertion passed: %s\n" ANSI_COLOR_RESET, message);
}
