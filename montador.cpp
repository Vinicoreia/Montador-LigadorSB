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
int numLinha;
int flagErros = -1;
typedef struct {
    string instrucaoOuDiretiva;
    int operando;
} tabInstrucaoOuDiretiva;

typedef struct {
    string simbolo;
    int posicao;
    int externo; /*0 ou 1*/
    int secdados;
} tabSimbolos;

typedef struct {
    string simbolo;
    int posicao;
} tabUso;

typedef struct {
    string simbolo;
    int posicao;
} tabDef;

/*
 * Como o número de instruções e diretivas é pequeno resolvemos criar a tabela ao invés de ler a tabela de um arquivo.
 * */
void GeraTabelaInstrucoesEDiretivas(std::vector<tabInstrucaoOuDiretiva> tabInstrucao,
                                    std::vector<tabInstrucaoOuDiretiva> tabDiretiva) {
    /*
     * VETOR DE DIRETIVAS
     * */
    tabInstrucao[0].instrucaoOuDiretiva = "SECTION";
    tabDiretiva[0].operando = 0;
    tabInstrucao[1].instrucaoOuDiretiva = "SPACE";
    tabDiretiva[1].operando = 1;
    tabDiretiva[2].instrucaoOuDiretiva = "CONST";
    tabDiretiva[2].operando = 1;
    tabDiretiva[3].instrucaoOuDiretiva = "EQU";
    tabDiretiva[3].operando = 1;
    tabDiretiva[4].instrucaoOuDiretiva = "IF";
    tabDiretiva[4].operando = 1;
    tabDiretiva[5].instrucaoOuDiretiva = "BEGIN";
    tabDiretiva[5].operando = 0;
    tabDiretiva[6].instrucaoOuDiretiva = "END";
    tabDiretiva[6].operando = 0;
    tabDiretiva[7].instrucaoOuDiretiva = "PUBLIC";
    tabDiretiva[7].operando = 0;
    tabDiretiva[8].instrucaoOuDiretiva = "EXTERN";
    tabDiretiva[8].operando = 0;

    /* Inicializando as instruções como um vetor de struct permite que o código da instrução seja o indice+1*/
    /*Da mesma maneira o tamaho da instrução será o número de operandos + 1*/
    tabInstrucao[0].instrucaoOuDiretiva = "ADD";
    tabInstrucao[0].operando = 1;
    tabInstrucao[1].instrucaoOuDiretiva = "SUB";
    tabInstrucao[1].operando = 1;
    tabInstrucao[2].instrucaoOuDiretiva = "MULT";
    tabInstrucao[2].operando = 1;
    tabInstrucao[3].instrucaoOuDiretiva = "DIV";
    tabInstrucao[3].operando = 1;
    tabInstrucao[4].instrucaoOuDiretiva = "JMP";
    tabInstrucao[4].operando = 1;
    tabInstrucao[5].instrucaoOuDiretiva = "JMPN";
    tabInstrucao[5].operando = 1;
    tabInstrucao[6].instrucaoOuDiretiva = "JMPP";
    tabInstrucao[6].operando = 1;
    tabInstrucao[7].instrucaoOuDiretiva = "JMPZ";
    tabInstrucao[7].operando = 1;
    tabInstrucao[8].instrucaoOuDiretiva = "COPY";
    tabInstrucao[8].operando = 2;
    tabInstrucao[9].instrucaoOuDiretiva = "LOAD";
    tabInstrucao[9].operando = 1;
    tabInstrucao[10].instrucaoOuDiretiva = "STORE";
    tabInstrucao[10].operando = 1;
    tabInstrucao[11].instrucaoOuDiretiva = "INPUT";
    tabInstrucao[11].operando = 1;
    tabInstrucao[12].instrucaoOuDiretiva = "OUTPUT";
    tabInstrucao[12].operando = 1;
    tabInstrucao[13].instrucaoOuDiretiva = "STOP";
    tabInstrucao[13].operando = 0;
}

size_t VerificaSeLinhaValida(string checaCaracter) {
    /*Lembrar que aqui a linha ja não é sensível ao caso
     *
     */
    size_t posicao = checaCaracter.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789+-*/=|&!?#%(){}[]_\'\",.;<>");
    if (posicao != string::npos) {
        cout << "Erro lexico na linha " << numLinha << " caractere invalido";
        return posicao;
    } else
        return 10000; // retorno se não existe caractere especial
}

// como vetor podemos usar a funcao find
int
PesquisaInstrucaoEDiretiva(std::string instrucaoOuDiretiva, std::vector<tabInstrucaoOuDiretiva> vetorInstOuDiretiva) {
    vector<tabInstrucaoOuDiretiva>::iterator it;
    it = find(vetorInstOuDiretiva.begin(), vetorInstOuDiretiva.end(), instrucaoOuDiretiva);
    if (it != vetorInstOuDiretiva.end())
        return (int) distance(vetorInstOuDiretiva.begin(), it); // retorna a posicao da instrucao ou diretiva
    else
        return -1;
}

// a pesquisa nao pode ser igual pois os tipos dos iteradores sao diferentes
int PesquisaSimbolo(std::string simbolo, std::vector<tabSimbolos> vetorSimbolos) {
    vector<tabSimbolos>::iterator it;
    it = find(vetorSimbolos.begin(), vetorSimbolos.end(), simbolo);
    if (it != vetorSimbolos.end())
        return (int) distance(vetorSimbolos.begin(), it); // retorna a posicao da instrucao ou diretiva
    else
        return -1;
}

bool ProcuraRotulo(string linha) {
    size_t posicao = linha.find_first_of(":");
    return posicao != string::npos;
}

void checaSeRotuloValido(string linha) {
    //  se o rotulo tem mais de 100 caracteres
    if (linha.size() > 100) {
        cout << "Erro Sintatico na linha " << numLinha << " Primeira letra do rotulo nao pode ser numero";
    }
    // se rotulo comeca com numero
    if (linha.find_first_of("0123456789") == 0) {
        cout << "Erro Sintatico na linha " << numLinha << " Tamanho maximo permitido pro rotulo eh 100 caracteres";
    }
}

void PrimeiraPassagem(string linha, vector<tabSimbolos> vetorSimbolos, vector<tabDef> vetorDefinicoes,
                      vector<tabUso> tabelaUso) {
    int i = 0, j = 0, chartoint = 0;
    int retorno;
    char c;
    string aux;
    string SPACE = "SPACE";
    string EXTERN = "EXTERN";
    string PUBLIC = "PUBLIC";
    string SECTION = "SECTION";
    vector<tabInstrucaoOuDiretiva> vetorInstrucao;
    vector<tabInstrucaoOuDiretiva> vetorDiretiva;
    flagErros = (int) VerificaSeLinhaValida(linha);
    GeraTabelaInstrucoesEDiretivas(vetorInstrucao, vetorDiretiva);

    if (ProcuraRotulo(linha)) {
        istringstream is(linha);
        string rotulo;
        is >> rotulo; // passa apenas a primeira palavra pra string acima

        checaSeRotuloValido(rotulo);


    }

    //primeiramente vamos achar os rotulos

}


int main_montador() {
    //primeiro vou escrever a ideia
    //recebe o arquivo preprocessado;
    fstream assembly;
    int contador_posicao = 0;
    int contador_linha = 1;
    /*vector<tabSimbolos> vetorSimbolos;
    vector<tabDef> vetorDef;
    vector<tabUso> vetorUso;
    char c;
    char string[1000], codigoObjeto[1000], TU[1000], TD[1000];
    int i, buff = 1;*/
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
        contador_linha++;

    }
}