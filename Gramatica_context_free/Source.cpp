
#include "grammar.h"
#include "push_down_automate.h"
#include "code_generator.h"
#include "matrix_generator.h"


int main() {

	/*srand(time(NULL));

	grammar gr;
	gr.initialize();
	gr.createStrings(10);*/

	/*APD apd;
	apd.initialize();
	apd.postTable();
	apd.pushDownAutomate();*/

	/*APDCG apdcg;
	apdcg.initialize();
	apdcg.postTable();
	apdcg.pushDownAutomateWithCodeGenerator();*/
	
	PrecedenceMatrix p;
	p.initialize();
	p.verifyAllOk();

	p.generateMatrix();
	p.postMatrix();
	

	return 0;
}

