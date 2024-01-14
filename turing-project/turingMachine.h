#include "parser.h"

using namespace std;

struct Tape
{
    int head = 0;        // 读写头位置
    int left = 0;        // 左端位置
    int right = 0;       // 右端位置
    map<int, char> tape; // 纸带下标与符号
    Tape()
    {
        tape[0] = '_';
    }
};

const string illegal = "_ ,;{}*";
const string direction = "lr*";

class TuringMachine
{
    vector<string> Q;           // 状态集 #Q
    vector<char> S;             // 输入符号集 #S
    vector<char> G;             // 纸带符号集 #G
    string q0;                  // 初始状态 #q0
    char B;                     // 空格符号 #B
    vector<string> F;           // 终结状态集 #F
    int N;                      // 纸带数 #N
    vector<transferFunc> delta; // 转移函数 delta
    Parser *parser;             // 解析器
    bool verbose;               // verbose模式
    string input;               // 输入
    int step;                   // 当前步数
    bool accept;                // 接收状态
    string state;               // 当前状态
    vector<Tape *> tapeset;     // 纸带集合
public:
    TuringMachine(){};
    TuringMachine(int argc, char *argv[]);
    void running();
    void printTape();
    string getSpace(int num);
    bool symbolCompare(string cur, string old);
};