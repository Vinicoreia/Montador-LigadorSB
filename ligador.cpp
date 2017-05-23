//
// Created by Vinicius on 13/05/17.
//

/*
 * recebe por linha de comando até 4 arquivos, onde os 3 primeiros serão .o e o ultimo, arquivo de saída, será um .e
 * deve verificar simbolos não definidos
 *
 * */
using namespace std;

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>

typedef struct {
    string simbolo; /* carrega o simbolo o endereco da tabela de uso */
    string valor;
    int posicaoCorrigida;
} tabela;


string codigoA;
string codigoB;
string codigoC;
string codigoFinal;
vector<tabela> tabUsoA;
vector<tabela> tabUsoB;
vector<tabela> tabUsoC;

vector<tabela> tabDefinicaoA;
vector<tabela> tabDefinicaoB;
vector<tabela> tabDefinicaoC;
vector<tabela> tabelaGlobalDefinicoes;
int vetorCorrecao[4];
int contaVetorCorrecao = 1;
int fatorCorrecao = 0;


/*
 * Essa funcao preenche a tabela de definicao em memoria, tabela de uso em memoria e o codigo em memoria
 * e tambem calcula o fator de correcao
 * */

void PreencheTabelas(fstream &arquivo, string &codigoAtual, vector<tabela> &vetorUso, vector<tabela> &vetorDefinicao) {
    string linha;
    string simbolo;
    string valor;
    int valorNumerico;
    getline(arquivo, linha);
    if (linha == "TABLE USE") {
        getline(arquivo, linha);
        while (!linha.empty()) {
            simbolo = linha.substr(0, linha.find_first_of(" \n"));
            valor = linha.substr(simbolo.size(), linha.find_first_of("\n"));
            vetorUso.push_back({simbolo, valor, 0});// valor eh a posicao nao corrigida;
            getline(arquivo, linha);
        }
    }
    getline(arquivo, linha);

    if (linha == "TABLE DEFINITION") {
        getline(arquivo, linha);
        while (!linha.empty()) {

            simbolo = linha.substr(0, linha.find_first_of(" \n"));
            valor = linha.substr(simbolo.size(), linha.find_first_of("\n"));
            valorNumerico = atoi(valor.c_str()) + fatorCorrecao;
            vetorDefinicao.push_back({simbolo, valor, valorNumerico});// valor eh a posicao nao corrigida;
            getline(arquivo, linha);
        }
    }
    tabelaGlobalDefinicoes.insert(tabelaGlobalDefinicoes.end(), vetorDefinicao.begin(), vetorDefinicao.end());
    getline(arquivo, linha);
    if (linha == "CODE") {
        while (getline(arquivo, linha)) {//le linha de codigo para o codigo respectivo e calcula fator de correcao
            codigoAtual.append(linha);
            fatorCorrecao += (int) count(linha.begin(), linha.end(), ' ') + 1;

        }
    }
    vetorCorrecao[contaVetorCorrecao] = fatorCorrecao;
    contaVetorCorrecao++;
}

/*
 * Essa funcao gera o codigo final calculando as referencias cruzadas e aplicando o fator de correcao
 *
 * */

