#include <iostream>
#include <fstream>
#include "CompilationEngine.h" 


using namespace std;

CompilationEngine::CompilationEngine(string input_file){
    input_file_title = input_file;
    tokenizer = new RiosTokenizer(input_file_title);
    string newFile = tokenizer->changeFileName();
    output_file.open(newFile, ios::out);
}

string CompilationEngine::xmlSpace(int spaceNumber){
    string tab = "";

    for(int i = 0; i<spaceNumber;i++){
     tab = tab + " ";
    }

    return tab;
}

void CompilationEngine::compileClass(){
        output_file <<"<class>" <<endl;
        string token;

        while(!tokenizer->riosFile.eof()){
			tokenizer->advance();

            if(tokenizer->tokenType() == "stringConstant"){
				token = tokenizer->stringVal();
			}else{
				token = tokenizer->getCurrentToken();
			}

            if(tokenizer->tokenType() == "keyword"){
                if(tokenizer->keyWord() == "STATIC" || tokenizer->keyWord() == "FIELD"){
                    *space_size_ptr = 1;
                    compileClassVarDec();
                }else if(tokenizer->keyWord() == "CONSTRUCTOR" || tokenizer->keyWord() == "FUNCTION" || tokenizer->keyWord() == "METHOD"){
                    *space_size_ptr = 1;
                    compileSubroutine();
                }
            }

            
    
			if(tokenizer->getCurrentToken() != "\n" && !tokenizer->riosFile.eof()){
				
				// <tokenType> token </tokenType>
				output_file<<" "<<"<"<<tokenizer->tokenType()<<"> "<<token<<" </"<<tokenizer->tokenType()<<">"<<endl;
			}
						
		}
        output_file<<"</class>";
}

void CompilationEngine::compileClassVarDec(){
    output_file<<xmlSpace(space_size)<<"<classVarDec>"<<endl;
    *space_size_ptr = 2;
    output_file<<xmlSpace(space_size)<<"<"<<tokenizer->tokenType()<<"> "<<tokenizer->getCurrentToken()<<" </"<<tokenizer->tokenType()<<">"<<endl;
    tokenizer->advance();
    output_file<<xmlSpace(space_size)<<"<"<<tokenizer->tokenType()<<"> "<<tokenizer->getCurrentToken()<<" </"<<tokenizer->tokenType()<<">"<<endl;
    tokenizer->advance();
    output_file<<xmlSpace(space_size)<<"<"<<tokenizer->tokenType()<<"> "<<tokenizer->getCurrentToken()<<" </"<<tokenizer->tokenType()<<">"<<endl;
    tokenizer->advance();
    output_file<<xmlSpace(space_size)<<"<"<<tokenizer->tokenType()<<"> "<<tokenizer->getCurrentToken()<<" </"<<tokenizer->tokenType()<<">"<<endl;
    tokenizer->advance();
    *space_size_ptr = 1;
    output_file<<xmlSpace(space_size)<<"</classVarDec>"<<endl;
}

void CompilationEngine::compileSubroutine(){
    output_file<<xmlSpace(space_size)<<"<subroutineDec>"<<endl;
    *space_size_ptr = 2;
    //function | method | constructor
    output_file<<xmlSpace(space_size)<<"<"<<tokenizer->tokenType()<<"> "<<tokenizer->getCurrentToken()<<" </"<<tokenizer->tokenType()<<">"<<endl;
    tokenizer->advance();
    //void | typw
    output_file<<xmlSpace(space_size)<<"<"<<tokenizer->tokenType()<<"> "<<tokenizer->getCurrentToken()<<" </"<<tokenizer->tokenType()<<">"<<endl;
    tokenizer->advance();
    //name | main | new
    output_file<<xmlSpace(space_size)<<"<"<<tokenizer->tokenType()<<"> "<<tokenizer->getCurrentToken()<<" </"<<tokenizer->tokenType()<<">"<<endl;
    tokenizer->advance();
    // '('
    output_file<<xmlSpace(space_size)<<"<"<<tokenizer->tokenType()<<"> "<<tokenizer->getCurrentToken()<<" </"<<tokenizer->tokenType()<<">"<<endl;
    compileParameterList();
    // ')'
    output_file<<xmlSpace(space_size)<<"<"<<tokenizer->tokenType()<<"> "<<tokenizer->getCurrentToken()<<" </"<<tokenizer->tokenType()<<">"<<endl;   
    tokenizer->advance();
    subroutineBody();
    output_file<<" </subroutineDec>"<<endl;

}

