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

int main_montador (){
    //primeiro vou escrever a ideia
    //recebe o arquivo preprocessado;
    fstream assembly;
    int contador_posicao = 0;
    int contador_linha = 0;
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