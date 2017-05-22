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

typedef struct {
    string simbolo; /* carrega o simbolo o endereco da tabela de uso */
    string valor;
    int posicaoCorrigida;
} tabela;

vector<tabela> codigo;
string codigoGeral;

vector<tabela> tabUsoA;
vector<tabela> tabUsoB;
vector<tabela> tabUsoC;

vector<tabela> definicaoA;
vector<tabela> definicaoB;
vector<tabela> definicaoC;
vector<tabela> tabelaGlobalDefinicoes;

int fatorCorrecao = 0;

void PreencheTabelas(fstream &arquivo, vector<tabela> &vetorUso, vector<tabela> &vetorDefinicao) {
    string linha;
    string simbolo;
    string valor;
    int valorNumerico;
    int contador = 0;
    getline(arquivo, linha);
    if (linha == "TABLE USE") {
        getline(arquivo, linha);
        while (!linha.empty()) {
            simbolo = linha.substr(0, linha.find_first_of(" \n"));
            valor = linha.substr(simbolo.size(), linha.find_first_of("\n"));
            vetorUso.push_back({simbolo, valor, 0});// valor eh a posicao nao corrigida;
            contador++;
            getline(arquivo, linha);
        }
    }
    getline(arquivo, linha);

    if (linha == "TABLE DEFINITION") {
        contador = 0;
        getline(arquivo, linha);
        while (!linha.empty()) {

            simbolo = linha.substr(0, linha.find_first_of(" \n"));
            valor = linha.substr(simbolo.size(), linha.find_first_of("\n"));
            valorNumerico = atoi(valor.c_str())+ fatorCorrecao;
            vetorDefinicao.push_back({simbolo, valor, valorNumerico});// valor eh a posicao nao corrigida;
            contador++;
            getline(arquivo, linha);
        }
    }
    tabelaGlobalDefinicoes.insert(tabelaGlobalDefinicoes.end(), vetorDefinicao.begin(), vetorDefinicao.end());
    getline(arquivo, linha);
    if (linha == "CODE")
        contador = 0;
        getline(arquivo, linha);
        while (!linha.empty()) {
            codigoGeral.append(linha);
            codigoGeral.append(" ");
            fatorCorrecao += (int)count(linha.begin(), linha.end(), ' ')+1;
            getline(arquivo, linha);
            contador++;
        }
}

void resolveReferenciasCruzadas(int argumentos){
/*
 * Deve substituir no codigo os enderecos relativos de acordo com a tabela de USO
 *
 * */
    string simboloProcurado;
    string posicaoNoCodigo;
    if (argumentos==4){
        /*Retira posicao no codigo da tabela de USO e atualiza com o valor na tabela de definicao
         *
         */
        for(int i =0; i< tabUsoA.size(); i++){
            simboloProcurado = tabUsoA[i].simbolo;
            posicaoNoCodigo = tabUsoA[i].valor;
        }

    }
}

int main(int argc, char *argv[]) {
    /*
     * Os argumentos 1 2 e 3 sao os arquivos de entrada
     * deve checar se existem 2 ou 3 arquivos
     * */
    if (argc <= 3 || argc >= 5) {
        cout << "\nNumero de argumentos invalido, eh preciso ao menos dois arquivos objetos para ligar";
    } else {
        /*processa os arquivos e gera o executavel com formato saida .e*/
        string arquivo1 = argv[1];
        arquivo1.append(".o");
        string arquivo2 = argv[2];
        arquivo2.append(".o");
        string arquivoSaida;
        fstream arquivo3Entrada;
        arquivoSaida = argv[3];
        if (argc == 5) {
            string arquivo3 = argv[3];
            arquivoSaida = argv[4];
            arquivo3.append(".o");
            arquivo3Entrada.open(arquivo3);
        }
        arquivoSaida.append(".e");

        fstream arquivo1Entrada(arquivo1);
        fstream arquivo2Entrada(arquivo2);

        if (arquivo1Entrada.is_open() && (arquivo2Entrada.is_open())) {
            /*Primeiro vamos escrever para dois arquivos*/
            PreencheTabelas(arquivo1Entrada, tabUsoA, definicaoA);
            PreencheTabelas(arquivo2Entrada, tabUsoB, definicaoB);
            if (argc == 5 && arquivo3Entrada.is_open()) {
                PreencheTabelas(arquivo3Entrada, tabUsoC, definicaoC);
            }
            cout<< codigoGeral;
            /*
            for(int i = 0; i<tabelaGlobalDefinicoes.size(); i++){
                cout<< tabelaGlobalDefinicoes[i].simbolo;
                cout<< tabelaGlobalDefinicoes[i].posicaoCorrigida;

            }*/
            resolveReferenciasCruzadas(argc);
        }
        arquivo1Entrada.close();
        arquivo1Entrada.close();
        arquivo3Entrada.close();
    }

    return 0;
}