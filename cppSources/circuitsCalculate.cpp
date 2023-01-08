#ifndef CIRCUITSCALCULATE_CPP
#define CIRCUITSCALCULATE_CPP
#include "../headers/circuitsCalculate.hpp"
#include <cmath>
double CircuitsCalculate::countIB(double powerConsumption, double voltage)
{
    double cosY=0.95;
    if(voltage > 270){
        return (powerConsumption/(sqrt(3)*voltage*cosY));
    }
    return (powerConsumption/(voltage*cosY));
}
bool CircuitsCalculate::checkIZ(double IZ, double IB, double correctionRate)
{
    return IZ > (IB/correctionRate)?1:0;
}
double CircuitsCalculate::countVoltageDrop(double powerConsumption, double wireLength, double crossSection, double voltage)
{
    int phaseRate{};
    (m_circuitType=="WLZ" || m_circuitType=="W") ? phaseRate = 100 : phaseRate = 200;
    double materialConductance{56};
    return (phaseRate * powerConsumption*wireLength)/(materialConductance*crossSection*pow(voltage,2));
}
bool CircuitsCalculate::checkVoltageDrop(double voltageDrop)
{
    double maxVoltageDrop = 4;
    return maxVoltageDrop > voltageDrop ? 1:0;
}
bool CircuitsCalculate::checkCircuitBreaker(double IB, double IZ, double circuitBreaker)
{
    if(m_circuitType=="WLZ") return 200;
    return (IZ >= circuitBreaker) && (circuitBreaker >= IB) ? 1:0;
}
double CircuitsCalculate::countR(double wireLength, double crossSection, double voltage){
    double materialConductance{56};
    if(voltage > 270){
        return ((wireLength)/(materialConductance * crossSection))*1000;
    }
    else{
        return ((wireLength)/(materialConductance * crossSection))*1000;
    }
}
double CircuitsCalculate::countX(double wireLength){
    double reactance{0.08};
    return (reactance * wireLength);
}
double CircuitsCalculate::countRsum(double voltage, double R){
    if(voltage > 270){
        return (57.32 + R);
    }
    else{
        return (8.32 + 1.24 * (2*43.3 + 2*5.7 + 2*R));
    }
}
double CircuitsCalculate::countXsum(double voltage, double X ){
    if(voltage > 270){
        return (38.95 + X);
    }
    else{
        return (1.171 + 28.08 + 2* (2*7.9 + 2*1.8 + 2*X));
    }
}
double CircuitsCalculate::countZ(double Xsum, double Rsum){
    return std::sqrt(Xsum*Xsum + Rsum*Rsum);
}
double CircuitsCalculate::countIK(double voltage, double Z){
    double c{};
    if(voltage > 270){
        c = 1;
        return ((c*voltage)/(std::sqrt(3)*Z*0.001));
    }
    else{
        c = 0.95;
        return ((c*voltage)/(Z*0.001));
    }
}
#endif