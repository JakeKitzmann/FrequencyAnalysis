#include "ParseMTF.h"
#include <fstream>

std::array<double, 2> ParseMTF::execute()
{
    // read MTF csv
    std::vector<double> frequencies, modulations;

    // header 
    std::ifstream csv(csvName);
    std::string line;

    // skip header
    while(std::getline(csv, line)){
        if(!line.empty()) break;
    }

    // read csv
    while(std::getline(csv, line)){
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string freqStr, modStr;

        std::getline(ss, freqStr, ',');
        std::getline(ss, modStr, ',');

        frequencies.push_back(std::stod(freqStr));
        modulations.push_back(std::stod(modStr));
    }

    // handle stupid user
    auto intermediate = 0.0;
    if(modLow < modHigh){
        intermediate = modLow;
        modLow = modHigh;
        modHigh = intermediate;
    }

    if (frequencies.empty() || modulations.empty() || frequencies.size() != modulations.size()) {
        throw std::runtime_error("No valid data rows in CSV (or mismatched columns): " + csvName);
    }

    // match closest modulations to desired
    std::size_t idxLow = closestModulationIndex(modulations, modLow);
    std::size_t idxHigh = closestModulationIndex(modulations, modHigh);

    double flow = frequencies[idxLow] * 0.1; // times one tenth to convert to cycles / mm
    double fhigh = frequencies[idxHigh] * 0.1;

    return {flow, fhigh};
}

ParseMTF::ParseMTF(double modLowIn, double modHighIn, std::string csvNameIn)
{
    modLow = modLowIn;
    modHigh = modHighIn;
    csvName = csvNameIn;
}

// find closest modulation to the modulation we want
std::size_t ParseMTF::closestModulationIndex(const std::vector<double>& modulations, double target)
{
    if (modulations.empty()) throw std::runtime_error("No modulations loaded");
    double bestDiff = std::numeric_limits<double>::infinity();
    std::size_t bestIdx = 0;
    for (std::size_t i = 0; i < modulations.size(); ++i) {
        double diff = std::fabs(modulations[i] - target);
        if (diff < bestDiff) { bestDiff = diff; bestIdx = i; }
    }
    return bestIdx;
}