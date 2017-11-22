#pragma once
#include "Grafo.h"
#include "Loading.h"
#include <algorithm>
#include <conio.h>

class GeradorDeGrafos
{
public:
	static Grafo * gerarK(int vertices) {
		Grafo * g = new Grafo(vertices, true);
		int total = vertices * (vertices - 1);
		int n = 0;
		for (int i = 0; i < vertices; i++) {
			for (int j = i; j < vertices; j++) {
				if (j == i)
					continue;
				n = n + 2;
				g->adj->cria_adjacencia(i, j, 1);
				Loading::showBar("Criando K" + to_string(vertices), "arestas", n, total);
			}
		}
		return g;
	}
	static Grafo * gerar(int vertices, int arestas, bool conexo) {
		Grafo * g = new Grafo(vertices, true);
		if (arestas < vertices) {
			arestas = vertices;
		}

		int n_arestas = 0;
		int fRand, sRand;
		vector<int> vec = vector<int>();
		for (int i = 0; i < vertices; i++) {
			vec.push_back(i);
		}

		random_shuffle(vec.begin(), vec.end());
		if (conexo) {
			for (int i = 1; i < vec.size(); i++) {
				g->adj->cria_adjacencia(i - 1, i, 1);
				n_arestas++;
				Loading::showBar("Gerando grafo conexo.", "arestas", n_arestas, arestas);
			}
			while(n_arestas < arestas) {
				fRand = rand() % vertices;
				do { sRand = rand() % vertices; } while (sRand == fRand);
				g->adj->cria_adjacencia(fRand, sRand, 1);
				n_arestas++;
				Loading::showBar("Gerando grafo conexo.", "arestas", n_arestas, arestas);
				
				
			}
			return g;
		}
		else {
			int componentes = (rand() % 9) + 2;

			int c = 0;
			vector<vector<int>> clusters = vector<vector<int>>();
			for (int i = 0; i < componentes; i++) {
				clusters.push_back(vector<int>());
			}
			for (vector<int>::iterator it = vec.begin(); it != vec.end(); it++) {
				c = c == componentes - 1 ? 0 : c + 1;
				clusters[c].push_back(*it);
			}
			for (vector<vector<int>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
				for (int i = 1; i < it->size(); i++) {
					g->adj->cria_adjacencia((*it)[i - 1], (*it)[i], 1);
					n_arestas++;
					Loading::showBar("Gerando grafo com " + to_string(componentes) + " componentes.", "arestas", n_arestas, arestas);
				}
			}
			while (n_arestas < arestas) {
				c = rand() % componentes;
				fRand = rand() % clusters[c].size();
				do { sRand = rand() % clusters[c].size(); } while (sRand == fRand);
				g->adj->cria_adjacencia(clusters[c][fRand], clusters[c][sRand], 1);
				n_arestas++;
				Loading::showBar("Gerando grafo com " + to_string(componentes) + " componentes.", "arestas", n_arestas, arestas);
			}
		}
		return g;
	}


	void static printarVetor(vector<int> vec) {
		if (vec.size() == 0) {
			cout << "{}";
			return;
		}
		cout << "{";
		for (int i = 0; i < vec.size() - 1;i++) {
			cout << vec[i] << ", ";
		}
		cout << vec[vec.size() - 1] << "}";
	}
};

