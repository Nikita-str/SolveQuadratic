#include "SolveQuadratic.h"
#include "TestMacro.h"

//++############[InOut]##################
bool QuadraticIn(Qparams* Qps)
    {
    assert(Qps);
    const int NUM_OF_READ_PARAM = 3;
    printf("input params:\n");
    int check_err = scanf(" %lf%lf%lf", &Qps->a, &Qps->b, &Qps->c);
    if (check_err != NUM_OF_READ_PARAM)
        {
        #ifdef stderr
        fprintf(stderr, "an error occurred while reading params\n");
        #else 
        printf("an error occurred while reading params\n");
        #endif
        return false;
        }
    return true;
    }

void QuadraticOut(Qresult Qres)
    {
    switch (Qres.solution_amount)
        {
        case SQ_NO_SOLUTION:
            printf("no solution\n");
            break;
        case SQ_ONE:
            printf("x = %lf\n", Qres.x1);
            break;
        case SQ_TWO:
            printf("x1 = %lf, x2 = %lf\n", Qres.x1, Qres.x2);
            break;
        case SQ_INF_SOLUTION:
            printf("infinity of solutions\n");
            break;
        default:
            assert(0);
            break;
        }
    }
//--############[InOut]##################

SQ_SOLUTION SolveLinear(double k, double b, double* x)
    {
    assert(x);
    if (DoubleEqualZero(k))
        {
        if (DoubleEqualZero(b))return SQ_INF_SOLUTION;
        return SQ_NO_SOLUTION;
        }
    *x = -b/k;
    return SQ_ONE;
    }

static SQ_SOLUTION SolveQuadratic_help(double a, double b, double c, double* x1, double* x2)
    {
    assert(x1);
    assert(x2);
    assert(x1 != x2);

    if (DoubleEqualZero(a))
        return SolveLinear(b, c, x1);

    double D = b*b - 4*a*c;
    double a_2 = 2*a;
    if (DoubleEqualZero(D))
        {
        *x1 = -b/a_2;
        return SQ_ONE;
        }
    else if (D < 0) return SQ_NO_SOLUTION;

    double sqD = sqrt(D);
    *x1 = (-b + sqD)/a_2;
    *x2 = (-b - sqD)/a_2;
    return SQ_TWO;
    }

void SolveQuadratic(Qparams Qps, Qresult* Qres)
    {
    assert(Qres);
    Qres->solution_amount = SolveQuadratic_help(Qps.a, Qps.b, Qps.c, &Qres->x1, &Qres->x2);
    if(DoubleEqualZero(Qres->x1))Qres->x1 = 0;
    if(DoubleEqualZero(Qres->x2))Qres->x2 = 0;
    }

inline double QuadraticSubstitute(Qparams Qps, double x) {return Qps.a*x*x + Qps.b*x + Qps.c;}

bool QuadraticChecker(Qparams Qps, const Qresult* Qres)
    {
    double a = Qps.a;
    double b = Qps.b;
    double c = Qps.c;

    bool a_is_0 = DoubleEqualZero(a);
    double x_peak = a_is_0 ? 0 : -b/(2*a);

    switch (Qres->solution_amount)
        {
        case SQ_NO_SOLUTION:
            {
            if (a_is_0) return DoubleEqualZero(b) && !DoubleEqualZero(c); // [c!=0] = 0
            double y_peak = QuadraticSubstitute(Qps, x_peak);
            if (DoubleEqualZero(y_peak))return false;//if (y_peak = 0) => x_peak is solution
            return ((y_peak * a) > 0); //if a and y_peak have different sign => solution exist
            break;
            }
        case SQ_ONE:
            {
            if (a_is_0) return !DoubleEqualZero(b);
            double y_peak = QuadraticSubstitute(Qps, x_peak);
            return DoubleEqualZero(y_peak); //if (y_peak != 0) => or no solution either 2 solution;
            break;
            }
        case SQ_TWO:
            if (a_is_0) return false;//bx+c=0 have 0,1 or inf solution, not 2
            if (DoubleEqualDouble(Qres->x1, Qres->x2)) return false;
            double y1 = QuadraticSubstitute(Qps, Qres->x1);
            double y2 = QuadraticSubstitute(Qps, Qres->x2);
            return DoubleEqualZero(y1) && DoubleEqualZero(y2);
            break;
        case SQ_INF_SOLUTION:
            return (a_is_0 && DoubleEqualZero(b) && DoubleEqualZero(c));// 0 = 0 => inf solution
        default: return false;
        }
    return true;
    }

