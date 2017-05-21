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

using namespace std;
int numLinha;
int flagErros = -1;
int flagModulo = 0;
int contadorDefinicoes = 0;
int contadorUso = 0;
int contadorSimbolos = 0;
string codigoObjeto;
vector<tabInstrucaoOuDiretiva> vetorInstrucao;
vector<tabInstrucaoOuDiretiva> vetorDiretiva;
vector<tabSimbolos> vetorSimbolos;
vector<tabDef> vetorDefinicoes;
vector<tabUso> vetorUso;

/*
 * Como o número de instruções e diretivas é pequeno resolvemos criar a tabela ao invés de ler a tabela de um arquivo.
 * */
void GeraTabelaInstrucoesEDiretivas() {
    /*
     * VETOR DE DIRETIVAS
//     * */
    vetorDiretiva.push_back({"SECTION", 0});
    vetorDiretiva.push_back({"SPACE", 1});
    vetorDiretiva.push_back({"CONST", 1});
    vetorDiretiva.push_back({"EQU", 1});
    vetorDiretiva.push_back({"IF", 1});
    vetorDiretiva.push_back({"BEGIN", 0});
    vetorDiretiva.push_back({"END", 0});
    vetorDiretiva.push_back({"PUBLIC", 0});
    vetorDiretiva.push_back({"EXTERN", 0});

//    /* Inicializando as instruções como um vetor de struct permite que o código da instrução seja o indice+1*/
//    /*Da mesma maneira o tamaho da instrução será o número de operandos + 1*/
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
    vetorInstrucao.push_back({"STOP", 1});
}

