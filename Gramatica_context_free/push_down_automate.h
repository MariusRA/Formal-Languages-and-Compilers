#pragma once
#include "grammar.h"

struct APD
{
	string precedenceMatrix[10][10];
	string initialString = "";

	void createPrecedenceMatrix();
	void goTo();
	void reduction();
	void accept();
	void error();
};

