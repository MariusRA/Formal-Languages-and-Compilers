#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <time.h>

using namespace std;

struct grammar
{
	vector<string> terminals;
	vector<string> nonTerminals;
	vector<string> startSymbol;
	vector<pair<string, string>> productions;
	string delimiters = ",->";

	bool verifyS(string, string);
	bool verifyNotT(string);
	void createStrings(int);
	void initialize();
	string possibilities(string);
	string derivExtremL();
	vector<string> parseTerminals(string);
	vector<string> parseNonTerminals(string);
	vector<pair<string, string>> parseProductions(string);
	vector<string> parseStartSymbol(string);

};

void grammar::initialize() {

	ifstream file("gramatica.txt");

	string nT, t, p, sS;

	getline(file, nT);
	getline(file, t);
	getline(file, p);
	getline(file, sS);

	nonTerminals = parseNonTerminals(nT);
	terminals = parseTerminals(t);
	productions = parseProductions(p);
	startSymbol = parseStartSymbol(sS);
}

bool grammar::verifyS(string s, string terminals) {
	char strng[10];
	bool flag = false;
	string::size_type i = 0;
	strcpy(strng, terminals.c_str());
	while (i < s.size()) {
		if (strchr(strng, s[i])) {
			flag = true;
		}
		else {
			return false;
		}
		i++;
	}
	return true;
}

bool grammar::verifyNotT(string s) {

	bool flag = false;
	for (int i = 0; i < nonTerminals.size(); i++) {
		if (s == nonTerminals[i]) {
			flag = true;
		}
	}
	return flag;
}

vector<string> grammar::parseTerminals(string s) {
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

vector<string> grammar::parseNonTerminals(string s) {
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

vector<string> grammar::parseStartSymbol(string s) {
	vector<string>v;
	v.push_back(s);
	return v;
}

vector<pair<string, string>> grammar::parseProductions(string s) {
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

string grammar::possibilities(string s) {

	vector<string>v;
	string output;

	for (int i = 0; i < productions.size(); i++) {
		if (s == productions[i].first) {
			v.push_back(productions[i].second);
		}
	}
	output.append(v[rand() % (v.size())]);
	return output;
}

string grammar::derivExtremL() {
	string output;
	output.append(startSymbol[0]);
	string term;
	for (int i = 0; i < terminals.size(); i++) {
		term.append(terminals[i]);
	}

	while (output.size() < 60) {
		for (string::size_type i = 0; i < output.size(); i++) {
			string y;
			y = y + output[i];

			if (verifyNotT(y)) {
				string x = possibilities(y);
				output.insert(i + 1, x);
				output.erase(i, 1);
				i = i + x.size();
			}
		}
		if (verifyS(output, term)) {
			break;
		}
	}

	return output;
}

void grammar::createStrings(int NOStrings) {

	for (int i = 0; i < NOStrings; i++) {
		cout << derivExtremL() << endl;
	}
	cout << "-------------------------------------------------------------------------------\n";
}