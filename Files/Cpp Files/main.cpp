#include "..\Header Files\Grafo.h"
#include "..\Header Files\GeradorDeGrafos.h"
#include "..\Header Files\Pajek.h"
#include "..\Header Files\Loading.h"
#include <iostream>
#include <stdio.h>
#include <conio.h>


using namespace std;

int showMA(Grafo * g) {
	cout << "\t\tLista de Adjacencias\n";
	g->adj->imprime_adjacencias();
	cout << "\n\nRotulos:\n";
	g->adj->imprime_rotulos();
	cout << endl;
	cout << "0 - Novo grafo." << endl;
	cout << "1 - Criar adjacencia." << endl;
	cout << "2 - Remover adjacencia." << endl;
	cout << "3 - Atualizar rotulo." << endl;
	cout << "4 - Quantidade de adjacentes." << endl;
	cout << "5 - Dijkstra." << endl;
	cout << "6 - Grafo conexo." << endl;
	cout << "7 - Grafo ciclico." << endl;
	cout << "8 - Grafo euleriano." << endl;
	cout << "9 - Gerar novo Grafo." << endl;
	cout << "10 - Gerar novo Grafo completo." << endl;
	cout << "11 - Salvar Pajek." << endl;
	cout << "12 - Carregar Pajek." << endl;
	cout << "13 - Centralidade por Proximidade." << endl;
	cout << "14 - Centralidade por Intermediacao." << endl;
	cout << "15 - Base Gnutella." << endl;
	cout << "16 - Sair." << endl;
	cout << "\n\nOperacao desejada: ";
	int option;
	cin >> option;
	return option;
}

void printarVetor(vector<int> vec) {
	cout << "{";
	for (int i = 0; i < vec.size() - 1;i++) {
		cout << vec[i] << ", ";
	}
	cout << vec[vec.size() - 1] << "}";
}

bool excecao(int i, int size) {
	if (i < 0 || i > size) {
		cout << "Valor invalido." << endl;
		_getch();
		return false;
	}
	return true;
}


int main() {
	int size;
	cout << "Tamanho do grafo: ";
	cin >> size;
	Grafo * g = new Grafo(size, true);
	int option;
	int i, j;
	while (true) {
		system("cls");
		option = showMA(g);

		if (option == 0) {
			cout << "Tamanho do novo grafo: ";
			cin >> size;
			cout << "Direcional: ";
			cin >> i;
			delete g;
			g = new Grafo(size, !i);
		}
		else if (option == 1) {
			float p;
			cout << "Origem da adjacencia: ";
			cin >> i;
			if (excecao(i, size) == false)
				continue;
			cout << "Destino da adjacencia: ";
			cin >> j;
			if (excecao(j, size) == false)
				continue;
			cout << "Peso: ";
			cin >> p;
			g->adj->cria_adjacencia(i, j, p);
		}
		else if (option == 2) {
			int i, j;
			cout << "Origem da adjacencia: ";
			cin >> i;
			if (excecao(i, size) == false)
				continue;
			cout << "Destino da adjacencia: ";
			cin >> j;
			if (excecao(j, size) == false)
				continue;
			g->adj->remove_adjacencia(i, j);
		}
		else if (option == 3) {
			string rotulo;
			int i;
			cout << "Vertice do rotulo: ";
			cin >> i;
			if (excecao(i, size) == false)
				continue;
			cout << "Rotulo: ";
			cin >> rotulo;
			g->adj->seta_informacao(i, rotulo);
		}
		else if (option == 4) {
			int i;
			cout << "Vertice: ";
			cin >> i;
			if (excecao(i, size) == false)
				continue;
			vector<int> * adj = g->adj->adjacentes(i);
			cout << "Quantidade de adjacencias: " << adj->size() << endl;
			cout << "Adjacencias: ";
			printarVetor(*adj);
			delete adj;
			_getch();
		}
		else if (option == 5) {
			int i, j;
			cout << "Origem: ";
			cin >> i;
			cout << "Destino: ";
			cin >> j;
			cout << "Melhor caminho: ";
			vector<int> geodesico = g->dijkstra(i, j);
			printarVetor(geodesico);
			cout << endl << "Distancia Total: " << g->distance(geodesico) << endl;
			cout << endl;
			_getch();
		}
		else if (option == 6) {
			cout << "Grafo conexo? ";
			int c = g->componentes();
			if (c == 1) {
				cout << "Sim" << endl;
			}
			else {
				cout << "Nao, " << c << " componentes." << endl;
			}
			_getch();
		}
		else if (option == 7) {
			cout << "Grafo ciclico? ";
			if (g->ciclo()) {
				cout << "Sim" << endl;
			}
			else {
				cout << "Nao" << endl;
			}
			_getch();
		}
		else if (option == 8) {
			cout << "Grafo euleriano? ";
			if (g->euleriano()) {
				cout << "Sim" << endl;
			}
			else {
				cout << "Nao" << endl;
			}
			_getch();
		}
		else if (option == 9) {
			int v, a;
			bool conexo;
			cout << "Vertices: ";
			cin >> v;
			cout << "Arestas: ";
			cin >> a;
			cout << "Conexo? ";
			cin >> conexo;
			delete g;
			g = GeradorDeGrafos::gerar(v, a, conexo);
			_getch();

		}
		else if (option == 10) {
			int v, a;
			bool conexo;
			cout << "Vertices: ";
			cin >> v;
			delete g;
			g = GeradorDeGrafos::gerarK(v);
			_getch();

		}
		else if (option == 11) {
			cout << "Salvando Pajek..." << endl;
			Pajek::gravacao(g);
			Loading::printCentered("Pajek salvo, pressione qualquer tecla para continuar.");
			_getch();
		}
		else if (option == 12) {
			cout << "Pajek sendo carregado..." << endl;
			delete g;
			g = Pajek::carregamento();
			Loading::printCentered("Pajek carregado, pressione qualquer tecla para continuar.");
			_getch();
		}
		else if (option == 13) {
			int vertice;
			cout << "Centralidade por Proximidade (Closeness)" << endl << endl;
			cout << "Vertice (-1 para calcular todas): " << endl;
			cin >> vertice;
			vector<double> coefs = vector<double>();
			if (vertice == -1) {
				for (int i = 0; i < g->size; i++) {
					Loading::showBar("Calculando Centralidade por Proximidade", "centralidades calculadas", i + 1, g->size);
					coefs.push_back(g->closeness(i));
				}
				_getch();
				for (int i = 0; i < coefs.size(); i++) {
					cout << "Vertice: " << i << " Coef: " << coefs[i] << endl;
				}
				_getch();

			}
			else {
				cout << "Coeficiente: " << g->closeness(vertice) << endl;
			}
		}
		else if (option == 14) {
			vector<double> intermediacao = g->intermediacao();
			_getch();
			for (int i = 0; i < intermediacao.size(); i++) {
				cout << "Vertice: " << i << " Coef: " << intermediacao[i] << endl;
			}
			_getch();
		}
		else if (option == 15) {
			cout << "Importando Base de Dados..." << endl;
			delete g;
			g = Pajek::importa_bd();
			cout << "Base carregada, pressione qualquer tecla para continuar." << endl;
			_getch();
		}
		else if (option == 16) {
			delete g;
			return 0;
		}
		else {
			cout << "Opcao invalida, pressione qualquer tecla para continuar.";
			option = 0;
			_getch();
		}

	}
	delete g;
	return 0;

}



