#include "DataList.h"
#include <windows.h>
#include <fstream>
#include <sstream>
#include <iterator>

DataList::DataList(void)
{
    _LineListA.clear();
}
DataList::~DataList(void)
{
    _LineListA.clear();
}
bool DataList::LoadFileA(const std::string afilename)
{
    _LineListA.clear();

    std::ifstream ifs;
    ifs.open(afilename, std::ifstream::in);
    if (!ifs){
        return false;
    }
    
    std::string line;
    while (std::getline(ifs, line) && line.length() > 0){
        _LineListA.push_back(line);
    }
    
    ifs.close();
    return true;
}
int DataList::spliterA(std::string& instring, const int c, std::vector<std::string> *output)
{
    std::back_insert_iterator<std::vector<std::string>> it(*output);
    const char *p   = instring.data();
    const char *end = instring.size() + p;
    while (p != end) {
        if (*p == c) {
            ++p;
        }
        else {
            const char *start = p;
            while (++p != end && *p != c);
            it++ = std::string(start, p - start);
        }
    }
    return output->size();
}
int DataList::GetValueStringA(std::string str, std::string& output)
{
    std::stringstream ss;
    unsigned long ret= 0;
    ss << std::hex << str;
    ss >> ret;
    const char ch1 = (ret & 0x0000FF00) >> 8;
    const char ch2 = (ret & 0x000000FF) >> 0;
    if (ch1 != 0) {
        output += ch1;
        output += ch2;
        output += "\0";
    }
    else {
        output += ch2;
        output += "\0";
    }
    return output.size();
}