void resolveReferenciasCruzadas(int argumentos, string codigoAtual, vector<tabela> &tabelaDeUso) {

    vector<tabela>::iterator it;
    string simboloProcurado;
    string linhaQuebrada;
    string token;
    string codigoAuxFinal;
    int contador = 0;
    int tokenInteiro;
    linhaQuebrada = codigoAtual;
    int posicaoNoCodigo;
    int posicaoTabelaDef;
    string codigoAux;
    int valorAtualizado;
    int flagNaoAtualiza = 0;
    int j = 0;


    stringstream linhastream(codigoAtual);
    contador = 0;
/*
 * ATUALIZA O CODIGO DE ACORDO COM A TABELA GLOBAL DE DEFINICOES
 * */
    for (int i = 0; i < tabelaDeUso.size(); i++) {
        simboloProcurado = tabelaDeUso[i].simbolo;
        posicaoNoCodigo = atoi(tabelaDeUso[i].valor.c_str());

        auto predicado = [&simboloProcurado](tabela &item) {
            return item.simbolo == simboloProcurado;
        };

        it = find_if(tabelaGlobalDefinicoes.begin(), tabelaGlobalDefinicoes.end(), predicado);
        if (it != tabelaGlobalDefinicoes.end()) {
            contador = 0;
            stringstream linhastream(codigoAtual);
            posicaoTabelaDef = (int) distance(tabelaGlobalDefinicoes.begin(), it); // retorna a posicao do simbolo
            valorAtualizado = tabelaGlobalDefinicoes[posicaoTabelaDef].posicaoCorrigida;
            stringstream valorAtualizadoConvertido;
            valorAtualizadoConvertido << valorAtualizado;
            codigoAux = "";
            while (getline(linhastream, token, ' ')) {
                /*Resolve pendencias cruzadas*/
                if (contador == posicaoNoCodigo) {
                    codigoAux.append(valorAtualizadoConvertido.str() + " ");
                } else {
                    codigoAux.append(token + " ");
                }
                contador++;
            }
            codigoAtual = codigoAux;

        } else {
            cout << "Erro durante o processo de ligacao:: Simbolo nao definido";
        }
    }
    stringstream linhastream2(codigoAtual);
    contador = 0;
    codigoAux = "";
/*
 *Resolve as referencias cruzadas
 */
    while (getline(linhastream2, token, ' ')) {
        if (contador % 2 == 1) {

            for (int i = 0; i < tabelaDeUso.size(); i++) {
                if (atoi(tabelaDeUso[i].valor.c_str()) == contador) {
                    codigoAux.append(token + " ");
                    flagNaoAtualiza = 1;
                    break;
                } else {
                    flagNaoAtualiza = 0;
                }
            }
            if (flagNaoAtualiza == 0) {
                tokenInteiro = atoi(token.c_str());
                valorAtualizado = tokenInteiro + vetorCorrecao[contaVetorCorrecao];
                ostringstream convToken;
                convToken << valorAtualizado;
                codigoAux.append(convToken.str() + " ");
            }
        } else {
            codigoAux.append(token + " ");
        }
        contador++;
    }
    codigoAtual = codigoAux;
    codigoFinal.append(codigoAux);
    contaVetorCorrecao++;
}

/*
 * A funcao main eh responsavel pelo controle dos arquivos de entrada e saida
 *
 * */
int main(int argc, char *argv[]) {
    if (argc <= 3 || argc >= 5) {
        cout << "\nNumero de argumentos invalido, eh preciso ao menos dois arquivos objetos para ligar";
    } else {
        string arquivo1 = argv[1];
        arquivo1.append(".o");
        string arquivo2 = argv[2];
        arquivo2.append(".o");
        string nomeArquivoSaida;
        fstream arquivo3Entrada;
        nomeArquivoSaida = argv[3];
        if (argc == 5) {
            string arquivo3 = argv[3];
            nomeArquivoSaida = argv[4];
            arquivo3.append(".o");
            arquivo3Entrada.open(arquivo3);
        }
        nomeArquivoSaida.append(".e");

        fstream arquivo1Entrada(arquivo1);
        fstream arquivo2Entrada(arquivo2);

        if (arquivo1Entrada.is_open() && (arquivo2Entrada.is_open())) {
            PreencheTabelas(arquivo1Entrada, codigoA, tabUsoA, tabDefinicaoA);
            PreencheTabelas(arquivo2Entrada, codigoB, tabUsoB, tabDefinicaoB);
            if (argc == 5 && arquivo3Entrada.is_open()) {
                PreencheTabelas(arquivo3Entrada, codigoC, tabUsoC, tabDefinicaoC);
            }
            contaVetorCorrecao = 0;
            resolveReferenciasCruzadas(argc, codigoA, tabUsoA);
            resolveReferenciasCruzadas(argc, codigoB, tabUsoB);
            if (argc == 5 && arquivo3Entrada.is_open()) {
                resolveReferenciasCruzadas(argc, codigoC, tabUsoC);
            }
            arquivo1Entrada.close();
            arquivo1Entrada.close();
            arquivo3Entrada.close();
        }
        fstream arquivoSaida(nomeArquivoSaida, fstream::in | fstream::out | fstream::trunc);
        codigoFinal.pop_back();
        arquivoSaida << codigoFinal;
        arquivoSaida.close();
    }

    return 0;
}