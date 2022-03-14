#include <iostream>
#include <string.h>
#include <bits/stdc++.h>
#include "./aloca.h"

meualoc::meualoc(int tamanhoMemoria, int politicaMem) {
	this->politica = politicaMem;
	this->memoria = (char *) malloc(sizeof(char)*tamanhoMemoria);
	memset(this->memoria, 0, sizeof(char)*tamanhoMemoria);
	this->livre.push_back(std::make_pair(this->memoria, tamanhoMemoria));
}

char * meualoc::aloca(unsigned short tamanho) {
	switch (this->politica) {
		case BESTFIT:
			std::cout << "bestfit" << std::endl;	
			return bestfit(tamanho);
			break;
		case FIRSTFIT:
			std::cout << "firstfit" << std::endl;
			return nextfit(tamanho);
			break;
		case NEXTFIT:
			std::cout << "nextfit" << std::endl;
			return firstfit(tamanho);
			break;
		default:
			std::cout << "error" << std::endl;
			return NULL;
	}
}

void 

char * meualoc::firstfit(unsigned short tamanho) {
	uint tamanho_total = tamanho + 4;
	int pos_livre = 0;
	bool _livre = false;
	int tam = this->livre.size();

	for (int i = 0; i < tam; i++) {
		if(tamanho_total <= this->livre[i].second) {
			_livre = true;
			pos_livre = i;
			break;
		}
	}
	if (!_livre) {
		return NULL;
	}
	
}

char * meualoc::bestfit(unsigned short tamanho) {

}
char * meualoc::nextfit(unsigned short tamanho) {

}


meualoc::~meualoc()
{

    free(this->memoria);
}
