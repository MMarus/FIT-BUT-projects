// fold2.cc
// Autor: Marek Marusic, FIT
// Řešení IJC-DU2, příklad 1 b), 20.4.2014

#include<iostream>
#include<string>
#include<climits>
#include<fstream>
#include<sstream>

using namespace std;

int printW(string word,unsigned int length,unsigned int line_max)
{
	int wLen= word.length();
	length += wLen;
	if(length >= line_max && length != wLen)
	{
		cout << "\n";
		length = wLen;
	}
	else if(length < line_max && length > wLen)
	{
		cout << " ";
		length++;
	}

	cout << word;
//cout << "\n" << length << " " << wLen << "\n";
	return length;
}

int func(string line, unsigned int length,unsigned int line_max,int Cut,int * ErrLine)
{
	string word;
	
	istringstream words (line);

	words >> word ;
	
	if(word.empty())
	{
		cout << "\n";
		length = line_max;
	}
	while(!word.empty())
	{
		if(Cut == 1 && word.length() > line_max)
		{
			word.erase(line_max);
			if(* ErrLine == 0)
			{
				cerr << "word too long for line\n";
				* ErrLine = 1;
			}
		}

		length = printW(word,length,line_max);
		word.clear();
		words >> word ;
	}
	return length;
}


//!!!!!!!dorobit vypis warningov!!!!!!!!!!!!!
int main(int argc, char* argv[])
{
	//-------argumenty-------
	unsigned int line_max = 80;
	int Cut =0;
	char * filename= NULL;

	ifstream fr;
	string line;
	string arg;
	unsigned int length = 0;

	int ErrLine = 0;
	
	//--spracovanie argumentov--
	ios::sync_with_stdio(false);

	for (int i = 1; i < argc; i++)
	{
		arg = argv[i];
		if(arg.compare("-c") == 0)
			Cut = 1;
		else if(arg.compare("-w") == 0)
		{
			if(argc <= i+1 )
			{
    		cerr << "ERROR Not num \n";
    		return 1;
    	}
    	i++;
    	istringstream buff(argv[i]);
    	buff >> line_max;
    	if(line_max < 1 || line_max > INT_MAX)
    	{
    		cerr << "ERROR Not num or num too big\n";
    		return 1;
    	}
		}
		else
		{
			filename = argv[i];
		}

	}

//	cout << line_max << " " << Cut << " " << filename << "\n";

	istream* pCin = &cin;
	if(filename != NULL)
	{
		fr.open(filename);
		if(!fr.is_open())
		{
			cerr<<("ERROR file not open\n");
			return 1;
		}
		pCin = &fr;
	}

	while(!pCin->eof())
	{
		getline(*pCin,line);

		length = func(line,length,line_max,Cut,&ErrLine);
	}
	
	if(filename != NULL)
		fr.close();
	return 0;
}