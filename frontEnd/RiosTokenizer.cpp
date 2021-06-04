#include <iostream>
#include <fstream>
#include <vector>
#include "RiosTokenizer.h"

using namespace std;

vector<string> TOKEN_TYPES = {"KEYWORD","SYMBOL", "IDENTIFIER","INT_CONST","STRING_CONST"};
vector<string> TOKEN_KEYWORDS = {"CLASS","METHOD", "FUNCTION","CONSTRUCTOR","INT","BOOLEAN",
								"CHAR","VOID","VAR","STATIC","FIELD","LET","DO","IF",
								"ELSE","WHILE","RETURN","TRUE","FALSE","NULL","THIS"};
vector<char> SYMBOLS = {'(',')','[',']','{','}',',',';','=','.','+','-','*','/','&','|','~','<','>'};
vector<char> WHITE_SPACE = {' ','\n','\t'};


RiosTokenizer::RiosTokenizer(string file){
	input_file = file;
	riosFile.open(input_file);
}

string RiosTokenizer::getCurrentToken(){
	return current_token;
}

void RiosTokenizer::setCurrentToken(string newToken){
	current_token = newToken;
}

string RiosTokenizer::getInputFile(){
	return input_file;
}

string RiosTokenizer::changeFileName(){
	string newWord = "";
	
	for(int i = 0; i<input_file.length(); i++){
		if(input_file[i] == '.') break;
		newWord= newWord + input_file[i];
	}
	newWord = newWord + ".xml";	
	return newWord;
}

string RiosTokenizer::to_upper(string word){
	string newWord = "";
	for(int i = 0; i<word.length(); i++){
		newWord = newWord + (char)toupper(word[i]);
	}
	return newWord;
}

bool RiosTokenizer::isInVector(string word, vector<string> words){	
	for(string currentWord : words){
		if(word == currentWord) return true;
	}
	return false;
}

bool RiosTokenizer::hasMoreTokens(){
	return (riosFile.eof())?false:true;
}

void RiosTokenizer::advance(){
	char letter;
	bool decreaseCount = false;
	if(hasMoreTokens()){
		string newCurrent = "";
		riosFile.get(letter);

		//avoid white spaces
		while(letter == ' ' || letter == '\n'){
			riosFile.get(letter);
		}

		//avoid comments
		if(letter == '/'){
			riosFile.get(letter);
			if(letter == '/'){
				while(letter != '\n'){
					riosFile.get(letter);
				}
			}else if(letter == '*'){
				riosFile.get(letter);				
				while(letter != '/'){
					riosFile.get(letter);
				}
				riosFile.get(letter);
			}else{
				riosFile.unget();
				riosFile.unget();
				riosFile.get(letter);
				current_token = letter;
			}
			
		}

		//read symbols
		if(!isalpha(letter) && letter != '/'){
			if(letter == '<'){
				current_token = "&lt;";
			}else if(letter == '>'){
				current_token = "&gt;";
			}else if(letter == '"'){
				current_token = "&quot;";
			}else if(letter == '&'){
				current_token = "&amp;";
			}else{
				current_token = letter;
			}	
		}

		//read words
		while(isalpha(letter)){			
			newCurrent = newCurrent + letter;
			riosFile.get(letter);
			decreaseCount = true;
		}


		//read numbers
		while(isdigit(letter)){
			newCurrent = newCurrent + letter;
			riosFile.get(letter);
	
			if(!isdigit(letter)){
				current_token = newCurrent;
				decreaseCount = true;
				break;
			}			
		}

		if(decreaseCount){
			current_token = newCurrent;
			riosFile.unget();
		}

		//read strings
		if(letter == '"'){
			newCurrent = newCurrent + letter;
			riosFile.get(letter);
			while(letter != '"'){
				newCurrent = newCurrent + letter;
				riosFile.get(letter);
			}
			newCurrent = newCurrent + letter;
			current_token = newCurrent;
		}
	}
					
}

string RiosTokenizer::tokenType(){	
	if(current_token[0] == '"'){
		return "stringConstant";
	}else if(isdigit(current_token[0])){
		return "integerConstant";
	}else if(!isalpha(current_token[0])){
		return "symbol";
	}else if(isInVector(to_upper(current_token), TOKEN_KEYWORDS)){
		return "keyword";
	}else
	{
		return "identifier";
	}
	
}

string RiosTokenizer::keyWord(){
	return to_upper(current_token);
}

string RiosTokenizer::symbol(){
	return current_token;
}

string RiosTokenizer::identifier(){
	return current_token;
}

int RiosTokenizer::intVal(){
	return stoi(current_token);
}

string RiosTokenizer::stringVal(){
	string stringValue = "";
	for(int i = 1; i<current_token.length()-1; i++){
		stringValue = stringValue + current_token[i];
	}
	return stringValue;
}

/*
int main(int argc, char* argv[]){
		RiosTokenizer tokenizer(argv[1]);
		
		//name of xml file (changin .rios for T.xml)
		string newFile = tokenizer.changeFileName();
		cout<<newFile;

		//make de xml file
		ofstream newfile;
		newfile.open(newFile);
		newfile<<"<tokens>"<<endl;
		string token;

		while(!tokenizer.riosFile.eof()){
			tokenizer.advance();

			if(tokenizer.tokenType() == "stringConstant"){
					token = tokenizer.stringVal();
			}else{
				token = tokenizer.getCurrentToken();
			}

			if(tokenizer.getCurrentToken() != "\n" && !tokenizer.riosFile.eof()){
				
				// <tokenType> token </tokenType>
				newfile<<"	<"<<tokenizer.tokenType()<<"> "<<token<<" </"<<tokenizer.tokenType()<<">"<<endl;
			}
						
		}
		newfile<<"</tokens>";
		return 0;
}*/