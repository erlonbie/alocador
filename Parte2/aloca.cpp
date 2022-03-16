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
			/* std::cout << "bestfit" << std::endl; */	
			return bestfit(tamanho);
			break;
		case FIRSTFIT:
			/* std::cout << "firstfit" << std::endl; */
			return firstfit(tamanho);
			break;
		case NEXTFIT:
			/* std::cout << "nextfit" << std::endl; */
			return nextfit(tamanho);
			break;
		default:
			/* std::cout << "error" << std::endl; */
			return NULL;
	}
}

void seta_tamanho(uint tamanho, char * pos) {
	(*(pos) |= ((tamanho & 0xFF00) >> 8));
	((*(pos + 1)) |= (tamanho & 0x00FF)); 
} 

void seta_magico(uint tamanho, char * pos) {
	(*(pos + 2) |= ((tamanho & 0xFF00) >> 8));
	((*(pos + 3)) |= (tamanho & 0x00FF)); 
} 

char * meualoc::aloca_aux(int pos_livre, unsigned short tamanho) {
	seta_tamanho(tamanho-4, this->livre[pos_livre].first);
	seta_magico(MAGICO, this->livre[pos_livre].first);

	char * _memoria = this->livre[pos_livre].first;

	if (tamanho == this->livre[pos_livre].second) {
		this->livre.erase(this->livre.begin() + pos_livre);

	} else {
		this->livre[pos_livre].first += tamanho;
		this->livre[pos_livre].second -= tamanho;
	}
	if(pos_livre + 1 == this->livre.size()) {
		this->nextLivre = 0;
	} else{
		this->nextLivre = pos_livre + 1;
	}
	/* std::cout << "NextLivre: " << this->nextLivre << std::endl; */

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
	int melhor_tamanho = INT_MAX;
	
	for(int i = 0; i < tam; i++){
		if(tamanho_total <= this->livre[i].second && (melhor_tamanho > this->livre[i].second)){
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

	for (int i = this->nextLivre; i < tam; i++){
		if (tamanho_total <= this->livre[i].second) {
			_livre = true;
			pos_livre = i;
			break;
		}
	}
	
	if(!_livre) {
		for (int i = 0; i < this->nextLivre; i++) {
			if (tamanho_total <= this->livre[i].second) {
				_livre = true;
				pos_livre = i;
				break;
			}
		}

		if(!_livre) return NULL;
	}

	return aloca_aux(pos_livre, tamanho_total);
}

int meualoc::libera(char* ponteiro) {
	if(!ponteiro || ponteiro > this->tamanhoMemoria + this->memoria || ponteiro - 4 < this->memoria) {
		return 0;
	}
	ponteiro -= 4;
	unsigned short magico = ((ponteiro[2] & 0xFF) << 8) + (ponteiro[3] & 0xFF);
	if(magico == MAGICO) {
			unsigned short _tam = ((ponteiro[0] & 0xFF) << 8) + (ponteiro[1] & 0xFF) + 4;
			this->livre.push_back(std::make_pair(ponteiro, _tam));
		ponteiro[0] = ponteiro[1] = ponteiro[2] = ponteiro[3] = 0; // Apagar cabeçalho
		coalesce();
		return _tam;		
	}
	return 0;
}

bool myfunction (std::pair<char *, int> p1, std::pair<char *, int> p2) { return (p1.first < p2.first); }

void meualoc::coalesce() {
	std::sort(this->livre.begin(), this->livre.end(), myfunction);
	
	for(int i = 0; i < this->livre.size(); i++){
		/* std::cout << "ponteiro: " << (void*) this->livre[i].first << " tamanho: " << this->livre[i].second << "\n"; */
	}
	
	for (int i = 1; i < this->livre.size(); i++) {
		if((this->livre[i].first) == (this->livre[i-1].first + this->livre[i-1].second)) {
			this->livre[i-1].second += this->livre[i].second;
			this->livre.erase(this->livre.begin() + i);	
			i--;
		}
	}
}

char * meualoc::verifica(char* ponteiro,int posicao) {
	if (!ponteiro || ponteiro - 4 < memoria || ponteiro > memoria + tamanhoMemoria) return NULL; 

	ponteiro -= 4;	 
	unsigned short tam = ((ponteiro[0] & 0xFF) << 8) + (ponteiro[1] & 0xFF);
	unsigned short magico = ((ponteiro[2] & 0xFF) << 8) + (ponteiro[3] & 0xFF);
	ponteiro += 4; 

	if (magico == MAGICO && tam >= posicao) return ponteiro;
	return NULL;
}

void meualoc::imprimeDados(){
	bool _livre = false;
	int tam = this->livre.size();
	int somatorio = 0, maior_tamanho = 0;
	
	for(int i = 0; i < tam; i++){
		/* std::cout << "Posicao: " << i << " " << "Tamanho: " << this->livre[i].second << std::endl; */
		if(this->livre[i].second > maior_tamanho){
			maior_tamanho = this->livre[i].second;
		}
		somatorio += this->livre[i].second;
	}
	std::cout << "Numero de elementos: " << tam << "\n";
	std::cout << "Maior tamanho: " << maior_tamanho << "\n";
	std::cout << "Media dos tamanhos: " << somatorio/tam << "\num_blocos";
}	

std::string meualoc::exporta_dados(std::string& palavra) {
	int num_blocos=0, media=0, total=0, maior=0;
	std::pair<char*, int> par_aux;

	for (int i = 0; i < this->livre.size(); i++) {
		par_aux = this->livre[i];
		num_blocos++;
		total += par_aux.second;
		if(par_aux.second > maior) {
			maior = par_aux.second;
		}
	}
	media = total/num_blocos;
	palavra += std::to_string(num_blocos) +","+ std::to_string(media) +","+ std::to_string(total);

	return palavra;
}

meualoc::~meualoc()
{

    free(this->memoria);
}
