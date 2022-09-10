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
    double materialConductance{58.8};
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
#endif