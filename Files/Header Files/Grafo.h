#pragma once
#include "Adjacencia.h"
#include "ListaAdjacencia.h"
#include "Loading.h"
#include <climits>
#include <algorithm>
#include <string>
#include <sstream>
using namespace std;


class Grafo{
  public:
	ListaAdjacencia * adj;
	int size;
    Grafo(int size, bool d){
		this->size = size;
		adj = new ListaAdjacencia(size, d);
	}

	bool euleriano() {
		bool euleriano = false;
		int impar_counter = 0;
		for (int i = 0; i < size; i++) {
			int a = adj->adjacentes(i)->size();
			if (a % 2 != 0) {
				impar_counter++;
			}
		}
		if ((impar_counter == 2 || impar_counter == 0) && componentes() == 1) {
			return true;
		}
		return false;
	}

	void visitadosProfundidade(int origem, vector<int> &visitados) {
		bool bool_aux = false;
		for (int i = 0; i < visitados.size(); i++) {
			if(visitados[i] == origem){
				bool_aux = true;
			}
		}
		if (bool_aux) {
			return;
		}
		visitados.push_back(origem);
		vector<int> local_adj;
		local_adj = *adj->adjacentes(origem);
		for (int i = 0; i < local_adj.size(); i++)
			visitadosProfundidade(local_adj[i], visitados);
	}

	bool ciclos_profundidade(int origem, vector<int> &visitados) {
		visitados.push_back(origem);
		vector<int> local_adj;
		local_adj = *adj->adjacentes(origem);
		bool ciclos = false;
		for (unsigned int i = 0; i < local_adj.size(); i++) {
			for (vector<int>::iterator it = visitados.begin(); it != visitados.end(); it++) {
				if (local_adj[i] == *it) {
					return true;
				}
			}
			ciclos = ciclos_profundidade(local_adj[i], visitados);
			if (ciclos)
				return ciclos;
		}
		return false;
	}

	bool ciclo() {
		vector<int> visitados = vector<int>();
		int vertice = 0;
		bool alreadyVisited = false;
		for (unsigned int i = 0; i < size; i++) {
			alreadyVisited = false;
			for (vector<int>::iterator it = visitados.begin(); it != visitados.end(); it++) {
				if (i == *it) {
					alreadyVisited = true;
				}
			}
			if (alreadyVisited) {
				continue;
			}
			if (ciclos_profundidade(i, visitados)) {
				return true;
			}
		}
		return false;
	}

	int componentes()
	{
		vector<int> visitados = vector<int>();
		int vertice = 0;
		int componentes = 1;
		bool alreadyVisited = false;
		for (unsigned int i = 0; i < size; i++) {
			alreadyVisited = false;
			for (vector<int>::iterator it = visitados.begin(); it != visitados.end(); it++) {
				if (i == *it) {
					alreadyVisited = true;
				}
			}
			if (alreadyVisited) {
				continue;
			}
			visitadosProfundidade(i, visitados);
			if (visitados.size() != size) {
				componentes++;
			}
		}
		return componentes;
	}

	void printarVetor(vector<int> vec) {
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


	vector<int> dijkstra(int o, int d) {
		using Dijkstra = struct DijStruct{
			int distance;
			int origem;
			bool done;
		} ;
		vector<Dijkstra> table = vector<Dijkstra>();
		Dijkstra dij;
		dij.distance = INT_MAX;
		dij.origem = -1;
		dij.done = false;
		for (int i = 0; i < size; i++) {
			table.push_back(dij);
		}
		int nextVertice = o;
		int lessDistance, dis;
		table[o].distance = 0;
		table[o].done = true;
		table[o].origem = o;
		while (!table[d].done) {
			for (vector<NoRef>::iterator it = adj->vertices[nextVertice].adjacentes.begin(); it != adj->vertices[nextVertice].adjacentes.end(); it++) {
				dis = it->peso + table[nextVertice].distance;
				if (table[it->node->cod].distance > dis) {
					table[it->node->cod].distance = dis;
					table[it->node->cod].origem = nextVertice;
				}
			}
			lessDistance = INT_MAX;
			for (int i = 0; i < table.size(); i++) {
				if (table[i].distance <= lessDistance && !table[i].done) {
					lessDistance = table[i].distance;
					nextVertice = i;
				}
			}
			table[nextVertice].done = true;
		}
		vector<int> path = vector<int>();
		nextVertice = d;
		while (nextVertice != o) {
			path.insert(path.begin(),nextVertice);
			nextVertice = table[nextVertice].origem;
		}
		path.insert(path.begin(), o);
		return path;
		
	}
	double Grafo::distance(vector<int> path) {
		int totalDistance = 0.0;
		for (vector<int>::iterator it = path.begin(); it + 1 != path.end(); it++) {
			for (vector<NoRef>::iterator jt = adj->vertices[*it].adjacentes.begin(); jt != adj->vertices[*it].adjacentes.end(); jt++) {
				if (*(it + 1) == jt->node->cod) {
					totalDistance += jt->peso;
				}
			}
		}
		return totalDistance;
	}

	double Grafo::closeness(int vertice) {
		double totalDistance = 0.0;
		for (int i = 0; i < size; i++) {
			if (vertice != i) {
				totalDistance += distance(dijkstra(vertice, i));
			}
		}
		return 1 / totalDistance;
	}
	vector<double> Grafo::intermediacao() {
		vector<int> frequencia = vector<int>();
		for (int i = 0; i < size; i++) {
			frequencia.push_back(0);
		}
		int n = 0, total = size * size, hashtags = 0;
		vector<int> tempPath;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				n++;
				if (adj->ndir && j == i)
					continue;
				if (((double) n / (double) total) * 100 >= 3.3 * hashtags) {
					hashtags++;
					Loading::showBar("Calculando Centralidade por Intermediacao", "geodesicos", n + 1, total);
				}
				tempPath = dijkstra(i, j);
				if (tempPath.size() == 2)
					continue;
				tempPath.erase(tempPath.begin());
				tempPath.pop_back();
				for (vector<int>::iterator it = tempPath.begin(); it != tempPath.end(); it++) {
					frequencia[*it] += 1;
				}
			}
		}
		Loading::showBar("Calculando Centralidade por Intermediacao", "geodesicos", total, total);
		vector<double> coefs = vector<double>();
		double totalPaths = 0.0;
		if (adj->ndir) {
			totalPaths = (size - 1) * (size - 2);
		}
		else {
			totalPaths = ((size - 1) * (size - 2)) / 2;
		}
		for (int i = 0; i < frequencia.size(); i++) {
			coefs.push_back(frequencia[i] / totalPaths);
		}
		return coefs;
	}

};