void CompilationEngine::compileParameterList(){
    output_file<<xmlSpace(space_size)<<"<parameterList>"<<endl;
    *space_size_ptr = 3;
    while(tokenizer->getCurrentToken() != ")"){
        tokenizer->advance();
        if(tokenizer->getCurrentToken() != ")"){
            output_file<<xmlSpace(space_size)<<"<"<<tokenizer->tokenType()<<"> "<<
            tokenizer->getCurrentToken()
            <<" </"<<tokenizer->tokenType()<<">"<<endl;
        }
    }
    *space_size_ptr = 2;
    output_file<<xmlSpace(space_size)<<"</parameterList>"<<endl;
}

void CompilationEngine::subroutineBody(){
    output_file<<xmlSpace(space_size)<<"<subroutineBody>"<<endl;
    *space_size_ptr = 3;
    output_file<<xmlSpace(space_size)<<"<"<<tokenizer->tokenType()<<"> "<<tokenizer->getCurrentToken()<<" </"<<tokenizer->tokenType()<<">"<<endl;
    tokenizer->advance();
    if(tokenizer->getCurrentToken() == "var"){
        *space_size_ptr = 3;
        compileVarDec();
    }
    compileStatements();
    output_file<<xmlSpace(space_size)<<"<"<<tokenizer->tokenType()<<"> "<<tokenizer->getCurrentToken()<<" </"<<tokenizer->tokenType()<<">"<<endl;
    *space_size_ptr = 2;
    output_file<<xmlSpace(space_size)<<"</subroutineBody>"<<endl;
}

void CompilationEngine::compileVarDec(){
    output_file<<"   "<<"<varDec>"<<endl;
    while(tokenizer->getCurrentToken() != ";"){
        output_file<<"    "<<"<"<<tokenizer->tokenType()<<"> "<<
        tokenizer->getCurrentToken()
        <<" </"<<tokenizer->tokenType()<<">"<<endl;
        tokenizer->advance();
    }
    output_file<<"    "<<"<"<<tokenizer->tokenType()<<"> "<<tokenizer->getCurrentToken()<<" </"<<tokenizer->tokenType()<<">"<<endl;
    output_file<<"   "<<"</varDec>"<<endl;
}

void CompilationEngine::compileStatements(){
    output_file<<"   "<<"<statements>"<<endl;
    tokenizer->advance();
    while(tokenizer->getCurrentToken() != "}"){
        if(tokenizer->getCurrentToken() == "if"){
            compileIf();
        }else if(tokenizer->getCurrentToken() == "let"){
            compileLet();
        }else if(tokenizer->getCurrentToken() == "while"){
            compileWhile();
        }else if(tokenizer->getCurrentToken() == "do"){
            compileDo();
        }else if(tokenizer->getCurrentToken() == "return"){
            compileReturn();
        }
        tokenizer->advance();
    }
    output_file<<"   "<<"</statements>"<<endl;
}

void CompilationEngine::compileIf(){
    output_file<<"    "<<"<ifStatement>"<<endl;
    // if (
    output_file<<"     "<<"<"<<tokenizer->tokenType()<<"> "<<tokenizer->getCurrentToken()<<" </"<<tokenizer->tokenType()<<">"<<endl;
    tokenizer->advance();
    output_file<<"     "<<"<"<<tokenizer->tokenType()<<"> "<<tokenizer->getCurrentToken()<<" </"<<tokenizer->tokenType()<<">"<<endl;
    compileExpression();
    
    output_file<<"    "<<"</ifStatement>"<<endl;
}

void CompilationEngine::compileLet(){
    output_file<<"    "<<"<letStatement>"<<endl;
    output_file<<"    "<<"</letStatement>"<<endl;
}

void CompilationEngine::compileWhile(){
    output_file<<"    "<<"<whileStatement>"<<endl;
    output_file<<"    "<<"</whileStatement>"<<endl;
}

void CompilationEngine::compileDo(){
    output_file<<"    "<<"<doStatement>"<<endl;
    output_file<<"    "<<"</dofStatement>"<<endl;
}

void CompilationEngine::compileReturn(){
    output_file<<"    "<<"<returnStatement>"<<endl;
    output_file<<"    "<<"</returnStatement>"<<endl;
}

void CompilationEngine::compileExpression(){
    output_file<<"     "<<"<expression>"<<endl;
    compileTerm();
    output_file<<"     "<<"</expression>"<<endl;
}

void CompilationEngine::compileTerm(){
    output_file<<"       "<<"<term>"<<endl;
    tokenizer->advance();
    if(tokenizer->getCurrentToken() == "("){
        output_file<<"        "<<"<"<<tokenizer->tokenType()<<"> "<<tokenizer->getCurrentToken()<<" </"<<tokenizer->tokenType()<<">"<<endl;
        compileExpression();
    }
    output_file<<"</term>"<<endl;
}

void CompilationEngine::compileExpressionList(){
    
}



//----------------------------------------------------------------------------------------------
int main(int argc, char* argv[]){
        CompilationEngine* compilationMachine = new CompilationEngine(argv[1]);
        compilationMachine->compileClass();
            
        return 0;
}
