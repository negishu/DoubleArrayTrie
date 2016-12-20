#ifndef DATALIST_H
#define DATALIST_H
#include <string>
#include <vector>
class DataList
{
public:
    DataList(void);
    virtual ~DataList(void);
    bool LoadFileA(const std::string afilename);
protected:
    std::vector<std::string>  _LineListA;
    int spliterA(std::string& instring, const int c, std::vector<std::string> *output);
    int GetValueStringA(std::string str, std::string& output);
};
#endif // DATALIST_H
