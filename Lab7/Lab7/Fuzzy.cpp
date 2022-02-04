#include "Fuzzy.h"

double Fuzzy::FuzzyGrade(double value, double x0, double x1)
{
    double result = 0;
    double x = value;

    if (x <= x0)
        result = 0;
    else if (x > x1)
        result = 1;
    else
        result = ((x - x0) / (x1 - x0));

    return result;
}

double Fuzzy::FuzzyTriangle(double value, double x0, double x1, double x2)
{
    double result = 0;
    double x = value;

    if ((x <= x0) || (x >= x2))
        result = 0;
    else if (x == x1)
        result = 1;
    else if ((x > x0) && (x < x1))
        result = ((x - x0) / (x1 - x0));
    else
        result = ((x2 - x) / (x2 - x1));

    return result;
}

double Fuzzy::FuzzyTrapezoid(double value, double x0, double x1, double x2, double x3)
{
    double result = 0;
    double x = value;

    if ((x <= x0) || (x >= x3))
        result = 0;
    else if ((x >= x1) && (x <= x2))
        result = 1;
    else if ((x > x0) && (x < x1))
        result = ((x - x0) / (x1 - x0));
    else
        result = ((x3 - x) / (x3 - x2));

    return result;
}

double Fuzzy::FuzzyAND(double A, double B)
{
    return std::min(A, B);
}

double Fuzzy::FuzzyOR(double A, double B)
{
    return std::max(A, B);
}

double Fuzzy::FuzzyNOT(double A)
{
    return 1.0 - A;
}
