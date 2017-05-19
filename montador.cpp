//
// Created by Vinicius on 13/05/17.
//


/* deve detectar erros e caso seja chamado com a operacao -o recebe um arquivo
 * de entrada .asm ou .pre e gera um arquivo .o na saida após realizar a montagem do programa.
 *
 * */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <map>
#include <algorithm>
#include <vector>
#include <numeric>
using namespace std;

typedef struct{
    char string[100];
    int operando;
}tabInstrucaoOuDiretiva;

typedef struct{
    char simbolo[50];
    int posicao;
    int externo; /*0 ou 1*/
    int secdados;
}tabSimbolos;

typedef struct{
    char simbolo[50];
    int posicao;
}tabUso;

typedef struct{
    char simbolo[50];
    int posicao;
}tabDef;

/*
 * Como o número de instruções e diretivas é pequeno resolvemos criar a tabela ao invés de ler a tabela de um arquivo.
 * */
void GeraTabelaInstrucoesEDiretivas(std::vector<tabInstrucaoOuDiretiva> tabInstrucao, std::vector<tabInstrucaoOuDiretiva> tabDiretiva){
    /*
     * VETOR DE DIRETIVAS
     * */

    strcpy(tabDiretiva[0].string,"SECTION");
    tabDiretiva[0].operando = 0;
    strcpy(tabDiretiva[1].string,"SPACE");
    tabDiretiva[1].operando = 1;
    strcpy(tabDiretiva[2].string,"CONST");
    tabDiretiva[2].operando = 1;
    strcpy(tabDiretiva[3].string,"EQU");
    tabDiretiva[3].operando = 1;
    strcpy(tabDiretiva[4].string,"IF");
    tabDiretiva[4].operando = 1;
    strcpy(tabDiretiva[5].string,"BEGIN");
    tabDiretiva[5].operando = 0;
    strcpy(tabDiretiva[6].string,"END");
    tabDiretiva[6].operando = 0;
    strcpy(tabDiretiva[7].string,"PUBLIC");
    tabDiretiva[7].operando = 0;
    strcpy(tabDiretiva[8].string,"EXTERN");
    tabDiretiva[8].operando = 0;

    /* Inicializando as instruções como um vetor de struct permite que o código da instrução seja o indice+1*/
    /*Da mesma maneira o tamaho da instrução será o número de operandos + 1*/
    strcpy(tabInstrucao[0].string,"ADD");
    tabInstrucao[0].operando = 1;
    strcpy(tabInstrucao[1].string,"SUB");
    tabInstrucao[1].operando = 1;
    strcpy(tabInstrucao[2].string,"MULT");
    tabInstrucao[2].operando = 1;
    strcpy(tabInstrucao[3].string,"DIV");
    tabInstrucao[3].operando = 1;
    strcpy(tabInstrucao[4].string,"JMP");
    tabInstrucao[4].operando = 1;
    strcpy(tabInstrucao[5].string,"JMPN");
    tabInstrucao[5].operando = 1;
    strcpy(tabInstrucao[6].string,"JMPP");
    tabInstrucao[6].operando = 1;
    strcpy(tabInstrucao[7].string,"JMPZ");
    tabInstrucao[7].operando = 1;
    strcpy(tabInstrucao[8].string,"COPY");
    tabInstrucao[8].operando = 2;
    strcpy(tabInstrucao[9].string,"LOAD");
    tabInstrucao[9].operando = 1;
    strcpy(tabInstrucao[10].string,"STORE");
    tabInstrucao[10].operando = 1;
    strcpy(tabInstrucao[11].string,"INPUT");
    tabInstrucao[11].operando = 1;
    strcpy(tabInstrucao[12].string,"OUTPUT");
    tabInstrucao[12].operando = 1;
    strcpy(tabInstrucao[13].string,"STOP");
    tabInstrucao[13].operando = 0;
}

int VerificaSeLinhaValida(std::string checaCaracter ){
    /*Lembrar que aqui a linha ja não é sensível ao caso
     *
     */
    int posicao =(int)checaCaracter.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789+-*/=|&!?#%(){}[]_\'\",.;<>");
    if (posicao >-1) {
        return posicao;
    }
}
// como vetor podemos usar a funcao find
int PesquisaInstrucaoEDiretiva (std::string instrucaoOuDiretiva, std::vector<tabInstrucaoOuDiretiva> vetorInstOuDiretiva) {
    vector<tabInstrucaoOuDiretiva>::iterator it;
    it = find(vetorInstOuDiretiva.begin(), vetorInstOuDiretiva.end(), instrucaoOuDiretiva);
    if(it!= vetorInstOuDiretiva.end())
        return (int)distance(vetorInstOuDiretiva.begin(), it); // retorna a posicao da instrucao ou diretiva
    else
        return -1;
}
// a pesquisa nao pode ser igual pois os tipos dos iteradores sao diferentes
int PesquisaSimbolo (std::string simbolo, std::vector<tabSimbolos> vetorSimbolos) {
    vector<tabSimbolos>::iterator it;
    it = find(vetorSimbolos.begin(), vetorSimbolos.end(), simbolo);
    if(it!= vetorSimbolos.end())
        return (int)distance(vetorSimbolos.begin(), it); // retorna a posicao da instrucao ou diretiva
    else
        return -1;
}


int main_montador (){
    //primeiro vou escrever a ideia
    //recebe o arquivo preprocessado;
    fstream assembly;
    int contador_posicao = 0;
    int contador_linha = 1;


    string linha;
    while (getline(assembly, linha)) {
        // aqui eu leio linha por linha do programa;

        // na primeira passagem eu construo a tabela de simbolos;

        //        primeira passagem

        /*  tokeniza a linha
         *  se existe rotulo: procura rotulo na Tabela de simbolos (se achou, erro simbolo redefinido)
         *  senão: insere o rotulo e contador_posicao na tabela de simbolos
         *  procura operacao na tabela de instrucoes
         *  se achou: soma contador de posicao com o tamanho da instrucao
         *  senão: procura operacao na tabela de diretivas
         *  se achou, executa diretiva e soma o contador_posicao ao valor_retornado pela subrotina
         *  senao: erro, operacao não identificada.
         *
         * */
        contador_linha ++;

        }
}