#ifndef CALCULATOR_VIEW_H
#define CALCULATOR_VIEW_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <imgui.h>

class CalculatorModel;

namespace RPN {
    class GraphView;
}

class CalculatorView {
public:
    struct ButtonCallback {
        std::function<void()> onNumberPress;
        std::function<void()> onOperationPress;
        std::function<void()> onEnterPress;
        std::function<void()> onDropPress;
        std::function<void()> onSwapPress;
        std::function<void()> onClearPress;
        std::function<void()> onBackspacePress;
    };
    
    CalculatorView();
    ~CalculatorView();
    
    void render(const CalculatorModel& model);
    
    void setNumberCallback(std::function<void(const std::string&)> callback);
    void setOperationCallback(std::function<void(const std::string&)> callback);
    void setEnterCallback(std::function<void()> callback);
    void setDropCallback(std::function<void()> callback);
    void setSwapCallback(std::function<void()> callback);
    void setClearCallback(std::function<void()> callback);
    void setBackspaceCallback(std::function<void()> callback);
    
    void toggleGraphView();

private:
    struct Button {
        std::string label;
        float width;
        std::function<void()> callback;
    };
    
    void renderStack(const std::vector<double>& stack);
    void renderInput(const std::string& input);
    void renderError(const std::string& error);
    void renderButtons();
    void renderButton(const std::string& label, float width, std::function<void()> callback);
    
    std::function<void(const std::string&)> numberCallback;
    std::function<void(const std::string&)> operationCallback;
    std::function<void()> enterCallback;
    std::function<void()> dropCallback;
    std::function<void()> swapCallback;
    std::function<void()> clearCallback;
    std::function<void()> backspaceCallback;
    
    float buttonWidth = 60.0f;
    float buttonHeight = 40.0f;
    float spacing = 4.0f;
    
    ImVec4 numberButtonColor = ImVec4(0.2f, 0.2f, 0.3f, 1.0f);
    ImVec4 operationButtonColor = ImVec4(0.3f, 0.2f, 0.2f, 1.0f);
    ImVec4 controlButtonColor = ImVec4(0.2f, 0.3f, 0.2f, 1.0f);
    
    std::unique_ptr<RPN::GraphView> graphView;
    CalculatorModel* modelPtr = nullptr;
};

#endif