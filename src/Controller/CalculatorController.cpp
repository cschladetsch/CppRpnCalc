#include "CalculatorController.h"
#include "../Model/CalculatorModel.h"
#include "../View/CalculatorView.h"
#include <imgui.h>
#include <GLFW/glfw3.h>

CalculatorController::CalculatorController(std::shared_ptr<CalculatorModel> model, 
                                         std::shared_ptr<CalculatorView> view)
    : model(model), view(view) {
}

void CalculatorController::initialize() {
    view->setNumberCallback([this](const std::string& num) { onNumberPressed(num); });
    view->setOperationCallback([this](const std::string& op) { onOperationPressed(op); });
    view->setEnterCallback([this]() { onEnterPressed(); });
    view->setDropCallback([this]() { onDropPressed(); });
    view->setSwapCallback([this]() { onSwapPressed(); });
    view->setClearCallback([this]() { onClearPressed(); });
    view->setBackspaceCallback([this]() { onBackspacePressed(); });
}

void CalculatorController::update() {
    handleKeyboardInput();
    view->render(*model);
}

void CalculatorController::handleKeyboardInput() {
    ImGuiIO& io = ImGui::GetIO();
    
    // Handle number keys
    for (int i = 0; i <= 9; i++) {
        if (ImGui::IsKeyPressed((ImGuiKey)(ImGuiKey_0 + i))) {
            onNumberPressed(std::to_string(i));
        }
    }
    
    // Handle numpad keys
    for (int i = 0; i <= 9; i++) {
        if (ImGui::IsKeyPressed((ImGuiKey)(ImGuiKey_Keypad0 + i))) {
            onNumberPressed(std::to_string(i));
        }
    }
    
    // Handle decimal point
    if (ImGui::IsKeyPressed(ImGuiKey_Period) || ImGui::IsKeyPressed(ImGuiKey_KeypadDecimal)) {
        onNumberPressed(".");
    }
    
    // Handle operations
    if (ImGui::IsKeyPressed(ImGuiKey_KeypadAdd) || 
        (ImGui::IsKeyPressed(ImGuiKey_Equal) && io.KeyShift)) {
        onOperationPressed("+");
    }
    
    if (ImGui::IsKeyPressed(ImGuiKey_KeypadSubtract) || ImGui::IsKeyPressed(ImGuiKey_Minus)) {
        onOperationPressed("-");
    }
    
    if (ImGui::IsKeyPressed(ImGuiKey_KeypadMultiply) || 
        (ImGui::IsKeyPressed(ImGuiKey_8) && io.KeyShift)) {
        onOperationPressed("*");
    }
    
    if (ImGui::IsKeyPressed(ImGuiKey_KeypadDivide) || ImGui::IsKeyPressed(ImGuiKey_Slash)) {
        onOperationPressed("/");
    }
    
    // Handle Enter
    if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)) {
        onEnterPressed();
    }
    
    // Handle Backspace
    if (ImGui::IsKeyPressed(ImGuiKey_Backspace)) {
        onBackspacePressed();
    }
    
    // Handle Delete (acts as drop)
    if (ImGui::IsKeyPressed(ImGuiKey_Delete)) {
        onDropPressed();
    }
    
    // Handle Escape (clear)
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        onClearPressed();
    }
    
    // Handle special operations with keyboard shortcuts
    if (io.KeyCtrl) {
        if (ImGui::IsKeyPressed(ImGuiKey_S)) {
            onSwapPressed();
        }
        if (ImGui::IsKeyPressed(ImGuiKey_D)) {
            onDropPressed();
        }
        if (ImGui::IsKeyPressed(ImGuiKey_C)) {
            onClearPressed();
        }
    }
}

void CalculatorController::onNumberPressed(const std::string& number) {
    model->appendToInput(number);
}

void CalculatorController::onOperationPressed(const std::string& operation) {
    // If there's input, enter it first
    if (!model->getInputBuffer().empty()) {
        model->enterInput();
    }
    model->executeOperation(operation);
}

void CalculatorController::onEnterPressed() {
    model->enterInput();
}

void CalculatorController::onDropPressed() {
    model->drop();
}

void CalculatorController::onSwapPressed() {
    model->swap();
}

void CalculatorController::onClearPressed() {
    model->clear();
    model->clearInput();
}

void CalculatorController::onBackspacePressed() {
    model->backspace();
}