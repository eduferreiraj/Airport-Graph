#pragma once
#include <iomanip>
#include <sstream>

using namespace std;

class Loading
{
public:
	// showBar(Titulo da operação, O plural dos elementos que está contando v.g. arestas geodesicos vertices, 
	//			Quantos foi realizado até agora, Quantos iram ser realizados por total);
	static void showBar(string caption, string element, int completed, int total) {
		system("cls");
		int sizeBar = 30;
		double percent = ((double)completed / (double)total)*100.0;
		printCentered(caption);
		printCentered(to_string(completed) + "/" + to_string(total) + " " + element + ".");
		ostringstream out;
		out << setprecision(0) << percent;
		string bar = out.str() + "% [";
		int hashtags = ((double)sizeBar / 100.0) * percent;
		for (int i = 0; i < hashtags; i++) {
			bar += "#";
		}
		for (int i = 0; i < (sizeBar - hashtags); i++) {
			bar += " ";
		}
		bar += "]";
		printCentered(bar);
	}
	static void printCentered(string str) {
		//cout << endl;
		tab(str.size());
		cout << str << endl;
	}
	static void tab(int t) {
		int spaces = (80 - t) / 2;
		//cout << endl << "Tamanho: " << t << "Tab: " << spaces << endl;
		for (int i = 0; i <= spaces; i++)
			cout << " ";
	}
};

