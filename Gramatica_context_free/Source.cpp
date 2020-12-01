
#include "grammar.h"
#include "push_down_automate.h"
#include "code_generator.h"


int main() {

	/*srand(time(NULL));

	grammar gr;
	gr.initialize();
	gr.createStrings(10);*/

	/*APD apd;
	apd.initialize();
	apd.postTable();
	apd.pushDownAutomate();*/

	APDCG apdcg;
	apdcg.initialize();
	apdcg.pushDownAutomateWithCodeGenerator();
	
	return 0;
}

