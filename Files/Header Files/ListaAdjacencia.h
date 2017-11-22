#pragma once
#include "Adjacencia.h"
#include <iostream>
#include <string>
#include <vector>
#define INFINITE 99999
using namespace std;


typedef struct NoRef {
	struct No* node;
	float peso;
} NoRef;

typedef struct No {
	string nome;
	int cod;
	vector<NoRef> adjacentes;
} No;

class ListaAdjacencia : virtual public Adjacencia
{
private:
	int size; //n� de vertices

public:
	int ndir = false;
	vector<No> vertices;
	ListaAdjacencia(int size)
	{

		this->size = size;
		for(int i =0;i < size; i++){
			No node;
			node.cod = i;
			node.nome = '-';
			vertices.push_back(node);
		}
	}
	ListaAdjacencia(int size, bool dir)
	{
		ndir = dir;
		this->size = size;
		for (int i = 0;i < size; i++) {
			No node;
			node.cod = i;
			node.nome = '-';
			vertices.push_back(node);
		}
	}


	~ListaAdjacencia()
	{
  }
	bool direcional() {
		return ndir;
	}

	bool cria_adj(int i, int j, float P) {
		for (vector<NoRef>::iterator it = vertices[i].adjacentes.begin(); it != vertices[i].adjacentes.end(); it++) {
			if (it->node->cod == j) {
				it->peso = P;
				return false;
			}
		}
		NoRef no;
		no.node = &vertices[j];
		no.peso = P;
		vertices[i].adjacentes.push_back(no);
		return true;
	}
	virtual bool cria_adjacencia(int i, int j, float P) {
		bool ret = cria_adj(i, j, P);
		if (ndir) {
			cria_adj(j, i, P);
		}
		return ret;
	}  // cria uma adjac�ncia entre i e j com custo P no grafo G;

	bool remove_adj(int i, int j){
		for (vector<NoRef>::iterator it = vertices[i].adjacentes.begin(); it != vertices[i].adjacentes.end(); it++) {
			if (it->node == &vertices[j]) {
				vertices[i].adjacentes.erase(it);
				return true;
			}
		}
		return false;
	}
	virtual bool remove_adjacencia(int i, int j) {
		bool ret = remove_adj(i, j);
		if (ndir) {
			remove_adj(j, i);
		}
		return ret;
	}  // remove a adjac�ncia entre i e j no grafo G;
	
	
	virtual void imprime_adjacencias() {
		cout << "Lista de adjacentes \nFormato: {Codigo, Peso}\n" << endl;
		for(int i = 0; i < size; i++){
			cout << "#" << i << ": ";
			for(vector<NoRef>::iterator it = vertices[i].adjacentes.begin(); it != vertices[i].adjacentes.end(); it++){
			  cout << "{" << it->node->cod << "," << it->peso << "} ";
			}
			cout << endl;
		}
	}  // imprime a matriz de adjac�ncias do grafo G

	virtual void imprime_rotulos() {
		for (int i = 0; i < size; i++) {
			cout << "#" << i << ": " << vertices[i].nome << endl;
		}
	}

	void seta_informacao(int i, string nome) {
		vertices[i].nome = nome;
	}  // atualiza a informa��o do n� i com o valor V (que deve ser uma string) no grafo G
	
	int n_adjacentes(int i) {
		return vertices[i].adjacentes.size();
	}

	virtual vector<int> * adjacentes(int j) {
		int n = vertices[j].adjacentes.size();
		vector<int> * adj = new vector<int>;
		for (int i = 0; i < n; i++){
		  adj->push_back(vertices[j].adjacentes[i].node->cod);
		}
		return adj;
	}  // retorna o n�mero de adjacentes ao v�rtice i no grafo G e os armazena no vetor adj
};
