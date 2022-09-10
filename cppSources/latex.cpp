#ifndef LATEX_HPP
#define LATEX_HPP
#include "../headers/basicLibraries.hpp"
#include "../headers/latexRaport.hpp"
//Template is necessary for vector
template <typename T>
class Latex
{
private:
    const std::vector<T> &m_vectorDatabase;
    // files necessary for latexRaport.cpp
    std::ofstream m_latexRaportFile{"../latexRaport.tex"};
    std::ofstream m_latexTableFile{"../latexTable.tex"};
public:
    Latex(std::vector<T> &properties) : m_vectorDatabase(properties) {}
    void generateRaport()
    {
        LatexRaport raport{
            m_latexRaportFile,
            m_latexTableFile
        };
        std::array circuitTypeArray{"WLZ", "G", "L", "W"};
        int circuitI{0};
        // The following for scope loop through every circuit Type if only second for detects that iteration circuit type has changed. Then iteration of circuitI is changing of 0+ alreadyIteratedItems
        for(auto element: circuitTypeArray){
            raport.generateSectionStart(element);
            for (int i = 0 + circuitI; m_vectorDatabase[i].circuitType == element; i++)
            {
                raport.generateConclusion(m_vectorDatabase[i].isIZCorrect, m_vectorDatabase[i].isVoltageDropCorrect, m_vectorDatabase[i].isCircuitBreakerCorrect);
                m_latexRaportFile << '\n'
                                << "\\begin{itemize}";
                raport.printData(
                    m_vectorDatabase[i].powerConsumption, 
                    m_vectorDatabase[i].voltage,
                    m_vectorDatabase[i].crossSection,
                    m_vectorDatabase[i].wireLength,
                    m_vectorDatabase[i].circuitBreaker
                    );

                raport.printIZCheck(m_vectorDatabase[i].isIZCorrect, m_vectorDatabase[i].IZ, m_vectorDatabase[i].IB);

                raport.printVoltageDropCheck(m_vectorDatabase[i].isVoltageDropCorrect, m_vectorDatabase[i].voltageDrop, m_vectorDatabase[i].circuitType, m_vectorDatabase[0].voltageDrop);

                raport.printCircuitBreakerCheck(m_vectorDatabase[i].isCircuitBreakerCorrect, m_vectorDatabase[i].IB, m_vectorDatabase[i].IZ, m_vectorDatabase[i].circuitBreaker, m_vectorDatabase[i].circuitType);
                m_latexRaportFile << '\n'
                                << "\\end{itemize}";
                raport.generateTable(m_vectorDatabase[i].circuitType, m_vectorDatabase[i].circuitID, m_vectorDatabase[i].wireLength, m_vectorDatabase[i].crossSection, m_vectorDatabase[i].powerConsumption, m_vectorDatabase[i].voltage, m_vectorDatabase[i].voltageDrop, m_vectorDatabase[i].IB);
                circuitI++;
            }
            raport.generateSectionEnd();
        }
    }
    void generateDocument() {}
};
#endif