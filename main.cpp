#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include "preprocessador.h"
#include "montador.h"

using namespace std;

/*
 * Essa funcao retorna a extensao o arquivo
 *
 *
 * */
string PegaExtensao(const string &FileName) {
    if (FileName.find_last_of(".") != string::npos)
        return FileName.substr(FileName.find_last_of(".") + 1);
    return "";
}

int main(int argc, char *argv[]) {
    // nesse arquivo os arquivos serao .asm ou . pre, dependendo do arquivo a execucao deve ser diferente
    // como o nome do arquivo ta em argv[1] basta definir o fluxo de acordo com a extensao, logo
    string nomeArquivoEntrada;
    if (argc != 4) {
        cout
                << "\nEsse programa precisa de 3 argumentos: \n./<nome_do_programa> <operacao>"
                        " <arquivo_entrada>.asm <arquivo_saida>.o \n\nOu então:\n./<nome_do_programa>"
                        " <operacao> <arquivo_entrada>.pre <arquivo_saida>.o\n";
    } else {
        string argumento = argv[1];
        string nomeArquivoSaida = argv[3];

        if (argumento == "-p") {
            nomeArquivoEntrada = argv[2];
            nomeArquivoEntrada.append(".asm");
            fstream entrada(nomeArquivoEntrada);
            if (entrada.is_open()) {
                nomeArquivoEntrada = nomeArquivoEntrada.substr(0, nomeArquivoEntrada.find_first_of("."));
                Preprocessa(entrada, nomeArquivoSaida);
                entrada.close();
            } else {
                cout << "\nErro ao abrir o arquivo!";
                return EXIT_FAILURE;
            }
        } else if (argumento == "-o") {
            //recebe tanto .pre quanto .asm e gera .o
            nomeArquivoEntrada = argv[2];
            nomeArquivoEntrada.append(".pre");
            fstream entradaPre(nomeArquivoEntrada);
            if (entradaPre.is_open()) {
                Monta(entradaPre, nomeArquivoSaida);
                entradaPre.close();
            } else {
                nomeArquivoEntrada = argv[2];
                nomeArquivoEntrada.append((".asm"));
                fstream entradaAsm(nomeArquivoEntrada);
                if (entradaAsm.is_open()) {
                    Preprocessa(entradaAsm, nomeArquivoSaida);
                    entradaAsm.close();
                    string arqPreProcessado = nomeArquivoSaida;
                    arqPreProcessado.append(".pre");
                    fstream saidaPreProcessamento(arqPreProcessado);
                    if (saidaPreProcessamento.is_open()) {
                        Monta(saidaPreProcessamento, nomeArquivoSaida);
                        saidaPreProcessamento.close();
                        if (remove(nomeArquivoSaida.append(".pre").c_str()) != 0) {
                            cout << "\nErro ao remover arquivo temporario de preprocessamento\n";
                        }
                    } else {
                        cout << "\nErro ao abrir arquivo para montagem\n";
                    }
                } else {
                    cout << "\nErro ao abrir o arquivo";
                    return EXIT_FAILURE;
                }
            }
        }
    }
    return 0;
}