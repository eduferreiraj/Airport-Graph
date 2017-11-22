#pragma once
#include "Adjacencia.h"
#include <iostream>
#include <string>
#define INFINITE 99999
using namespace std;

typedef struct No {
	string nome;
} Label;

class MatrizAdjacencia : virtual public Adjacencia
{
private:
	int size; //n� de vertices
	float ** matriz;
	Label * vertices;

public:

	MatrizAdjacencia(int size)
	{
		this->size = size;
		matriz = new float*[size];
		for (int i = 0; i < size; i++)
			matriz[i] = new float[size];

		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				matriz[i][j] = INFINITE;

		vertices = new Label[size];
		for (int i = 0; i < size; i++)
			vertices[i].nome = "-";
	}

	~MatrizAdjacencia()
	{
		delete[] vertices;
		for (int i = 0; i < size; i++)
			delete[] matriz[i];
		delete[] matriz;
	}

	virtual void cria_adjacencia(int i, int j, float P) {
		matriz[i][j] = P;
	}  // cria uma adjac�ncia entre i e j com custo P no grafo G;
	virtual void remove_adjacencia(int i, int j) {
		matriz[i][j] = INFINITE;
	}  // remove a adjac�ncia entre i e j no grafo G;
	virtual void imprime_adjacencias() {
		for (int i = 0;i < size;i++) {
			for (int j = 0;j < size;j++) {
				if (matriz[i][j] != INFINITE) {
					cout << matriz[i][j] << "\t";
				}
				else {
					cout << "#\t";
				}
			}
				//cout << ((matriz[i][j] == INFINITE) ? (char)35 : matriz[i][j]) << "\t";
			cout << endl;
		}
	}  // imprime a matriz de adjac�ncias do grafo G

	virtual void imprime_rotulos() {
		for (int i = 0;i < size; i++) {
			cout << i << ") " << vertices[i].nome << endl;
		}
	}

	void seta_informacao(int i, string nome) {
		vertices[i].nome = nome;
	}  // atualiza a informa��o do n� i com o valor V (que deve ser uma string) no grafo G
	int n_adjacentes(int i) {
		int count = 0;
		for (int j = 0; j < size; j++)
			if (matriz[i][j] != INFINITE)
				count++;
		return count;
	}
	virtual int adjacentes(int i, int * adj) {
		int n = n_adjacentes(i);
		int count = 0;
		for (int j = 0; j < size; j++) {
			if (matriz[i][j] != INFINITE) {
				adj[count] = j;
				count++;
			}
		}
		return n;
	}  // retorna o n�mero de adjacentes ao v�rtice i no grafo G e os armazena no vetor adj
};
