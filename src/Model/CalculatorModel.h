#ifndef CALCULATOR_MODEL_H
#define CALCULATOR_MODEL_H

#include <vector>
#include <string>
#include <functional>
#include <unordered_map>
#include <queue>

class CalculatorModel {
public:
    enum class OperationType {
        UNARY,
        BINARY,
        SPECIAL
    };

    struct Operation {
        std::string name;
        OperationType type;
        std::function<double(double, double)> func;
        
        Operation(const std::string& n, OperationType t, std::function<double(double, double)> f)
            : name(n), type(t), func(f) {}
    };

    struct Function {
        std::string name;
        std::vector<std::string> body;
        
        Function() = default;
        Function(const std::string& n, const std::vector<std::string>& b)
            : name(n), body(b) {}
    };

    CalculatorModel();

    void pushValue(double value);
    bool popValue(double& value);
    void drop();
    void swap();
    void clear();
    void duplicate();
    
    bool executeOperation(const std::string& opName);
    
    bool defineFunction(const std::string& name, const std::vector<std::string>& body);
    bool executeFunction(const std::string& name);
    bool isFunctionDefined(const std::string& name) const;
    bool parseFunctionDefinition(const std::string& input);
    
    void setInputBuffer(const std::string& buffer);
    void appendToInput(const std::string& str);
    void backspace();
    void clearInput();
    bool enterInput();
    
    const std::vector<double>& getStack() const { return stack; }
    const std::string& getInputBuffer() const { return inputBuffer; }
    const std::vector<std::string>& getHistory() const { return history; }
    const std::unordered_map<std::string, Function>& getFunctions() const { return functions; }
    
    bool hasError() const { return !errorMessage.empty(); }
    const std::string& getError() const { return errorMessage; }
    void clearError() { errorMessage.clear(); }

private:
    std::vector<double> stack;
    std::string inputBuffer;
    std::vector<std::string> history;
    std::string errorMessage;
    std::unordered_map<std::string, Operation> operations;
    std::unordered_map<std::string, Function> functions;
    
    void registerOperations();
    void addToHistory(const std::string& entry);
    void setError(const std::string& error);
};

#endif