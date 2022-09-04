#ifndef CIRCUITS_HPP
#define CIRCUITS_HPP
#include "../headers/basicLibraries.hpp"
#include "../headers/json.hpp"
template <typename T>
class Circuits
{
private:
    std::vector<T> &m_vectorDatabase;
    using json = nlohmann::json;
    json &m_JSONFile;
    json m_IZValues;
    json m_correctionRateValues;
    int m_vectorDatabaseIterator = 0;
    void convertToStruct(std::string segment)
    {
        std::string main = "circuits";
        for (int i = 0; i < m_JSONFile[main][segment].size(); i++)
        {
            T tempStruct{};
            tempStruct.circuitType = segment;
            tempStruct.circuitID = m_JSONFile[main][segment][i]["circuitID"];
            tempStruct.wireLength = m_JSONFile[main][segment][i]["wireLength"];
            tempStruct.crossSection = m_JSONFile[main][segment][i]["crossSection"];
            tempStruct.powerConsumption = m_JSONFile[main][segment][i]["powerConsumption"];
            tempStruct.voltage = m_JSONFile[main][segment][i]["voltage"];
            tempStruct.cableManagementMethod = m_JSONFile[main][segment][i]["cableManagementMethod"];
            tempStruct.neighborlyCables = m_JSONFile[main][segment][i]["neighborlyCables"];
            tempStruct.circuitBreaker = m_JSONFile[main][segment][i]["circuitBreaker"];
            std::cout<<"TEst";
            tempStruct.IZ = getIZ(tempStruct.cableManagementMethod, tempStruct.crossSection, calculateAmountOfWires(tempStruct.voltage));
            std::cout<<"TEst1";
            tempStruct.correctionRate = getCorrectionRate(tempStruct.cableManagementMethod, tempStruct.neighborlyCables);
            m_vectorDatabase.push_back(tempStruct);
        }
    }
    double getIZ(std::string_view cableMngmtMthd, double crossSection, int amountOfWires)
    {
        int crossSectionInt = (crossSection*10);

        std::string amountOfWiresString = std::to_string(amountOfWires);
        std::string crossSectionString = std::to_string(crossSection);
        crossSectionString.erase ( crossSectionString.find_last_not_of('0') + 1, std::string::npos );
        crossSectionString.erase ( crossSectionString.find_last_not_of('.') + 1, std::string::npos );
        std::cout<<"amountOfwires and cross section"<<amountOfWiresString<<"and"<<crossSectionString;
        return m_IZValues[crossSectionString][cableMngmtMthd][amountOfWiresString];
    }
    double getCorrectionRate(std::string_view cableMngmtMthd, double neighborlyCables)
    {
        if (neighborlyCables > 20)
        {
            neighborlyCables = 20;
        }
        std::string neighborlyCablesString = std::to_string(neighborlyCables);
        neighborlyCablesString.erase (neighborlyCablesString.find_last_not_of('0') + 1, std::string::npos );
        neighborlyCablesString.erase (neighborlyCablesString.find_last_not_of('.') + 1, std::string::npos );
        return m_correctionRateValues[cableMngmtMthd][neighborlyCablesString];
    }
    double calculateAmountOfWires(double voltage)
    {
        return voltage > 270 ? 3 : 2;
    }

public:
    Circuits(std::vector<T> &properties, json &jsonFile) : m_vectorDatabase{properties}, m_JSONFile{jsonFile}
    {
        std::ifstream IZPath{"../IZ.json"};
        std::ifstream correctionRatePath{"../correctionRate.json"};
        m_IZValues = json::parse(IZPath);
        //std::cout<<m_IZValues.dump(2);
        m_correctionRateValues = json::parse(correctionRatePath);
        //std::cout<<m_correctionRateValues.dump(1);
    }
    void calculateAll()
    {
        convertToStruct("G");
        printAllStructs();
    }
    void printAllStructs()
    {
        for (int i{0}; auto circuit : m_vectorDatabase)
        {
            std::cout << "\nTYPE:" << circuit.circuitType;
            std::cout << "\nID:" << circuit.circuitID;
            std::cout << "\nWireLength:" << circuit.wireLength;
            std::cout << "\ncrossSection:" << circuit.crossSection;
            std::cout << "\npowerConsumption:" << circuit.powerConsumption;
            std::cout << "\nVoltage:" << circuit.voltage;
            std::cout << "\nCableManagementMethod:" << circuit.cableManagementMethod;
            std::cout << "\nneighborlyCables:" << circuit.neighborlyCables;
            std::cout << "\ncircuitBreaker:" << circuit.circuitBreaker;
            std::cout << "\nIB:" << circuit.IB;
            std::cout << "\nIZ:" << circuit.IZ;
            std::cout << "\nCorrectionRate:" << circuit.correctionRate;
            std::cout << "\nvoltageDrop:" << circuit.voltageDrop;
            std::cout << "\npowerShutdown:" << circuit.powerShutdown;
            std::cout << "\nIZCorrect:" << circuit.isIZCorrect;
            std::cout << "\nisVoltageDropCorrect:" << circuit.isVoltageDropCorrect;
            std::cout << "\nisCircuitBreakerCorrect:" << circuit.isCircuitBreakerCorrect;
        }
    }
};
#endif