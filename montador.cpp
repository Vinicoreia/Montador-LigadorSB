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
#include "montador.h"


/*---------------------------------------------------------------------------------------------
 *| O codigo abaixa representa um montador de duas passagens do assembly visto em sala de aula|
 *|___________________________________________________________________________________________|
 * */

//TODO printar as tabelas de definicao e USO quando for modulo

using namespace std;
int numLinha;
int flagErros = 0;
int flagModulo = 0;
int contadorDefinicoes = 0;
int contadorUso = 0;
int posicao;
int contadorSimbolos = 0;
string codigoObjeto;
vector<tabInstrucaoOuDiretiva> vetorInstrucao;
vector<tabInstrucaoOuDiretiva> vetorDiretiva;
vector<tabSimbolos> vetorSimbolos;
vector<tabDef> vetorDefinicoes;
vector<tabUso> vetorUso;

/*
 * Como o número de instruções e diretivas é pequeno resolvi criar a tabela ao inves de ler a tabela de um arquivo.
 * */

void GeraTabelaInstrucoesEDiretivas() {
    /*
     * VETOR DE DIRETIVAS
     * */
    vetorDiretiva.push_back({"SECTION", 0});
    vetorDiretiva.push_back({"SPACE", 1});
    vetorDiretiva.push_back({"CONST", 1});
    vetorDiretiva.push_back({"EQU", 1}); //o valor aqui nao importa ja que EQU e IF sao tratados no preprocessamento
    vetorDiretiva.push_back({"IF", 1});
    vetorDiretiva.push_back({"BEGIN", 0});
    vetorDiretiva.push_back({"END", 0});
    vetorDiretiva.push_back({"PUBLIC", 0});
    vetorDiretiva.push_back({"EXTERN", 0});

    /*
     * Inicializando as instruções como um vetor de struct permite que o código da instrução seja o indice+1
     * Da mesma maneira o tamaho da instrução será o número de operandos + 1
    */
    vetorInstrucao.push_back({"ADD", 1});
    vetorInstrucao.push_back({"SUB", 1});
    vetorInstrucao.push_back({"MULT", 1});
    vetorInstrucao.push_back({"DIV", 1});
    vetorInstrucao.push_back({"JMP", 1});
    vetorInstrucao.push_back({"JMPN", 1});
    vetorInstrucao.push_back({"JMPP", 1});
    vetorInstrucao.push_back({"JMPZ", 1});
    vetorInstrucao.push_back({"COPY", 1});
    vetorInstrucao.push_back({"LOAD", 1});
    vetorInstrucao.push_back({"STORE", 1});
    vetorInstrucao.push_back({"INPUT", 1});
    vetorInstrucao.push_back({"OUTPUT", 1});
    vetorInstrucao.push_back({"STOP", 0});
}


/*
 * Essa funcao verifica se existe simbolo especial na linha
 *
 * */
