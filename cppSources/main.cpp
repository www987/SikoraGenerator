#include "circuits.cpp"
#include "../headers/basicLibraries.hpp"
#include "../headers/jsonValidation.hpp"
#include "../headers/json.hpp"
class Project
{
private:
    //WLZ always on the first(0) index!
    struct m_CircuitProperties
    {
        std::string circuitType;
        double circuitID;
        double wireLength;
        double crossSection;
        double powerConsumption;
        double voltage;
        std::string cableManagementMethod;
        double neighborlyCables;
        double circuitBreaker;
        double IB;
        double IZ;
        double correctionRate;
        double voltageDrop;
        double powerShutdown;
        bool isIZCorrect;
        bool isVoltageDropCorrect;
        bool isCircuitBreakerCorrect;
    };
    std::vector<m_CircuitProperties> m_CircuitDatabase;
    using json = nlohmann::json;
    json m_JSONFile;
public:
    Project()
    {
        std::cout << "Witamy w generatorze sprawozdania dla Sikory: \n";
        try
        {
            JSONValidation jsonValidation{m_JSONFile};
            jsonValidation.validateAll();
            Circuits circuits{m_CircuitDatabase, m_JSONFile};
            circuits.calculateAll();
        }
        catch (const char *exception)
        {
            std::cout<<"\nWykryto nieoczekiwany blad. Sprobuj ponownie pozniej";
            std::cout << exception;
        }
    }
};

int main(int, char **)
{
    Project home;
}
