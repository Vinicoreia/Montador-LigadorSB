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

// Essa função retorna a extensão o arquivo
string PegaExtensao(const string &FileName) {
    if (FileName.find_last_of(".") != string::npos)
        return FileName.substr(FileName.find_last_of(".") + 1);
    return "";
}

int main(int argc, char *argv[]) {

    // nesse arquivo os arquivos serao .asm ou . pre, dependendo do arquivo a execucao deve ser diferente
    // como o nome do arquivo ta em argv[1] basta definir o fluxo de acordo com a extensao, logo
    string extensao = PegaExtensao(argv[1]);

    if (extensao == "asm") {
        //chama preprocessador e depois o montador (nao esquecer de setar working directory e program arguments no Clion)
        string line;
        ifstream assembly(argv[1]);
        string linha;
        if (assembly.is_open()) {
            //chama preprocessador
            preprocessa(assembly);

            // Agora precisamos desenvolver o montador

            assembly.close();
            fstream preprocessado("preprocessado.pre");

            Monta(preprocessado);

            preprocessado.close();


        } else {
            cout << "Erro ao abrir arquivo";
            return EXIT_FAILURE;
        }
    } else if (extensao == "pre") {
        //chama somente o preprocessador, o preprocessador deve retirar linhas em branco, comentarios e espaçamentos
        //tambem deve expandir as macros IF e EQU
    } else {
        cout << "Arquivo incompativel";
        return EXIT_FAILURE;
    }

    return 0;
}