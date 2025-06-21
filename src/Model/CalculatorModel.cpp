#include "CalculatorModel.h"
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <algorithm>

CalculatorModel::CalculatorModel() {
    registerOperations();
}

void CalculatorModel::registerOperations() {
    // Basic arithmetic
    operations.emplace("+", Operation("+", OperationType::BINARY, 
        [](double a, double b) { return a + b; }));
    
    operations.emplace("-", Operation("-", OperationType::BINARY, 
        [](double a, double b) { return a - b; }));
    
    operations.emplace("*", Operation("*", OperationType::BINARY, 
        [](double a, double b) { return a * b; }));
    
    operations.emplace("/", Operation("/", OperationType::BINARY, 
        [this](double a, double b) { 
            if (b == 0) {
                setError("Division by zero");
                return 0.0;
            }
            return a / b; 
        }));
    
    operations.emplace("^", Operation("^", OperationType::BINARY, 
        [](double a, double b) { return std::pow(a, b); }));
    
    // Trigonometric functions
    operations.emplace("sin", Operation("sin", OperationType::UNARY, 
        [](double a, double) { return std::sin(a); }));
    
    operations.emplace("cos", Operation("cos", OperationType::UNARY, 
        [](double a, double) { return std::cos(a); }));
    
    operations.emplace("tan", Operation("tan", OperationType::UNARY, 
        [](double a, double) { return std::tan(a); }));
    
    // Math functions
    operations.emplace("sqrt", Operation("sqrt", OperationType::UNARY, 
        [this](double a, double) { 
            if (a < 0) {
                setError("Square root of negative number");
                return 0.0;
            }
            return std::sqrt(a); 
        }));
    
    operations.emplace("1/x", Operation("1/x", OperationType::UNARY, 
        [this](double a, double) { 
            if (a == 0) {
                setError("Division by zero");
                return 0.0;
            }
            return 1.0 / a; 
        }));
    
    operations.emplace("+/-", Operation("+/-", OperationType::UNARY, 
        [](double a, double) { return -a; }));
    
    operations.emplace("ln", Operation("ln", OperationType::UNARY, 
        [this](double a, double) { 
            if (a <= 0) {
                setError("Logarithm of non-positive number");
                return 0.0;
            }
            return std::log(a); 
        }));
    
    operations.emplace("log", Operation("log", OperationType::UNARY, 
        [this](double a, double) { 
            if (a <= 0) {
                setError("Logarithm of non-positive number");
                return 0.0;
            }
            return std::log10(a); 
        }));
    
    operations.emplace("exp", Operation("exp", OperationType::UNARY, 
        [](double a, double) { return std::exp(a); }));
    
    operations.emplace("dup", Operation("dup", OperationType::UNARY, 
        [this](double a, double) { 
            pushValue(a);
            return a; 
        }));
    
    // Comparison operations
    operations.emplace(">", Operation(">", OperationType::BINARY, 
        [](double a, double b) { return (a > b) ? 1.0 : 0.0; }));
    
    operations.emplace("<", Operation("<", OperationType::BINARY, 
        [](double a, double b) { return (a < b) ? 1.0 : 0.0; }));
    
    operations.emplace(">=", Operation(">=", OperationType::BINARY, 
        [](double a, double b) { return (a >= b) ? 1.0 : 0.0; }));
    
    operations.emplace("<=", Operation("<=", OperationType::BINARY, 
        [](double a, double b) { return (a <= b) ? 1.0 : 0.0; }));
    
    operations.emplace("==", Operation("==", OperationType::BINARY, 
        [](double a, double b) { return (std::abs(a - b) < 1e-10) ? 1.0 : 0.0; }));
    
    operations.emplace("!=", Operation("!=", OperationType::BINARY, 
        [](double a, double b) { return (std::abs(a - b) >= 1e-10) ? 1.0 : 0.0; }));
    
    // More stack operations
    operations.emplace("abs", Operation("abs", OperationType::UNARY, 
        [](double a, double) { return std::abs(a); }));
    
    operations.emplace("mod", Operation("mod", OperationType::BINARY, 
        [this](double a, double b) { 
            if (b == 0) {
                setError("Division by zero");
                return 0.0;
            }
            return std::fmod(a, b); 
        }));
    
    operations.emplace("round", Operation("round", OperationType::UNARY, 
        [](double a, double) { return std::round(a); }));
    
    operations.emplace("floor", Operation("floor", OperationType::UNARY, 
        [](double a, double) { return std::floor(a); }));
    
    operations.emplace("ceil", Operation("ceil", OperationType::UNARY, 
        [](double a, double) { return std::ceil(a); }));
    
    operations.emplace("min", Operation("min", OperationType::BINARY, 
        [](double a, double b) { return std::min(a, b); }));
    
    operations.emplace("max", Operation("max", OperationType::BINARY, 
        [](double a, double b) { return std::max(a, b); }));
}

