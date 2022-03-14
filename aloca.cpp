#include <iostream>
#include <string.h>
#include <bits/stdc++.h>
#include "./aloca.h"

meualoc::meualoc(int tamanhoMemoria, int politicaMem) {
	this->politica = politicaMem;
	this->tamanhoMemoria = tamanhoMemoria;
	this->memoria = (char *) malloc(sizeof(char)*tamanhoMemoria);
	memset(this->memoria, 0, sizeof(char)*tamanhoMemoria);
	this->livre.push_back(std::make_pair(this->memoria, tamanhoMemoria));
	this->nextLivre = 0;
}

char * meualoc::aloca(unsigned short tamanho) {
	switch (this->politica) {
		case BESTFIT:
			std::cout << "bestfit" << std::endl;	
			return bestfit(tamanho);
			break;
		case FIRSTFIT:
			std::cout << "firstfit" << std::endl;
			return firstfit(tamanho);
			break;
		case NEXTFIT:
			std::cout << "nextfit" << std::endl;
			return nextfit(tamanho);
			break;
		default:
			std::cout << "error" << std::endl;
			return NULL;
	}
}

void seta_tamanho(uint tamanho, char * pos) {
	((*(pos + 1)) |= (tamanho & 0x00FF)); 
	(*(pos) |= ((tamanho & 0xFF00) >> 8));
} 

void seta_magico(uint tamanho, char * pos) {
	((*(pos + 3)) |= (tamanho & 0x00FF)); 
	(*(pos + 2) |= ((tamanho & 0xFF00) >> 8));
} 

char * meualoc::aloca_aux(int pos_livre, unsigned short tamanho) {
	seta_tamanho(tamanho, this->livre[pos_livre].first);
	seta_tamanho(MAGICO, this->livre[pos_livre].first);

	char * _memoria = this->livre[pos_livre].first;

	if (tamanho == this->livre[pos_livre].second) {
		this->livre.erase(this->livre.begin() + pos_livre);
	} else {
		this->livre[pos_livre].first += tamanho;
		this->livre[pos_livre].second -= tamanho;
	}

	return _memoria + 4;
}

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
	return aloca_aux(pos_livre, tamanho_total);
}
char * meualoc::bestfit(unsigned short tamanho) {
	int tamanho_total = tamanho + 4;
	int i, pos_livre = 0;
	bool _livre = false;
	int tam = this->livre.size();
	int melhor_tamanho = 0;
	
	for(int i = 0; i < tam; i++){
		if(tamanho_total >= this->livre[i].second && (melhor_tamanho > this->livre[i].second)){
			melhor_tamanho = this->livre[i].second;
			pos_livre = i;
			_livre = true;
		}
	}
	if(!_livre){
		return NULL;
	}
	return aloca_aux(pos_livre, tamanho_total);
}

char * meualoc::nextfit(unsigned short tamanho) {
	uint tamanho_total = tamanho + 4;
	bool _livre = false;
	int pos_livre = 0;
	int tam = this->livre.size();

	for (int i = 0; i < tam; i++){
		if (tamanho_total <= this->livre[i].second) {
			_livre = true;
			this->nextLivre = i;
			break;
		}
	}
	
	if(!_livre) {
		return NULL;
	}

	return aloca_aux(this->nextLivre, tamanho_total);
}

int meualoc::libera(char* ponteiro) {
	if(!ponteiro || ponteiro > this->tamanhoMemoria + this->memoria || ponteiro - 4 < this->memoria) {
		return 0;
	}
	ponteiro -= 4;
	std::cout << (void *) ponteiro << std::endl;
	unsigned short magic = ((ponteiro[0] & 0xFF) << 8) + (ponteiro[1] & 0xFF);
	unsigned short magico = ((ponteiro[0] & 0xFF) << 8) + (ponteiro[1] & 0xFF);
	std::cout << magico << std::endl;
	if(magico == MAGICO) {
		std::cout << "Eh magico" << std::endl;
		unsigned short _tam = ((ponteiro[0] & 0xFF) >> 8) + (ponteiro[1] & 0xFF) + 4;
		this->livre.push_back(std::make_pair(ponteiro, _tam));
		ponteiro[0] = ponteiro[1] = ponteiro[2] = ponteiro[3] = 0; // Apagar cabeçalho
		return _tam;
		coalesce();
	}
	return 0;
}

bool myfunction (std::pair<char *, int> p1, std::pair<char *, int> p2) { return (p1.first < p2.first); }

void meualoc::coalesce() {
	std::sort(this->livre.begin(), this->livre.end(), myfunction);
	
	for (int i = 1; i < this->livre.size(); i++) {
		if((this->livre[i].first) == (this->livre[i-1].first + this->livre[i-1].second)) {
			this->livre[i-1].second += this->livre[i].second;
			this->livre.erase(this->livre.begin() + i);	
			i--;
		}
	}
}

char * meualoc::verifica(char* ponteiro,int posicao) {
	int tam = this->livre.size();
	bool _livre = false;

	for(int i = 0; i < tam; i++){
		if(this->livre[i].first <= ponteiro+posicao && ponteiro+posicao < this->livre[i].first + this->livre[i].second){
			_livre = true;
			break;
		}
	}
	if(!_livre){
		return ponteiro+posicao;
	}
	return NULL;
}

meualoc::~meualoc()
{

    free(this->memoria);
}
