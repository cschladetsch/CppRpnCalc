#include "GraphFunction.h"
#include "CalculatorModel.h"
#include <sstream>
#include <cmath>
#include <algorithm>
#include <regex>

namespace RPN {

GraphFunction::GraphFunction(CalculatorModel* calc) : calculator(calc) {}

bool GraphFunction::SetExpression(const std::string& expr) {
    if (!IsValidExpression(expr)) {
        lastError = "Invalid expression format";
        return false;
    }
    
    expression = expr;
    lastError.clear();
    return true;
}

std::shared_ptr<GraphData> GraphFunction::Evaluate(double xMin, double xMax, int numPoints) {
    auto data = std::make_shared<GraphData>();
    
    if (expression.empty()) {
        lastError = "No expression set";
        return data;
    }
    
    if (numPoints < 2) {
        lastError = "Need at least 2 points";
        return data;
    }
    
    double step = (xMax - xMin) / (numPoints - 1);
    std::vector<double> xValues, yValues;
    xValues.reserve(numPoints);
    yValues.reserve(numPoints);
    
    for (int i = 0; i < numPoints; ++i) {
        double x = xMin + i * step;
        double y = EvaluateAtPoint(x);
        
        if (!std::isnan(y) && !std::isinf(y)) {
            xValues.push_back(x);
            yValues.push_back(y);
        }
    }
    
    data->SetData(xValues, yValues);
    data->SetLabel(expression);
    
    return data;
}

double GraphFunction::EvaluateAtPoint(double x) {
    calculator->clear();
    
    std::string preparedExpr = PrepareExpression(expression, x);
    
    std::istringstream iss(preparedExpr);
    std::string token;
    
    while (iss >> token) {
        try {
            double value = std::stod(token);
            calculator->pushValue(value);
        } catch (...) {
            if (!calculator->executeOperation(token) && 
                calculator->isFunctionDefined(token)) {
                calculator->executeFunction(token);
            }
        }
    }
    
    double result = 0.0;
    if (!calculator->popValue(result)) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    
    return result;
}

bool GraphFunction::IsValidExpression(const std::string& expr) {
    if (expr.empty()) return false;
    
    return expr.find('x') != std::string::npos || expr.find('X') != std::string::npos;
}

std::string GraphFunction::PrepareExpression(const std::string& expr, double xValue) {
    std::string result = expr;
    
    std::regex xRegex("\\b[xX]\\b");
    result = std::regex_replace(result, xRegex, std::to_string(xValue));
    
    return result;
}

}