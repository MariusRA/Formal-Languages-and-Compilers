#include "includes.h"

const static int matrixLines = 11;
const static int matrixColumns = 11;

struct PrecedenceMatrix {

	string pMatrix[matrixLines][matrixColumns];
	vector<string> terminals;
	vector<string> nonTerminals;
	vector<string> startSymbol;
	vector<pair<string, string>> productions;
	string delimiters = ",->";
	string delimitersT = ",";
	string start;

	void initialize();
	void setElement(string, string, string);
	void postMatrix();
	void generateMatrix();
	string getProductionType(int);
	string getElement(string, string);
	vector<string> parseTableLine(string);
	vector<string> tini(string);
	vector<string> tfin(string);
	vector<pair<string, string>>initial_pairs();
	vector<pair<string, string>>do_TFINpairs(pair<string,string>);
	vector<pair<string, string>>do_TINIpairs(pair<string,string>);
	vector<string> parseTerminals(string);
	vector<string> parseNonTerminals(string);
	vector<pair<string, string>> parseProductions(string);
	vector<string> parseStartSymbol(string);


	void verifyAllOk();

};

void PrecedenceMatrix::initialize() {

	ifstream file("matrix_generator.txt");

	string nT, t, p, sS, tableLine;
	vector<string>tableLineV;

	getline(file, nT);
	getline(file, t);
	getline(file, p);
	getline(file, sS);

	nonTerminals = parseNonTerminals(nT);
	terminals = parseTerminals(t);
	productions = parseProductions(p);
	startSymbol = parseStartSymbol(sS);
	for (int i = 0; i < matrixLines; i++) {
		getline(file, tableLine);
		tableLineV = parseTableLine(tableLine);
		for (int j = 0; j < tableLineV.size(); j++) {
			pMatrix[i][j] = tableLineV[j];
		}
	}

	file.close();

	this->start = "$" + startSymbol[0] + "$";

}

vector<string> PrecedenceMatrix::parseTerminals(string s) {
	char strng[100];
	char* pch;
	string x;
	vector<string>v;
	strcpy(strng, s.c_str());
	pch = strtok(strng, delimiters.c_str());
	while (pch != NULL) {
		x = pch;
		v.push_back(x);
		pch = strtok(NULL, delimiters.c_str());
	}
	return v;
}

vector<string> PrecedenceMatrix::parseNonTerminals(string s) {
	char strng[100];
	char* pch;
	string x;
	vector<string>v;
	strcpy(strng, s.c_str());
	pch = strtok(strng, delimiters.c_str());
	while (pch != NULL) {
		x = pch;
		v.push_back(x);
		pch = strtok(NULL, delimiters.c_str());
	}
	return v;
}

vector<string> PrecedenceMatrix::parseStartSymbol(string s) {
	vector<string>v;
	v.push_back(s);
	return v;
}

vector<pair<string, string>> PrecedenceMatrix::parseProductions(string s) {
	char strng[100];
	char* pch;
	string x, y;
	vector<pair<string, string>>v;
	strcpy(strng, s.c_str());
	pch = strtok(strng, delimiters.c_str());
	while (pch != NULL)
	{
		x = pch;
		pch = strtok(NULL, delimiters.c_str());
		y = pch;
		pch = strtok(NULL, delimiters.c_str());
		v.push_back(make_pair(x, y));
	}
	return v;
}

void PrecedenceMatrix::verifyAllOk() {

	for (size_t i = 0; i < productions.size(); i++) {
		cout << productions[i].first << " " << productions[i].second << " ";
	}
	cout << endl;
	for (size_t i = 0; i < terminals.size(); i++) {
		cout << terminals[i] << " ";
	}
	cout << endl;
	for (size_t i = 0; i < nonTerminals.size(); i++) {
		cout << nonTerminals[i] << " ";
	}
	cout << endl;
	for (size_t i = 0; i < startSymbol.size(); i++) {
		cout << startSymbol[i] << " ";
	}
	cout << endl;
	cout << this->start;
	cout << endl;

	vector<pair<string, string>>vv;
	vv = initial_pairs();

	for (int i = 0; i < vv.size(); i++) {
		cout << vv[i].first << " " << vv[i].second << " ";
	}
	cout << endl;

	vector<pair<string, string>>vvv;
	pair<string, string>pa;
	pa.first = "T";
	pa.second = "*";
	vvv = do_TFINpairs(pa);

	for (int i = 0; i < vvv.size(); i++) {
		cout << vvv[i].first << " " << vvv[i].second << " ";
	}
	cout << endl;

	vector<pair<string, string>>v2;
	pair<string, string>pa1;
	pa.first = "*";
	pa.second = "F";
	v2 = do_TINIpairs(pa);

	for (int i = 0; i < v2.size(); i++) {
		cout << v2[i].first << " " << v2[i].second << " ";
	}
	cout << endl;

}

