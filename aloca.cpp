#include <iostream>
#include "./aloca.h"
#include <string.h>

meualoc::meualoc(int tamanhoMemoria, int politicaMem) {
	this->politica = politicaMem;
	this->memoria = (char *) malloc(sizeof(char)*tamanhoMemoria);
	memset(this->memoria, 0, sizeof(char)*tamanhoMemoria);
}

char * meualoc::aloca(unsigned short tamanho) {
	switch (this->politica) {
		case BESTFIT:
			std::cout << "bestfit" << std::endl;	
			break;
		case FIRSTFIT:
			std::cout << "firstfit" << std::endl;
			break;
		case NEXTFIT:
			std::cout << "nextfit" << std::endl;
			break;
		default:
			std::cout << "error" << std::endl;
	}
}


