//
// Created by Vinicius on 13/05/17.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <tic.h>

using namespace std;
/*
 * A função do pre-processador eh tirar espacos em branco desnecessarios, comentarios, e processar as macros EQU e IF.
 * comentarios começam com ;
 * Caso o programa receba o comando -p deve gerar apenas o arquivo de pre-processamento com extensão .pre
 **/

void RemoveComentarios(ifstream& assembly, fstream& semComentarios){ // porque precisa do &?
    string linha;
    int posicao;
    while(getline(assembly, linha)){
        posicao = (int) linha.find_first_of(';');
        if(posicao >= 0) {
            //se existe ; na string, remove e coloca no arquivo de saida
            linha.erase(posicao, string::npos); // apaga do ; até o final da linha
        }
        semComentarios << linha <<'\n';
    }
}

void RemoveEspacosEmBranco(fstream& SemComentarios, fstream& semEspacos){
    string linha;
    string novaLinha;
    istringstream is(linha);
    string token;
    int posicao;
    while(getline(SemComentarios, linha)){
        while(getline(is, token, ' ')){
            novaLinha.append(token);
            novaLinha.append(" ");
        }
        novaLinha.erase(novaLinha.end()-1 );
        novaLinha.append("\n");
    }
    novaLinha.erase(novaLinha.end()-1 ); //apaga o ultimo \n
}


void ExpandeMacros(){

}
int PreProcessaAssembly(ifstream& assembly){
    fstream preprocessado("preprocessado.pre");
    RemoveComentarios(assembly, preprocessado);
    RemoveEspacosEmBranco(preprocessado, preprocessado);
    preprocessado << "testando se escreve no arquivo";
    preprocessado.close();
    return 1;
}