size_t VerificaSeLinhaValida(string checaCaracter) {
    /*Lembrar que aqui a linha ja não é sensível ao caso
     *
     */
    size_t posicaoChar = checaCaracter.find_first_not_of(
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-*/=|&!?#%(){}[]_\'\",.:;<>\n ");
    if (posicaoChar != string::npos) {
        cout << "\nErro lexico na linha " << numLinha << " caractere " << posicaoChar - 1 << " invalido\n";
        flagErros++; // retorno se não existe caractere especial
    }
}

/*
 * Essa funcao pesquisa se existe instrucao ou diretiva na tabela de instrucao ou de diretivas
 *      retorna -1 caso nao exista
 *      retorna a posicao caso exista
 * */
int PesquisaInstrucaoEDiretiva(string instrOuDiretiva, vector<tabInstrucaoOuDiretiva> vetorInstOuDiretiva) {
    vector<tabInstrucaoOuDiretiva>::iterator it;

    /*O predicado abaixo é usado para a funcao find_if*/
    auto predicado = [&instrOuDiretiva](const tabInstrucaoOuDiretiva &item) {
        return item.instrucaoOuDiretiva == instrOuDiretiva;
    };
    it = find_if(vetorInstOuDiretiva.begin(), vetorInstOuDiretiva.end(), predicado);
    if (it != vetorInstOuDiretiva.end())
        return (int) distance(vetorInstOuDiretiva.begin(), it); // retorna a posicao da instrucao ou diretiva
    else
        return -1;
}


/*
 * Essa funcao Pesquisa se existe simbolo na tabela de simbolos
 *      retorna -1 caso nao exista
 *      retorna a posicao caso exista
 * */
int PesquisaSimbolo(string simbol) {
    vector<tabSimbolos>::iterator it;
    auto predicado = [&simbol](tabSimbolos &item) {
        return item.simbolo == simbol;
    };

    it = find_if(vetorSimbolos.begin(), vetorSimbolos.end(), predicado);
    if (it != vetorSimbolos.end())
        return (int) distance(vetorSimbolos.begin(), it); // retorna a posicao do simbolo
    else
        return -1;
}

/*
 * Essa funcao retorna 1 se existe rotulo na linha
 * */
bool ProcuraRotulo(string linha) {
    size_t posicaoChar = linha.find_first_of(":");
    return posicaoChar != string::npos;
}

/*
 * Essa funcao checa se o rotulo nao possui caracter especial nem tamanho maior que 100 caracteres
 * */
void ChecaSeRotuloValido(string rotulo) {
    //  se o rotulo tem mais de 100 caracteres
    if (rotulo.size() > 100) {
        cout << "\nErro Sintatico na linha " << numLinha << " Tamanho maximo permitido pro rotulo eh 100 caracteres\n";

    }
    // se rotulo comeca com numero
    if (rotulo.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == 0) {
        cout << "\nErro Sintatico na linha " << numLinha
             << " Primeira letra do rotulo nao pode ser numero ou caractere especial\n";

    }
}

/*
 * Essa funcao cria e atualiza a tabela de simbolos.
 * */
void PrimeiraPassagem(string linha) {
    int retorno;
    int offset;
    string proxtoken;
    VerificaSeLinhaValida(linha);
    if (ProcuraRotulo(linha)) {
        linha.append("\n");
        /*Caso ache um rótulo, verifica se ta na tabela de simbolos, se não tiver adicione*/
        string rotulo = linha.substr(0, linha.find_first_of(":")); // pega a primeira palavra como rotulo
        linha = linha.substr(rotulo.size() + 2, linha.size()); // retira rotulo da linha e pula o : e o ' '
        ChecaSeRotuloValido(rotulo);
        if (PesquisaSimbolo(rotulo) != -1) { // se eu procurar na tabela o simbolo e ele já estiver lá
            cout << "\nErro Semantico na linha " << numLinha << " Simbolo " << rotulo << " redefinido\n";
            flagErros++;
        } else { // insere o simbolo na tabela de simbolos
            vetorSimbolos.push_back({rotulo, posicao});
            contadorSimbolos++;
            cout<< posicao;
            if (linha.find_first_of(" \n") != string::npos) {
                proxtoken = linha.substr(0, linha.find_first_of(" \n"));
                linha = linha.substr(proxtoken.size() + 1, linha.size() - proxtoken.size() - 1);

            } else {
                proxtoken = linha.substr(0, linha.find_first_of("\n"));
                linha = linha.substr(proxtoken.size() + 1, linha.size() - proxtoken.size() - 1);

            }
            /*VERIFICA SE INSTRUCAO EH VALIDA*/
            retorno = PesquisaInstrucaoEDiretiva(proxtoken, vetorInstrucao);
            if (retorno != -1) {
                posicao = posicao + vetorInstrucao[retorno].operando + 1;
            } else {
                retorno = PesquisaInstrucaoEDiretiva(proxtoken, vetorDiretiva);
                if (retorno != -1) { /*Se encontrar a diretiva */
                    vetorSimbolos[contadorSimbolos - 1].secdados = 0;
                    if (vetorDiretiva[retorno].instrucaoOuDiretiva == "EXTERN") {
                        vetorSimbolos[contadorSimbolos - 1].externo = 1;
                        vetorSimbolos[contadorSimbolos - 1].posicao = 0;
                        flagModulo = 1;
                    } else {//Se a diretiva nao for extern
                        vetorSimbolos[contadorSimbolos - 1].externo = 0;
                        if (vetorDiretiva[retorno].instrucaoOuDiretiva == "SPACE") {
                            vetorSimbolos[contadorSimbolos - 1].secdados = 1;
                            if (!linha.empty()) {
                                proxtoken = linha.substr(0, linha.find_first_of(" \n"));
                                linha = linha.substr(proxtoken.size() + 1, linha.size());
                                stringstream tok(proxtoken);
                                tok >> offset; // converte de string pra inteiro
                                posicao = posicao + offset; // offset é quanto espaço é reservado pela diretiva
                            } else
                                posicao++;
                        } else {
                            if (retorno >= 1 && retorno < 3) {
                                vetorSimbolos[contadorSimbolos - 1].secdados = 1;
                            }
                            posicao = posicao + vetorDiretiva[retorno].operando;
                        }
                    }
                } else {
                    cout << "\nErro Sintatico na linha " << numLinha << " Diretiva ou Instrucao " << proxtoken
                         << " invalida\n";
                    flagErros++;
                }
            }

        }
    } else {
        proxtoken = linha.substr(linha.find_first_not_of(" \n"), linha.find_first_of(" \n"));
        linha = linha.substr(proxtoken.size(), linha.size());
        if (proxtoken == "SECTION") {
        } else if (proxtoken == "PUBLIC") {
            flagModulo = 1; // existe outro modulo e devemos copiar o simbolo pra a TD
            proxtoken = linha.substr(linha.find_first_not_of(" \n"), linha.find_first_of(" \n") - 1);
            vetorDefinicoes.push_back({proxtoken});
        } else {
            retorno = PesquisaInstrucaoEDiretiva(proxtoken, vetorInstrucao);
            if (retorno != -1) {
                posicao = posicao + vetorInstrucao[retorno].operando + 1;
            } else {
                retorno = PesquisaInstrucaoEDiretiva(proxtoken, vetorDiretiva);
                if (retorno != -1) {
                    if (retorno != 6) {
                        cout << "\nErro Sintatico na linha " << numLinha << " Diretiva " << proxtoken
                             << " sem rotulo\n";
                        flagErros++;
                    }
                    if ((retorno >= 1) && (retorno <= 3)) {
                        cout << "\nErro Semantico na linha " << numLinha << " Diretiva " << proxtoken
                             << " fora da secao de dados\n";
                        flagErros++;
                    }
                } else {

                    cout << "\nErro Sintatico na linha " << numLinha << " Diretiva ou instrucao " << proxtoken
                         << " nao identificada\n";
                    flagErros++;

                }
                posicao++;
            }
        }
    }
}

/*
 * Essa Funcao copia as posicoes da tabela de simbolos para a tabela de definicoes caso exista mais de um modulo.
 * */
void AtualizaTabelaDeDefinicao() {

    string simboloProcurado;
    vector<tabDef>::iterator itDef;
    vector<tabSimbolos>::iterator itSimb;
    int indice;
    if (flagModulo == 1) {

        for (int i = 0; i < vetorDefinicoes.size(); i++) {
            simboloProcurado = vetorDefinicoes[i].simbolo; // procura esse simbolo na tabela de Simbolos.
            auto predicado = [simboloProcurado](const tabSimbolos &item) {
                return item.simbolo == simboloProcurado;
            };
            itSimb = find_if(begin(vetorSimbolos), end(vetorSimbolos), predicado);
            if (itSimb != end(vetorSimbolos)) {
                /*Atualiza a tabela de definicoes com as novas posicoes*/
                indice = (int) distance(vetorSimbolos.begin(), itSimb);
                vetorDefinicoes[i].posicao = vetorSimbolos[indice].posicao;
            } else {
                flagErros++;
                cout << "\nErro Semantico na linha " << numLinha << "Simbolo " << simboloProcurado
                     << " nao declarado\n";
            }

        }
    }
}

/*
 * Essa funcao escreve o codigo objeto de acordo com os opcodes e os argumentos no arquivo preprocessado
 * 
 * */
void AdicionaCodigoObjeto(int diretiva, int flagInstrucao) {
    string aux, aux2;
    stringstream diretivaString;
    diretivaString << diretiva;
    aux.append(diretivaString.str());
    if ((flagInstrucao == 1) && (aux.size() == 1)) {
        aux2[0] = '0';
        aux2[1] = '\0';
        aux2.append(aux);
        aux = aux2;
    }

    codigoObjeto.append(aux + " ");
}

/*
 * Essa funcao verifica se o vetor esta correto retornando por referencia o numero de espacos
 * */
void VerificaVetor(string token, int *espacos, int *flagVetorErrado) {
    string aux, aux2, linha;
    aux = token;
    aux.append("+");
    aux2[0] = '\0';
    aux2 = linha.substr(0, linha.find_first_not_of(
            " \n+"));// aux 3 deve ser o proximo token lembrando que aqui a string nao possui o +
    if (aux2[aux2.size()] < 48 || aux2[aux2.size()] > 57) {
        *flagVetorErrado = 1;

    } else {
        aux.append(aux2);
        stringstream(aux2) >> *espacos;
    }
}

/*
 * Essa funcao checa se o formato da instrucao esta correta e se existe pulo pra secao de dados
 * */
void ChecaArgumentos(int flagMemoria, int retorno, int numLinha) {
    if (flagMemoria == 1) {
        if (vetorSimbolos[retorno].secdados != 1) {
            flagErros++;
            cout << "\nErro Semantico na linha " << numLinha << " Argumento invalido\n";
        }
    } else if (flagMemoria == 2) {// JUMP TENTANDO ACESSAR MEM DE DADOS
        if (vetorSimbolos[retorno].secdados != 0) {
            flagErros++;
            cout << "\nErro Semantico na linha " << numLinha << " Pulo para a secao de dados nao eh permitido\n";
        }
    }
}

/*
 * Essa funcao executa a segunda passagem do montador de duas passagens, tendo em conta que a tabela de simbolos
 * foi feita na primeira passagem.
 * */
void SegundaPassagem(fstream &preprocessado) {
    string linha;
    string proxtoken;
    string rotulo;
    int retorno;
    int flagBegin = 0;
    int flagEnd = 0;
    int flagInstrucao;
    int flagCode = 0;
    int flagData = 0;
    int flagStop = 0;
    int checarDIV[30];
    int k = 0, i = 0, j = 0, l = 0;
    int espacos;
    int flagMemoria = 0;
    int flagVetorErrado = 0, flagConst = 0;
    int numerosDIV[20];
    int checarMEM[30], espacosMEM[30], posicaoMEM[30];
    int constAlterada[50];
    int posicao = 0;
    numLinha = 1;

    while (getline(preprocessado, linha)) {
        flagMemoria = 0;
        flagVetorErrado = 0;
        flagInstrucao = 0;
        flagConst = 0;
        espacos = 0;
        if (ProcuraRotulo(linha)) {
            rotulo = linha.substr(0, linha.find_first_of(":"));
            linha = linha.substr(rotulo.size() + 2, linha.size()); // retira rotulo da linha e pula o : e o ' '
        }
        linha.append("\n");
        proxtoken = linha.substr(0, linha.find_first_of(" \n"));
        linha = linha.substr(proxtoken.size() + 1, linha.size());
        retorno = PesquisaInstrucaoEDiretiva(proxtoken, vetorInstrucao);
        if (retorno != -1) {
            flagInstrucao = 1;
            if (flagCode == 0) {
                flagErros++;
                cout << "\nErro Semantico na linha " << numLinha << " instrucao deve estar na SECTION TEXT\n";

            }
            if (flagData == 0) {
                /*copia instrucao no codigo objeto*/
                AdicionaCodigoObjeto(retorno + 1, flagInstrucao);
                checarDIV[k] = -1;/*Para verificar divisao por 0*/
                if (retorno == 3) {
                    checarDIV[k] = numLinha;
                }
                if ((retorno <= 3 || retorno >= 8) && (retorno != 13)) {
                    flagMemoria = 1;
                } else if (retorno != 13)
                    flagMemoria = 2;
                if (retorno == 8 || retorno == 10 || retorno == 11) {
                    flagConst = 1;
                }
                posicao++; /*Incrementa contador de posicao para instrucao*/
                flagInstrucao = 0;

                if (vetorInstrucao[retorno].operando == 1) {
                    proxtoken = linha.substr(0, linha.find_first_of(" \n+-,"));// pega o token antes de , ou + ou espaço
                    linha = linha.substr(proxtoken.size(), linha.size() - proxtoken.size());
                    retorno = PesquisaSimbolo(proxtoken);
                    if (!proxtoken.empty()) {

                        if (retorno != -1) {
                            if (linha.at(0) == '+') {
                                linha = linha.substr(1, linha.size() - 1);
                                flagVetorErrado = -1;
                                // chama uma funcao pra verificar se o vetor esta correto e corrigir
                                VerificaVetor(proxtoken, &espacos, &flagVetorErrado);
                            }

                            if (flagVetorErrado != 1) {
                                /*se for variavel externa usada, coloca na tabela de uso*/
                                if (vetorSimbolos[retorno].externo == 1) {
                                    vetorUso.push_back({vetorSimbolos[retorno].simbolo, posicao});
                                    contadorUso++;
                                    if (espacos == 1) {
                                        k = 0;
                                        AdicionaCodigoObjeto(0, flagInstrucao);
                                    } else {
                                        AdicionaCodigoObjeto(espacos, flagInstrucao);
                                    }
                                    posicao++;
                                } else {
                                    /*Checa se os argumentos sao dados*/
                                    ChecaArgumentos(flagMemoria, retorno, numLinha);
                                    if (espacos == 1) {
                                        k = 0;
                                        AdicionaCodigoObjeto(espacos, flagInstrucao);
                                    } else {
                                        AdicionaCodigoObjeto(vetorSimbolos[retorno].posicao + espacos,
                                                             flagInstrucao);
                                    }
                                    posicao++;

                                    espacosMEM[l] = espacos;
                                    posicaoMEM[l] = vetorSimbolos[retorno].posicao;
                                    checarMEM[l] = numLinha;
                                    constAlterada[l] = flagConst;
                                    l++;
                                    if (checarDIV[k] != -1) {
                                        numerosDIV[k] = vetorSimbolos[retorno].posicao;
                                        k++;
                                    }

                                }
                            } else {
                                flagErros++;
                                cout << "\nErro Sintatico na linha " << numLinha
                                     << " vetor declarado incorretamente\n";
                            }
                        } else {
                            flagErros++;
                            posicao++;
                            cout << "\nErro Semantico na linha " << numLinha << " Simbolo " << proxtoken
                                 << " indefinido\n";
                        }
                    }
                } else if (vetorInstrucao[retorno].operando == 2) {
                    proxtoken = linha.substr(0, linha.find_first_of(", \n"));
                    linha = linha.substr(proxtoken.size() + 1, linha.size() - proxtoken.size() - 1);
                    retorno = PesquisaSimbolo(proxtoken);

                    if (retorno != -1) {

                        if (vetorSimbolos[retorno].externo == 1) {
                            vetorUso.push_back({vetorSimbolos[retorno].simbolo, posicao});
                            AdicionaCodigoObjeto(0, flagInstrucao);
                            posicao++;
                        } else {
                            ChecaArgumentos(flagMemoria, retorno, numLinha);
                            if (espacos == 1) {
                                k = 0;
                            }
                            AdicionaCodigoObjeto(vetorSimbolos[retorno].posicao + k, flagInstrucao);
                            posicao++;
                            espacosMEM[l] = espacos;
                            posicaoMEM[l] = vetorSimbolos[retorno].posicao;
                            checarMEM[l] = numLinha;
                            constAlterada[l] = flagConst;
                            l++;
                        }
                    } else {
                        flagErros++;
                        posicao++;
                        cout << "\nErro Semantico na linha " << numLinha << " Simbolo " << proxtoken
                             << " indefinido\n";
                    }
                    if (linha[0] == ',') {//COPY
                        linha = linha.substr(1, linha.size() - 1);
                        flagConst = 0;

                        proxtoken = linha.substr(0, linha.find_first_of(" \n+-"));
                        retorno = PesquisaSimbolo(proxtoken);
                        if (retorno != -1) {
                            if (vetorSimbolos[retorno].externo == 1) {
                                vetorUso.push_back({vetorSimbolos[retorno].simbolo, posicao});
                                AdicionaCodigoObjeto(0, flagInstrucao);
                                posicao++;
                            } else {
                                ChecaArgumentos(flagMemoria, retorno, numLinha);
                                if (espacos == 1) {
                                    k = 0;
                                }
                                AdicionaCodigoObjeto(vetorSimbolos[retorno].posicao + k, flagInstrucao);
                                posicao++;
                                espacosMEM[l] = espacos;
                                posicaoMEM[l] = vetorSimbolos[retorno].posicao;
                                checarMEM[l] = numLinha;
                                constAlterada[l] = flagConst;
                                l++;
                            }
                        } else {
                            flagErros++;
                            cout << "\nErro Semantico na linha " << numLinha << " Simbolo " << proxtoken
                                 << " indefinido\n";
                        }
                    } else {
                        flagErros++;
                        cout << "\nErro Sintatico na linha " << numLinha << " formato da instrucao COPY errado\n";
                    }
                } else if (retorno == 13) {// STOP
                    flagStop++;
                    if (!linha.empty()) {
                        proxtoken = linha.substr(0, linha.find_first_of(" \n"));
                        if (proxtoken.size() > 0) {
                            flagErros++;
                            cout << "\nErro Sintatico na linha " << numLinha
                                 << " Instrucao STOP nao possui operandos\n"
                                 << endl;
                        }
                    }
                }
                if (!linha.empty() && retorno != 13 && flagVetorErrado == 1) {
                    flagErros;
                    cout << "\nErro Sintatico na linha " << numLinha << " numero de operandos incorreto\n";
                }
            }
        } else {
            retorno = PesquisaInstrucaoEDiretiva(proxtoken, vetorDiretiva);
            if (retorno != -1) {
                if (retorno >= 7 && retorno <= 8) {
                    if (flagCode == 0) {
                        flagErros++;
                        cout << "\nErro Semantico na linha " << numLinha << " diretiva fora da SECTION TEXT\n"
                             << endl;
                    }
                } else {
                    switch (retorno) {
                        case 0:
                            proxtoken = linha.substr(0, linha.find_first_of(" \n"));
                            if (proxtoken == "DATA") {
                                if (flagCode == 0) {
                                    cout << "\nErro Semantico na linha " << numLinha
                                         << " SECTION DATA deve ser declarada depois da SECTION TEXT\n";
                                    flagData = 1;
                                    flagErros++;
                                } else if (flagData == 0) {
                                    flagCode = 2;
                                    flagData = 1;
                                } else {
                                    flagErros++;
                                    cout << "\nErro Semantico na linha " << numLinha
                                         << " SECTION DATA deve ser declarada depois da SECTION TEXT\n";
                                }
                            } else if (proxtoken == "TEXT") {
                                if (flagCode == 0) {
                                    flagCode = 1;
                                } else {
                                    flagErros++;
                                    cout << "\nErro Semantico na linha " << numLinha
                                         << " multiplas declaracoes da SECTION TEXT\n";

                                }
                            } else {
                                flagErros++;
                                cout << "\nErro Semantico na linha " << numLinha
                                     << " Diretiva SECTION com parametro desconhecido\n";
                            }
                            break;
                        case 1:
                            /*SPACE*/
                            if (!linha.empty()) {
                                proxtoken = linha.substr(0, linha.find_first_of(" \n"));
                                stringstream(proxtoken) >> espacos;
                                j = 0;
                                for (i = 0; i < l; i++) {
                                    retorno = 0;
                                    while ((j <= l) && (retorno == 0)) {
                                        if (posicao == posicaoMEM[j]) {
                                            retorno = 1;
                                        }
                                        j++;
                                    }
                                    if ((espacos <= espacosMEM[j - 1]) && retorno == 1) {
                                        flagErros++;
                                        cout << "\nErro Semantico na linha " << numLinha
                                             << " memoria nao reservada\n";
                                    }
                                }
                                posicao = posicao + espacos;
                                /* Reserva espacos necessarios pro vetor*/
                                for (j = 0; j < espacos; j++) {
                                    AdicionaCodigoObjeto(0, flagInstrucao);
                                }
                            } else {
                                AdicionaCodigoObjeto(0, flagInstrucao);
                                posicao++;
                            }
                            if (flagData != 1) {
                                flagErros++;
                                cout << "\nErro Semantico na linha " << numLinha
                                     << " diretiva SPACE fora da SECTION DATA\n";
                            }
                            break;
                        case 2:
                            /*CONST*/
                            if (!linha.empty()) {
                                proxtoken = linha.substr(0, linha.find_first_of(" \n+-"));
                                if (proxtoken[0] == 0 && proxtoken[1] == 'X') {
                                    stringstream(proxtoken) >> std::hex >> espacos;
                                } else {
                                    stringstream(proxtoken) >> espacos;
                                }
                                j = 0;
                                retorno = 0;

                                /*se dividiu por 0*/
                                for (i = 0; i < k; i++) {
                                    retorno = 0;
                                    while ((j <= k) && (retorno == 0)) {
                                        if (posicao == numerosDIV[j]) {
                                            retorno = 1;
                                        }

                                        j++;
                                    }

                                    if (retorno == 1 && espacos == 0) {
                                        flagErros++;
                                        cout << "\nErro Semantico na linha " << numLinha
                                             << " divisao por zero\n";
                                    }
                                }
                                j = 0;
                                /* se tentou alterar constante*/
                                for (i = 0; i < l; i++) {
                                    retorno = 0;
                                    while ((j <= l) && (retorno == 0)) {
                                        if (posicao == posicaoMEM[j]) {
                                            retorno = 1;
                                        }
                                        j++;
                                    }
                                    if (retorno == 1 && constAlterada[j - 1] != 0) {
                                        flagErros++;
                                        cout << "\nErro Semantico na linha " << numLinha
                                             << " impossivel alterar valor de constante\n";
                                    }
                                }
                                posicao++;
                                AdicionaCodigoObjeto(espacos, flagInstrucao);
                            } else {
                                flagErros++;
                                cout << "\nErro Sintatico na linha " << numLinha
                                     << " constante nao iniciada\n";
                            }
                            break;
                        case 5:
                            /*BEGIN*/
                            flagBegin++;
                            flagStop++; // Se for modulo nao precisa ter STOP
                            break;
                        case 6:
                            /*END*/
                            flagEnd++;
                            flagStop++;
                            break;
                            //TODO: se nao for modulo deve ter ao menos uma instrucao STOP
                            // TODO: Fazer o ligador e testar
                    }

                }
            }
        }
        numLinha++;
    }
    if (flagStop == 0) {
        flagErros++;
        cout << "\nErro Semantico falta diretiva STOP\n";
    }
    if (flagBegin != flagEnd) {
        flagErros++;
        cout << "\nErro Semantico na linha " << numLinha
             << " Numero de diretivas BEGIN diferente do numero de diretivas END";
    }
    if (flagCode == 0) {
        flagErros++;
        cout << "\nErro Semantico na linha " << numLinha << " SECTION TEXT nao definida\n";
    }
}


int Monta(fstream &preprocessado, string nomeArquivoSaida) {
    posicao = 0;
    string linha;
    GeraTabelaInstrucoesEDiretivas();
    numLinha = 1;
    while (getline(preprocessado, linha)) {

        VerificaSeLinhaValida(linha);
        if (!linha.empty()) {
            PrimeiraPassagem(linha);
        }
        numLinha += 1;
    }
    AtualizaTabelaDeDefinicao();
    preprocessado.clear();
    preprocessado.seekg(0, ios::beg);
    SegundaPassagem(preprocessado);

    if (flagErros == 0) {
        nomeArquivoSaida.append(".o");
        fstream arquivoObjeto(nomeArquivoSaida, fstream::in | fstream::out | fstream::trunc);
        codigoObjeto.pop_back();
        cout << endl << "Codigo Objeto em memoria: " << codigoObjeto;
        if (arquivoObjeto.is_open()) {
            if (flagModulo != 0) {
                arquivoObjeto << "TABLE USE\n";
                for (int i = 0; i < vetorUso.size(); i++) {
                    arquivoObjeto << vetorUso[i].simbolo << " " << vetorUso[i].posicao << endl;
                }
                arquivoObjeto << "\nTABLE DEFINITION\n";
                for (int i = 0; i < vetorDefinicoes.size(); i++) {
                    arquivoObjeto << vetorDefinicoes[i].simbolo << " " << vetorDefinicoes[i].posicao << "\n";
                }
                arquivoObjeto << "\nCODE\n";
                arquivoObjeto << codigoObjeto<<endl;

                arquivoObjeto.close();
            }
        }
    }
}