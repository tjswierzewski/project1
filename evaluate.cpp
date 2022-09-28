#include <string>
#include <math.h>
#include "ap/ap.hpp"

using namespace std;

ap_int<1024> eval_add(ap_int<1024> a, ap_int<1024> b)
{
    ap_int<1024> out = a + b;
    return out;
}

ap_int<1024> eval_sub(ap_int<1024> a, ap_int<1024> b)
{
    ap_int<1024> out = a - b;
    return out;
}

ap_int<1024> eval_mult(ap_int<1024> a, ap_int<1024> b)
{
    ap_int<1024> out = a * b;
    return out;
}

ap_int<1024> eval_div(ap_int<1024> a, ap_int<1024> b)
{
    if (b == 0)
    {
        throw exception();
    }

    ap_int<1024> out = a / b;

    if (((a.handle.reg.sign == 1) && (b.handle.reg.sign == 0)) || ((a.handle.reg.sign == 0) && (b.handle.reg.sign == 1)))
    {
        out--;
    }
    return out;
}

ap_int<1024> eval_shift(ap_int<1024> a, ap_int<1024> b)
{
    ap_int<1024> out = (a << 13) ^ b;
    return out;
}

ap_int<1024> evaluate(string txpr)
{
    string op;
    string txpr1;
    string txpr2;
    int parse;
    if (txpr[0] != '(')
    {

        parse = txpr.find(" ");
        return stol(txpr.substr(0., parse).c_str());
    }

    int count = 1;
    size_t i;
    for (i = 2; i < txpr.size(); i++)
    {
        if (txpr[i] == '(')
        {
            count++;
        }
        else if (txpr[i] == ')')
        {
            count--;
        }
        if (count <= 1)
        {
            break;
        }
    }

    if (i == 2)
    {
        txpr.erase(0, 2);
        parse = txpr.find(" ");
        txpr1 = txpr.substr(0, parse);
        txpr.erase(0, parse + 1);
    }
    else
    {

        txpr1 = txpr.substr(2, i - 1);
        txpr.erase(0, i + 2);
    }
    parse = txpr.find(" ");
    op = txpr.substr(0, parse);
    txpr.erase(0, parse + 1);
    txpr2 = txpr;
    ap_int<1024> eval2;
    switch (op[0])
    {
    case '+':
        return eval_add(evaluate(txpr1), evaluate(txpr2));
    case '-':
        return eval_sub(evaluate(txpr1), evaluate(txpr2));
    case '*':
        return eval_mult(evaluate(txpr1), evaluate(txpr2));
    case '/':
        eval2 = evaluate(txpr2);
        if (eval2 == 0)
        {
            throw exception();
        }
        return eval_div(evaluate(txpr1), eval2);
    case '<':
        return eval_shift(evaluate(txpr1), evaluate(txpr2));
    default:
        return 0;
    }
}