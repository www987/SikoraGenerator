#ifndef LATEXRAPORT_HPP
#define LATEXRAPORT_HPP
#include "basicLibraries.hpp"
#include <sstream>
// Class to generate single circuit to Raport. 
class LatexRaport
{
private:
    std::ofstream& m_latexFile;
    std::ofstream& m_latexFileTable;
public:
    LatexRaport(
        std::ofstream& latexFile, std::ofstream& latexFileTable
    ): m_latexFile(latexFile), m_latexFileTable(latexFileTable){}
    void generateCircuit();
    void generateSectionStart(std::string typeSection);
    void generateSectionEnd();
    void printData(double powerConsumption, double voltage, double crossSection, double wireLength, double CircuitBreaker);
    void printIZCheck(bool isIZCorrect, double IZ, double IB);
    void printVoltageDropCheck(bool isVDCorrect, double voltageDrop, std::string circuitType, double voltageDropWLZ);
    void printCircuitBreakerCheck(bool isCBCorrect, double IB, double IZ, double CB, const std::string& circuitType);
    void generateConclusion(bool isCBCorrect, bool isVDCorrect, bool isIZCorrect);
    void generateTable(std::string typeSection,double ID, double wireLength,double crossSection, double powerConsumption, double voltage, double VD, double IB, double R, double X, double Rsum, double Xsum, double Z, double IK);
};
#endif