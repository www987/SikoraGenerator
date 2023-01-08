#ifndef LATEXRAPORT_CPP
#define LATEXRAPORT_CPP
#include "../headers/latexRaport.hpp"
#include <cmath>
// Creates The beginning of raport in latex structure
void LatexRaport::generateSectionStart(std::string typeSection)
{
    m_latexFile <<'\n'<< "\\section{Obwód " << typeSection << ":}\n";
    m_latexFile <<'\n'<< "\\begin{enumerate}";
}

// Creates the end of raport in latex structure
void LatexRaport::generateSectionEnd()
{
    m_latexFile <<'\n'<< "\\end{enumerate}";
}
//Send latex basic data to pointed file 
void LatexRaport::printData(double powerConsumption, double voltage, double crossSection, double wireLength, double circuitBreaker){
    m_latexFile <<'\n'<< "\\item[] \\textbf{DANE: } \\: $ P = \\:" << powerConsumption << "\\, W";
    m_latexFile <<"\\: U = \\:" << voltage <<"\\: V";
    m_latexFile <<"\\: S = \\:" << crossSection <<"\\, mm^2";
    m_latexFile <<"\\: l = \\:" << ceil(wireLength * 100.0) / 100.0 <<"\\, m";
    m_latexFile <<"\\: I_N= \\: $" << circuitBreaker;
}
//Send latex IZ check test to pointed file
void LatexRaport::printIZCheck(bool isIZCorrect, double IZ, double IB)
{
    if (isIZCorrect)
    {
        m_latexFile <<'\n'<< "\\item[\\textcolor{Green}{\\cmark}] $ \\pmb{I_Z \\, >  \\, I_B: }" << ceil(IZ * 100.0) / 100.0 << " \\, A \\, > \\," << (ceil(IB * 100.0) / 100.0) <<" \\, A $";
    }
    else
    {
        m_latexFile <<'\n'<< "\\item[\\textcolor{red}{\\cmark}] $ \\pmb{I_Z \\, >  \\, I_B: }" << ceil(IZ * 100.0) / 100.0 << " \\, A \\, > \\," << (ceil(IB * 100.0) / 100.0) <<" \\, A $";
    }
}
// Send latex VoltageDrop check test to pointed file
void LatexRaport::printVoltageDropCheck(bool isVDCorrect, double voltageDrop, std::string circuitType, double voltageDropWLZ)
{
    if (circuitType == "WLZ")
        voltageDropWLZ = 0;
    if (isVDCorrect)
    {
        m_latexFile <<'\n'<< "\\item[\\textcolor{Green}{\\cmark}] $ \\pmb{ \\delta U_\\% \\, < \\, 4\\%:} \\:"<< ceil((voltageDropWLZ + voltageDrop) * 100.0) / 100.0 << " \\, \\% \\,< \\, 4\\%"<< " $";
    }
    else
    {
       m_latexFile <<'\n'<< "\\item[\\textcolor{red}{\\cmark}] $ \\pmb{\\delta U_\\% \\, < \\, 4\\%:} \\:"<< ceil((voltageDropWLZ + voltageDrop) * 100.0) / 100.0 << " \\, \\% \\,< \\, 4\\%"<< " $";
    }
}
// Send latex printCircuitBreaker test to pointed file
void LatexRaport::printCircuitBreakerCheck(bool isCBCorrect, double IB, double IZ, double CB, const std::string& circuitType)
{
    if (circuitType == "WLZ")
        return;
    if (isCBCorrect)
    {
        m_latexFile <<'\n'<< "\\item[\\textcolor{Green}{\\cmark}] $ \\pmb{I_B \\le I_N \\le I_Z: } \\, " << ceil((IB) * 100.0) / 100.0 <<" \\, \\le \\,"<< CB <<" \\, \\le \\,"<< ceil((IZ) * 100.0) / 100.0<< " $";
    }
    else
    {
        m_latexFile <<'\n'<< "\\item[\\textcolor{red}{\\cmark}] $ \\pmb{I_B \\le I_N \\le I_Z: } \\, " << ceil((IB) * 100.0) / 100.0 <<" \\, \\le \\,"<< CB <<" \\, \\le \\,"<< ceil((IZ) * 100.0) / 100.0<< " $";
    }
}
//Send latex conclusion to pointed file
void LatexRaport::generateConclusion(bool isCBCorrect, bool isVDCorrect, bool isIZCorrect)
{
    if (isCBCorrect && isVDCorrect && isIZCorrect)
    {
        m_latexFile <<'\n'<< "\\item \\textcolor{Green}{\\cmark} \\textbf{Ten obwod spelnia normy} ";
    }
    else
    {
        m_latexFile <<'\n'<< "\\item \\textcolor{Green}{\\xmark} \\textbf{Ten obwod nie spelnia normy:} ";
        if(!isIZCorrect){
            m_latexFile <<'\n'<< "\\\\ $I_Z$ \\, - \\, Wybierz większy przekrój żyły w kablu";
        }
        if(!isVDCorrect){
            m_latexFile <<'\n'<< "\\\\ $\\delta U_\\%$ \\, - \\,Zmniejsz odległość od punktu odbiorczego do rozdzielni lub zwiększ przekrój żyły w kablu";
        }
        if(!isCBCorrect){
            m_latexFile <<'\n'<< "\\\\ $I_N$ \\, - \\, Wybierz wyłącznik nadprądowy o większym prądzie znamionowym";
        }
    }
}
//Send latex row table to pointed file
void LatexRaport::generateTable(std::string typeSection, double ID, double wireLength,double crossSection, double powerConsumption, double voltage, double VD, double IB, double R, double X, double Rsum, double Xsum, double Z, double IK){
    m_latexFileTable <<'\n'<< "\\hline 10" << " & " << typeSection << ID << " & "<< wireLength << " & " << crossSection << " & " << powerConsumption << " & " << voltage << " & "<< ceil((VD) * 100.0)/100<< " & " << ceil((IB) * 100.0)/100 << " & " << "R" << R << "X" << Rsum << "Rsum" << Xsum << "Xsum" << Z << "Z" << IK << "IK";
}
#endif