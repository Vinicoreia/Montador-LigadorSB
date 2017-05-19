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
}tabInstrucao;


typedef struct{
    char string[100];
    int operando;
}tabDiretiva;

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
void GeraTabelaInstrucoesEDiretivas(tabInstrucao vetorInst[], tabDiretiva vetorDiretiva[]){
    /*
     * VETOR DE DIRETIVAS
     * */
    strcpy(vetorDiretiva[0].string,"SECTION");
    vetorDiretiva[0].operando = 0;
    strcpy(vetorDiretiva[1].string,"SPACE");
    vetorDiretiva[1].operando = 1;
    strcpy(vetorDiretiva[2].string,"CONST");
    vetorDiretiva[2].operando = 1;
    strcpy(vetorDiretiva[3].string,"EQU");
    vetorDiretiva[3].operando = 1;
    strcpy(vetorDiretiva[4].string,"IF");
    vetorDiretiva[4].operando = 1;
    strcpy(vetorDiretiva[5].string,"BEGIN");
    vetorDiretiva[5].operando = 0;
    strcpy(vetorDiretiva[6].string,"END");
    vetorDiretiva[6].operando = 0;
    strcpy(vetorDiretiva[7].string,"PUBLIC");
    vetorDiretiva[7].operando = 0;
    strcpy(vetorDiretiva[8].string,"EXTERN");
    vetorDiretiva[8].operando = 0;

    /* Inicializando as instruções como um vetor de struct permite que o código da instrução seja o indice+1*/
    /*Da mesma maneira o tamaho da instrução será o número de operandos + 1*/
    strcpy(vetorInst[0].string,"ADD");
    vetorInst[0].operando = 1;
    strcpy(vetorInst[1].string,"SUB");
    vetorInst[1].operando = 1;
    strcpy(vetorInst[2].string,"MULT");
    vetorInst[2].operando = 1;
    strcpy(vetorInst[3].string,"DIV");
    vetorInst[3].operando = 1;
    strcpy(vetorInst[4].string,"JMP");
    vetorInst[4].operando = 1;
    strcpy(vetorInst[5].string,"JMPN");
    vetorInst[5].operando = 1;
    strcpy(vetorInst[6].string,"JMPP");
    vetorInst[6].operando = 1;
    strcpy(vetorInst[7].string,"JMPZ");
    vetorInst[7].operando = 1;
    strcpy(vetorInst[8].string,"COPY");
    vetorInst[8].operando = 2;
    strcpy(vetorInst[9].string,"LOAD");
    vetorInst[9].operando = 1;
    strcpy(vetorInst[10].string,"STORE");
    vetorInst[10].operando = 1;
    strcpy(vetorInst[11].string,"INPUT");
    vetorInst[11].operando = 1;
    strcpy(vetorInst[12].string,"OUTPUT");
    vetorInst[12].operando = 1;
    strcpy(vetorInst[13].string,"STOP");
    vetorInst[13].operando = 0;
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
int PesquisaInstrucaoEDiretiva (std::string instrucaoOuDiretiva, std::vector<tabInstrucao> vetorInstOuDiretiva) {
    vector<tabInstrucao>::iterator it;
    it = find(vetorInstOuDiretiva.begin(), vetorInstOuDiretiva.end(), instrucaoOuDiretiva);
    if(it!= vetorInstOuDiretiva.end())
        return (int)distance(vetorInstOuDiretiva.begin(), it); // retorna a posicao da instrucao ou diretiva
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