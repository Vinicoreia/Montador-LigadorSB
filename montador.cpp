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
int flagModulo = 0;
int contadorDefinicoes = 0;
int contadorUso = 0;
int contadorSimbolos = 0;
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
void GeraTabelaInstrucoesEDiretivas(vector<tabInstrucaoOuDiretiva> tabInstrucao,
                                    vector<tabInstrucaoOuDiretiva> tabDiretiva) {
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
    size_t posicaoChar = checaCaracter.find_first_not_of(
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789+-*/=|&!?#%(){}[]_\'\",.:;<>\n ");
    if (posicaoChar != string::npos) {
        cout << "Erro lexico na linha " << numLinha << " caractere invalido" << endl;
        return posicaoChar;
    } else
        return 10000; // retorno se não existe caractere especial
}

// como vetor podemos usar a funcao find
int PesquisaInstrucaoEDiretiva(string instrOuDiretiva, vector<tabInstrucaoOuDiretiva> vetorInstOuDiretiva) {
    vector<tabInstrucaoOuDiretiva>::iterator it;
    /*O predicado abaixo é usado para a funcao find_if*/
    auto predicado = [instrOuDiretiva](const tabInstrucaoOuDiretiva &item) {
        return item.instrucaoOuDiretiva == instrOuDiretiva;
    };
    it = find_if(vetorInstOuDiretiva.begin(), vetorInstOuDiretiva.end(), predicado);
    if (it != vetorInstOuDiretiva.end())
        return (int) distance(vetorInstOuDiretiva.begin(), it); // retorna a posicao da instrucao ou diretiva
    else
        return -1;
}

// a pesquisa nao pode ser igual pois os tipos dos iteradores sao diferentes
int PesquisaSimbolo(string simbol, vector<tabSimbolos> vetorSimbolos) {
    vector<tabSimbolos>::iterator it;
    auto predicado = [simbol](const tabSimbolos &item) {
        return item.simbolo == simbol;
    };
    it = find_if(vetorSimbolos.begin(), vetorSimbolos.end(), predicado);
    if (it != vetorSimbolos.end())
        return (int) distance(vetorSimbolos.begin(), it); // retorna a posicao da instrucao ou diretiva
    else
        return -1;
}

bool ProcuraRotulo(string linha) {
    size_t posicaoChar = linha.find_first_of(":");
    return posicaoChar != string::npos;
}

void checaSeRotuloValido(string rotulo) {
    //  se o rotulo tem mais de 100 caracteres
    if (rotulo.size() > 100) {
        cout << "Erro Sintatico na linha " << numLinha << " Tamanho maximo permitido pro rotulo eh 100 caracteres";

    }
    // se rotulo comeca com numero
    if (rotulo.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == 0) {
        cout << "Erro Sintatico na linha " << numLinha
             << " Primeira letra do rotulo nao pode ser numero ou caractere especial";

    }
}

void PrimeiraPassagem(string linha, vector<tabSimbolos> vetorSimbolos, vector<tabDef> vetorDefinicoes,
                      vector<tabUso> tabelaUso) {
    int posicao = 0;
    int i = 0, j = 0, chartoint = 0;
    int retorno;
    int offset;
    char c;
    string proxtoken;
    string SPACE = "SPACE";
    string EXTERN = "EXTERN";
    string PUBLIC = "PUBLIC";
    string SECTION = "SECTION";
    vector<tabInstrucaoOuDiretiva> vetorInstrucao;
    vector<tabInstrucaoOuDiretiva> vetorDiretiva;
    flagErros = (int) VerificaSeLinhaValida(linha);
    GeraTabelaInstrucoesEDiretivas(vetorInstrucao, vetorDiretiva);

    if (ProcuraRotulo(linha)) {
        /*Caso ache um rótulo, verifica se ta na tabela de simbolos, se não tiver adicione*/
        string rotulo = linha.substr(0, linha.find_first_of(":") - 1); // pega a primeira palavra como rotulo
        checaSeRotuloValido(rotulo);

        if (PesquisaSimbolo(rotulo, vetorSimbolos) != -1) { // se eu procurar na tabela o simbolo e ele já estiver lá
            cout << "Erro Semantico na linha " << numLinha << " Simbolo " << rotulo << " redefinido";
            flagErros++;
        } else { // insere o simbolo na tabela de simbolos
            vetorSimbolos[contadorSimbolos].simbolo = rotulo;
            vetorSimbolos[contadorSimbolos].posicao = posicao; // posicao vai ter que ser global depois
            contadorSimbolos++;

            linha = linha.substr(rotulo.size() + 1, linha.size()); // retira rotulo da linha
            proxtoken = linha.substr(0, linha.find_first_of(" \n") - 1);
            linha = linha.substr(proxtoken.size() + 1, linha.size());

            retorno = PesquisaInstrucaoEDiretiva(proxtoken, vetorInstrucao);
            if (retorno != -1) { /*Se encontrar a instrucao */
                posicao = posicao + vetorInstrucao[retorno].operando;
            } else {//Se não encontrar instrucao
                retorno = PesquisaInstrucaoEDiretiva(proxtoken, vetorDiretiva);
                if (retorno != -1) { // Existe diretiva
                    vetorSimbolos[contadorSimbolos - 1].secdados = 0;
                    if (vetorDiretiva[retorno].instrucaoOuDiretiva == "EXTERN") {
                        vetorSimbolos[contadorSimbolos - 1].externo = 1;
                        vetorSimbolos[contadorSimbolos - 1].posicao = 0;
                        flagModulo = 1;
                    } else {
                        vetorSimbolos[contadorSimbolos - 1].externo = 0;
                        if (vetorDiretiva[retorno].instrucaoOuDiretiva == "SPACE") {
                            vetorSimbolos[contadorSimbolos - 1].secdados = 1;
                            if (!linha.empty()) {
                                proxtoken = linha.substr(0, linha.find_first_of(" \n") - 1);
                                linha = linha.substr(proxtoken.size() + 1, linha.size());

                                stringstream tok(proxtoken);
                                tok >> offset; // converte de string pra inteiro
                                posicao = posicao + offset;
                            } else
                                posicao = posicao + 1;
                        } else {
                            if (retorno >= 1 && retorno < 3) {
                                vetorSimbolos[contadorSimbolos - 1].secdados = 1;
                            }
                            posicao = posicao + vetorDiretiva[retorno].operando;
                        }
                    }
                } else {
                    cout << "Erro Sintatico na linha " << numLinha << " Diretiva ou Instrucao " << rotulo
                         << " invalida";
                    flagErros++;
                }
            }

        }
    } else {/*Caso não ache um rótulo*/
        proxtoken = linha.substr(0, linha.find_first_of(' \n') - 1);
        linha = linha.substr(proxtoken.size() + 1, linha.size());
        size_t posicaoChar;
        if (proxtoken == "SECTION") {
            //nao faz nada
        } else if (proxtoken == "PUBLIC") {
            flagModulo = 1; // existe outro modulo e devemos copiar o simbolo pra a TD
            proxtoken = linha.substr(linha.find_first_not_of(" "), linha.find_first_of(" \t") - 1);
            vetorDefinicoes[contadorDefinicoes].simbolo = proxtoken;
            contadorDefinicoes++;
        } else {
            retorno = PesquisaInstrucaoEDiretiva(proxtoken, vetorInstrucao);
            if (retorno != -1) {
                posicao = posicao + vetorInstrucao[retorno].operando + 1;
            } else {
                retorno = PesquisaInstrucaoEDiretiva(proxtoken, vetorDiretiva);
                if (retorno != -1) {
                    if (retorno != 6) {
                        cout << "Erro Sintatico na linha " << numLinha << " Diretiva " << proxtoken << " sem rotulo"
                             << endl;
                        flagErros ++;
                    }
                    if ((retorno >= 1) && (retorno <= 3)) {
                        cout << "Erro Semantico na linha " << numLinha << " Diretiva " << proxtoken << " fora da secao de dados"
                             << endl;
                        flagErros++;
                    }
                } else {

                    cout << "Erro Sintatico na linha " << numLinha << " Diretiva ou instrucao " << proxtoken << " nao identificada"
                         << endl;
                    flagErros++;

                }
                posicao = posicao + 1;
            }
        }
    }
}

int Monta(fstream &preprocessado) {
    //primeiro vou escrever a ideia
    //recebe o arquivo preprocessado;
    int contador_posicao = 0;
    int contador_linha = 1;
    string linha;
    vector<tabSimbolos> vetorSimbolos;
    vector<tabDef> vetorDef;
    vector<tabUso> vetorUso;

    numLinha = 1;
    while (getline(preprocessado, linha)) {

        VerificaSeLinhaValida(linha);
        if (!linha.empty()) {
            PrimeiraPassagem(linha, vetorSimbolos, vetorDef, vetorUso);
        }
        numLinha += 1;

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