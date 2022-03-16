#include <iostream>
#include <bits/stdc++.h>
#include "aloca.h"

int main(int argc, char *argv[]) {

    std::string nome_arquivo = argv[1];
    std::string nome_saida = nome_arquivo + "_saida.csv";
    std::string linha;

    std::ifstream entrada (nome_arquivo);
    int tamanho_memoria, id_politica, n_operacoes, num_falhas = 0;

    entrada >> tamanho_memoria >> id_politica;
    entrada >> n_operacoes;

    std::ofstream saida(nome_saida);
    saida << "indice,falhas,blocos,tam_medio,total" << std::endl;
    
    meualoc * alocador = new meualoc(tamanho_memoria, id_politica);

    std::vector<char * > vetor_processos(n_operacoes, NULL);
    std::vector<std::string > vetor_linha(3 , "");
    //[ A, 0, 34234]

    for (int i = 0; i < n_operacoes; i++) {
       linha = ""; 
       entrada >> vetor_linha[0] >> vetor_linha[1];
       linha += std::to_string(i)+","; // linha = "0"
       if(vetor_linha[0] == "A") {
           /* linha +="A,"; //linha = "0,A" */
           entrada >> vetor_linha[2];
           vetor_processos[stoi(vetor_linha[1])] = alocador->aloca(stoi(vetor_linha[2]));
           if(!vetor_processos[stoi(vetor_linha[1])]) {
               num_falhas += 1;
           }
           linha += std::to_string(num_falhas)+",";
       } else {
           /* linha +="L,"; */
           tamanho_memoria = alocador->libera(vetor_processos[stoi(vetor_linha[1])]);
           linha += std::to_string(num_falhas)+",";
       }
       alocador->exporta_dados(linha);
       saida << linha << std::endl; 
    }
    /* alocador->imprimeDados(); */
    return 0;
}
