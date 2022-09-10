#ifndef JSONVALIDATION_HPP
#define JSONVALIDATION_HPP
#include "basicLibraries.hpp"
#include <bitset>
#include "json.hpp"
//Class header. CPP file in cppSources with the same name
class JSONValidation
{
private:
    std::bitset<15> m_validationFlag{0b000'0000'0000};
    std::string m_JSONException{""};
    using json = nlohmann::json;
    json &m_JSONFile;
    std::ifstream m_file;
  
    std::array<std::string,8> m_JSONPropertiesKey{"circuitID", "wireLength", "crossSection", "powerConsumption", "voltage", "cableManagementMethod", "neighborlyCables", "circuitBreaker"};
public:
    JSONValidation(json &JSONFile) : m_JSONFile(JSONFile) {}
    bool isFileExist();
    bool isJSONParsed();
    bool isSegmentExist(std::string_view segment, int segmentID);
    bool isObjectExist(std::string_view segment, int segmentID);
    bool isObjectTypeCorrect(std::string_view segment, int segmentID);
    void validateAll();
    bool isFlagCorrect(int beginToValidate, int endToValidate);
    void setFlag(int beginToSet, int endToSet, bool value);
    bool isBitOn(int bit);
    std::string newLineErase(std::string toErase);
};
#endif