#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <fstream>
#include <algorithm>
#include <valarray>
using namespace std;

const string helpOut = "usage: turing [-v|--verbose] [-h|--help] <tm> <input>";
const string endLine = "==================== END ====================";
const string runLine = "==================== RUN ====================";
const string errLine = "==================== ERR ====================";
const string cutLine = "---------------------------------------------";

enum Type
{
    help,
    syntaxError,
    illegalInput
};

struct transferFunc
{
    int count = 0;
    string oldState;     // 旧状态
    string oldSymbolSet; // 旧符号组
    string newSymbolSet; // 新状态
    string directionSet; // 方向组
    string newState;     // 新符号组
    void push(string x)
    {
        switch (count)
        {
        case 0:
            oldState = x;
            break;
        case 1:
            oldSymbolSet = x;
            break;
        case 2:
            newSymbolSet = x;
            break;
        case 3:
            directionSet = x;
            break;
        case 4:
            newState = x;
            break;
        default:
            break;
        }
        count++;
    }
};

class Parser
{
public:
    string stringQ;
    string stringS;
    string stringG;
    string stringq0;
    string stringB;
    string stringF;
    string stringN;
    vector<string> stringDelta;
    string stringInput;
    bool verbose = false;

    Parser(){};
    Parser(int argc, char *argv[]);

    string deleteAnnotation(string s);

    void report(string info, Type code);

    void checkQ(string s);

    void checkS(string s);

    void checkG(string s);

    void checkq0(string s);

    void checkB(string s);

    void checkF(string s);

    void checkN(string s);

    void checkDelta(string s);
};