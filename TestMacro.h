#pragma once
#define _CRT_SECURE_NO_WARNINGS //for VS
#include <stdio.h>
#include <string.h>

#define TEST_SUCCESS_OUT  1

#define helpmacro_GET_FILE(_file) \
(strrchr(_file, '\\') ? strrchr(_file, '\\') : strrchr(_file, '/') ? strrchr(_file, '/') : _file)

//======================[EXPRESSION TEST]======================================+++
#define helpmacro_EXPRESSION_TEST_out(s, expression)                         \
{                                                                            \
char* _file = __FILE__;                                                      \
char* file = helpmacro_GET_FILE(_file);                                      \
printf("[%s] expression: "#expression"    file: %s    line: %d\n",           \
    s, file, __LINE__);                                                      \
}     

#ifdef _DEBUG
/**
* @brief проверяет совпадает ли значение выражения с предпологаемым результатом
*/
#define macro_EXPRESSION_TEST(expression, result)                      \
{                                                                      \
if((expression) != (result))                                           \
    helpmacro_EXPRESSION_TEST_out("TEST FAILED", expression)           \
else if(TEST_SUCCESS_OUT)                                              \
    helpmacro_EXPRESSION_TEST_out("TEST PASSED", expression)           \
}
#else
#define macro_EXPRESSION_TEST(expression, result) ;
#endif
//======================[EXPRESSION TEST]=======================================---


//======================[FUNC CHECK]=============================+++
#define helpmacro_FUNC_CHECK_out(s, func, checker)             \
    {                                                          \
    char* _file = __FILE__;                                    \
    char* file = helpmacro_GET_FILE(_file);                    \
    printf("[%s] func:"#func"   checker:"#checker              \
           "file: %s    line: %d\n", s, file, __LINE__);       \
    }                                                          \

#ifdef _DEBUG
/**
* @brief проверяет согласуется ли действие функции с предполагаемым действием
* @note предполагается что результат выполнения функции передается через параметры как из самой func так и в checker
*/
#define macro_FUNC_CHECK(func, f_params, checker, c_params)    \
{                                                              \
func f_params;                                                 \
if(!(checker c_params))                                        \
    helpmacro_FUNC_CHECK_out("CHECK FAILED", func, checker)    \
else if(TEST_SUCCESS_OUT)                                      \
    helpmacro_FUNC_CHECK_out("CHECK PASSED", func, checker)    \
}  
#else
#define macro_FUNC_CHECK(func, f_params, checker, c_params) ;
#endif
//======================[FUNC CHECK]=============================---

