#include "CalculatorView.h"
#include "../Model/CalculatorModel.h"
#include "GraphView.h"
#include <sstream>
#include <iomanip>

CalculatorView::CalculatorView() {
}

CalculatorView::~CalculatorView() {
}

void CalculatorView::render(const CalculatorModel& model) {
    if (!modelPtr) {
        modelPtr = const_cast<CalculatorModel*>(&model);
        graphView = std::make_unique<RPN::GraphView>(modelPtr);
    }
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Calculator", nullptr, 
        ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse);
    
    renderStack(model.getStack());
    renderInput(model.getInputBuffer());
    
    if (model.hasError()) {
        renderError(model.getError());
    }
    
    renderButtons();
    
    ImGui::End();
    
    if (graphView) {
        graphView->Render();
    }
}

void CalculatorView::renderStack(const std::vector<double>& stack) {
    ImGui::BeginChild("Stack", ImVec2(0, 200), true);
    ImGui::Text("Stack:");
    ImGui::Separator();
    
    for (int i = stack.size() - 1; i >= 0; i--) {
        std::stringstream ss;
        ss << std::setprecision(10) << stack[i];
        ImGui::Text("%zu: %s", stack.size() - i, ss.str().c_str());
    }
    
    if (stack.empty()) {
        ImGui::TextDisabled("(empty)");
    }
    
    ImGui::EndChild();
}

void CalculatorView::renderInput(const std::string& input) {
    ImGui::Text("Input: %s", input.empty() ? "_" : input.c_str());
    ImGui::Separator();
}

void CalculatorView::renderError(const std::string& error) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
    ImGui::Text("Error: %s", error.c_str());
    ImGui::PopStyleColor();
}