void QuadraticGen(Qparams* Qps, Q_GEN_PARAM gen_param, double x1, double x2)
    {
    assert(Qps);
    int k_sign = sign_rand();
    double k = k_sign * (d_rand_range(1, 50) - 0.5);
    switch (gen_param)
        {
        case Q_GEN_TWO:
            assert(!DoubleEqualDouble(x1, x2));
            Qps->a = k;
            Qps->b = -k * (x1+x2);
            Qps->c = k * x1*x2;
            return;

        case Q_GEN_ZERO_REALQ:
            //b^2 < 4*a*c
            Qps->a = k;
            Qps->b = sign_rand() * (d_rand_range(1, 50) - 0.5);
            Qps->c = k_sign * (fabs((Qps->b * Qps->b)/(4 * Qps->a)) + d_rand_range(0, 50) + 0.01);
            return;

        case Q_GEN_ZERO_LINEAR:
            Qps->a = 0;
            Qps->b = 0;
            Qps->c = k;
            return;

        case Q_GEN_ONE_REALQ:
            //b^2 = 4ac
            //2ax + b = 0
            Qps->a = k;
            Qps->b = -2*Qps->a*x1;
            Qps->c = (Qps->b * Qps->b)/(4 * Qps->a);
            return;

        case Q_GEN_ONE_LINEAR:
            //kx + c = 0
            Qps->a = 0;
            Qps->b = k;
            Qps->c = -k*x1;
            return;

        case Q_GEN_INF:
            Qps->a = 0;
            Qps->b = 0;
            Qps->c = 0;
            return;

        default: assert(0); return;
        }

    }

SQ_SOLUTION QGenToQsolution(Q_GEN_PARAM q_gen)
    {
    switch (q_gen)
        {
        case Q_GEN_ZERO_LINEAR:
        case Q_GEN_ZERO_REALQ:
            return SQ_NO_SOLUTION;
        case Q_GEN_ONE_LINEAR:
        case Q_GEN_ONE_REALQ:
            return SQ_ONE;
        case Q_GEN_TWO: return SQ_TWO;
        case Q_GEN_INF: return SQ_INF_SOLUTION;
        default: assert(0);
        }
    }

void QuadraticAutoTest(int amount_gen, bool success_out)
    {
    Qparams Qps;

    int q_gen_type[] = {Q_GEN_TWO, Q_GEN_ZERO_REALQ, Q_GEN_ONE_REALQ, Q_GEN_ONE_LINEAR, Q_GEN_ZERO_LINEAR, Q_GEN_INF};
    int len = sizeof(q_gen_type)/sizeof(*q_gen_type);

    for (int i = 0; i < len; i++)
        {
        int to = amount_gen;
        if (q_gen_type[i] == Q_GEN_ZERO_LINEAR || q_gen_type[i] == Q_GEN_INF)to = 4; //: there is no point checking it many times
        for (int j = 0; j < to; j++)
            {
            double x1 = sign_rand() * (d_rand_range(0, 20));
            double x2 = sign_rand() * (d_rand_range(0, 20));
            if (DoubleEqualDouble(x1, x2)) x2 += 0.05;
            QuadraticGen(&Qps, q_gen_type[i], x1, x2);
            Qresult Qres;
            SolveQuadratic(Qps, &Qres);
            if ((q_gen_type[i] == Q_GEN_TWO) && ((x1 < x2 && Qres.x1 > Qres.x2) || (x1 > x2 && Qres.x1 < Qres.x2)))
                {
                //if we have 2 solution => arrenge in order it
                double temp = Qres.x1;
                Qres.x1 = Qres.x2;
                Qres.x2 = temp;
                }

            bool x1eq = DoubleEqualDouble(x1, Qres.x1);
            bool x2eq = DoubleEqualDouble(x2, Qres.x2);

            //if (!QuadraticChecker(Qps, &Qres))
            if ( Qres.solution_amount != QGenToQsolution(q_gen_type[i]) || 
                ((Qres.solution_amount == SQ_TWO) && !x1eq && !x2eq) || 
                ((Qres.solution_amount == SQ_ONE) && !x1eq))
                {
                printf("[ERROR] : a=%lf   b=%lf   c=%lf  |"
                       "  supposed: gen_type=%d   x1=%lf   x2=%lf  |"
                       "  received: solution_amount=%d   x1=%lf   x2=%lf\n",
                       Qps.a, Qps.b, Qps.c, 
                       q_gen_type[i], x1, x2, 
                       Qres.solution_amount, Qres.x1, Qres.x2);
                }
            else if(success_out)
                {
                printf("[TEST PASSED]\n");
                }
            }
        }

    }
