#include "trim.h"
#include <string>
namespace cppx
{
    std::string trim(const std::string &str)
    {
        int end = str.find_last_not_of(" \t\n\r\f\v");

        if (end == std::string::npos)
            return "";

        return str.substr(0, end + 1);
    }
}
