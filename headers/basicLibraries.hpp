// File includes the most basic libraries used in this program. 
#ifndef BASICLIBRARIES_HPP
#define BASICLIBRARIES_HPP
#include <iostream>
#include <string_view>
#include <string>
#include <vector>
#include <fstream>
#include <array>
#include <algorithm>


//I know that I should do this in diffrent way than inline by definining constant and do not change variable type but I did not predice that
namespace messageHeaders{
    inline std::string info{"\n\t[INFO] "};
    inline std::string error{"\n\t[ERROR] "};
    inline std::string pass{"\n\t[PASS] "};
    inline std::string end{"\n[END] "};
    inline std::string start{"\n[START] "};
}
#endif