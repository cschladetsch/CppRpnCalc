#ifndef INFIX_TO_RPN_H
#define INFIX_TO_RPN_H

#include <string>
#include <vector>

namespace RPN {

class InfixToRPN {
public:
    static std::vector<std::string> convert(const std::string& infix);
    
private:
    static int getPrecedence(const std::string& op);
    static bool isOperator(const std::string& token);
    static bool isFunction(const std::string& token);
    static bool isNumber(const std::string& token);
    static std::vector<std::string> tokenize(const std::string& expression);
};

}

#endif