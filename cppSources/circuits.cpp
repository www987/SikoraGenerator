#ifndef CIRCUITS_CPP
#define CIRCUITS_CPP
#include "../headers/basicLibraries.hpp"
#include "../headers/json.hpp"
#include "../headers/circuitsCalculate.hpp"
template <typename T>
class Circuits
{
private:
    std::vector<T> &m_vectorDatabase;
    using json = nlohmann::json;
    json &m_JSONFile;
    json m_IZValues;
    json m_correctionRateValues;
    //This function convert JSON properties to struct in main.cpp. JSON have to be validated first 
    void convertToStruct(std::string segment)
    {
        std::string main = "circuits";
        // if W is empty we can not allow to loop through JSON
        if (segment == "W" && (m_JSONFile["circuits"][segment].is_null() || m_JSONFile["circuits"][segment][0].size() == 0))
            return;
        // Here we insert data to our struct
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
            tempStruct.IZ = getIZ(tempStruct.cableManagementMethod, tempStruct.crossSection, calculateAmountOfWires(tempStruct.voltage));
            tempStruct.correctionRate = getCorrectionRate(tempStruct.cableManagementMethod, tempStruct.neighborlyCables);
            m_vectorDatabase.push_back(tempStruct);
        }
    }
    // Get IZ from json file IZ.json
    double getIZ(std::string_view cableMngmtMthd, double crossSection, int amountOfWires)
    {
        int crossSectionInt = (crossSection * 10);
        // because of converstion from double to string we need to erase additional 0 and .
        std::string amountOfWiresString = std::to_string(amountOfWires);
        std::string crossSectionString = std::to_string(crossSection);
        crossSectionString.erase(crossSectionString.find_last_not_of('0') + 1, std::string::npos);
        crossSectionString.erase(crossSectionString.find_last_not_of('.') + 1, std::string::npos);
        return m_IZValues[crossSectionString][cableMngmtMthd][amountOfWiresString];
    }
    // Get CorrectionRate from json file correctionRate.json
    double getCorrectionRate(std::string_view cableMngmtMthd, double neighborlyCables)
    {
        //In PN-IEC standard did not mentioned about more than 20
        if (neighborlyCables > 20)
        {
            neighborlyCables = 20;
        }
        // because of converstion from double to string we need to erase additional 0 and .
        std::string neighborlyCablesString = std::to_string(neighborlyCables);
        neighborlyCablesString.erase(neighborlyCablesString.find_last_not_of('0') + 1, std::string::npos);
        neighborlyCablesString.erase(neighborlyCablesString.find_last_not_of('.') + 1, std::string::npos);
        return m_correctionRateValues[cableMngmtMthd][neighborlyCablesString];
    }
    double calculateAmountOfWires(double voltage)
    {
        // I took 270 to be sure that no one with 2 wires will get 3 wires
        return voltage > 270 ? 3 : 2;
    }
    //Count total amount of power ponsumption from every circuit = L,W,G
    void countWLZPowerConsumption()
    {
        // 1 Because the 0 is WLZ and we can not count power consumption from WLZ
        double sum{};
        for (int i = 1; i < m_vectorDatabase.size(); i++)
        {
            sum += m_vectorDatabase[i].powerConsumption;
        }
        m_vectorDatabase[0].powerConsumption = sum;
    }

