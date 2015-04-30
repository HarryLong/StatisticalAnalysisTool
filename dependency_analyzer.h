#include <map>
#include <set>
#include <vector>

class AnalysisPoint;
class DependencyAnalyzer{
public:
    typedef std::map<int, std::set<int> > Dependencies;

    static Dependencies getDependencies(std::map<int, std::vector<AnalysisPoint*> > points, int analysis_window_width, int analysis_window_height);
};
