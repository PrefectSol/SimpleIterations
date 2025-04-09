#include <cstdint>
#include <exception>
#include <iostream>
#include <string>
#include <cmath>

#include "EzExpr.hpp"

#define DEFAULT_EXOPERATOR "x-1/5*(2*x+cos(x)+(2*x)/(1+x*x))"

typedef struct Parameters
{
    float epsilon;
    float alpha;
    float x0;
    std::string exOperator;

    explicit Parameters();

    void print() const;
} Parameters_t;

Parameters_t::Parameters()
    : epsilon(0.0f), alpha(0.0f), x0(0.0f), exOperator() {} 

void Parameters_t::print() const 
{
    std::cout << "Parameters:" << std::endl;
    std::cout << " - epsilon:\t" << epsilon << std::endl;
    std::cout << " - alpha:\t" << alpha << std::endl;
    std::cout << " - x0:\t\t" << x0 << std::endl;
    std::cout << " - exOperator Phi[x]=x:\t" << exOperator << "=x" << std::endl;
}

bool toFloat(std::string str, float *value)
{
    if (!value || str.empty())
    {
        return false;
    }

    try 
    {
        *value = std::stof(str);
    }
    catch (const std::exception &exp)
    {
        std::cerr << "Failed parse to float(" << str << "): " << exp.what() << std::endl;
        return false;
    }

    return true;
}

bool parse(bool isDefaultOp, const char *argv[], Parameters_t *params)
{
    if (!argv || !params)
    {
        std::cerr << "Unexpected: nullptr arg" << std::endl;
        return false;
    }

    if (!toFloat(argv[1], &params->epsilon))
    {
        return false;
    }

    if (params->epsilon <= 0)
    {
        std::cerr << "Invalid argument: expected: epsilon > 0, but got: " << argv[1] << std::endl;
        return false;
    }

    if (!toFloat(argv[2], &params->alpha))
    {
        return false;
    }

    if (params->alpha <= 0 || params->alpha >= 1)
    {
        std::cerr << "Invalid argument: expected: 0 < alpha < 1, but got: " << argv[2] << std::endl;
        return false;
    }

    if (!toFloat(argv[3], &params->x0))
    {
        return false;
    }

    const std::string exprStr = isDefaultOp ? DEFAULT_EXOPERATOR : argv[4];
    
    ez::Expr evaluator;
    try 
    {
        evaluator.parse(exprStr).set("x", 0.0f).eval();
    }
    catch (const ez::ExprException &exp)
    {
        std::cerr << "Failed to validate expression: " << exp.what() << std::endl;
        return false;
    }

    params->exOperator = exprStr;

    return true;
}

int approximate(const Parameters_t &params) 
{
    params.print();
    
    ez::Expr evaluator;
    evaluator.parse(params.exOperator);
    
    float x = params.x0;
    float ax = evaluator.set("x", x).eval().getResult();
    
    const uint32_t apriori = std::ceil(std::log(params.epsilon * (1 - params.alpha) / std::abs(ax - x)) / std::log(params.alpha));
    const float alphaRate = params.alpha / (1 - params.alpha);

    float aposteriori;
    uint32_t i = 0;
    do 
    {
        x = ax;
        ax = evaluator.set("x", x).eval().getResult();
        aposteriori = alphaRate * std::abs(ax - x);
        
        ++i;
    } while (i < apriori && aposteriori > params.epsilon);
    
    std::cout << "Results:" << std::endl;
    std::cout << " - Apriori\t\t= " << apriori << std::endl;
    std::cout << " - Aposteriori\t\t= " << aposteriori << std::endl;
    std::cout << " - Computed steps\t= " << i << std::endl;
    std::cout << " - x\t\t\t= " << ax << std::endl;
    
    return 0;
}

int main(int argc, const char *argv[])
{
    if (argc != 4 && argc != 5)
    {
        std::cerr << "The method of simple iterations" << std::endl;
        std::cerr << "Example:\n" << argv[0] << " <epsilon> <alpha> <x0> <exOperator>(default=\"" << DEFAULT_EXOPERATOR << "\")" << std::endl;
        
        return 1;
    }
    
    Parameters_t params;
    if (!parse(argc == 4, argv, &params))
    {
        return 1;
    }
    
    return approximate(params);
}
