#include "SolveQuadratic.h"
#include "TestMacro.h"
#include "GenericFunc.h"

void Test_0(_Bool success_out)
    {
    printf("AUTO TESTING START\n");
    const int magic_number = 228;// just amount tests
    QuadraticAutoTest(magic_number, success_out);
    printf("AUTO TESTING END\n");
    }

void Test_1()
    {
    Qparams Qps;
    Qresult Qres;
    #define macro_SQ_CHECK(a, b, c) Qps = (Qparams){ a, b, c }; \
    macro_FUNC_CHECK(SolveQuadratic, (Qps, &Qres), QuadraticChecker, (Qps, &Qres));

    macro_SQ_CHECK(1, 2, 3);
    macro_SQ_CHECK(7, 2, -3);
    macro_SQ_CHECK(1, 2, -3);
    macro_SQ_CHECK(0, 2, -3);
    macro_SQ_CHECK(-3, 0, 0);
    macro_SQ_CHECK(0, 0, 1);
    macro_SQ_CHECK(0, 0, 0);
    macro_SQ_CHECK(1, 8, 2);
    macro_SQ_CHECK(17, 0, 3);
    macro_SQ_CHECK(0, 0.01, 0.03);
    macro_SQ_CHECK(0.2, 0.01, 0);
    macro_SQ_CHECK(1, 2, 0);
    }


void QuadraticInSolveOut()
    {
    Qparams Qps;
    QuadraticIn(&Qps);
    Qresult Qres;
    SolveQuadratic(Qps, &Qres);
    QuadraticOut(Qres);
    }

void PrintHelp()
    {
    printf("cmd|action\n"
        "h  |write this text\n"
        "e  |[e]xit from program\n"
        "t  |execute [t]ests for fnction 'SolveQuadratic'\n"
        "q  |in solve and out [q]uadratic equation\n"
        "s  |[s]witch success out in autotest\n");
    }

int main()
    {
    _Bool exit = 0;
    _Bool autotest_success_out = 1;
    PrintHelp();
    while (true)
        {
        char cmd;
        scanf("%c", &cmd);
        switch (cmd)
            {
            case 'h': PrintHelp();  break;
            case 'e': exit = 1; break;
            case 't': Test_0(autotest_success_out); Test_1(); break;
            case 'q': QuadraticInSolveOut(); break;
            case 's': 
                autotest_success_out = !autotest_success_out; 
                printf(autotest_success_out ? 
                       "now autotest will notify in case of success\n" :
                       "now autotest will not notify in case of success\n");
                break;
            }

        if(exit)break;
        }
    }
