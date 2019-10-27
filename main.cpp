#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "ilha.h"

using namespace std;
using namespace std::chrono;

// Comparador entre ilhas (retorna a ilha com maior custo benefício)
bool sort_comparator(Ilha i1, Ilha i2) {
	return (i1.pontos/i1.custo) > (i2.pontos/i2.custo);
}

// Máximo entre dois números
float max(float a, float b) {
	return a > b ? a : b;
}

// GreedySolution: solução gulosa para o problema.
// Pega todas as ilhas, ordena por custo benefício, e vai escolhendo
// a ilha com maior custo benefício sempre que puder (podendo repetir)
void GreedySolution(vector<Ilha> ilhas, int custo_max) {
	// Estabelecendo início do tempo de execução
	auto start_clock = high_resolution_clock::now();

	int custo = custo_max;

	// O sort da biblioteca padrão do C++ usa o intro sort,
	// um algoritmo que se inicia como quicksort, mas caso a
	// pilha de recursão fique muito grande, troca para um heap
	// sort. Pior caso: O(N log N)
	sort(ilhas.begin(), ilhas.end(), sort_comparator);

	// Pega as ilhas com maior custo benefício e vai retirando
	// o seu custo do dinheiro que sobra
	int num_dias = 0, num_pontos = 0;
	for(int i = 0; i < ilhas.size(); i++) {
		while(custo >= 0) {
			// Caso retirar uma dada ilha não seja possível
			// (custa mais do que sobrou), vai pra próxima
			if(custo - ilhas[i].custo < 0) {
				break;
			}
			// Caso contrário, pega essa ilha
			else {
				custo -= ilhas[i].custo;
				num_dias++;
				num_pontos += ilhas[i].pontos;
			}
		}
	}

	// Terminando o tempo de execução e calculando em microssegundos
	auto end_clock = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(end_clock - start_clock);
	int time = int(duration.count());

	cout << num_pontos << " " << num_dias << endl;
}

// DPSolution: solução para o problema usando programação dinâmica.
// É feita uma matriz com linhas = ilhas e colunas = custo máximo, indo
// até o número total de ilhas e o custo máximo fornecido.
// Para cada posição da matriz, é checado o máximo de pontos entre
// incluir uma dada ilha ou não.
void DPSolution(vector<Ilha> ilhas, int custo_max) {
	// Estabelecendo início do tempo de execução
	auto start_clock = high_resolution_clock::now();

	// Matriz que irá guardar os resultados graduais da
	// prog. dinâmica
	float **dp_solutions = new float*[ilhas.size()+1];
	for(int i = 0; i < ilhas.size()+1; i++)
		dp_solutions[i] = new float[custo_max+1];

	int dp_dias = 0;
	// Populando casos base
	for(int i = 0; i < ilhas.size() + 1; i++) {
		dp_solutions[i][0] = 0;
	}
	for(int i = 0; i < custo_max+1; i++) {
		dp_solutions[0][i] = 0;	
	}

	for(int i = 1; i < ilhas.size()+1; i++) {
		for(int j = 1; j < custo_max+1; j++) {
			float max_pontos_com_i = 0;
			// Checando se a ilha pode ser colocada com um custo j
			// (se seu custo é menor ou igual a j, ela pode)
			if(ilhas[i-1].custo <= j) {
				max_pontos_com_i = ilhas[i-1].pontos;
				int sobrando = (int)(j-ilhas[i-1].custo);
				// Caso sobre dinheiro após adicionar essa ilha,
				// adicione o máximo de pontos possível com o dinheiro
				// que sobrou
				if(sobrando >= 0)
					max_pontos_com_i += dp_solutions[i-1][sobrando];
			}

			// O valor de pontos máximo será ou incluindo a ilha ou não
			dp_solutions[i][j] = max(dp_solutions[i-1][j], max_pontos_com_i);
		}
	}

	// Calculando número de dias gastos
	// Começando do final, caso o valor da matriz seja diferente
	// do valor imediatamente acima, quer dizer que essa ilha foi
	// escolhida, logo adiciona um dia.
	int i = ilhas.size();
	int j = custo_max;
	while(i >= 1) {
		if(dp_solutions[i][j] != dp_solutions[i-1][j]) {
			dp_dias++;
			j = (int)(j-ilhas[i-1].custo);
		}
		i--;
	}

	// Terminando o tempo de execução e calculando em microssegundos
	auto end_clock = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(end_clock - start_clock);
	int time = int(duration.count());

	cout << dp_solutions[ilhas.size()][custo_max] << " " << dp_dias << endl;

	for(int i = 0; i < ilhas.size()+1; i++)
		delete [] dp_solutions[i];
	delete [] dp_solutions;
}


int main(int argc, char *argv[]) {

	// Lendo arquivo pela linha de comando
	auto file = std::ifstream(std::string(argv[1]));	
	std::string line;

	// Lendo linha do arquivo e criando um stream para obter
	// informações da linha
	std::getline(file, line);
	auto line_stream = std::stringstream(line);

	int custo_max, num_ilhas;
	
	line_stream >> custo_max >> num_ilhas;

	vector<Ilha> ilhas;
	for(int i = 0; i < num_ilhas; i++) {
		float custo, pontos;

		getline(file,line);
		line_stream = stringstream(line);

		line_stream >> custo >> pontos;
		Ilha ilha_atual;
		ilha_atual.custo = custo;
		ilha_atual.pontos = pontos;
		ilhas.push_back(ilha_atual);
	}

	GreedySolution(ilhas, custo_max);
	DPSolution(ilhas, custo_max);

	file.close();

	return 0;
}
