#pragma once
#include <string>
#include <functional>
#include <vector>
#include <memory>
#include "GraphData.h"

class CalculatorModel;

namespace RPN {

class GraphFunction {
public:
    GraphFunction(CalculatorModel* calculator);
    
    bool SetExpression(const std::string& expr);
    std::string GetExpression() const { return expression; }
    
    std::shared_ptr<GraphData> Evaluate(double xMin, double xMax, int numPoints = 1000);
    
    double EvaluateAtPoint(double x);
    
    std::string GetLastError() const { return lastError; }
    bool HasError() const { return !lastError.empty(); }

private:
    CalculatorModel* calculator;
    std::string expression;
    std::string lastError;
    
    bool IsValidExpression(const std::string& expr);
    std::string PrepareExpression(const std::string& expr, double xValue);
};

}