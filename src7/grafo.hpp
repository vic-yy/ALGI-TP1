#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <map>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <limits>

class Grafo {
private:
    int numCidades;
    std::map<std::string, int> city_to_index;
    std::vector<std::string> index_to_city;
    std::vector<std::vector<int>> adjacencias;
    std::vector<std::vector<int>> adjacenciasTranspostas;

    void dfsOriginal(int v, std::vector<bool>& visitado, std::stack<int>& pilha);
    void dfsTransposto(int v, std::vector<bool>& visitado, std::vector<int>& componente);

public:
    Grafo(int numCidades);
    void adicionarCidade(const std::string& cidade);
    void adicionarEstrada(const std::string& cidadeA, const std::string& cidadeB);
    std::pair<bool, int> bfs(int start);
    std::vector<int> bfsDistancias(int start);
    std::string encontrarCapital();
    std::vector<std::vector<int>> encontrarComponentesFortementeConexas();
    std::vector<std::string> contarBatalhoesSecundarios();
    std::vector<std::vector<std::string>> patrulhamento();
};

#endif
