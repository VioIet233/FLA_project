#include "turingMachine.h"
using namespace std;

TuringMachine::TuringMachine(int argc, char *argv[])
{
    parser = new Parser(argc, argv);
    string pstring;
    // Q
    pstring = parser->stringQ + ",";
    size_t pos = pstring.find(',');
    while (pos != pstring.npos)
    {
        string temp = pstring.substr(0, pos);
        if (temp == "")
            parser->report("incorrect Q", syntaxError);
        Q.push_back(temp);
        pstring = pstring.substr(pos + 1, pstring.size());
        pos = pstring.find(',');
    }

    // cout << "Q:";
    // for (int i = 0; i < Q.size(); i++)
    //     cout << Q[i] << " ";
    // cout << endl;

    // S
    pstring = parser->stringS;
    for (int i = 0; i < pstring.size(); i++)
    {
        char c = pstring[i];
        if (i % 2 == 0)
        {
            if (illegal.find(c) == illegal.npos)
                S.push_back(c);
            else
                parser->report("incorrect S", syntaxError);
        }
        else if (c != ',')
            parser->report("incorrect S", syntaxError);
    }

    // cout << "S:";
    // for (int i = 0; i < S.size(); i++)
    //     cout << S[i] << " ";
    // cout << endl;

    // G

    pstring = parser->stringG;
    for (int i = 0; i < pstring.size(); i++)
    {
        char c = pstring[i];
        if (i % 2 == 0)
        {
            if (illegal.find(c) == illegal.npos || c == '_')
                G.push_back(c);
            else
                parser->report("incorrect G", syntaxError);
        }
        else if (c != ',')
            parser->report("incorrect G", syntaxError);
    }

    // cout << "G:";
    // for (int i = 0; i < G.size(); i++)
    //     cout << G[i] << " ";
    // cout << endl;

    // q0
    q0 = parser->stringq0;
    if (find(Q.begin(), Q.end(), q0) == Q.end())
        parser->report("q0 not found", syntaxError);

    // B
    B = parser->stringB[0];
    if (B != '_')
        parser->report("B is not _", syntaxError);

    if (find(G.begin(), G.end(), B) == G.end())
        parser->report("B not found", syntaxError);

    // F
    pstring = parser->stringF + ",";
    pos = pstring.find(',');
    while (pos != pstring.npos)
    {
        string temp = pstring.substr(0, pos);
        if (temp == "")
            parser->report("incorrect F", syntaxError);
        if (find(Q.begin(), Q.end(), temp) == Q.end())
            parser->report("F not found", syntaxError);
        F.push_back(temp);
        pstring = pstring.substr(pos + 1, pstring.size());
        pos = pstring.find(',');
    }

    // cout << "F:";
    // for (int i = 0; i < F.size(); i++)
    //     cout << F[i] << " ";
    // cout << endl;

    // N
    N = atoi(parser->stringN.c_str());

    // delta
    for (int i = 0; i < parser->stringDelta.size(); i++)
    {
        pstring = parser->stringDelta[i] + ' ';

        transferFunc trans;
        pos = pstring.find(' ');
        while (pos != pstring.npos)
        {
            string temp = pstring.substr(0, pos);
            if (temp == "")
                parser->report("incorrect delta", syntaxError);
            trans.push(temp);
            pstring = pstring.substr(pos + 1, pstring.size());
            pos = pstring.find(' ');
        }

        // cout << trans.oldState << " " << trans.oldSymbolSet << " " << trans.newSymbolSet << " " << trans.directionSet << " " << trans.newState << endl;

        if (find(Q.begin(), Q.end(), trans.oldState) == Q.end())
            parser->report(trans.oldState + " state not found", syntaxError);
        if (find(Q.begin(), Q.end(), trans.newState) == Q.end())
            parser->report(trans.newState + " state not found", syntaxError);

        if (trans.oldSymbolSet.size() != N || trans.oldSymbolSet.size() != N || trans.oldSymbolSet.size() != N)
            parser->report("trans length is not N", syntaxError);

        for (int i = 0; i < N; i++)
        {
            if (direction.find(trans.directionSet[i]) == direction.npos)
                parser->report("direction is not lr*", syntaxError);
            if (find(G.begin(), G.end(), trans.oldSymbolSet[i]) == G.end() && trans.oldSymbolSet[i] != '*')
                parser->report("old Symbol not found", syntaxError);
            if (find(G.begin(), G.end(), trans.newSymbolSet[i]) == G.end() && trans.newSymbolSet[i] != '*')
                parser->report("new Symbol not found", syntaxError);
        }
        delta.push_back(trans);
    }

    // verbose
    verbose = parser->verbose;

    // line
    input = parser->stringInput;
    for (int i = 0; i < input.size(); i++)
    {
        if (find(S.begin(), S.end(), input[i]) == S.end())
        {
            if (verbose)
            {
                cerr << "Input: " + input << endl;
                cerr << errLine << endl;
                cerr << (string) "error: Symbol \"" + input[i] + (string) "\" in input is not defined in the set of input symbols " << endl;
                cerr << "Input: " + input << endl;

                cerr << getSpace(7 + i) + '^' << endl;
                cerr << endLine << endl;
            }
            parser->report("", illegalInput);
        }
    }

    step = 0;
    accept = false;
    state = q0;
    for (int i = 0; i < N; i++)
    {
        Tape *tape = new Tape();
        tapeset.push_back(tape);
    }
}