void CalculatorModel::pushValue(double value) {
    stack.push_back(value);
    if (stack.size() > 100) {
        stack.erase(stack.begin());
    }
}

bool CalculatorModel::popValue(double& value) {
    if (stack.empty()) {
        setError("Stack underflow");
        return false;
    }
    value = stack.back();
    stack.pop_back();
    return true;
}

void CalculatorModel::drop() {
    if (!stack.empty()) {
        stack.pop_back();
    } else {
        setError("Stack is empty");
    }
}

void CalculatorModel::swap() {
    if (stack.size() >= 2) {
        std::swap(stack[stack.size() - 1], stack[stack.size() - 2]);
    } else {
        setError("Need at least 2 values on stack");
    }
}

void CalculatorModel::clear() {
    stack.clear();
    clearError();
}

void CalculatorModel::duplicate() {
    if (!stack.empty()) {
        pushValue(stack.back());
    } else {
        setError("Stack is empty");
    }
}

bool CalculatorModel::executeOperation(const std::string& opName) {
    // Special stack operations
    if (opName == "rot") {
        if (stack.size() >= 3) {
            double c = stack.back(); stack.pop_back();
            double b = stack.back(); stack.pop_back();
            double a = stack.back(); stack.pop_back();
            stack.push_back(b);
            stack.push_back(c);
            stack.push_back(a);
            addToHistory(opName);
            return true;
        } else {
            setError("Need at least 3 values on stack for rot");
            return false;
        }
    } else if (opName == "over") {
        if (stack.size() >= 2) {
            double b = stack.back(); stack.pop_back();
            double a = stack.back();
            stack.push_back(b);
            stack.push_back(a);
            addToHistory(opName);
            return true;
        } else {
            setError("Need at least 2 values on stack for over");
            return false;
        }
    } else if (opName == "pick") {
        if (!stack.empty()) {
            double n = stack.back(); stack.pop_back();
            int index = static_cast<int>(n);
            if (index >= 0 && index < stack.size()) {
                double value = stack[stack.size() - 1 - index];
                stack.push_back(value);
                addToHistory(opName);
                return true;
            } else {
                stack.push_back(n);
                setError("Invalid index for pick");
                return false;
            }
        } else {
            setError("Stack is empty");
            return false;
        }
    } else if (opName == "roll") {
        if (!stack.empty()) {
            double n = stack.back(); stack.pop_back();
            int count = static_cast<int>(n);
            if (count > 0 && count <= stack.size()) {
                std::rotate(stack.end() - count, stack.end() - 1, stack.end());
                addToHistory(opName);
                return true;
            } else {
                stack.push_back(n);
                setError("Invalid count for roll");
                return false;
            }
        } else {
            setError("Stack is empty");
            return false;
        }
    }
    
    auto it = operations.find(opName);
    if (it == operations.end()) {
        if (isFunctionDefined(opName)) {
            return executeFunction(opName);
        }
        setError("Unknown operation: " + opName);
        return false;
    }
    
    const Operation& op = it->second;
    clearError();
    
    if (op.type == OperationType::UNARY) {
        if (stack.size() < 1) {
            setError("Need at least 1 value on stack");
            return false;
        }
        double a;
        popValue(a);
        double result = op.func(a, 0);
        if (!hasError()) {
            pushValue(result);
            addToHistory(opName);
            return true;
        }
        pushValue(a);
        return false;
    } else {
        if (stack.size() < 2) {
            setError("Need at least 2 values on stack");
            return false;
        }
        double b, a;
        popValue(b);
        popValue(a);
        double result = op.func(a, b);
        if (!hasError()) {
            pushValue(result);
            addToHistory(opName);
            return true;
        }
        pushValue(a);
        pushValue(b);
        return false;
    }
}

