#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<string> TOKEN_TYPES = {"KEYWORD","SYMBOL", "IDENTIFIER","INT_CONST","STRING_CONST"};
vector<string> TOKEN_KEYWORDS = {"CLASS","METHOD", "FUNCTION","CONSTRUCTOR","INT","BOOL",
								"CHAR","VOID","VAR","STATIC","FIELD","ASIGN","DO","IF",
								"ELSE","WHILE","RETURN","TRUE","FALSE","NULL","THIS"};
vector<char> SYMBOLS = {'(',')','[',']','{','}',',',';','=','.','+','-','*','/','&','|','~','<','>'};
vector<char> WHITE_SPACE = {' ','\n','\t'};

class RiosTokenizer{

	private:
		string input_file;		
		string current_token;
   
	public:
		fstream riosFile;

		RiosTokenizer(string argv){
			riosFile.open(argv);
		}

		string getCurrentToken();
		string to_upper(string word);
		bool isInVector(string word, vector<string> words);
		bool hasMoreTokens();
		void advance();
		string tokenType();
		string keyWord();
		char symbol();
		string identifier();
		int intVal();
		string stringVal();
		

};

string RiosTokenizer::getCurrentToken(){
	return current_token;
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
				riosFile.get(letter);
			}else{
				riosFile.get(letter);				
				while(letter != '*'){
					riosFile.get(letter);
				}
				riosFile.get(letter);
				riosFile.get(letter);
				riosFile.get(letter);
			}
			
		}

		//read symbols
		if(!isalpha(letter)){	
			current_token = letter;
		}

		//read words
		while(isalpha(letter)){			
			newCurrent = newCurrent + letter;
			riosFile.get(letter);
			decreaseCount = true;
		}

		if(decreaseCount){
			current_token = newCurrent;
			riosFile.unget();
		}

		//read numbers
		while(isdigit(letter)){
			newCurrent = newCurrent + letter;
			riosFile.get(letter);
	
			if(!isdigit(letter)){
				current_token = newCurrent;
				break;
			}			
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
		return "STRING_CONST";
	}else if(isdigit(current_token[0])){
		return "INT_CONST";
	}else if(!isalpha(current_token[0])){
		return "SYMBOL";
	}else if(isInVector(to_upper(current_token), TOKEN_KEYWORDS)){
		return "KEYWORD";
	}else
	{
		return "IDENTIFIER";
	}
	
}

string RiosTokenizer::keyWord(){
	return to_upper(current_token);
}

char RiosTokenizer::symbol(){
	return current_token[0];
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


int main(int argc, char* argv[]){
		RiosTokenizer tokenizer(argv[1]);
		while(!tokenizer.riosFile.eof()){
			tokenizer.advance();
			cout<<tokenizer.getCurrentToken()<<" -> "<<tokenizer.tokenType()<<endl;
		}
		return 0;
}