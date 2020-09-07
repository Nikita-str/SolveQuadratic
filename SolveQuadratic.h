#pragma once
#define _CRT_SECURE_NO_WARNINGS //for VS
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include <assert.h>

#include "GenericFunc.h"

///prefix SQ mean SolveQuadratic, or something linked with it

/**
* @brief количество решений в квадратном уравнении
*/
typedef enum
    {
    SQ_NO_SOLUTION = 0,
    SQ_ONE = 1,
    SQ_TWO = 2,
    SQ_INF_SOLUTION = 3,
    } SQ_SOLUTION;

/**
* @brief параметры генерации квадратного уравнения
*/
typedef enum
    {
    Q_GEN_TWO = 0, ///< генерирует квадратное уравнение с двумя решениями
    Q_GEN_ZERO_REALQ = 1, ///< генерирует квадратное уравнение не имеющее действительных решений
    Q_GEN_ZERO_LINEAR = 2, ///< генерирует линейное уравнение не имеющее решений
    Q_GEN_ONE_REALQ = 3, ///< генерирует квадратное уравнение имеющее одно решение
    Q_GEN_ONE_LINEAR = 4, ///< генерирует линейное уравнение имеющее одно решение
    Q_GEN_INF = 5 ///< генерирует (не)уравнение имеющее бесконечно решений, все коэф. = 0
    }Q_GEN_PARAM;

/**
* @brief возвращает сколько решений будет при данном параметре генерации
*/
extern SQ_SOLUTION QGenToQsolution(Q_GEN_PARAM q_gen);

/**
* @brief структура задающая квадратное уравнение a*x^2 + b*x + c = 0
*/
typedef struct
    {
    double a, b, c;
    }Qparams;

/**
* @brief структура хранящая решение квадратного уравнения
*/
typedef struct
    {
    double x1;///< если solution_amount = SQ_ONE то тут содержится единственный корень уравнения, елси ... = SQ_TWO то один из корней уравнения, в остальных случаях может одержать что угодно
    double x2;///< если елси ... = SQ_TWO то тут содержится отличный от x1 корень уравнения, в остальных случаях может одержать что угодно
    SQ_SOLUTION solution_amount; ///< количество решений уравнения
    }Qresult;


/**
* @brief считывает параметры квадратного уравнения и заполняет соответствующую структуру
* @param[out] Qps параметры задающие квадратное урвнение
* @return все ли параметры считались
*/
extern bool QuadraticIn(Qparams* Qps);


/**
* @brief выводит решение уравнения
*/
extern void QuadraticOut(Qresult Qres);

/**
* @brief решает линейное уравнение k*x + b = 0
* @return количество решений
*/
extern SQ_SOLUTION SolveLinear(double k, double b, double* x);

/**
* @brief решает квадратное уравнение
* @param Qps параметры задающие квадратное урвнение
* @param[out] Qres структура в которую будет сохранено решение
*/
extern void SolveQuadratic(Qparams Qps, Qresult* Qres);

/**
* @brief подставляет в уравнение x
* @param Qps параметры задающие квадратное урвнение
* @return результат подстановки x'а в уравнение
*/
extern double QuadraticSubstitute(Qparams Qps, double x);
/**
* @brief проверяет правильность решения квадратного уравнения
* @param Qps параметры задающие квадратное урвнение
* @param Qres структура в которую было сохранено решение
* @return действительно ли данное уравнение решено правильно
*/
extern bool QuadraticChecker(Qparams Qps, const Qresult* Qres);

/**
* @brief генерирует квадратное уравнение
* @param[out] Qps структура в которую генерируется уравнение
* @param gen_param вид генерируемого уравнения
* @param x1 одно из решений генерируемого уравнения
* @param x2 другое решение генерируемого уравнения
* @note в случае когда у уравнение есть только одно решение оно должно передаваться через параметр x1
*/
extern void QuadraticGen(Qparams* Qps, Q_GEN_PARAM gen_param, double x1, double x2);

/**
* @brief автоматически тестирует функцию SolveQuadratic
* @param amount_gen количество генерируемых тестов для каждого вида генерации
* @param success_out выводить ли надпись об успешном прохождении теста, если false то выводит только сообщения об ошибках(если они есть)
*/
extern void QuadraticAutoTest(int amount_gen, bool success_out);