size_t VerificaSeLinhaValida(string checaCaracter) {
    /*Lembrar que aqui a linha ja não é sensível ao caso
     *
     */
    size_t posicaoChar = checaCaracter.find_first_not_of(
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789+-*/=|&!?#%(){}[]_\'\",.:;<>\n ");
    if (posicaoChar != string::npos) {
        cout << "\nErro lexico na linha " << numLinha << " caractere " << posicaoChar - 1 << " invalido\n";
    } else
        flagErros++; // retorno se não existe caractere especial
}

// como vetor podemos usar a funcao find
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

// a pesquisa nao pode ser igual pois os tipos dos iteradores sao diferentes
int PesquisaSimbolo(string simbol, vector<tabSimbolos> vetorSimbolos) {
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

bool ProcuraRotulo(string linha) {
    size_t posicaoChar = linha.find_first_of(":");
    return posicaoChar != string::npos;
}

void checaSeRotuloValido(string rotulo) {
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

void PrimeiraPassagem(string linha) {
    int posicao = 0;
    int retorno;
    int offset;
    string proxtoken;
    VerificaSeLinhaValida(linha);
    if (ProcuraRotulo(linha)) {
        /*Caso ache um rótulo, verifica se ta na tabela de simbolos, se não tiver adicione*/
        string rotulo = linha.substr(0, linha.find_first_of(":")); // pega a primeira palavra como rotulo
        linha = linha.substr(rotulo.size() + 2, linha.size()); // retira rotulo da linha e pula o : e o ' '
        checaSeRotuloValido(rotulo);
        if (PesquisaSimbolo(rotulo, vetorSimbolos) != -1) { // se eu procurar na tabela o simbolo e ele já estiver lá
            cout << "\nErro Semantico na linha " << numLinha << " Simbolo " << rotulo << " redefinido\n";
            flagErros++;
        } else { // insere o simbolo na tabela de simbolos
            vetorSimbolos.push_back({rotulo, posicao});
            contadorSimbolos++;
            if (linha.find_first_of(" \n") != string::npos) {
                proxtoken = "";
                proxtoken = linha.substr(0, linha.find_first_of(" \n"));
            } else {
                linha.append("\n");
                proxtoken = linha.substr(0, linha.find_first_of("\n"));
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
                                posicao = posicao + 1;
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
                posicao = posicao + 1;
            }
        }
    }
}

void atualizaTabelaDeDefinicao() {
    /*
     * Copia as posicoes da tabela de simbolos para a tabela de definicoes caso exista mais de um modulo.
     * */
    string simboloProcurado;
    vector<tabDef>::iterator itDef;
    vector<tabSimbolos>::iterator itSimb;
    int indice;
    if (flagModulo == 1) {
        for (int i = 0; i < contadorDefinicoes; i++) {
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

void adicionaCodigoObjeto(int diretiva, int flagInstrucao) {
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
    cout << codigoObjeto;
}

void verificarVetor(string token, int *espacos, int *flagVetorErrado) {
    string aux, aux2, linha;
    int j;
    aux = token;
    aux.append("+");
    j = 0;
    aux2[0] = '\0';
    aux2 = linha.substr(0, linha.find_first_not_of(
            " \n+-"));// aux 3 deve ser o proximo token lembrando que aqui a string nao possui o +
    if (aux2[aux2.size()] < 48 || aux2[aux2.size()] > 57) {
        *flagVetorErrado = 1;

    } else {
        aux.append(aux2);
        stringstream(aux2) >> *espacos;
    }
}

void checaArgumentos(int flagMemoria, int retorno, int numeroLinha) {
    if (flagMemoria == 1) {
        if (vetorSimbolos[retorno].secdados != 1) {
            flagErros++;
            cout << "\nErro Semantico na linha " << numeroLinha << " Argumento invalido\n";
        }
    } else if (flagMemoria == 2) {// JUMP TENTANDO ACESSAR MEM DE DADOS
        if (vetorSimbolos[retorno].secdados != 0) {
            flagErros++;
            cout << "\nErro Semantico na linha " << numeroLinha << " Pulo para a secao de dados nao eh permitido\n";
        }
    }
}

void SegundaPassagem(fstream &preprocessado) {
    string linha;
    string proxtoken;
    string rotulo;
    int retorno;
    int flagInstrucao;
    int flagCode = 0;
    int flagData = 0;
    int checarDIV[30];
    int k = 0;
    int i = 0, j = 0, l = 0, m = 0;
    int numeroLinha, espacos;
    int flagMemoria = 0;
    int flagVetorErrado = 0, flagConst = 0;
    int numerosDIV[20];
    char checarMEM[30], espacosMEM[30], posicaoMEM[30];
    char constAlterada[50];
    char aux[40], aux2[40], aux3[10], string[1000];
    char const DATA[] = "DATA";
    char const TEXT[] = "TEXT";
    int posicao = 0;
    numeroLinha = 1;

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
                cout << "\nErro Semantico na linha " << numeroLinha << " instrucao deve estar na SECTION TEXT\n";

            }
            if (flagData == 0) {
                /*copia instrucao no codigo objeto*/
                adicionaCodigoObjeto(retorno + 1, flagInstrucao);
                checarDIV[k] = -1;
                /*Para verificar divisao por 0*/
                if (retorno == 3) {
                    checarDIV[k] = numeroLinha;
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
                    retorno = PesquisaSimbolo(proxtoken, vetorSimbolos);
                    if (!proxtoken.empty()) {

                        if (retorno != -1) {
                            if (linha.at(0) == '+') {
                                linha = linha.substr(1, linha.size() - 1);
                                flagVetorErrado = -1;
                                // chama uma funcao pra verificar se o vetor esta correto e corrigir
                                verificarVetor(proxtoken, &espacos, &flagVetorErrado);
                            }

                            /* se for um vetor no formato certo uma variavel normal*/
                            if (flagVetorErrado != 1) {
                                /*se for variavel externa usada, coloca na tabela de uso*/
                                if (vetorSimbolos[retorno].externo == 1) {
                                    vetorUso.push_back({vetorSimbolos[retorno].simbolo, posicao});
                                    contadorUso++;
                                    if (espacos == 1) {
                                        k = 0;
                                        adicionaCodigoObjeto(0, flagInstrucao);
                                    } else {
                                        adicionaCodigoObjeto(espacos, flagInstrucao);
                                    }
                                    posicao++;
                                } else {
                                    /*Checa se os argumentos sao dados*/
                                    checaArgumentos(flagMemoria, retorno, numeroLinha);
                                    if (espacos == 1) {
                                        k = 0;
                                        adicionaCodigoObjeto(espacos, flagInstrucao);
                                    } else {
                                        adicionaCodigoObjeto(vetorSimbolos[retorno].posicao + espacos,
                                                             flagInstrucao);
                                    }
                                    posicao++;
                                    espacosMEM[l] = espacos;
                                    posicaoMEM[l] = vetorSimbolos[retorno].posicao;
                                    checarMEM[l] = numeroLinha;
                                    constAlterada[l] = flagConst;
                                    l++;
                                    if (checarDIV[k] != -1) {
                                        numerosDIV[k] = vetorSimbolos[retorno].posicao;
                                        k++;
                                    }

                                }
                            } else {
                                flagErros++;
                                cout << "\nErro Sintatico na linha " << numeroLinha
                                     << " vetor declarado incorretamente\n";
                            }
                        } else {
                            flagErros++;
                            posicao++;
                            cout << "\nErro Semantico na linha " << numeroLinha << "Simbolo " << proxtoken
                                 << " indefinido\n";
                        }
                    }
                } else if (vetorInstrucao[retorno].operando == 2) {
                    proxtoken = linha.substr(0, linha.find_first_of(", \n"));
                    linha = linha.substr(proxtoken.size() + 1, linha.size() - proxtoken.size() - 1);
                    retorno = PesquisaSimbolo(proxtoken, vetorSimbolos);

                    if (retorno != -1) {// Se for variavel externa usada

                        if (vetorSimbolos[retorno].externo == 1) {//Se for externo
                            vetorUso.push_back({vetorSimbolos[retorno].simbolo, posicao});
                            adicionaCodigoObjeto(0, flagInstrucao);
                            posicao++;
                        } else {
                            checaArgumentos(flagMemoria, retorno, numeroLinha);
                            if (espacos == 1) {
                                k = 0;
                            }
                            adicionaCodigoObjeto(vetorSimbolos[retorno].posicao + k, flagInstrucao);
                            posicao++;
                            espacosMEM[l] = espacos;
                            posicaoMEM[l] = vetorSimbolos[retorno].posicao;
                            checarMEM[l] = numeroLinha;
                            constAlterada[l] = flagConst;
                            l++;
                        }
                    } else {
                        flagErros++;
                        posicao++;
                        cout << "\nErro Semantico na linha " << numeroLinha << "Simbolo " << proxtoken
                             << " indefinido\n";
                    }
                    if (linha[0] == ',') {//COPY
                        linha = linha.substr(1, linha.size() - 1);
                        flagConst = 0;
                        proxtoken = linha.substr(0, linha.find_first_of(" \n+-"));
                        retorno = PesquisaSimbolo(proxtoken, vetorSimbolos);
                        if (retorno != -1) {
                            if (vetorSimbolos[retorno].externo == 1) {//Se for externo
                                vetorUso.push_back({vetorSimbolos[retorno].simbolo, posicao});
                                adicionaCodigoObjeto(0, flagInstrucao);
                                posicao++;
                            } else {// se nao for externo
                                checaArgumentos(flagMemoria, retorno, numeroLinha);
                                if (espacos == 1) {
                                    k = 0;
                                }
                                adicionaCodigoObjeto(vetorSimbolos[retorno].posicao + k, flagInstrucao);
                                posicao++;
                                espacosMEM[l] = espacos;
                                posicaoMEM[l] = vetorSimbolos[retorno].posicao;
                                checarMEM[l] = numeroLinha;
                                constAlterada[l] = flagConst;
                                l++;
                            }
                        } else {
                            flagErros++;
                            //posicao++;
                            cout << "\nErro Semantico na linha " << numeroLinha << " Simbolo " << proxtoken
                                 << " indefinido\n";
                        }
                    } else {
                        flagErros++;
                        cout << "\nErro Sintatico na linha " << numeroLinha << " formato da instrucao COPY errado\n";
                    }
                } else if (retorno == 13) {// STOP
                    if (!linha.empty()) {
                        proxtoken = linha.substr(0, linha.find_first_of(" \n"));
                        if (proxtoken.size() > 0) {
                            flagErros++;
                            cout << "\nErro Sintatico na linha " << numeroLinha
                                 << " Instrucao STOP nao possui operandos\n"
                                 << endl;
                        }
                    }
                }
                if (!linha.empty() && retorno != 13 && flagVetorErrado == 1) {
                    flagErros;
                    cout << "\nErro Sintatico na linha " << numeroLinha << " numero de operandos incorreto\n";
                }
            }
        } else {// flag data != 0
            retorno = PesquisaInstrucaoEDiretiva(proxtoken, vetorDiretiva);
            if (retorno != -1) {
                if (retorno >= 5 && retorno <= 8) {
                    if (flagCode == 0) {
                        flagErros++;
                        cout << "\nErro Semantico na linha " << numeroLinha << " diretiva fora da SECTION TEXT\n"
                             << endl;
                    }
                } else {
                    /*Tratar as diretivas*/
                    switch (retorno) {
                        case 0:
                            /* j = 0 i++;*/
                            proxtoken = linha.substr(0, linha.find_first_of(" \n"));
                            if (proxtoken == "DATA") {
                                if (flagCode == 0) {
                                    cout << "\nErro Semantico na linha " << numeroLinha
                                         << " SECTION DATA deve ser declarada depois da SECTION TEXT\n";
                                    flagData = 1;
                                    flagErros++;
                                } else if (flagData == 0) {
                                    flagCode = 2;
                                    flagData = 1;
                                } else {
                                    flagErros++;
                                    cout << "\nErro Semantico na linha " << numeroLinha
                                         << " SECTION DATA deve ser declarada depois da SECTION TEXT\n";
                                }
                            } else if (proxtoken == "TEXT") {
                                if (flagCode == 0) {
                                    flagCode = 1;
                                } else {
                                    flagErros++;
                                    cout << "\nErro Semantico na linha " << numeroLinha
                                         << " multiplas declaracoes da SECTION TEXT\n";

                                }
                            } else {
                                flagErros++;
                                cout << "\nErro Semantico na linha " << numeroLinha
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
                                        cout << "\nErro Semantico na linha " << numeroLinha
                                             << " memoria nao reservada\n";
                                    }
                                }
                                posicao = posicao + espacos;
                                /* Se for um vetor, reservar quantos espacos na memoria forem necessarios */
                                for (j = 0; j < espacos; j++) {
                                    adicionaCodigoObjeto(0, flagInstrucao);
                                }
                            } else {
                                adicionaCodigoObjeto(0, flagInstrucao);
                                posicao++;
                            }
                            if (flagData != 1) {
                                flagErros++;
                                cout << "\nErro Semantico na linha " << numeroLinha
                                     << " diretiva SPACE fora da SECTION DATA\n";
                            }
                            break;
                        case 2:
                            /*CONST*/
                            if (!linha.empty()) {
                                proxtoken = linha.substr(0, linha.find_first_of(" \n +-"));
                                if (proxtoken[0] == 0 && proxtoken[1] == 'X') {
                                    stringstream(proxtoken) >> std::hex >> espacos;
                                } else {
                                    stringstream(proxtoken) >> espacos;
                                }
                                j = 0;
                                retorno = 0;

                                /*Verifica divisao por 0*/
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
                                        cout << "\nErro Semantico na linha " << numeroLinha
                                             << " divisao por zero\n";
                                    }
                                }
                                j = 0;
                                /*verifica se tentou alterar constante*/
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
                                        cout << "\nErro Semantico na linha " << numeroLinha
                                             << " impossivel alterar valor de constante\n";
                                    }
                                }
                                posicao++;
                                adicionaCodigoObjeto(espacos, flagInstrucao);
                            } else {
                                flagErros++;
                                cout << "\nErro Sintatico na linha " << numeroLinha
                                     << " constante nao iniciada\n";
                            }
                            break;
                    }

                }
            }
        }
        numeroLinha++;
    }
    if (flagCode == 0) {
        flagErros++;
        cout << "Erro semantico, section text nao definida";
    }
}

int Monta(fstream &preprocessado) {
    //primeiro vou escrever a ideia
    //recebe o arquivo preprocessado;
    int contador_posicao = 0;
    int contador_linha = 1;
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
    atualizaTabelaDeDefinicao();
    preprocessado.clear();
    preprocessado.seekg(0, ios::beg);
    SegundaPassagem(preprocessado);
    cout << endl << "Codigo Objeto: " << codigoObjeto;

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