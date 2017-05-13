#include <iostream>
#include <fstream>
#include <string>
using namespace std;
// Essa função retorna a extensão o arquivo
string GetFileExtension(const string&FileName)
{
    if(FileName.find_last_of(".") != string::npos)
        return FileName.substr(FileName.find_last_of(".")+1);
    return "";
}

int main(int argc, char* argv[]) {

    // nesse arquivo os arquivos serao .asm ou . pre, dependendo do arquivo a execucao deve ser diferente
    // como o nome do arquivo ta em argv[1] basta definir o fluxo de acordo com a extensao, logo
    string extensao = GetFileExtension(argv[1]);

    if (extensao == ".asm"){
    ifstream assembly(argv[1]);
        //chama preprocessador e depois o montador
    }
    else if (extensao == ".pre"){
        //chama somente o preprocessador, o preprocessador deve retirar linhas em branco, comentarios e espaçamentos
        //tambem deve expandir as macros IF e EQU
    }
    else{
        cout << "Arquivo incompativel";
        return EXIT_FAILURE;
    }

    return 0;
}