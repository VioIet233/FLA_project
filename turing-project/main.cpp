#include "turingMachine.h"
using namespace std;
int main(int argc, char* argv[]){
    TuringMachine * turingmachine = new TuringMachine (argc, argv);
    turingmachine->running();
}
