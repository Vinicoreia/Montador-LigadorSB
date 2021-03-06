//
// created by vinicius on 13/05/17.
//
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
/*
 * a função do pre-processador eh tirar espacos em branco desnecessarios, comentarios, e processar as macros equ e if.
 * comentarios começam com ;
 * caso o programa receba o comando -p deve gerar apenas o arquivo de pre-processamento com extensão .pre
 **/



/*
 * Essa funcao transforma o texto do arquivo em uppercase
 *
 * */
void TransformaEmUpperCase(fstream &assembly, fstream &semcomentarios) {
    string linha;
    while (getline(assembly, linha)) {
        transform(linha.begin(), linha.end(), linha.begin(), ::toupper);
        linha.append("\n");
        semcomentarios << linha;
    }
}

/*
 * Essa funcao remove os comentarios enquanto procura pela diretiva EQU
 * */
map<string, string> RemoveComentariosEAchaEQU(fstream &uppertexto, fstream &semcomentarios) {
    string linha;
    cout << "Procurando macros";
    int temMacro;
    int posicaoMacro;
    string macro;
    string valorMacro;
    map<string, string> macroValor;
    int posicaoFimValor;
    int posicaoInicioValor;
    cout << "\nRemovendo comentarios";
    int posicao;

    while (getline(uppertexto, linha)) {
        posicao = (int) linha.find_first_of(';');
        if (posicao >= 0) {
            //se existe ; na string, remove e coloca no arquivo de saida
            linha.erase((unsigned) posicao, string::npos); // apaga do ; até o final da linha
            linha.erase(linha.find_last_not_of(" ") + 1); // faz trim dos espaços em branco no final da linha
        }
        temMacro = (int) linha.find("EQU");
        if (temMacro >= 0) {
            posicaoMacro = (int) linha.find_first_of(':');
            macro = linha.substr(0, (unsigned) posicaoMacro);
            posicaoFimValor = (int) linha.find_last_not_of(' ');
            posicaoInicioValor = (int) linha.find_last_of(' ');
            valorMacro = linha.substr((unsigned) posicaoInicioValor + 1, (unsigned) posicaoFimValor);
            macroValor.insert(pair<string, string>(macro, valorMacro));
            getline(uppertexto, linha);
        }
        semcomentarios << linha << '\n';
    }
    return macroValor;
}

/*
 * Essa funcao remove os espacos em branco desnecessarios e substitui a diretiva EQU pelo seu respectivo valor
 * */
void RemoveEspacosEmBrancoESubstituiEqu(fstream &semcomentarios, fstream &semEspacos, map<string, string> macros) {
    string linha;
    string novalinha;
    string token;
    string separado;
    vector<string> v;
    cout << "\nRemovendo espacos em branco";
    while (getline(semcomentarios, linha)) {
        istringstream is(linha);
        while (getline(is, token, '\t')) {
            istringstream tokens(token);
            while (getline(tokens, separado, ' ')) {
                v.insert(v.end(), separado);
            }
        }

        for (unsigned i = 0; i < v.size(); i++) {
            if (!v[i].empty()) {
                if (macros.find(v[i]) != macros.end()) { // se a palavra lida no momento for macro
                    cout << "\nMacro encontrada... expandindo...";
                    novalinha.append(macros[v[i]]);
                    novalinha.append(" ");
                } else {
                    novalinha.append(v[i]);
                    novalinha.append(" ");
                }
            }
        }
        if (novalinha.size() > 0) {
            novalinha.erase(novalinha.end() - 1);
            novalinha.append("\n");
        }
        semEspacos << novalinha;
        novalinha.clear();
        v.clear();
    }
}

/*
 * Essa funcao expande a macro IF durante o preprocessamento
 * */
void ExpandeMacroIF(fstream &semEspacos, fstream &macroexpandido) {
    string linha;
    string flagIF = "1";
    int posicao;
    while (getline(semEspacos, linha)) {
        flagIF = "1";
        posicao = (int) linha.find("IF");
        if (posicao >= 0) {
            posicao = (int) linha.find_last_of(' ');
            flagIF = linha.substr((unsigned) (posicao + 1));
            getline(semEspacos, linha);
        }
        if (flagIF == "0") {
            getline(semEspacos, linha);
            linha.append("\n");
            macroexpandido << linha;
        } else if (flagIF == "1") {
            linha.append("\n");
            macroexpandido << linha;
        } else {// caso seja uma flag invalida o codigo nao eh gerado.
            cout << "\n\nErro: Diretiva IF associada a valor inválido";
            macroexpandido.close();
            macroexpandido.open("preprocessado.pre", fstream::out | fstream::trunc);
            macroexpandido.close();
            break;
        }

    }
}

/* Essa função chama os outros metodos para preprocessar o arquivo
*/
int Preprocessa(fstream &arquivoEntrada, string nomeArquivoSaida) {
    fstream arqA;
    fstream arqB; // dependendo da operação o arquivo A é o de saída ou de entrada
    map<string, string> retornoMacros;
    //abrir o arquivo de acordo com a função;
    arqA.open("preprocessando.pre", fstream::out | fstream::in | fstream::trunc);
    TransformaEmUpperCase(arquivoEntrada, arqA);
    arqA.clear();// limpa failbit
    arqA.seekg(0, arqA.beg);// volta para o inicio do arquivo

    arqB.open("preprocessado.pre",
              fstream::out | fstream::in | fstream::trunc); //trunc cria o arquivo mesmo que n exista
    retornoMacros = RemoveComentariosEAchaEQU(arqA, arqB);

    arqA.close();
    arqB.close();

    remove("preprocessando.pre");
    rename("preprocessado.pre", "preprocessando.pre");

    arqA.open("preprocessando.pre", fstream::in);
    arqB.open("preprocessado.pre", fstream::out | fstream::trunc);
    RemoveEspacosEmBrancoESubstituiEqu(arqA, arqB, retornoMacros);
    arqA.close();
    arqB.close();

    remove("preprocessando.pre");
    rename("preprocessado.pre", "preprocessando.pre");

    arqA.open("preprocessando.pre", fstream::in);
    arqB.open("preprocessado.pre", fstream::out | fstream::trunc);
    ExpandeMacroIF(arqA, arqB);
    arqA.close();
    arqB.close();

    if (remove("preprocessando.pre") != 0) {
        cout << "\n Erro ao apagar arquivo temporario\n";
    }
    nomeArquivoSaida.append(".pre"); // extensao do arquivo
    rename("preprocessado.pre", nomeArquivoSaida.c_str());
    return 1;
}
