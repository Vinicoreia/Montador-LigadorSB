//
// created by vinicius on 13/05/17.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <map>

using namespace std;
/*
 * a função do pre-processador eh tirar espacos em branco desnecessarios, comentarios, e processar as macros equ e if.
 * comentarios começam com ;
 * caso o programa receba o comando -p deve gerar apenas o arquivo de pre-processamento com extensão .pre
 **/

// lembrar que não é case sensitive, logo precisamos criar uma função pra jogar o codigo pra uppercase

void transformaemuppercase(ifstream &assembly, fstream &semcomentarios) {
}


map<string, string> removecomentarios(fstream &uppertexto, fstream &semcomentarios) { // porque precisa do &?
    string linha;
    cout << "expandindo macros";
    int temMacro;
    int posicaoMacro;
    string macro;
    string valorMacro;
    map<string, string> macroValor;
    int posicaoValor;
    int posicaoFimValor;
    int posicaoInicioValor;
    cout << "removendo comentarios";
    int posicao;

    while (getline(uppertexto, linha)) {
        posicao = (int) linha.find_first_of(';');
        if (posicao >= 0) {
            //se existe ; na string, remove e coloca no arquivo de saida
            linha.erase(posicao, string::npos); // apaga do ; até o final da linha
            linha.erase(linha.find_last_not_of(" ") + 1); // faz trim dos espaços em branco no final da linha
        }
        temMacro = (int) linha.find("EQU");
        if (temMacro >= 0) {
            cout << linha;
            posicaoMacro = (int) linha.find_first_of(':');
            macro = linha.substr(0, posicaoMacro);
            posicaoFimValor = (int) linha.find_last_not_of(' ');
            posicaoInicioValor = (int) linha.find_last_of(' ');
            valorMacro = linha.substr(posicaoInicioValor, posicaoFimValor);
            cout << valorMacro;
            macroValor.insert(pair<string, string>(macro, valorMacro));
        }
        semcomentarios << linha << '\n';
    }
    return macroValor;
}

void removeEspacosEmBrancoESubstituiEQU(fstream &semcomentarios, fstream &semEspacos, map<string, string> macros) {
    string linha;
    string novalinha;
    string token;
    string separado;
    cout << "removendo espacos em branco";
    while (getline(semcomentarios, linha)) {
        istringstream is(linha);
        while (getline(is, token, ' ')) {
            istringstream tokens(token);
            while (getline(tokens, separado, '\t')) {
                if (macros.find(separado) != macros.end()) {
                    separado = macros[separado];
                }
                novalinha.append(separado);
                novalinha.append(" ");
            }
        }
        novalinha.pop_back(); // retira espaço do ultimo token
        novalinha.append("\n");
        semEspacos << novalinha;
        novalinha.clear();
    }
}


void expandemacroIF(fstream &semEspacos, fstream &macroexpandido) {
    string linha;
    char flagIF;
    while (getline(semEspacos, linha)) {
        if(linha.find("IF")>=0){
            flagIF = linha.back();
        }
        if(flagIF == '0'){
            getline(semEspacos, linha);
        }
        else{
            macroexpandido << linha;
        }
    }
}
// eh mais facil de expandir macros com os tokens separados.


int preprocessaassembly(ifstream &assembly) {
    fstream preprocessado("preprocessado.pre");
    transformaemuppercase(assembly, preprocessado);
    removecomentarios(preprocessado, preprocessado);
    preprocessado << "testando se escreve no arquivo";
    preprocessado.close();
    return 1;
}