void CalculatorView::renderButtons() {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));
    
    // Row 1: 7 8 9 / sin
    ImGui::PushStyleColor(ImGuiCol_Button, numberButtonColor);
    renderButton("7", buttonWidth, [this]() { if (numberCallback) numberCallback("7"); });
    ImGui::SameLine();
    renderButton("8", buttonWidth, [this]() { if (numberCallback) numberCallback("8"); });
    ImGui::SameLine();
    renderButton("9", buttonWidth, [this]() { if (numberCallback) numberCallback("9"); });
    ImGui::PopStyleColor();
    
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, operationButtonColor);
    renderButton("/", buttonWidth, [this]() { if (operationCallback) operationCallback("/"); });
    ImGui::SameLine();
    renderButton("sin", buttonWidth, [this]() { if (operationCallback) operationCallback("sin"); });
    ImGui::PopStyleColor();
    
    // Row 2: 4 5 6 * cos
    ImGui::PushStyleColor(ImGuiCol_Button, numberButtonColor);
    renderButton("4", buttonWidth, [this]() { if (numberCallback) numberCallback("4"); });
    ImGui::SameLine();
    renderButton("5", buttonWidth, [this]() { if (numberCallback) numberCallback("5"); });
    ImGui::SameLine();
    renderButton("6", buttonWidth, [this]() { if (numberCallback) numberCallback("6"); });
    ImGui::PopStyleColor();
    
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, operationButtonColor);
    renderButton("*", buttonWidth, [this]() { if (operationCallback) operationCallback("*"); });
    ImGui::SameLine();
    renderButton("cos", buttonWidth, [this]() { if (operationCallback) operationCallback("cos"); });
    ImGui::PopStyleColor();
    
    // Row 3: 1 2 3 - tan
    ImGui::PushStyleColor(ImGuiCol_Button, numberButtonColor);
    renderButton("1", buttonWidth, [this]() { if (numberCallback) numberCallback("1"); });
    ImGui::SameLine();
    renderButton("2", buttonWidth, [this]() { if (numberCallback) numberCallback("2"); });
    ImGui::SameLine();
    renderButton("3", buttonWidth, [this]() { if (numberCallback) numberCallback("3"); });
    ImGui::PopStyleColor();
    
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, operationButtonColor);
    renderButton("-", buttonWidth, [this]() { if (operationCallback) operationCallback("-"); });
    ImGui::SameLine();
    renderButton("tan", buttonWidth, [this]() { if (operationCallback) operationCallback("tan"); });
    ImGui::PopStyleColor();
    
    // Row 4: 0 . +/- + sqrt
    ImGui::PushStyleColor(ImGuiCol_Button, numberButtonColor);
    renderButton("0", buttonWidth, [this]() { if (numberCallback) numberCallback("0"); });
    ImGui::SameLine();
    renderButton(".", buttonWidth, [this]() { if (numberCallback) numberCallback("."); });
    ImGui::PopStyleColor();
    
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, operationButtonColor);
    renderButton("+/-", buttonWidth, [this]() { if (operationCallback) operationCallback("+/-"); });
    ImGui::SameLine();
    renderButton("+", buttonWidth, [this]() { if (operationCallback) operationCallback("+"); });
    ImGui::SameLine();
    renderButton("sqrt", buttonWidth, [this]() { if (operationCallback) operationCallback("sqrt"); });
    ImGui::PopStyleColor();
    
    // Row 5: ENTER DROP SWAP ^
    ImGui::PushStyleColor(ImGuiCol_Button, controlButtonColor);
    renderButton("ENTER", buttonWidth * 2 + spacing, [this]() { if (enterCallback) enterCallback(); });
    ImGui::SameLine();
    renderButton("DROP", buttonWidth, [this]() { if (dropCallback) dropCallback(); });
    ImGui::SameLine();
    renderButton("SWAP", buttonWidth, [this]() { if (swapCallback) swapCallback(); });
    ImGui::PopStyleColor();
    
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, operationButtonColor);
    renderButton("^", buttonWidth, [this]() { if (operationCallback) operationCallback("^"); });
    ImGui::PopStyleColor();
    
    // Row 6: CLEAR BACK 1/x ln log
    ImGui::PushStyleColor(ImGuiCol_Button, controlButtonColor);
    renderButton("CLEAR", buttonWidth * 2 + spacing, [this]() { if (clearCallback) clearCallback(); });
    ImGui::SameLine();
    renderButton("BACK", buttonWidth, [this]() { if (backspaceCallback) backspaceCallback(); });
    ImGui::PopStyleColor();
    
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, operationButtonColor);
    renderButton("1/x", buttonWidth, [this]() { if (operationCallback) operationCallback("1/x"); });
    ImGui::SameLine();
    renderButton("ln", buttonWidth * 0.5f - spacing/2, [this]() { if (operationCallback) operationCallback("ln"); });
    ImGui::SameLine();
    renderButton("log", buttonWidth * 0.5f - spacing/2, [this]() { if (operationCallback) operationCallback("log"); });
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.4f, 1.0f));
    if (ImGui::Button("Graph View", ImVec2(-1, buttonHeight))) {
        toggleGraphView();
    }
    ImGui::PopStyleColor();
    
    ImGui::PopStyleVar();
}

void CalculatorView::renderButton(const std::string& label, float width, std::function<void()> callback) {
    if (ImGui::Button(label.c_str(), ImVec2(width, buttonHeight))) {
        if (callback) {
            callback();
        }
    }
}

void CalculatorView::setNumberCallback(std::function<void(const std::string&)> callback) {
    numberCallback = callback;
}

void CalculatorView::setOperationCallback(std::function<void(const std::string&)> callback) {
    operationCallback = callback;
}

void CalculatorView::setEnterCallback(std::function<void()> callback) {
    enterCallback = callback;
}

void CalculatorView::setDropCallback(std::function<void()> callback) {
    dropCallback = callback;
}

void CalculatorView::setSwapCallback(std::function<void()> callback) {
    swapCallback = callback;
}

void CalculatorView::setClearCallback(std::function<void()> callback) {
    clearCallback = callback;
}

void CalculatorView::setBackspaceCallback(std::function<void()> callback) {
    backspaceCallback = callback;
}

void CalculatorView::toggleGraphView() {
    if (graphView) {
        graphView->ToggleVisible();
    }
}