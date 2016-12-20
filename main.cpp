#include <string>
#include "DoubleArrayTrie.h"

int main(int argc, char* argv[])
{
    DoubleArrayTrie theDoubleArrayTrie;

    char* keys[] = { "a","ab","abc","abd","abdd"};
    int values[] = {  10,  11,   12,   99,  100};

    int value = 0;

    theDoubleArrayTrie.Build((const char **)keys, values, 5);
    theDoubleArrayTrie.Lookup((const unsigned char *)"abdd", 4, &value);

    return 0;
}
