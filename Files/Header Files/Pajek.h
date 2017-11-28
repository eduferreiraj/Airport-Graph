#pragma once
#include "Grafo.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <tuple>

class Pajek
{
public:
	Pajek() {

	};

	//SPACE SPLIT TEMPLATE
	template<typename Out>
	static void split(const string &s, char delim, Out result) {
		stringstream ss(s);
		string item;
		while (getline(ss, item, delim)) {
			*(result++) = item;
		}
	}
	static vector<string> split(const string &s, char delim) {
		vector<string> elems;
		split(s, delim, back_inserter(elems));
		return elems;
	}

	//MODULO DE IMPORTACAO DE BASE DE DADOS
	static void importa_pajek(Grafo * g) {

	}

	//MODULO DE GRAVACAO PAJEK
	static void gravacao(Grafo * g) {
		string filename;
		cout << "Digite somente o nome do arquivo PAJEK: " << endl;
		cin >> filename;
		filename.append(".paj");
		ofstream file;

		file.open(filename, ofstream::out);
		if (file.is_open() == false) {
			cout << "Erro ao abrir o arquivo: " << filename << endl;
			exit(0);
		}

		//ESTRUTURA PAJEK
		file << "*Vertices  " << g->size << "\n\n";
		for (int i = 0; i < g->size; i++) {
			file << (i + 1) << " \"" << g->adj->vertices[i].nome << "\"\n\n";
		}
		if (!g->adj->direcional()) {
			file << "*Arcs" << "\n\n";
			for (int i = 0; i < g->size; i++) {
				for (vector<NoRef>::iterator it = g->adj->vertices[i].adjacentes.begin(); it != g->adj->vertices[i].adjacentes.end(); it++) {
					file << i + 1 << " " << it->node->cod + 1 << " " << it->peso << "\n\n";
				}
			}
		}
		else {
			file << "*Edges" << "\n\n";
			vector<tuple<int, int, float>> coords;
			for (int i = 0; i < g->size; i++) {
				for (vector<NoRef>::iterator it = g->adj->vertices[i].adjacentes.begin(); it != g->adj->vertices[i].adjacentes.end(); it++) {
					//file << i + 1 << " " << it->node->cod + 1 << " " << it->peso << "\n\n";
					coords.push_back(make_tuple(i, it->node->cod, it->peso));
				}
			}
			/* DEBUG TUPLA ANTES DO REMOVE
			for (auto it = coords.begin(); it != coords.end(); ++it) {
			cout << "TUPLA ANTES DO REMOVE" << endl;
			cout << get<0>(*it) << " " << get<1>(*it) << " " << get<2>(*it) << endl;
			}*/

			int coord1 = 0, coord2 = 0, coord3 = 0, coord4 = 0;
			float peso1 = 0, peso2 = 0;
			for (int it = 0; it < coords.size(); ++it) {
				for (int jt = 0; jt < coords.size(); ++jt) {
					tie(coord1, coord2, peso1) = coords[it];
					tie(coord3, coord4, peso2) = coords[jt];
					if (coord1 == coord4 && coord2 == coord3) {
						coords.erase(coords.begin() + jt);
					}
				}
			}

			for (auto it = coords.begin(); it != coords.end(); ++it) {
				//cout << "TUPLA DEPOIS DO REMOVE" << endl;
				//cout << get<0>(*it) << " " << get<1>(*it) << " " << get<2>(*it) << endl;
				//INSERIR + 1 POR CONTA DO FORMATO PAJEK QUE INICIA EM 1
				file << get<0>(*it) + 1 << " " << get<1>(*it) + 1 << " " << get<2>(*it) << "\n\n";
			}
		}

		file.close();
	}
	//MODULO DE CARREGAMENTO PAJEK
	static Grafo * carregamento() {
		string filename, linha;
		string L1 = "*Vertices";
		string L2 = "*Edges";
		string L3 = "*Arcs";
		vector<string> vertices_temp;
		int tempsize;
		//Grafo * g = new Grafo(tempsize); //altera 
		cout << "Digite somente o nome do arquivo PAJEK: " << endl;
		cin >> filename;
		filename.append(".paj");
		cout << filename << endl;
		ifstream file;

		file.open(filename, ifstream::in);
		if (file.is_open() == false) {
			cout << "Erro ao abrir o arquivo: " << filename << endl;
			exit(0);
		}

		getline(file, linha);
		cout << "linha 1:" << linha << endl;
		//_getch();
		size_t found = linha.find(L1);
		cout << found << endl;
		if (found == std::string::npos) {
			cout << "Erro de leitura PAJEK, Falha no carregamento!" << endl;
			exit(-1);
		}
		else {
			linha.replace(linha.find(L1), L1.length(), "");
			tempsize = stoi(linha);
			cout << "linha tempsize:" << tempsize << endl;
			cout << "linha linha:" << linha << endl;
			//_getch();
		}

		for (int i = 0; i < tempsize; i++) {
			getline(file, linha);
			getline(file, linha);
			cout << "linha for:" << linha << endl;
			//_getch();
			found = linha.find("\"");
			linha.erase(0, static_cast<int>(found));
			//ADICIONA ROTULOS -> linha = rotulos
			//g->adj->seta_informacao(i, linha);
			cout << "linha erased:" << linha << endl;
			linha.erase(0, 1);
			linha.erase(linha.size() - 1);
			cout << "linha erased2:" << linha << endl;
			vertices_temp.push_back(linha);
		}

		getline(file, linha);
		getline(file, linha);
		cout << "linha Edges or Arcs:" << linha << endl;
		//_getch();
		found = linha.find(L2);
		if (found == std::string::npos) {
			found = linha.find(L3);
			if (found == std::string::npos) {
				cout << "Erro de leitura PAJEK, Falha no carregamento!" << endl;
				exit(-1);
			}
			else {
				//CHAMA ARCS
				Grafo * g1 = new Grafo(tempsize, false);
				for (int i = 0; i < vertices_temp.size(); i++) {
					g1->adj->seta_informacao(i, vertices_temp[i]);
				}
				do {
					getline(file, linha);
					if (linha == "") { continue; }
					//SPACE SPLIT
					vector<string> tokens_split;
					vector<int> tokens;
					float token_peso = 0;
					tokens_split = split(linha, ' ');
					tokens.resize(tokens_split.size() - 1);
					for (int i = 0; i < tokens_split.size() - 1; i++) {
						tokens[i] = stoi(tokens_split[i]);
					}
					token_peso = stof(tokens_split[2]);
					//SPACE SPLIT
					//TOKEN[0] = LINHA TOKEN[1] = COLUNA TOKEN[2] = PESO
					g1->adj->cria_adjacencia(tokens[0] - 1, tokens[1] - 1, token_peso);
					tokens.clear();
					tokens_split.clear();
				} while (!file.eof());
				file.close();
				vertices_temp.clear();
				return g1;
			}
		}
		else {
			//CHAMA EDGES
			Grafo * g2 = new Grafo(tempsize, true);
			for (int i = 0; i < vertices_temp.size(); i++) {
				g2->adj->seta_informacao(i, vertices_temp[i]);
			}
			do {
				getline(file, linha);
				//getline(file, linha);
				//SPACE SPLIT
				if (linha == "") { continue; }
				vector<string> tokens_split;
				vector<int> tokens;
				float token_peso = 0;
				tokens_split = split(linha, ' ');
				tokens.resize(tokens_split.size() - 1);
				for (int i = 0; i < tokens_split.size() - 1; i++) {
					tokens[i] = stoi(tokens_split[i]);
				}
				token_peso = stof(tokens_split[2]);
				//SPACE SPLIT
				//TOKEN[0] = LINHA TOKEN[1] = COLUNA TOKEN[2] = PESO
				g2->adj->cria_adjacencia(tokens[0] - 1, tokens[1] - 1, token_peso);
				tokens.clear();
				tokens_split.clear();
			} while (!file.eof());
			file.close();
			vertices_temp.clear();
			return g2;
		}

	}
	//VERIFICA DIRECIONAL
	/*
	bool direcional(Grafo * g) {
	for (int i = 0; i < g->size; i++) {
	for (vector<NoRef>::iterator it = g->adj->vertices[i].adjacentes.begin(); it != g->adj->vertices[i].adjacentes.end(); it++) {
	for (vector<NoRef>::iterator jt = g->adj->vertices[it->node->cod].adjacentes.begin(); jt != g->adj->vertices[it->node->cod].adjacentes.end(); jt++) {
	if (it->peso != jt->peso) { return true; }
	}
	}
	}
	return false;
	}*/

	~Pajek() {

	};
};

