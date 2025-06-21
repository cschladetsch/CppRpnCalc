#include "InfixToRPN.h"
#include <stack>
#include <sstream>
#include <regex>
#include <cctype>

namespace RPN {

std::vector<std::string> InfixToRPN::convert(const std::string& infix) {
    std::vector<std::string> tokens = tokenize(infix);
    std::vector<std::string> output;
    std::stack<std::string> operators;
    
    for (const auto& token : tokens) {
        if (isNumber(token)) {
            output.push_back(token);
        } else if (isFunction(token)) {
            operators.push(token);
        } else if (token == "(") {
            operators.push(token);
        } else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                output.push_back(operators.top());
                operators.pop();
            }
            if (!operators.empty()) {
                operators.pop();
                if (!operators.empty() && isFunction(operators.top())) {
                    output.push_back(operators.top());
                    operators.pop();
                }
            }
        } else if (isOperator(token)) {
            while (!operators.empty() && 
                   operators.top() != "(" &&
                   getPrecedence(operators.top()) >= getPrecedence(token)) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }
    }
    
    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }
    
    return output;
}

int InfixToRPN::getPrecedence(const std::string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/" || op == "%") return 2;
    if (op == "^") return 3;
    return 0;
}

bool InfixToRPN::isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || 
           token == "/" || token == "%" || token == "^";
}

bool InfixToRPN::isFunction(const std::string& token) {
    return token == "sin" || token == "cos" || token == "tan" ||
           token == "ln" || token == "log" || token == "sqrt" ||
           token == "abs" || token == "exp";
}

bool InfixToRPN::isNumber(const std::string& token) {
    try {
        std::stod(token);
        return true;
    } catch (...) {
        return false;
    }
}

std::vector<std::string> InfixToRPN::tokenize(const std::string& expression) {
    std::vector<std::string> tokens;
    std::string current;
    
    for (size_t i = 0; i < expression.length(); ++i) {
        char ch = expression[i];
        
        if (std::isspace(ch)) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        } else if (ch == '(' || ch == ')') {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            tokens.push_back(std::string(1, ch));
        } else if (isOperator(std::string(1, ch))) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            tokens.push_back(std::string(1, ch));
        } else {
            current += ch;
        }
    }
    
    if (!current.empty()) {
        tokens.push_back(current);
    }
    
    return tokens;
}

}