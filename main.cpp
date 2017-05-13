#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
        if (assembly.is_open()) {
            //chama preprocessador

            //TESTAR FUNCAO ABAIXO

            fstream preprocessado("preprocessado.pre");
            string linha;
            string novaLinha;
            string token;
            string separado;
            while (getline(assembly, linha)) {
                istringstream is(linha);
                while (getline(is, token, '\t')) {
                    istringstream tokens(token);
                    while (getline(tokens, separado,' ')) {
                        cout<< separado;
                        novaLinha.append(separado);
                        novaLinha.append(" ");
                        }
                    }
                    novaLinha.pop_back(); // retira espaço do ultimo token
                    novaLinha.append("\n");
                }
                novaLinha.erase(novaLinha.end() - 1); //apaga o ultimo \n
                preprocessado << novaLinha;
                novaLinha.clear();
                preprocessado.close();

                assembly.close();
            }
            /*

            if(assembly.is_open()){
                while(getline(assembly, line)){
                    cout << line << '\n';
                }
                assembly.close();
            }*/
            else
            cout << "Erro ao abrir arquivo";
            return EXIT_FAILURE;

        } else if (extensao == "pre") {
            //chama somente o preprocessador, o preprocessador deve retirar linhas em branco, comentarios e espaçamentos
            //tambem deve expandir as macros IF e EQU
        } else {
            cout << "Arquivo incompativel";
            return EXIT_FAILURE;
        }

        return 0;
    }