void CalculatorModel::setInputBuffer(const std::string& buffer) {
    inputBuffer = buffer;
}

void CalculatorModel::appendToInput(const std::string& str) {
    inputBuffer += str;
}

void CalculatorModel::backspace() {
    if (!inputBuffer.empty()) {
        inputBuffer.pop_back();
    }
}

void CalculatorModel::clearInput() {
    inputBuffer.clear();
}

bool CalculatorModel::enterInput() {
    if (!inputBuffer.empty()) {
        if (inputBuffer.find('{') != std::string::npos && inputBuffer.find('}') != std::string::npos) {
            bool result = parseFunctionDefinition(inputBuffer);
            inputBuffer.clear();
            return result;
        }
        
        try {
            double value = std::stod(inputBuffer);
            pushValue(value);
            addToHistory(inputBuffer);
            inputBuffer.clear();
            return true;
        } catch (const std::exception&) {
            if (executeOperation(inputBuffer)) {
                inputBuffer.clear();
                return true;
            }
            setError("Invalid input: " + inputBuffer);
            return false;
        }
    } else if (!stack.empty()) {
        duplicate();
        return true;
    }
    return false;
}

void CalculatorModel::addToHistory(const std::string& entry) {
    history.push_back(entry);
    if (history.size() > 50) {
        history.erase(history.begin());
    }
}

void CalculatorModel::setError(const std::string& error) {
    errorMessage = error;
}

bool CalculatorModel::defineFunction(const std::string& name, const std::vector<std::string>& body) {
    if (operations.find(name) != operations.end()) {
        setError("Cannot redefine built-in operation: " + name);
        return false;
    }
    
    functions[name] = Function(name, body);
    addToHistory("def " + name);
    return true;
}

bool CalculatorModel::executeFunction(const std::string& name) {
    auto it = functions.find(name);
    if (it == functions.end()) {
        return false;
    }
    
    const Function& func = it->second;
    clearError();
    
    for (const std::string& token : func.body) {
        try {
            double value = std::stod(token);
            pushValue(value);
        } catch (const std::exception&) {
            if (operations.find(token) != operations.end()) {
                if (!executeOperation(token)) {
                    return false;
                }
            } else if (functions.find(token) != functions.end()) {
                if (!executeFunction(token)) {
                    return false;
                }
            } else {
                setError("Unknown token in function: " + token);
                return false;
            }
        }
    }
    
    addToHistory(name);
    return true;
}

bool CalculatorModel::isFunctionDefined(const std::string& name) const {
    return functions.find(name) != functions.end();
}

bool CalculatorModel::parseFunctionDefinition(const std::string& input) {
    size_t openBrace = input.find('{');
    size_t closeBrace = input.find('}');
    
    if (openBrace == std::string::npos || closeBrace == std::string::npos || openBrace >= closeBrace) {
        setError("Invalid function definition syntax. Use: functionName { body }");
        return false;
    }
    
    std::string namepart = input.substr(0, openBrace);
    std::string bodypart = input.substr(openBrace + 1, closeBrace - openBrace - 1);
    
    namepart.erase(0, namepart.find_first_not_of(" \t"));
    namepart.erase(namepart.find_last_not_of(" \t") + 1);
    
    if (namepart.empty()) {
        setError("Function name cannot be empty");
        return false;
    }
    
    std::vector<std::string> body;
    std::istringstream iss(bodypart);
    std::string token;
    while (iss >> token) {
        body.push_back(token);
    }
    
    return defineFunction(namepart, body);
}