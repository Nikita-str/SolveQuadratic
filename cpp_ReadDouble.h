#pragma once
#ifdef __cplusplus
#include <string>
#include <iostream>
#include <cassert>
#endif
#include <ctype.h>

/*
#ifdef __cplusplus
constexpr int NO_ERR = 0;
constexpr int ERR_LOST_PRECISION = 1;
constexpr int ERR_BAD_INPUT = 2;
constexpr int ERR_TOO_MANY_READ = 3;
constexpr int ERR_STREAM = 4;
#else*/
enum
{
    NO_ERR = 0,
    ERR_LOST_PRECISION = 1,
    ERR_BAD_INPUT = 2,
    ERR_TOO_MANY_READ = 3,
    ERR_STREAM = 4,
};
//#endif

enum
{
    FIRST_STAGE = 0,
    SIGN_STAGE = 1,
    PRE_DOT_STAGE = 2,
    AFTER_DOT_STAGE = 3,
};

#ifdef __cplusplus
extern "C" {
#endif
extern int read_line_to_doubles(double *ds, int max_amount, int *amount);
#ifdef __cplusplus
}
#endif