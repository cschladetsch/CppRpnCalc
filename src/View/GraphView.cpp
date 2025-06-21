#include "GraphView.h"
#include "../Model/GraphData.h"
#include "../Model/GraphFunction.h"
#include "../Model/CalculatorModel.h"
#include <imgui.h>
#include <implot.h>
#include <algorithm>
#include <cmath>

namespace RPN {

GraphView::GraphView(CalculatorModel* calc) 
    : calculator(calc), function(std::make_unique<GraphFunction>(calc)) {
    ImPlot::CreateContext();
}

GraphView::~GraphView() {
    ImPlot::DestroyContext();
}

void GraphView::Render() {
    if (!visible) return;
    
    ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Graph View", &visible)) {
        RenderControls();
        ImGui::Separator();
        RenderPlot();
    }
    ImGui::End();
}

void GraphView::RenderControls() {
    static char expressionBuffer[256] = "";
    
    ImGui::Text("Function Expression (use 'x' as variable):");
    if (ImGui::InputText("f(x)", expressionBuffer, sizeof(expressionBuffer), 
                         ImGuiInputTextFlags_EnterReturnsTrue)) {
        SetExpression(expressionBuffer);
        PlotExpression();
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Plot")) {
        SetExpression(expressionBuffer);
        PlotExpression();
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        Clear();
        expressionBuffer[0] = '\0';
    }
    
    if (!errorMessage.empty()) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Error: %s", errorMessage.c_str());
    }
    
    ImGui::Separator();
    
    ImGui::Text("Range Settings:");
    ImGui::Checkbox("Auto Fit", &autoFit);
    
    if (!autoFit) {
        ImGui::PushItemWidth(100);
        ImGui::InputDouble("X Min", &xMin);
        ImGui::SameLine();
        ImGui::InputDouble("X Max", &xMax);
        ImGui::InputDouble("Y Min", &yMin);
        ImGui::SameLine();
        ImGui::InputDouble("Y Max", &yMax);
        ImGui::PopItemWidth();
    }
    
    if (ImGui::Button("Reset Range")) {
        xMin = yMin = -10.0;
        xMax = yMax = 10.0;
    }
}

void GraphView::RenderPlot() {
    if (autoFit && !plotData.empty()) {
        AutoRange();
    }
    
    if (ImPlot::BeginPlot("##Graph", ImVec2(-1, -1))) {
        ImPlot::SetupAxes("x", "y");
        ImPlot::SetupAxisLimits(ImAxis_X1, xMin, xMax);
        ImPlot::SetupAxisLimits(ImAxis_Y1, yMin, yMax);
        
        for (const auto& data : plotData) {
            if (!data->IsEmpty()) {
                std::vector<double> xData, yData;
                xData.reserve(data->GetSize());
                yData.reserve(data->GetSize());
                
                for (const auto& point : data->GetPoints()) {
                    xData.push_back(point.x);
                    yData.push_back(point.y);
                }
                
                ImPlot::PlotLine(data->GetLabel().c_str(), 
                                xData.data(), yData.data(), 
                                static_cast<int>(xData.size()));
            }
        }
        
        ImPlot::EndPlot();
    }
}

void GraphView::SetExpression(const std::string& expr) {
    currentExpression = expr;
    errorMessage.clear();
}

void GraphView::PlotExpression() {
    if (currentExpression.empty()) {
        errorMessage = "Please enter an expression";
        return;
    }
    
    if (!function->SetExpression(currentExpression)) {
        errorMessage = function->GetLastError();
        return;
    }
    
    auto data = function->Evaluate(xMin, xMax);
    if (data->IsEmpty()) {
        errorMessage = "Failed to evaluate function";
        return;
    }
    
    plotData.clear();
    plotData.push_back(data);
    errorMessage.clear();
}

void GraphView::PlotData(std::shared_ptr<GraphData> data) {
    if (data && !data->IsEmpty()) {
        plotData.push_back(data);
    }
}

void GraphView::Clear() {
    plotData.clear();
    currentExpression.clear();
    errorMessage.clear();
}

void GraphView::SetRange(double xMinVal, double xMaxVal, double yMinVal, double yMaxVal) {
    xMin = xMinVal;
    xMax = xMaxVal;
    yMin = yMinVal;
    yMax = yMaxVal;
    autoFit = false;
}

void GraphView::AutoRange() {
    if (plotData.empty()) return;
    
    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();
    
    for (const auto& data : plotData) {
        double dataMinX, dataMaxX, dataMinY, dataMaxY;
        data->GetBounds(dataMinX, dataMaxX, dataMinY, dataMaxY);
        
        minX = std::min(minX, dataMinX);
        maxX = std::max(maxX, dataMaxX);
        minY = std::min(minY, dataMinY);
        maxY = std::max(maxY, dataMaxY);
    }
    
    double xMargin = (maxX - minX) * 0.1;
    double yMargin = (maxY - minY) * 0.1;
    
    xMin = minX - xMargin;
    xMax = maxX + xMargin;
    yMin = minY - yMargin;
    yMax = maxY + yMargin;
}

}