public:
    Circuits(std::vector<T> &properties, json &jsonFile) : m_vectorDatabase{properties}, m_JSONFile{jsonFile}
    {
        std::ifstream IZPath{"../IZ.json"};
        std::ifstream correctionRatePath{"../correctionRate.json"};
        m_IZValues = json::parse(IZPath);
        m_correctionRateValues = json::parse(correctionRatePath);
    }
    void calculateAll()
    {
        std::cout<<messageHeaders::start<<"Przystepuje do obliczen obwodow";
        convertToStruct("WLZ");
        convertToStruct("G");
        convertToStruct("L");
        convertToStruct("W");
        countWLZPowerConsumption();
        CircuitsCalculate circuitObj{"WLZ"};

        for (int i=0;i<m_vectorDatabase.size();i++)
        {
            circuitObj = CircuitsCalculate{m_vectorDatabase[i].circuitType};
    
            m_vectorDatabase[i].IB = circuitObj.countIB(m_vectorDatabase[i].powerConsumption, m_vectorDatabase[i].voltage);

            m_vectorDatabase[i].isIZCorrect = circuitObj.checkIZ(m_vectorDatabase[i].IZ, m_vectorDatabase[i].IB, m_vectorDatabase[i].correctionRate);

            m_vectorDatabase[i].voltageDrop = circuitObj.countVoltageDrop(m_vectorDatabase[i].powerConsumption, m_vectorDatabase[i].wireLength, m_vectorDatabase[i].crossSection, m_vectorDatabase[i].voltage);

            // we need to make sure that WLZ power consumption will be excluded from counting because otherwise our calculations will be incorrect
            if (m_vectorDatabase[i].circuitType == "WLZ")
                m_vectorDatabase[i].isVoltageDropCorrect = circuitObj.checkVoltageDrop(m_vectorDatabase[i].voltageDrop);
            else
                m_vectorDatabase[i].isVoltageDropCorrect = circuitObj.checkVoltageDrop(m_vectorDatabase[0].voltageDrop + m_vectorDatabase[i].voltageDrop);

            m_vectorDatabase[i].isCircuitBreakerCorrect = circuitObj.checkCircuitBreaker(m_vectorDatabase[i].IB, m_vectorDatabase[i].IZ, m_vectorDatabase[i].circuitBreaker);

            m_vectorDatabase[i].R = circuitObj.countR(m_vectorDatabase[i].wireLength, m_vectorDatabase[i].crossSection, m_vectorDatabase[i].voltage);

            m_vectorDatabase[i].X = circuitObj.countX(m_vectorDatabase[i].wireLength);

            m_vectorDatabase[i].Rsum = circuitObj.countRsum(m_vectorDatabase[i].voltage, m_vectorDatabase[i].R);

            m_vectorDatabase[i].Xsum = circuitObj.countXsum(m_vectorDatabase[i].voltage, m_vectorDatabase[i].X);

            m_vectorDatabase[i].Z = circuitObj.countZ(m_vectorDatabase[i].Xsum, m_vectorDatabase[i].Rsum);

            m_vectorDatabase[i].IK = circuitObj.countIK(m_vectorDatabase[i].voltage, m_vectorDatabase[i].Z);
        }
        std::cout<<messageHeaders::pass<<"Wszystkie obwody zostaly poprawnie policzone!";
        std::cout<<messageHeaders::end;
        printAllStructs();
    }
    void printAllStructs()
    {
        for (int i{0}; auto circuit : m_vectorDatabase)
        {
            std::cout << '\n';
            std::cout << "\nTYPE:" << circuit.circuitType;
            std::cout << "\n\tID:" << circuit.circuitID;
            std::cout << "\n\tWireLength:" << circuit.wireLength;
            std::cout << "\n\tcrossSection:" << circuit.crossSection;
            std::cout << "\n\tpowerConsumption:" << circuit.powerConsumption;
            std::cout << "\n\tVoltage:" << circuit.voltage;
            std::cout << "\n\tCableManagementMethod:" << circuit.cableManagementMethod;
            std::cout << "\n\tneighborlyCables:" << circuit.neighborlyCables;
            std::cout << "\n\tcircuitBreaker:" << circuit.circuitBreaker;
            std::cout << "\n\tIB:" << circuit.IB;
            std::cout << "\n\tIZ:" << circuit.IZ;
            std::cout << "\n\tR:" << circuit.R;
            std::cout << "\n\tX:" << circuit.X;
            std::cout << "\n\tRsum:" << circuit.Rsum;
            std::cout << "\n\tXsum:" << circuit.Xsum;
            std::cout << "\n\tZ:" << circuit.Z;
            std::cout << "\n\tIK:" << circuit.IK;
            std::cout << "\n\tCorrectionRate:" << circuit.correctionRate;
            std::cout << "\n\tvoltageDrop:" << circuit.voltageDrop;
            std::cout << "\n\tIZCorrect:" << circuit.isIZCorrect;
            std::cout << "\n\tisVoltageDropCorrect:" << circuit.isVoltageDropCorrect;
            std::cout << "\n\tisCircuitBreakerCorrect:" << circuit.isCircuitBreakerCorrect;
        }
    }
};
#endif