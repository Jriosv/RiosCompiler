#include <iostream>
#include <fstream>
#include "RiosTokenizer.h"


using namespace std;

class CompilationEngine{
    private:
        string input_file_title;
    public:
        ofstream output_file;

        RiosTokenizer* tokenizer;
        CompilationEngine(string input_file);
        void compileClass();
        void compileClassVarDec();
        void compileSubroutine();
        void compileParameterList();
        void subroutineBody();
        void compileVarDec();
        void compileStatements();
        void compileDo();
        void compileLet();
        void compileWhile();
        void compileReturn();
        void compileIf();
        void compileExpression();
        void compileTerm();
        void compileExpressionList();

};