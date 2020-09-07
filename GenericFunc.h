#pragma once
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

extern const double GENERIC_EPS;

/**
* @brief проверяет равны ли два числа типа double друг другу с точностью до GENERIC_EPS
*/
extern bool DoubleEqualDouble(double d1, double d2);

/**
* @brief проверяет равно ли число типа double нулю с точностью до GENERIC_EPS
*/
extern bool DoubleEqualZero(double d);



/**
* @brief возвращает ~случайное число от 0 до 1
*/
extern double d_rand_0to1();

/**
* @brief возвращает ~случайное 32х битное число
*/
extern int32_t i32_rand();

/**
* @brief возвращает ~случайное 64х битное число
*/
extern int64_t i64_rand();

/**
* @brief возвращает случайный bool
*/
extern bool bool_rand();

/**
* @brief возвращает случайный знак числа (1 или -1)
*/
extern int sign_rand();

/**
* @brief возвращает ~случайное 32х битное целое число в указанных пределах
*/
extern int32_t i32_rand_range(int32_t min, int32_t max);

/**
* @brief возвращает ~случайное 64х битное целое число в указанных пределах
*/
extern int64_t i64_rand_range(int64_t min, int64_t max);

/**
* @brief возвращает ~случайное 64х битное число с плавающей запятой в указанных пределах
*/
extern double d_rand_range(int64_t min, int64_t max);