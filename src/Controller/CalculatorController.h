#ifndef CALCULATOR_CONTROLLER_H
#define CALCULATOR_CONTROLLER_H

#include <memory>
#include <string>

class CalculatorModel;
class CalculatorView;

class CalculatorController {
public:
    CalculatorController(std::shared_ptr<CalculatorModel> model, 
                        std::shared_ptr<CalculatorView> view);
    ~CalculatorController() = default;
    
    void initialize();
    void update();
    
    void handleKeyboardInput();
    
private:
    std::shared_ptr<CalculatorModel> model;
    std::shared_ptr<CalculatorView> view;
    
    void onNumberPressed(const std::string& number);
    void onOperationPressed(const std::string& operation);
    void onEnterPressed();
    void onDropPressed();
    void onSwapPressed();
    void onClearPressed();
    void onBackspacePressed();
    
    void processKeyPress(int key);
};

#endif