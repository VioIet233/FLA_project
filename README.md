### 形式语言与自动机-实验一
#### 分析与设计思路
1. parser:命令行语法解析器：构造函数中将.tm文件的内容删去行尾注释后、分类，初步转化为各类字符串，方便后续转化。
    同时，还有一个report函数，负责输出help帮助或错误信息
```c++
class Parser
{
public:
    string stringQ;
    ......
    vector<string> stringDelta;
    string stringInput;
    bool verbose = false;

    Parser(){};
    Parser(int argc, char *argv[]);

    string deleteAnnotation(string s);

    void report(string info, Type code);
    void checkQ(string s);
    ......
    void checkDelta(string s);
};
```
2. turingMachine:构造函数中创建一个解析器，再将解析器中的各个字符串转为图灵机的各个变量。
    对于各条纸带，创建一个结构体，
```c++
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
```