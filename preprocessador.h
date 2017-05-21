//
// Created by Vinicius on 14/05/17.
//

#ifndef MONTADOR_LIGADORSB_PREPROCESSADOR_H
#define MONTADOR_LIGADORSB_PREPROCESSADOR_H

#include <fstream>
#include <sstream>
#include <map>

using namespace std;

void Transformaemuppercase(fstream &assembly, fstream &semcomentarios);

map<string, string> RemoveComentariosEAchaEqu(fstream &uppertexto, fstream &semcomentarios);

void RemoveEspacosEmBrancoESubstituiEQU(fstream &semcomentarios, fstream &semEspacos, map<string, string> macros);

void ExpandemacroIF(fstream &semEspacos, fstream &macroexpandido);

int Preprocessa(fstream &assembly, string nomeArquivoSaida);

#endif //MONTADOR_LIGADORSB_PREPROCESSADOR_H
