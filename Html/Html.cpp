// Html.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <exception>
#include <string>
#include <stack>
using namespace std;
//-------------------------------------------------
class Lista
{
public:
	fstream _plikL;
	vector<string>* _listWord;
	//---------------------------------------------
	Lista()
	{
		_plikL.open("Lista.txt", ios::in);
		if (_plikL.good() != true)
		{
			//cout << "File List cant open" << endl;
			throw exception("File List cant open");
		}
		_listWord = new vector<string>;
	}
	~Lista()
	{
		_plikL.close();
		delete _listWord;
	}

	void ReadFile()
	{
		string buff;

		while (!_plikL.eof())
		{
			getline(_plikL, buff);
			_listWord->push_back(buff);
		}
	}

};
//-------------------------------------------------
class PlikHTML
{
public:
	fstream _plik1;
	stack < string > _stack;
	Lista* _wordList;
	//------------------------------------------
	PlikHTML(char* s, Lista* w)
	{
		_plik1.open(s, ios::in);
		if (_plik1.good() != true)
		{
			//cout << "Can not open file html" << endl;
			throw exception("Can not open file html");
		}
		_wordList = w;

	}
	~PlikHTML()
	{
		_plik1.close();
	}
	//-------------------------------------------

	int IsCorrect()
	{
		char znak;
		_plik1 >> znak;
		// zczytyje wyrazy z html , sprawdzam czy znajduja sie w liscie , jesli tak to rzucam na stos 
		while (!_plik1.eof())
		//do
		{
			if (znak == '<')// fcja sprawdzajaca czy to znacznik 
			{
				if (IsIndex() == 0) return 0;
			}
			else if (znak == '/')
			{
				// zamkniecie znacznika ; 
				// czytam znacznik ,
				if (IsOnStack() == 0) return 0; 
				//patrze czy ten sam jest na stosie , jesli tak zrzucam, popranie ; nie blad html 
			}
			else
			{
				// wiec jest to teks , sprawdzam czy wczesniej wystapil znacznik , jesli tak jest to napis naglowka itp ; 
				cout << znak;
				ReadText();
			}
			_plik1 >> znak;
		} //while (znak != EOF);
		return 1;

	}
	//--------------------------------------------------------------------------
	void ReadText()
	{
		char znak3;
		int i = 0;
		char buff[30];
		memset(buff, '\0',sizeof(buff));
		_plik1 >> znak3;
		while (znak3 != '<')
		{
			buff[i] = znak3;
			cout << znak3; // wyswietle na ekran 
			i++;
			_plik1 >> znak3;
		}
		
		cout << endl;
	}
	//--------------------------------------------------------------------------
	/*char* ReadIndex()
	{
		char z;
		int i = 0;
		char buff[10];
		memset(buff, '\0',  sizeof(buff));
		_plik1 >> z;
		while (z != '>')
		{
			buff[i] = z;
			i++;
			_plik1 >> z;
		}
		return buff;
	}*/
	//--------------------------------------------------------------------------
	int IsOnStack( )
	{
		char z;
		int i = 0;
		char buff[10];
		memset(buff, '\0',sizeof(buff));
		_plik1 >> z;
		while (z != '>')
		{
			buff[i] = z;
			i++;
			_plik1 >> z;
		}
		if (strcmp((_stack.top()).c_str(), buff ) == 0)
		{
			_stack.pop();
			cout << "Zamkniecie znacznika : "<< buff << endl;
			return 1;
		}

		return 0;
	}
	//--------------------------------------------------------------------------
	int IsIndex()
	{
		char znak1;
		char buff[10];
		//memset(buff, '\0', sizeof(buff));
		memset(buff, 0, sizeof(buff));

		int i = 0;
		_plik1 >> znak1;
	//	if (znak1 == EOF) return 0; 
		if (znak1 == '/')
		{
			// zamkniecie znacznika ; 
			// czytam znacznik ,
			if (IsOnStack() == 0) return 0;
			else return 1; 
			//patrze czy ten sam jest na stosie , jesli tak zrzucam, popranie ; nie blad html 
		}

		while (znak1 != '>')
		{
			buff[i] = znak1;
			i++;
			_plik1 >> znak1;
		}
		// koniec czytania do > 
		// sprawdzam czy jest na liscie jesli tak to push na stos 
	
		if (IsOnList(buff))
		{
			_stack.push(buff);
			cout << "Otwarcie znacznika : " << buff << endl;
		}
		else
		{
			// niepoprawny znacznik ; 
			return 0;
		}
		return 1;
	}

	//--------------------------------------------------------------------
	int IsOnList(char* nazwa)
	{
		string napis;
		for (int i = 0; i < (_wordList->_listWord->size()); i++)
		{

			if (strcmp(((*_wordList->_listWord)[i]).c_str(), nazwa) == 0)
				return 1;
		}
		return 0;
	}
};
//-------------------------------------------------------------------------



int main()
{
	Lista* b = new Lista();
	b->ReadFile();
	try
	{

		PlikHTML a("home.html", b);
		if (a.IsCorrect())
			cout << "Correct plik html" << endl;
		else
			cout << "This file is not correct!" << endl;
	}
	catch (exception& ex)
	{
		cout << ex.what();
	}

	return 0;
}

