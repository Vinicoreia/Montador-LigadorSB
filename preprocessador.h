//
// Created by Vinicius on 14/05/17.
//

#ifndef MONTADOR_LIGADORSB_PREPROCESSADOR_H
#define MONTADOR_LIGADORSB_PREPROCESSADOR_H

#include <fstream>
#include <sstream>
#include <map>

using namespace std;

void transformaemuppercase(fstream &assembly, fstream &semcomentarios);

map<string, string> removeComentariosEAchaEqu(fstream &uppertexto, fstream &semcomentarios);

void removeEspacosEmBrancoESubstituiEQU(fstream &semcomentarios, fstream &semEspacos, map<string, string> macros);

void expandemacroIF(fstream &semEspacos, fstream &macroexpandido);

int preprocessa(fstream &assembly, string nomeArquivoSaida);

#endif //MONTADOR_LIGADORSB_PREPROCESSADOR_H
