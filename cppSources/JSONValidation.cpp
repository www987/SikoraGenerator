#ifndef JSONVALIDATION_CPP
#define JSONVALIDATION_CPP
#include "../headers/jsonValidation.hpp"
namespace bitsetValidation
{
    constexpr std::bitset<15> isWLZEntered{0b000'0000'0000'0001};
    constexpr std::bitset<15> isGEntered{0b000'0000'0000'0010};
    constexpr std::bitset<15> isLEntered{0b000'0000'0000'0100};
    constexpr std::bitset<15> isWEntered{0b000'0000'0000'1000};
    constexpr std::bitset<15> EMPTYFORNOW{0b000'0000'0001'0000};
    constexpr std::bitset<15> isWLZDescribed{0b000'0000'0010'0000};
    constexpr std::bitset<15> isGDescribed{0b000'0000'0100'0000};
    constexpr std::bitset<15> isLDescribed{0b000'0000'1000'0000};
    constexpr std::bitset<15> isWDescribed{0b000'0001'0000'0000};
    constexpr std::bitset<15> isFileExist{0b000'0010'0000'0000};
    constexpr std::bitset<15> isJSONParsed{0b000'0100'0000'0000};
    constexpr std::bitset<15> isWLZDataTypeCorrect{0b000'1000'0000'0000};
    constexpr std::bitset<15> isGDataTypeCorrect{0b001'0000'0000'0000};
    constexpr std::bitset<15> isLDataTypeCorrect{0b010'0000'0000'0000};
    constexpr std::bitset<15> isWDataTypeCorrect{0b100'0000'0000'0000};
    std::array bitArray{isWLZEntered, isGEntered, isLEntered, isWEntered, EMPTYFORNOW, isWLZDescribed, isGDescribed, isLDescribed, isWDescribed, isFileExist, isJSONParsed, isWLZDataTypeCorrect, isGDataTypeCorrect, isLDataTypeCorrect, isWDataTypeCorrect};
}
/* Validating functions. Every function change bit flag to 0 if ERROR and 1 if PASS */
//check whether json file name is correct. If it not correct return false. Currently I have no idea how to check file path with polish letter. 
bool JSONValidation::isFileExist()
{
    std::string dataPathFile;
    std::cout << infoMessage << "Podaj sciezke pliku JSON: ";
    std::getline(std::cin, dataPathFile);
    m_file.open(dataPathFile);
    if (!m_file)
    {
        setFlag(9, 9, 0);
        m_file.close();
        return false;
    }
    setFlag(9, 9, 1);
    return true;
}
//check whether json file structure is correct. If it not correct return false
bool JSONValidation::isJSONParsed()
{
    try
    {
        m_JSONFile = json::parse(m_file);
        setFlag(10, 10, 1);
        return true;
    }
    catch (nlohmann::json::parse_error &e)
    {
        m_JSONException = e.what();
        setFlag(10, 10, 0);
        return false;
    }
}
//check whether segment in file exist and if is not empty!. IF not exist or empty return false
bool JSONValidation::isSegmentExist(std::string_view segment, int segmentID)
{
    if (m_JSONFile["circuits"][segment].is_null() || m_JSONFile["circuits"][segment][0].size() == 0)
    {
        setFlag(segmentID, segmentID, 0);
        return 0;
    }
    setFlag(segmentID, segmentID, 1);
    return 1;
}
//check whether every key in every circuit exist including default values.
//If it not exist print JSON with in-line comments "BRAK DANYCH" and return false
bool JSONValidation::isObjectExist(std::string_view segment, int segmentID)
{
    bool isMissing{false};
    for (int i = 0; i < m_JSONFile["circuits"][segment].size(); i++)
    {
        for (int j = 0; j < m_JSONPropertiesKey.size(); j++)
        {
            if (!m_JSONFile["circuits"][segment][i].contains(m_JSONPropertiesKey[j]))
            {
                if (m_JSONFile["defaultValues"][segment][0].contains(m_JSONPropertiesKey[j]))
                {
                    m_JSONFile["circuits"][segment][i][m_JSONPropertiesKey[j]] = m_JSONFile["defaultValues"][segment][0][m_JSONPropertiesKey[j]];
                }
                else
                {
                    m_JSONFile["circuits"][segment][i][m_JSONPropertiesKey[j]] = "BRAK DANYCH";
                    isMissing = true;
                }
            }
        }
    }
    if(isMissing){
        setFlag(segmentID, segmentID,0);
        return false;
    }
    else{
        setFlag(segmentID, segmentID, 1);
        return true;
    }
    return true;
}
//check whether every value type of key  is correct including non standard
//cableManagementMethod. If it is not correct print JSON with in-line comments
//"ZLY TYP DANYCH" where the data type is incorrect and return false
bool JSONValidation::isObjectTypeCorrect(std::string_view segment, int segmentID){
    bool isMissing{false};
    bool isMethodMissing{true};
    //cableManagementMethod allowed values
    std::array mngmtMethod{"A1", "A2", "B1", "B2", "C", "D", "E", "F", "G", "E", "F"};
    // iterate through every circuit in Segment type
    for (int i = 0; i < m_JSONFile["circuits"][segment].size(); i++)
    {
        for (int j = 0; j < m_JSONPropertiesKey.size(); j++)
        {
            if(m_JSONPropertiesKey[j] == "cableManagementMethod"){
                for(auto method: mngmtMethod){
                    if(m_JSONFile["circuits"][segment][i][m_JSONPropertiesKey[j]] == method){
                        // if value from mngmtMethod was found it means that method is not missed
                        isMethodMissing = false;
                    }
                }
                if(isMethodMissing){
                    m_JSONFile["circuits"][segment][i][m_JSONPropertiesKey[j]] = "ZLY TYP DANYCH";
                    isMissing= true;
                } 
            }
            else{
                if(!m_JSONFile["circuits"][segment][i][m_JSONPropertiesKey[j]].is_number()){
                     m_JSONFile["circuits"][segment][i][m_JSONPropertiesKey[j]] = "ZLY TYP DANYCH";
                    isMissing= true;
                }
            }
        }
        isMethodMissing = true;
    }
    isMethodMissing = true;
    // The individual for is necessary because of non existing values in default values. Unfortunately even if key does not exist json Library will create one leaving us with null values 
    for (int j = 0; j < m_JSONPropertiesKey.size(); j++)
    {
        if(m_JSONPropertiesKey[j] == "cableManagementMethod"){
            for(auto method: mngmtMethod){
                if(m_JSONFile["defaultValues"][segment][0][m_JSONPropertiesKey[j]] == method){
                    isMethodMissing = false;
                }
            }
            if(isMethodMissing){
                m_JSONFile["defaultValues"][segment][0][m_JSONPropertiesKey[j]] = "ZLY TYP DANYCH";
                isMissing= true;
            } 
        }
        else{
            if(m_JSONFile["defaultValues"][segment][0].contains(m_JSONPropertiesKey[j])){
                if(!m_JSONFile["defaultValues"][segment][0][m_JSONPropertiesKey[j]].is_number()){
                    m_JSONFile["defaultValues"][segment][0][m_JSONPropertiesKey[j]] = "ZLY TYP DANYCH";
                    isMissing= true;
                } 
            }
        }
    }
    // if at least one of value has bad type isMissing is true
    if(isMissing){
        setFlag(segmentID, segmentID,0);
        return false;
    }
    else{
        setFlag(segmentID, segmentID, 1);
        return true;
    }
}
// get through every stage of file validation till the file will not correct
void JSONValidation::validateAll()
{
    //
    do
    {
        std::cout<<startMessage<<"Rozpoczynam walidacje";
        std::cout << "\n1) Etap pierwszy walidacji - sprawdzanie sciezki pliku:";
        isFileExist();
        if (!isFlagCorrect(9, 9)){
            std::cout<<endMessage;
            continue;
        }
            
        
        std::cout << "\n2) Etap drugi walidacji - sprawdzanie struktury formatu JSON:";
        isJSONParsed();
        if (!isFlagCorrect(10, 10)){
            std::cout<<endMessage;
            continue;
        }
            

        std::cout << "\n3) Etap trzeci walidacji - sprawdzanie istnienia kazdego typu obwodu:";
        isSegmentExist("WLZ", 0);
        isSegmentExist("G", 1);
        isSegmentExist("L", 2);
        isSegmentExist("W", 3);
        std::cout<<m_validationFlag;
        if (!isFlagCorrect(0, 3)){
            std::cout<<endMessage<<"Popraw wskazane bledy";
            continue;
        }
            

        std::cout << "\n4) Etap czwarty walidacji - sprawdzanie istnienia wartosci dla kazdego obwody danego typu";
        isObjectExist("WLZ", 5);
        isObjectExist("G", 6);
        isObjectExist("L", 7);
        isObjectExist("W", 8);
        std::cout<<m_validationFlag;
        std::cout<< m_JSONFile.dump(2);
        //Flag bit 8 is W which is not neccessary so it can't trigger countinue
        isFlagCorrect(8,8 );
        if (!isFlagCorrect(5, 7 ) ){
            std::cout<<endMessage<<"Wstaw dane w miejscu BRAK DANYCH";
            continue;
        }

        std::cout << "\n5) Etap piaty walidacji - sprawdzanie typu wartosci kazdego wlasciwosci typu obwodu";
        isObjectTypeCorrect("WLZ", 11);
        isObjectTypeCorrect("G", 12);
        isObjectTypeCorrect("L", 13);
        isObjectTypeCorrect("W", 14);
        std::cout<<m_validationFlag;
        std::cout<< m_JSONFile.dump(2);
        if (!isFlagCorrect(11, 14 ) ){
            std::cout<<endMessage<<"Wstaw poprawne typy danych";
            continue;
        }
        else{
             std::cout<<endMessage<<"Walidacja przebiegla poprawnie!";
             break;
        }
    } while (1);
}
/* Supportive functions */
// The following function check pointed flag and print status message 
bool JSONValidation::isFlagCorrect(int beginToValidate, int endToValidate)
{
    bool isBug{false};
    for (int i = beginToValidate; i <= endToValidate; i++)
    {
        //switch between every pointed bit and print PASS or ERROR message
        switch (i)
        {
        case 0:
            if (isBitOn(i))
                std::cout << m_passMessage << "Wpis WLZ zostal znaleziony";
            else
            {
                std::cout << m_errorMessage << "Nie znaleziono wpisu WLZ lub jest on pusty";
                isBug = 1;
            }
            break;
        case 1:
            if (isBitOn(i))
                std::cout << m_passMessage << "Wpis G zostal znaleziony";
            else
            {
                std::cout << m_errorMessage << "Nie znaleziono wpisu G lub jest on pusty";
                isBug = 1;
            }
            break;
        case 2:
            if (isBitOn(i))
                std::cout << m_passMessage << "Wpis L zostal znaleziony";
            else
            {
                std::cout << m_errorMessage << "Nie znaleziono wpisu L lub jest on pusty";
                isBug = 1;
            }
            break;
        case 3:
            if (isBitOn(i))
                std::cout << m_passMessage << "Wpis W zostal znaleziony";
            else
            {
                std::cout << m_errorMessage << "Nie znaleziono wpisu W lub jest on pusty";
                isBug = 1;
            }
            break;
        case 4:
            if (isBitOn(i))
                std::cout << m_passMessage << "DO UZUPELNIENIA POZNIEJ";
            else
            {
                std::cout << m_errorMessage << "DO UZUPELNIENIA POZNIEJ";
                isBug = 1;
            }
            break;
        case 5:
            if (isBitOn(i))
                std::cout << m_passMessage << "Dane obwodu WLZ zostaly poprawnie podane";
            else
            {
                std::cout << m_errorMessage << "Dane obwodu WLZ NIE zostaly poprawnie podane i nie mozna bylo ich znalezc w wartosciach domyslnych";
                isBug = 1;
            }
            break;
        case 6:
            if (isBitOn(i))
                std::cout << m_passMessage << "Dane obwodu G zostaly poprawnie podane";
            else
            {
                std::cout << m_errorMessage << "Dane obwodu G NIE zostaly poprawnie podane i nie mozna bylo ich znalezc w wartosciach domyslnych";
                isBug = 1;
            }
            break;
        case 7:
            if (isBitOn(i))
                std::cout << m_passMessage << "Dane obwodu L zostaly poprawnie podane";
            else
            {
                std::cout << m_errorMessage << "Dane obwodu L NIE zostaly poprawnie podane i nie mozna bylo ich znalezc w wartosciach domyslnych";
                isBug = 1;
            }
            break;
        case 8:
            if (isBitOn(i))
                std::cout << m_passMessage << "Dane obwodu W zostaly poprawnie podane";
            else
            {
                std::cout << m_errorMessage << "Dane obwodu W zostaly poprawnie podane i nie mozna bylo ich znalezc w wartosciach domyslnych. Aczkolwiek nie jest to konieczne";
                isBug = 1;
            }
            break;
        case 9:
            if (isBitOn(i))
                std::cout << newLineErase(m_passMessage) << "Plik z podanej sciezki zostal znaleziony";
            else
            {
                std::cout << newLineErase(m_errorMessage) << "Plik z podanej sciezki NIE zostal znaleziony";
                isBug = 1;
            }
            break;
        case 10:
            if (isBitOn(i))
                std::cout << m_passMessage << "Struktura pliku JSON jest poprawna";
            else
            {
                std::cout << m_errorMessage << "Struktura pliku JSON NIE jest poprawna " << m_JSONException;
                isBug = 1;
            }
            break;
        case 11:
            if (isBitOn(i))
                std::cout << m_passMessage << "Typ danych wartosci dla WLZ jest prawidlowy";
            else
            {
                std::cout << m_errorMessage << "Typ danych wartosci dla WLZ NIE jest prawidlowy. Niezgodnosci sa opisane w pliku";
                isBug = 1;
            }
            break;
        case 12:
            if (isBitOn(i))
                std::cout << m_passMessage << "Typ danych wartosci dla G jest prawidlowy";
            else
            {
                std::cout << m_errorMessage << "Typ danych wartosci dla G NIE jest prawidlowy. Niezgodnosci sa opisane w pliku";
                isBug = 1;
            }
            break;
        case 13:
            if (isBitOn(i))
                std::cout << m_passMessage << "Typ danych wartosci dla L jest prawidlowy";
            else
            {
                std::cout << m_errorMessage << "Typ danych wartosci dla L NIE jest prawidlowy. Niezgodnosci sa opisane w pliku";
                isBug = 1;
            }
            break;
        case 14:
            if (isBitOn(i))
                std::cout << m_passMessage << "Typ danych wartosci dla W jest prawidlowy";
            else
            {
                std::cout << m_errorMessage << "Typ danych wartosci dla W NIE jest prawidlowy. Niezgodnosci sa opisane w pliku";
                isBug = 1;
            }
            break;
        }
    }
    // negation is necessary because of the function name
    return !isBug;
}
// turn on or turn off pointed bit flag
void JSONValidation::setFlag(int beginToSet, int endToSet, bool value)
{
    for (int i = beginToSet; i <= endToSet; i++)
    {
        if (value)
            //SET 1 BIT
            m_validationFlag |= bitsetValidation::bitArray[i];
        else{
            //SET 0 BIT
            m_validationFlag &= ~bitsetValidation::bitArray[i];
        }      
    }
}
//check whether pointed bit in flag is on or off
bool JSONValidation::isBitOn(int bit)
{
    return m_validationFlag.test(bit);
}
// Function neccessary only in isFlagCorrect because of generation new line by user when getline is used
std::string JSONValidation::newLineErase(std::string toErase){
    std::erase_if(toErase,
                [](auto ch)
                {
                    return (ch == '\n' ||
                            ch == '\r'); 
                });
    return toErase;
}
#endif