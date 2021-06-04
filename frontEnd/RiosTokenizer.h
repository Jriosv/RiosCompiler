#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class RiosTokenizer{
    private:
        string input_file;
        string current_token;
    
    public:
		fstream riosFile;

		RiosTokenizer(string argv);
        string getCurrentToken();
		void setCurrentToken(string newToken);
		string getInputFile();
		string changeFileName();
		string to_upper(string word);
		bool isInVector(string word, vector<string> words);
		bool hasMoreTokens();
		void advance();
		string tokenType();
		string keyWord();
		string symbol();
		string identifier();
		int intVal();
		string stringVal();
		

};