#include "ImageTypes.h"
#include <cstdlib>

class ParseMTF
{
    private:
        double modLow;
        double modHigh;
        std::string csvName;
        std::size_t closestModulationIndex(const std::vector<double>& modulations, double target);
    public:
        ParseMTF(double modLowIn, double modHighIn, std::string csvNameIn);
        std::array<double, 2> execute();
};