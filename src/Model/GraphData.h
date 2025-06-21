#pragma once
#include <vector>
#include <string>

namespace RPN {

class GraphData {
public:
    struct Point {
        double x;
        double y;
    };

    GraphData() = default;
    
    void Clear();
    void AddPoint(double x, double y);
    void SetData(const std::vector<double>& xData, const std::vector<double>& yData);
    
    const std::vector<Point>& GetPoints() const { return points; }
    bool IsEmpty() const { return points.empty(); }
    size_t GetSize() const { return points.size(); }
    
    void GetBounds(double& xMin, double& xMax, double& yMin, double& yMax) const;
    
    void SetLabel(const std::string& label) { this->label = label; }
    const std::string& GetLabel() const { return label; }

private:
    std::vector<Point> points;
    std::string label;
};

}