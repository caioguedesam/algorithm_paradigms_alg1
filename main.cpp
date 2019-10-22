#include <iostream>
#include <vector>
#include <algorithm>
#include "ilha.h"
using namespace std;

bool sort_comparator(Ilha i1, Ilha i2) {
	return (i1.pontos/i1.custo) > (i2.pontos/i2.custo);
}

float max(float a, float b) {
	return a > b ? a : b;
}

void GreedySolution(vector<Ilha> ilhas, int custo_max) {
	sort(ilhas.begin(), ilhas.end(), sort_comparator);

	int num_dias = 0, num_pontos = 0;
	for(int i = 0; i < ilhas.size(); i++) {
		while(custo_max >= 0) {
			if(custo_max - ilhas[i].custo < 0) {
				break;
			}
			else {
				custo_max -= ilhas[i].custo;
				num_dias++;
				num_pontos += ilhas[i].pontos;
			}
		}
	}

	cout << num_pontos << " " << num_dias << endl;
}

void DPSolution(vector<Ilha> ilhas, int custo_max) {
	float dp_solutions[ilhas.size()+1][custo_max+1];
	int dp_dias = 0;
	// Populating base cases
	for(int i = 0; i < ilhas.size() + 1; i++) {
		dp_solutions[i][0] = 0;
	}
	for(int i = 0; i < custo_max+1; i++) {
		dp_solutions[0][i] = 0;	
	}

	for(int i = 1; i < ilhas.size()+1; i++) {
		for(int j = 1; j < custo_max+1; j++) {
			float max_pontos_com_i = 0;
			// Checking if island cost outweights capacity j
			if(ilhas[i-1].custo <= j) {
				max_pontos_com_i = ilhas[i-1].pontos;
				int sobrando = (int)(j-ilhas[i-1].custo);
				if(sobrando >= 0)
					max_pontos_com_i += dp_solutions[i-1][sobrando];
			}

			dp_solutions[i][j] = max(dp_solutions[i-1][j], max_pontos_com_i);
		}
	}

	int i = ilhas.size();
	int j = custo_max;
	while(i >= 1) {
		if(dp_solutions[i][j] != dp_solutions[i-1][j]) {
			dp_dias++;
			j = (int)(j-ilhas[i-1].custo);
		}
		i--;
	}

	cout << dp_solutions[ilhas.size()][custo_max] << " " << dp_dias << endl;
}


int main() {

	int custo_max, num_ilhas;
	
	cin >> custo_max >> num_ilhas;

	vector<Ilha> ilhas;
	for(int i = 0; i < num_ilhas; i++) {
		float custo, pontos;
		cin >> custo >> pontos;
		Ilha ilha_atual;
		ilha_atual.custo = custo;
		ilha_atual.pontos = pontos;
		ilhas.push_back(ilha_atual);
	}

	GreedySolution(ilhas, custo_max);
	DPSolution(ilhas, custo_max);

	return 0;
}
