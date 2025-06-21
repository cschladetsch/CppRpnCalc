#pragma once
#include <memory>
#include <vector>
#include <string>

class CalculatorModel;

namespace RPN {

class GraphData;
class GraphFunction;

class GraphView {
public:
    GraphView(CalculatorModel* calculator);
    ~GraphView();
    
    void Render();
    
    void SetExpression(const std::string& expr);
    void PlotExpression();
    void PlotData(std::shared_ptr<GraphData> data);
    void Clear();
    
    void SetRange(double xMin, double xMax, double yMin, double yMax);
    void AutoRange();
    
    bool IsVisible() const { return visible; }
    void SetVisible(bool vis) { visible = vis; }
    void ToggleVisible() { visible = !visible; }

private:
    CalculatorModel* calculator;
    std::unique_ptr<GraphFunction> function;
    std::vector<std::shared_ptr<GraphData>> plotData;
    
    bool visible = false;
    bool autoFit = true;
    
    double xMin = -10.0;
    double xMax = 10.0;
    double yMin = -10.0;
    double yMax = 10.0;
    
    std::string currentExpression;
    std::string errorMessage;
    
    void RenderControls();
    void RenderPlot();
};

}