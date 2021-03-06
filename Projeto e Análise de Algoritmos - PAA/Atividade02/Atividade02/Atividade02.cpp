#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector <int> somas;
vector <string> resultados;
void combinacoes(vector<int> elementos, int k, int p, int sum = 0)
{
	if (k > p)
	{
		for (int i = 0; i < somas.size(); i++) {
			if (somas[i] == sum)
			{
				resultados[i] = "yes";
			}
		}
		return;
	}
	combinacoes(elementos, k + 1, p, sum + elementos[k]);
	combinacoes(elementos, k + 1, p, sum);
}

int main()
{
	int n = 0, m = 0, valor = 0;
	vector <int> elementos;
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> valor;
		elementos.push_back(valor);
	}
	cin >> m;
	valor = 0;
	for (int i = 0; i < m; i++)
	{
		cin >> valor;
		somas.push_back(valor);
		resultados.push_back("no");
	}
	combinacoes(elementos, 0, n - 1);
	for (int i = 0; i < somas.size(); i++) {
		cout << resultados[i] << endl;
	}
	return 0;
}