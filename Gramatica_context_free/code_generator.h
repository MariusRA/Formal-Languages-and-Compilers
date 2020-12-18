#include "includes.h"

#define _CRT_SECURE_NO_WARNINGS

const static int linii = 10;
const static int coloane = 10;
static ofstream f;

struct APDCG
{
	string pMatrix[linii][coloane];
	string initialString = "(a+a)*(a+a)$";
	string delimitersP = ",->";
	string delimitersT = ",";
	vector<pair<string, string>> productions;

	void initialize();
	void postTable();
	void writeIntermediaryCodeToFile(string);
	void pushDownAutomateWithCodeGenerator();
	void pushDownAutomate();
	char getOperator(int);
	string getType(int);
	string tempVar(int);
	string emmit(string, string, string, char);
	string getElement(string, string);
	vector<string> parseTableLine(string);
	vector<string> parseTerminals(string);
	vector<string> parseNonTerminals(string);
	vector<pair<string, string>> parseProductions(string);

};

void APDCG::initialize() {

	ifstream file("apdcg.txt");
	string p;
	string tableLine;
	vector<string>tableLineV;

	getline(file, p);

	productions = parseProductions(p);

	for (int i = 0; i < linii; i++) {
		getline(file, tableLine);
		tableLineV = parseTableLine(tableLine);
		for (int j = 0; j < tableLineV.size(); j++) {
			pMatrix[i][j] = tableLineV[j];
		}
		
	}

}

vector<pair<string, string>> APDCG::parseProductions(string s) {
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

vector<string>APDCG::parseTableLine(string s) {
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

void APDCG::postTable() {

	for (int i = 0; i < linii; i++) {
		for (int j = 0; j < coloane; j++) {
			cout << std::setw(5);
			cout << this->pMatrix[i][j];
		}
		cout << endl;
	}
	cout << endl;
}

string APDCG::getElement(string line, string column) {

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

void APDCG::pushDownAutomate() {

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

void APDCG::pushDownAutomateWithCodeGenerator() {

	stack<string> principalStack, principalStackCopy, attributeStack;
	string initialStringCopy;
	string element;
	int ct = 1;

	initialStringCopy = this->initialString;
	principalStack.push("$");
	attributeStack.push("$");

	f.open("apdcg_output.txt");

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

			string type;
			type = getType(_index);
			if (type == "simple") {
				attributeStack.push(productions[_index].second);
			}
			else if (type == "complex") {
				string o1, o2, temp, output;
				char op;
				o1 = attributeStack.top();
				attributeStack.pop();
				o2 = attributeStack.top();
				attributeStack.pop();

				temp = tempVar(ct);
				attributeStack.push(temp);

				op = getOperator(_index);
				output = emmit(temp, o2, o1, op);
				writeIntermediaryCodeToFile(output);
				ct++;
			}

		}

		if (element == "acc") {
			cout << "STRING ACCEPTED!" << endl;
			f.close();
			break;
		}
		if (element == "-") {
			cout << "STRING IS NOT ACCEPTED!" << endl;
			f.close();
			break;
		}
	}
}

void APDCG::writeIntermediaryCodeToFile(string s) {

	f << s << endl;
}

string APDCG::getType(int index) {

	if (index == -1) {
		return "negligible";
	}
	if (((productions[index].second).length() > 1) && (productions[index].second[1] == '+' || productions[index].second[1] == '*')) {
		return "complex";
	}
	if (((productions[index].second).length() == 1) && (productions[index].second == "a")) {
		return "simple";
	}
	return "negligible";
}

string APDCG::emmit(string temp, string o1, string o2, char op) {

	string s;
	s = temp + '=' + o1 + op + o2;
	return s;
}

char APDCG::getOperator(int index) {
	return productions[index].second[1];
}

string APDCG::tempVar(int ct) {
	string t;
	t = "t" + to_string(ct);
	return t;
}
