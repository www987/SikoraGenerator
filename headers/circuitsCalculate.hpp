#ifndef CIRCUITSCALCULATE_HPP
#define CIRCUITSCALCULATE_HPP
#include "basicLibraries.hpp"
class CircuitsCalculate{
private:
    std::string m_circuitType;
    double m_maxVoltageDrop{0.4};
public:
    CircuitsCalculate(std::string circuitType): m_circuitType{circuitType}{};
    double countIB(double powerConsumption, double voltage);
    bool checkIZ(double IZ, double IB, double correctionRate);
    double countVoltageDrop(double powerConsumption, double wireLength,double crossSection,double voltage);
    bool checkVoltageDrop(double voltageDrop);
    bool checkCircuitBreaker(double IB, double IZ, double circuitBreaker);
    double countR(double wireLength, double crossSection, double voltage);
    double countX(double wireLength);
    double countRsum(double voltage, double R);
    double countXsum(double voltage,double X);
    double countZ(double Xsum, double Rsum);
    double countIK(double voltage, double Z);
};
#endif