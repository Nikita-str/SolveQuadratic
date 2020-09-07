#pragma once
#define _CRT_SECURE_NO_WARNINGS //for VS
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include <assert.h>

#include "GenericFunc.h"

///prefix SQ mean SolveQuadratic, or something linked with it

/**
* @brief ���������� ������� � ���������� ���������
*/
typedef enum
    {
    SQ_NO_SOLUTION = 0,
    SQ_ONE = 1,
    SQ_TWO = 2,
    SQ_INF_SOLUTION = 3,
    } SQ_SOLUTION;

/**
* @brief ��������� ��������� ����������� ���������
*/
typedef enum
    {
    Q_GEN_TWO = 0, ///< ���������� ���������� ��������� � ����� ���������
    Q_GEN_ZERO_REALQ = 1, ///< ���������� ���������� ��������� �� ������� �������������� �������
    Q_GEN_ZERO_LINEAR = 2, ///< ���������� �������� ��������� �� ������� �������
    Q_GEN_ONE_REALQ = 3, ///< ���������� ���������� ��������� ������� ���� �������
    Q_GEN_ONE_LINEAR = 4, ///< ���������� �������� ��������� ������� ���� �������
    Q_GEN_INF = 5 ///< ���������� (��)��������� ������� ���������� �������, ��� ����. = 0
    }Q_GEN_PARAM;

/**
* @brief ���������� ������� ������� ����� ��� ������ ��������� ���������
*/
extern SQ_SOLUTION QGenToQsolution(Q_GEN_PARAM q_gen);

/**
* @brief ��������� �������� ���������� ��������� a*x^2 + b*x + c = 0
*/
typedef struct
    {
    double a, b, c;
    }Qparams;

/**
* @brief ��������� �������� ������� ����������� ���������
*/
typedef struct
    {
    double x1;///< ���� solution_amount = SQ_ONE �� ��� ���������� ������������ ������ ���������, ���� ... = SQ_TWO �� ���� �� ������ ���������, � ��������� ������� ����� �������� ��� ������
    double x2;///< ���� ���� ... = SQ_TWO �� ��� ���������� �������� �� x1 ������ ���������, � ��������� ������� ����� �������� ��� ������
    SQ_SOLUTION solution_amount; ///< ���������� ������� ���������
    }Qresult;


/**
* @brief ��������� ��������� ����������� ��������� � ��������� ��������������� ���������
* @param[out] Qps ��������� �������� ���������� ��������
* @return ��� �� ��������� ���������
*/
extern bool QuadraticIn(Qparams* Qps);


/**
* @brief ������� ������� ���������
*/
extern void QuadraticOut(Qresult Qres);

/**
* @brief ������ �������� ��������� k*x + b = 0
* @return ���������� �������
*/
extern SQ_SOLUTION SolveLinear(double k, double b, double* x);

/**
* @brief ������ ���������� ���������
* @param Qps ��������� �������� ���������� ��������
* @param[out] Qres ��������� � ������� ����� ��������� �������
*/
extern void SolveQuadratic(Qparams Qps, Qresult* Qres);

/**
* @brief ����������� � ��������� x
* @param Qps ��������� �������� ���������� ��������
* @return ��������� ����������� x'� � ���������
*/
extern double QuadraticSubstitute(Qparams Qps, double x);
/**
* @brief ��������� ������������ ������� ����������� ���������
* @param Qps ��������� �������� ���������� ��������
* @param Qres ��������� � ������� ���� ��������� �������
* @return ������������� �� ������ ��������� ������ ���������
*/
extern bool QuadraticChecker(Qparams Qps, const Qresult* Qres);

/**
* @brief ���������� ���������� ���������
* @param[out] Qps ��������� � ������� ������������ ���������
* @param gen_param ��� ������������� ���������
* @param x1 ���� �� ������� ������������� ���������
* @param x2 ������ ������� ������������� ���������
* @note � ������ ����� � ��������� ���� ������ ���� ������� ��� ������ ������������ ����� �������� x1
*/
extern void QuadraticGen(Qparams* Qps, Q_GEN_PARAM gen_param, double x1, double x2);

/**
* @brief ������������� ��������� ������� SolveQuadratic
* @param amount_gen ���������� ������������ ������ ��� ������� ���� ���������
* @param success_out �������� �� ������� �� �������� ����������� �����, ���� false �� ������� ������ ��������� �� �������(���� ��� ����)
*/
extern void QuadraticAutoTest(int amount_gen, bool success_out);