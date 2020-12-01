#include "includes.h"

#define _CRT_SECURE_NO_WARNINGS

const static int _pMatrixLines = 10;
const static int _pMatrixColumns = 10;

struct APD
{
	string pMatrix[_pMatrixLines][_pMatrixColumns];
	string initialString = "(a+a)*a$";
	string delimitersP = ",->";
	string delimitersT = ",";
	vector<pair<string, string>> productions;

	void initialize();
	void postTable();
	void pushDownAutomate();
	string getElement(string, string);
	vector<string> parseTableLine(string);
	vector<pair<string, string>> parseProductions(string);

};

void APD::initialize() {

	ifstream file("productions_tables_apd.txt");
	string p;
	string tableLine;
	vector<string>tableLineV;

	getline(file, p);

	productions = parseProductions(p);

	for (int i = 0; i < _pMatrixLines; i++) {
		getline(file, tableLine);
		tableLineV = parseTableLine(tableLine);
		for (int j = 0; j < tableLineV.size(); j++) {
			pMatrix[i][j] = tableLineV[j];
		}
	}

}

vector<pair<string, string>> APD::parseProductions(string s) {
	char strng[100];
	char* pch;
	string x, y;
	vector<pair<string, string>>v;
	strcpy(strng, s.c_str());
	pch = strtok(strng, delimitersP.c_str());
	while (pch != NULL)
	{
		x = pch;
		pch = strtok(NULL, delimitersP.c_str());
		y = pch;
		pch = strtok(NULL, delimitersP.c_str());
		v.push_back(make_pair(x, y));
	}
	return v;
}

vector<string>APD::parseTableLine(string s) {
	char strng[100];
	char* pch;
	string x, y;
	vector<string>v;
	strcpy(strng, s.c_str());
	pch = strtok(strng, delimitersT.c_str());
	while (pch != NULL)
	{
		x = pch;
		pch = strtok(NULL, delimitersT.c_str());
		v.push_back(x);
	}
	return v;
}

void APD::postTable() {

	for (int i = 0; i < _pMatrixLines; i++) {
		for (int j = 0; j < _pMatrixColumns; j++) {
			cout << std::setw(5);
			cout << this->pMatrix[i][j];
		}
		cout << endl;
	}
	cout << endl;
}

string APD::getElement(string line, string column) {

	int _line, _column = 0;
	for (int i = 0; i < _pMatrixLines; i++) {
		if (pMatrix[i][0] == line) {
			_line = i;
		}
	}
	for (int j = 0; j < _pMatrixColumns; j++) {
		if (pMatrix[0][j] == column) {
			_column = j;
		}
	}
	return pMatrix[_line][_column];
}

void APD::pushDownAutomate() {

	stack<string> principalStack, principalStackCopy;
	string initialStringCopy;
	string element;

	initialStringCopy = this->initialString;
	principalStack.push("$");

	while (initialStringCopy.length() > 0) {

		int _length = -1;
		int _index = -1;
		string auxString;
		auxString = auxString + initialStringCopy[0];
		element = getElement(principalStack.top(), auxString);
		cout << principalStack.top() << " " << auxString << " " << element << endl;

		if (element == "<." || element == "=.") {
			principalStack.push(auxString);
			initialStringCopy = initialStringCopy.substr(1);
		}
		if (element == ">.") {
			principalStackCopy = principalStack;
			string inStack;
			while (!principalStackCopy.empty()) {
				inStack.append(principalStackCopy.top());
				principalStackCopy.pop();
				reverse(inStack.begin(), inStack.end());
				for (int k = 0; k < productions.size(); k++) {
					if (inStack == productions[k].second) {
						_length = (productions[k].second).size();
						_index = k;
					}
				}
				reverse(inStack.begin(), inStack.end());
			}
			for (int h = 0; h < _length; h++) {
				principalStack.pop();
			}
			principalStack.push(productions[_index].first);
		}
		if (element == "acc") {
			cout << "STRING ACCEPTED!" << endl;
			break;
		}
		if (element == "-") {
			cout << "STRING IS NOT ACCEPTED!" << endl;
			break;
		}
	}
}
