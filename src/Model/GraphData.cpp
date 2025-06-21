#include "GraphData.h"
#include <limits>
#include <algorithm>

namespace RPN {

void GraphData::Clear() {
    points.clear();
}

void GraphData::AddPoint(double x, double y) {
    points.push_back({x, y});
}

void GraphData::SetData(const std::vector<double>& xData, const std::vector<double>& yData) {
    points.clear();
    
    size_t minSize = std::min(xData.size(), yData.size());
    points.reserve(minSize);
    
    for (size_t i = 0; i < minSize; ++i) {
        points.push_back({xData[i], yData[i]});
    }
}

void GraphData::GetBounds(double& xMin, double& xMax, double& yMin, double& yMax) const {
    if (points.empty()) {
        xMin = xMax = yMin = yMax = 0.0;
        return;
    }
    
    xMin = yMin = std::numeric_limits<double>::max();
    xMax = yMax = std::numeric_limits<double>::lowest();
    
    for (const auto& point : points) {
        xMin = std::min(xMin, point.x);
        xMax = std::max(xMax, point.x);
        yMin = std::min(yMin, point.y);
        yMax = std::max(yMax, point.y);
    }
}

}