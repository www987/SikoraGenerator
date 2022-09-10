#include "circuits.cpp"
#include "../headers/basicLibraries.hpp"
#include "../headers/jsonValidation.hpp"
#include "latex.cpp"
#include "../headers/json.hpp"

// The following class contains every class needed to create our Project
class Project
{
private:
    // WLZ always on the first(0) index! Next time I will declare struct in basicLibraries to not make templates.
    using json = nlohmann::json;
    json m_JSONFile;

public:
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
        bool isIZCorrect;
        bool isVoltageDropCorrect;
        bool isCircuitBreakerCorrect;
    };
    std::vector<m_CircuitProperties> m_CircuitDatabase;
    Project()
    {
        std::cout << "Witamy w generatorze sprawozdania dla Sikory: \n";
        try
        {
            JSONValidation jsonValidation{m_JSONFile};
            jsonValidation.validateAll();
            {
            Circuits<m_CircuitProperties> circuits{m_CircuitDatabase, m_JSONFile};
            circuits.calculateAll();
            }
            Latex<m_CircuitProperties> raport{m_CircuitDatabase};
            raport.generateRaport();
        }// For undefined errors
        catch (const char *exception)
        {
            std::cout << "\nWykryto nieoczekiwany blad. Sprobuj ponownie pozniej";
            std::cout << exception;
        }
    }
};

int main(int, char **)
{
    std::string userAnswer{"1"};
    bool isUserAnswerOK = true;
    while(1){
        Project home;
        isUserAnswerOK = true;
        std::cout<< "\nCzy chcesz obliczyc kolejne Obwody: 1 - TAK, 0 - NIE: ";
        std::getline(std::cin, userAnswer);
        if(userAnswer == "1") continue;
        else if(userAnswer == "0") break;
        // I could use second while loop but wanted to check somehting different
        else{
            std::cout <<"\nWprowadziles niepoprawne znaki!";
            isUserAnswerOK = false;
        } 
    }
        
    
    
}
