#pragma once
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

extern const double GENERIC_EPS;

/**
* @brief ��������� ����� �� ��� ����� ���� double ���� ����� � ��������� �� GENERIC_EPS
*/
extern bool DoubleEqualDouble(double d1, double d2);

/**
* @brief ��������� ����� �� ����� ���� double ���� � ��������� �� GENERIC_EPS
*/
extern bool DoubleEqualZero(double d);



/**
* @brief ���������� ~��������� ����� �� 0 �� 1
*/
extern double d_rand_0to1();

/**
* @brief ���������� ~��������� 32� ������ �����
*/
extern int32_t i32_rand();

/**
* @brief ���������� ~��������� 64� ������ �����
*/
extern int64_t i64_rand();

/**
* @brief ���������� ��������� bool
*/
extern bool bool_rand();

/**
* @brief ���������� ��������� ���� ����� (1 ��� -1)
*/
extern int sign_rand();

/**
* @brief ���������� ~��������� 32� ������ ����� ����� � ��������� ��������
*/
extern int32_t i32_rand_range(int32_t min, int32_t max);

/**
* @brief ���������� ~��������� 64� ������ ����� ����� � ��������� ��������
*/
extern int64_t i64_rand_range(int64_t min, int64_t max);

/**
* @brief ���������� ~��������� 64� ������ ����� � ��������� ������� � ��������� ��������
*/
extern double d_rand_range(int64_t min, int64_t max);