vector<string>PrecedenceMatrix::tini(string s) {

	vector<string> v;
	string x;
	int index = -1;

	for (int i = 0; i < productions.size(); i++) {
		if (s == productions[i].first) {
			index = i;
			break;
		}
	}

	for (int i = index; i < productions.size(); i++) {
		string y;
		y = y + productions[i].second[0];
		v.push_back(y);

	}

	vector<string>::iterator ip;
	ip = unique(v.begin(), v.end());
	v.resize(std::distance(v.begin(), ip));

	return v;
}

vector<string>PrecedenceMatrix::tfin(string s) {
	vector<string> v;
	string x;
	int index = -1;

	for (int i = 0; i < productions.size(); i++) {
		if (s == productions[i].first) {
			index = i;
			break;
		}
	}

	for (int i = index; i < productions.size(); i++) {
		string y;
		y = y + productions[i].second[(productions[i].second).length() - 1];
		v.push_back(y);
	}

	vector<string>::iterator ip;
	ip = unique(v.begin(), v.end());
	v.resize(std::distance(v.begin(), ip));

	return v;
}

string PrecedenceMatrix::getElement(string line, string column) {

	int _line, _column = 0;
	for (int i = 0; i < matrixLines; i++) {
		if (pMatrix[i][0] == line) {
			_line = i;
		}
	}
	for (int j = 0; j < matrixColumns; j++) {
		if (pMatrix[0][j] == column) {
			_column = j;
		}
	}
	return pMatrix[_line][_column];
}

void PrecedenceMatrix::setElement(string line, string column, string elem) {

	int _line, _column = 0;
	for (int i = 0; i < matrixLines; i++) {
		for (int j = 0; j < matrixColumns; j++) {
			if (pMatrix[i][0] == line && pMatrix[0][j] == column) {
				pMatrix[i][j] = elem;
			}
		}
	}
}

void PrecedenceMatrix::postMatrix() {
	cout << endl << endl;
	for (int i = 0; i < matrixLines; i++) {
		for (int j = 0; j < matrixColumns; j++) {
			cout << std::setw(6);
			cout << this->pMatrix[i][j];
		}
		cout << endl << setw(8);
	}
	cout << endl;
}

vector<string>PrecedenceMatrix::parseTableLine(string s) {
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

string PrecedenceMatrix::getProductionType(int index) {

	if (index == -1) {
		return "negligible";
	}
	if (((productions[index].second).length() >= 2)) {
		return "complex";
	}
	return "negligible";
}

vector<pair<string, string>>PrecedenceMatrix::initial_pairs() {

	vector<pair<string, string>> v;
	string a, b, c;
	a = a + start[0];
	b = b + start[1];
	c = c + start[2];

	v.push_back(make_pair(a, b));
	v.push_back(make_pair(b, c));

	for (size_t i = 0; i < productions.size(); i++) {
		string type = getProductionType(i);
		if (type == "complex") {
			string x, y, z;
			x = x + productions[i].second[0];
			y = y + productions[i].second[1];
			z = z + productions[i].second[2];
			v.push_back(make_pair(x, y));
			v.push_back(make_pair(y, z));
		}
	}
	return v;
}

vector<pair<string, string>>PrecedenceMatrix::do_TFINpairs(pair<string,string>p) {

	
	vector<string> t = tfin(p.first);

	vector<pair<string, string>> out;

	for (size_t j = 0; j < t.size(); j++) {
		out.push_back(make_pair(t[j], p.second));
	}

	return out;
}

vector<pair<string, string>>PrecedenceMatrix::do_TINIpairs(pair<string,string>p) {

	vector<string> t = tini(p.second);

	vector<pair<string, string>> out;


	for (size_t j = 0; j < t.size(); j++) {
		out.push_back(make_pair(p.first, t[j]));
	}

	return out;
}

void PrecedenceMatrix::generateMatrix() {

	vector<pair<string, string>> v = initial_pairs();

	setElement(v[0].first, v[0].second, "=.");
	setElement(v[1].first, v[1].second, "acc");

	for (size_t i = 2; i < v.size(); i++) {
		setElement(v[i].first, v[i].second, "=.");
	}
	for (size_t j = 0; j < v.size()-1; j=j+2) {
		
		vector<pair<string, string>> v1;
		vector<pair<string, string>> v2;
		pair<string, string>p1 = v[j];
		pair<string, string>p2 = v[j + 1];

		if (p1.first == "E" || p1.first == "T" || p1.first == "F") {
			v1 = do_TFINpairs(p1);
			v2 = do_TINIpairs(p2);
		}
		else {
			v1 = do_TFINpairs(p2);
			v2 = do_TINIpairs(p1);
		}
		
		for (size_t n = 0; n < v1.size(); n++) {
			setElement(v1[n].first, v1[n].second, ">.");
		}
		for (size_t k = 0; k < v2.size(); k++) {
			setElement(v2[k].first, v2[k].second, "<.");
		}
	}
	
}
