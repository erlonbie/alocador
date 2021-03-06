#define BESTFIT 0
#define FIRSTFIT 1
#define NEXTFIT 2
#include <vector>
// comentario teste
//TAMANHO MÁXIMO DE ALOCACAO: 65535 (maior unsigned short)

#define MAGICO 0xFBF3

class meualoc{
	private:
		char* memoria; //char* pois eh byte a byte
		int politica, tamanhoMemoria;
		std::vector<std::pair<char *, int>> livre;
		int nextLivre;


	public:
		//tamanhoMemoria vai definir o tamanho da memória que o alocador vai utilizar
		//politicaMem define como escolher o bloco de onde saira a memória
		meualoc(int tamanhoMemoria,int politicaMem);

		//Pega um pedaco da variavel memoria e aloca, retornando o ponteiro para o comeco dessa regiao e atualizando a lista de livres.
		char *aloca(unsigned short int tamanho);

		//Verifica se a posicao dada pelo ponteiro esta alocada
		char* verifica(char* ponteiro,int posicao=0);
		
		//Libera o espaco ocupado na posicao, de forma analoga ao free. Ou seja, liberar toda a memoria alocada para este ponteiro na funcao aloca.
		int libera(char* ponteiro);

		//Imprime o numero de elementos na lista de vazios, o maior e a media de tamanhos dos blocos vazios
		void imprimeDados();

		char * bestfit(unsigned short tamanho);

		char * nextfit(unsigned short tamanho);

		char * firstfit(unsigned short tamanho);

		char * aloca_aux(int pos_livre, unsigned short tamanho);

		void coalesce();

		~meualoc();


};
