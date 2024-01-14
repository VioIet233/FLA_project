#include "turingMachine.h"
using namespace std;

#define judgeQSGF (s[2] == ' ' && s[3] == '=' && s[4] == ' ' && s[5] == '{' && s[s.size() - 1] == '}')
#define judgeBN (s[2] == ' ' && s[3] == '=' && s[4] == ' ' && s.length() == 6)
#define judgeq0 (s[3] == ' ' && s[4] == '=' && s[5] == ' ' && s.length() == 7)

Parser::Parser(int argc, char *argv[])
{
    if (argc <= 1 || argc >= 5)
        report("wrong number", syntaxError);
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
        report("", help);
    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--verbose") == 0)
        verbose = true;
    string tm = argv[1 + (int)verbose];

    stringInput = argv[2 + (int)verbose];
    if (tm == "")
        report("tm is null", syntaxError);

    ifstream in;
    in.open(tm.c_str());

    if (in.fail())
        report("file not exist", syntaxError);

    string temp;
    while (getline(in, temp))
    {
        if (temp == "")
            continue;
        if (temp[0] == ';')
            continue;

        temp = deleteAnnotation(temp);

        if (temp[0] == '#')
        {

            char type = temp[1];
            switch (type)
            {
            case 'Q':
                checkQ(temp);
                break;
            case 'S':
                checkS(temp);
                break;
            case 'G':
                checkG(temp);
                break;
            case 'q':
                checkq0(temp);
                break;
            case 'B':
                checkB(temp);
                break;
            case 'F':
                checkF(temp);
                break;
            case 'N':
                checkN(temp);
                break;
            default:
                break;
            }
        }
        else if (count(temp.begin(), temp.end(), ' ') == 4)
            checkDelta(temp);
        else
            report("tm format fault", syntaxError);
    }

    // cout << "Q:" << stringQ << endl;
    // cout << "S:" << stringS << endl;
    // cout << "G:" << stringG << endl;
    // cout << "q0:" << stringq0 << endl;
    // cout << "B:" << stringB << endl;
    // cout << "F:" << stringF << endl;

    in.close();
}

void Parser::report(string info, Type code)
{
    switch (code)
    {
    case syntaxError:
        cerr << "syntax error" << endl;
        break;
    case help:
        cout << helpOut;
        break;
    case illegalInput:
        cerr << "illegal input string" << endl;
        break;
    default:
        break;
    }
    //cout << info << endl;
    exit(code);
}

string Parser::deleteAnnotation(string s)
{
    size_t pos = s.find(';');
    if (pos != s.npos)
    {
        s = s.substr(0, pos);
        while (s[s.size() - 1] == ' ')
        {
            s = s.substr(0, s.size() - 1);
        }
    }
    return s;
}

void Parser::checkQ(string s)
{
    if (judgeQSGF)
    {
        s.erase(0, 6);
        s.erase(s.size() - 1, 1);
        stringQ = s;
    }
    else
        report("error check Q", syntaxError);
}

void Parser::checkS(string s)
{
    if (judgeQSGF)
    {
        s.erase(0, 6);
        s.erase(s.size() - 1, 1);
        stringS = s;
    }
    else
        report("error check S", syntaxError);
}

void Parser::checkG(string s)
{
    if (judgeQSGF)
    {
        s.erase(0, 6);
        s.erase(s.size() - 1, 1);
        stringG = s;
    }
    else
        report("error check G", syntaxError);
}

void Parser::checkq0(string s)
{
    if (judgeq0)
    {
        s.erase(0, 6);
        stringq0 = s;
    }
    else
        report("error check q0", syntaxError);
}

void Parser::checkB(string s)
{
    if (judgeBN)
        stringB = s[5];
    else
        report("error check B", syntaxError);
}

void Parser::checkF(string s)
{
    if (judgeQSGF)
    {
        s.erase(0, 6);
        s.erase(s.size() - 1, 1);
        stringF = s;
    }
    else
        report("error check F", syntaxError);
}

void Parser::checkN(string s)
{
    if (judgeBN)
    {
        s.erase(0, 5);
        stringN = s;
    }
    else
        report("error check N", syntaxError);
}

void Parser::checkDelta(string s)
{
    stringDelta.push_back(s);
}