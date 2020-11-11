
#include "grammar.h"
#include "push_down_automate.h"


int main() {

	/*srand(time(NULL));

	grammar gr;
	gr.initialize();
	gr.createStrings(10);*/

	APD apd;
	apd.initialize();
	apd.postTable();
	apd.pushDownAutomate();

	return 0;
}