string TuringMachine::getSpace(int num)
{
    string space;
    for (int i = 0; i < num; i++)
    {
        space += ' ';
    }
    return space;
}

void TuringMachine::running()
{
    if (verbose)
    {
        cout << "Input: " << input << endl;
        cout << runLine << endl;
    }

    for (int i = 0; i < input.size(); i++)
    {
        tapeset[0]->tape[i] = input[i];
    }
    if (input != "")
        tapeset[0]->right = input.size() - 1;

    while (1)
    {
        if (verbose)
            printTape();
        // 当前符号符号组
        string currentSymbolSet;
        for (int i = 0; i < N; i++)
            currentSymbolSet += tapeset[i]->tape[tapeset[i]->head];

        // 对应转移函数
        transferFunc trans;
        for (int i = 0; i < delta.size(); i++)
        {
            if (delta[i].oldState == state && symbolCompare(currentSymbolSet, delta[i].oldSymbolSet))
                trans = delta[i];
        }
        // 无对应转移函数
        if (trans.count == 0)
        {
            break;
        }
        step++;
        state = trans.newState;
        if (find(F.begin(), F.end(), state) != F.end())
            accept = true;
        for (int i = 0; i < N; i++)
        {
            Tape *t = tapeset[i];
            // 修改纸带符号
            if (trans.newSymbolSet[i] != '*')
                t->tape[t->head] = trans.newSymbolSet[i];
            // 移动读写头 并修改左右界限
            if (trans.directionSet[i] == 'r')
            {
                t->head++;
                t->right = t->right > t->head ? t->right : t->head;
                if (t->tape.count(t->head) == 0)
                    t->tape[t->head] = '_';
            }
            else if (trans.directionSet[i] == 'l')
            {
                t->head--;
                t->left = t->left < t->head ? t->left : t->head;
                if (t->tape.count(t->head) == 0)
                    t->tape[t->head] = '_';
            }
        }
    }
    string result;
    for (int i = tapeset[0]->left; i <= tapeset[0]->right; i++)
    {
        if (tapeset[0]->tape[i] == '_')
        {
            if (result != "")
                result += "_";
        }
        else
            result += tapeset[0]->tape[i];
    }
    while (result[result.size() - 1] == '_')
        result.erase(result.size() - 1, 1);
    if (verbose)
    {
        if (accept)
            cout << "ACCEPTED" << endl
                 << "Result: " << result << endl;
        else
            cout << "UNACCEPTED" << endl
                 << "Result: " << result << endl;
        cout << endLine << endl;
    }
    else if (accept)
        cout << "(ACCEPTED) " << result << endl;
    else
        cout << "(UNACCEPTED) " << result << endl;
}

void TuringMachine::printTape()
{
    int log = log10(N - 1);
    cout << "Step   " << getSpace(log) << ": " << step << endl;
    cout << "State  " << getSpace(log) << ": " << state << endl;
    cout << "Acc    " << getSpace(log) << ": " << (accept ? "Yes" : "No") << endl;

    for (int i = 0; i < N; i++)
    {
        Tape *t = tapeset[i];
        int logi = log10(i);

        int left = t->left, right = t->right;
        // 找到最左侧的非空且或读写头
        for (int j = t->left; j <= t->right; j++)
        {
            if (t->tape[j] != '_' || j == t->head)
            {

                left = j;
                break;
            }
        }
        // 找到最右侧的非空且或读写头
        for (int j = t->right; j >= t->left; j--)
        {
            if (t->tape[j] != '_' || j == t->head)
            {
                right = j;
                break;
            }
        }

        cout << "Index" << i << getSpace(log - logi) << " :";
        for (int j = left; j <= right; j++)
        {
            cout << " " << abs(j);
        }
        cout << endl;

        cout << "Tape" << i << getSpace(log - logi) << "  :";

        int space = 0;

        for (int j = left; j <= right; j++)
        {
            if (j > 0)
                space += log10(j);
            cout << " " << t->tape[j] << getSpace(log10(j));
        }
        cout << endl;

        if (space > 0)
            space--;

        cout << "Head" << i << getSpace(log - logi) << "  : " << getSpace((t->head - left) * 2 + space) << "^" << endl;
    }
    cout << cutLine << endl;
}

bool TuringMachine::symbolCompare(string cur, string old)
{
    for (int i = 0; i < N; i++)
    {
        if (old[i] == '*' && cur[i] != '_')
            continue;
        if (cur[i] != old[i])
            return false;
    }
    return true;
}
