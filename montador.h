//
// Created by Vinicius on 19/05/17.
//

#ifndef MONTADOR_LIGADORSB_MONTADOR_H
#define MONTADOR_LIGADORSB_MONTADOR_H

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
typedef struct {
    string instrucaoOuDiretiva;
    int operando;
} tabInstrucaoOuDiretiva;

typedef struct {
    string simbolo;
    int posicao;
    int externo; /*0 ou 1*/
    int secdados;
} tabSimbolos;

typedef struct {
    string simbolo;
    int posicao;
} tabUso;

typedef struct {
    string simbolo;
    int posicao;
} tabDef;

void GeraTabelaInstrucoesEDiretivas(vector<tabInstrucaoOuDiretiva> tabInstrucao,
                                    vector<tabInstrucaoOuDiretiva> tabDiretiva);

size_t VerificaSeLinhaValida(string checaCaracter);

int PesquisaInstrucaoEDiretiva(string instrOuDiretiva, vector<tabInstrucaoOuDiretiva> vetorInstOuDiretiva);

int PesquisaSimbolo(string simbol, vector<tabSimbolos> vetorSimbolos);

bool ProcuraRotulo(string linha);

void checaSeRotuloValido(string rotulo);

void PrimeiraPassagem(string linha, vector<tabSimbolos> vetorSimbolos, vector<tabDef> vetorDefinicoes,
                      vector<tabUso> tabelaUso);

int Monta(fstream &preprocessado, string nomeArquivoSaida);

#endif //MONTADOR_LIGADORSB_MONTADOR_H
