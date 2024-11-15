#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <unordered_map>
#include <string>

using namespace std;

const int INF = numeric_limits<int>::max();

vector<vector<pair<int, int>>> grafo;
unordered_map<string, int> nome_para_indice;
vector<string> indice_para_nome;

// Função para executar o algoritmo de Dijkstra
vector<int> dijkstra(int origem, int n) {
    vector<int> dist(n, INF);
    dist[origem] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, origem});

    while (!pq.empty()) {
        int distancia = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (distancia > dist[u]) continue;

        for (auto &vizinho : grafo[u]) {
            int v = vizinho.first;
            int peso = vizinho.second;

            if (dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

// Função para encontrar o centro urbano mais adequado como capital
int encontrarCapital(int n) {
    int melhor_candidato = -1;
    int menor_diametro = INF;

    for (int i = 0; i < n; i++) {
        vector<int> distancias = dijkstra(i, n);

        int diametro = *max_element(distancias.begin(), distancias.end());
        if (diametro < menor_diametro) {
            menor_diametro = diametro;
            melhor_candidato = i;
        }
    }

    return melhor_candidato;
}

// Função para mapear o nome de um centro urbano para um índice
int obterIndice(string nome) {
    if (nome_para_indice.find(nome) == nome_para_indice.end()) {
        int indice = nome_para_indice.size();
        nome_para_indice[nome] = indice;
        indice_para_nome.push_back(nome);
    }
    return nome_para_indice[nome];
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <nome_do_arquivo_de_entrada>" << endl;
        return 1;
    }

    string nome_arquivo = argv[1];
    ifstream entrada(nome_arquivo);
    if (!entrada) {
        cerr << "Erro ao abrir o arquivo de entrada: " << nome_arquivo << endl;
        return 1;
    }

    int n, e;
    entrada >> n >> e;

    grafo.assign(n, vector<pair<int, int>>());

    for (int i = 0; i < e; i++) {
        string nome1, nome2;
        entrada >> nome1 >> nome2;
        int u = obterIndice(nome1);
        int v = obterIndice(nome2);
        grafo[u].push_back({v, 1});  // peso 1 para cada estrada
    }

    entrada.close();

    int capital = encontrarCapital(n);
    if (capital != -1) {
        cout << "O melhor candidato a capital é o centro urbano: " << indice_para_nome[capital] << endl;
    } else {
        cout << "Não foi possível encontrar um candidato a capital.\n";
    }

    return 0;
}
