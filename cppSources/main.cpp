#include "../headers/calculations.hpp"
#include "../headers/basicLibraries.hpp"
#include "../headers/json.hpp"
class Project
{
private:
    struct m_CircuitProperties
    {
        std::string_view circuitID;
        double IB;
        double IZ;
        double correctionRate;
        double voltageDrop;
        double powerShutdown;
        double circuitBreaker;
    };
    std::vector<m_CircuitProperties> m_CircuitDatabase;
    std::string m_fileName;
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
