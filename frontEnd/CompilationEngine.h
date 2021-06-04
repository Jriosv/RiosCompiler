#include <iostream>
#include <fstream>
#include "RiosTokenizer.h"


using namespace std;

class CompilationEngine{
    private:
        string input_file_title;
    public:
        ofstream output_file;
        int space_size = 0;
        int* space_size_ptr = &space_size;

        RiosTokenizer* tokenizer;
        CompilationEngine(string input_file);
        string xmlSpace(int spaceNumber);
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