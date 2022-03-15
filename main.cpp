#include <iostream>
#include "aloca.h"
#define MEMORIA 10000

int main(int argc, char *argv[])
{	
	meualoc* alocador = new meualoc(MEMORIA, FIRSTFIT);
    
    char *A = alocador->aloca(10);
    char *B = alocador->aloca(20);
    char *C = alocador->aloca(30);
    char *D = alocador->aloca(50);

	alocador->libera(B);
    alocador->imprimeDados();
	alocador->libera(D);
    alocador->imprimeDados();
	alocador->libera(C);
    alocador->imprimeDados();
	
	int tam = 20;
    if (alocador->verifica(B,tam))
        printf("B está alocado com pelo menos %d bytes!\n", tam);
    else
        printf("B não está alocado com pelo menos %d bytes.\n", tam);

    if (alocador->verifica(A,10))
        printf("A está alocado com pelo menos %d bytes!\n", tam);
    else
        printf("A não está alocado com pelo menos %d bytes.\n", tam);
	return 0;
}
