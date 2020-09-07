#include "GenericFunc.h"

#define MIN_RAND_BIT 16

const double GENERIC_EPS = 1e-6;

inline bool DoubleEqualDouble(double d1, double d2) { return fabs(d1 - d2) < GENERIC_EPS; }
inline bool DoubleEqualZero(double d) { return DoubleEqualDouble(d, 0); }

static void rand_start()
    {
    static bool first = true;
    if (first)
        {
        first = false;
        srand(time(NULL));
        }
    }

double d_rand_0to1() 
    {
    const double rand_max = RAND_MAX;
    rand_start();
    return rand()/rand_max; 
    }

int32_t i32_rand()
    {
    rand_start();
    return ((int32_t)rand())<<MIN_RAND_BIT + rand();
    }

int64_t i64_rand()
    {
    rand_start();
    int64_t ret = ((int64_t)rand())<<(MIN_RAND_BIT*3) + ((int64_t)rand())<<(MIN_RAND_BIT*2) 
                + ((int64_t)rand())<<MIN_RAND_BIT + rand();
    return ret;
    }

bool bool_rand()
    {
    rand_start();
    return rand()%2;
    }

int sign_rand() { return bool_rand() ? 1 : -1; }

int32_t i32_rand_range(int32_t min, int32_t max)
    {
    if (min < max)
        {
        int32_t temp = min;
        min = max;
        max = temp;
        }
    return (i32_rand() % (max - min + 1)) + min;
    }

int64_t i64_rand_range(int64_t min, int64_t max)
    {
    if (min < max)
        {
        int64_t temp = min;
        min = max;
        max = temp;
        }
    return (i64_rand() % (max - min + 1)) + min;
    }

double d_rand_range(int64_t min, int64_t max){ return i64_rand_range(min, max) + d_rand_0to1(); }