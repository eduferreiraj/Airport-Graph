#pragma once
#include <string>
#include <vector>
using namespace std;

class Adjacencia
{
public:
	int size;
	virtual bool cria_adjacencia(int i, int j, float P) { return false; };  // cria uma adjac�ncia entre i e j com custo P no grafo G;
	virtual bool remove_adjacencia(int i, int j) { return false;  };  // remove a adjac�ncia entre i e j no grafo G;
	virtual void imprime_adjacencias() {};  // imprime a matriz de adjac�ncias do grafo G
	virtual void seta_informacao(int i, string V) {};  // atualiza a informa��o do n� i com o valor V (que deve ser uma string) no grafo G
	virtual vector<int> * adjacentes(int i) { return new vector<int>(); };  // retorna o n�mero de adjacentes ao v�rtice i no grafo G e os armazena no vetor adj
	virtual void imprime_rotulos() {  };
};
