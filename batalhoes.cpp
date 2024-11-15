#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <set>
#include <queue>
#include <algorithm>
#include <functional> // Necessário para std::function
// #include "FindCapital.cpp"

using namespace std;

// Função para implementar o algoritmo de Tarjan e encontrar os CFCs
void strongconnect(int v, int& index, vector<int>& indices, vector<int>& lowlink,
                   vector<bool>& onStack, stack<int>& S, vector<vector<int>>& sccs,
                   vector<int>& scc_id, const vector<vector<int>>& adj) {
    indices[v] = index;
    lowlink[v] = index;
    index++;
    S.push(v);
    onStack[v] = true;

    for (int w : adj[v]) {
        if (indices[w] == -1) {
            strongconnect(w, index, indices, lowlink, onStack, S, sccs, scc_id, adj);
            lowlink[v] = min(lowlink[v], lowlink[w]);
        } else if (onStack[w]) {
            lowlink[v] = min(lowlink[v], indices[w]);
        }
    }

    if (lowlink[v] == indices[v]) {
        // Encontrou um CFC
        vector<int> scc;
        int w;
        do {
            w = S.top();
            S.pop();
            onStack[w] = false;
            scc_id[w] = sccs.size();
            scc.push_back(w);
        } while (w != v);
        sccs.push_back(scc);
    }
}

int main() {
    int N, M;
    cin >> N >> M;

    map<string, int> name_to_id;
    vector<string> id_to_name;
    vector<vector<int>> adj;

    // Leitura das estradas e mapeamento dos nomes para IDs
    for (int i = 0; i < M; ++i) {
        string u_name, v_name;
        cin >> u_name >> v_name;
        if (name_to_id.find(u_name) == name_to_id.end()) {
            name_to_id[u_name] = id_to_name.size();
            id_to_name.push_back(u_name);
            adj.push_back(vector<int>());
        }
        if (name_to_id.find(v_name) == name_to_id.end()) {
            name_to_id[v_name] = id_to_name.size();
            id_to_name.push_back(v_name);
            adj.push_back(vector<int>());
        }
        int u = name_to_id[u_name];
        int v = name_to_id[v_name];
        adj[u].push_back(v);
    }

    int n = id_to_name.size();
    int index = 0;
    vector<int> indices(n, -1);
    vector<int> lowlink(n, -1);
    vector<bool> onStack(n, false);
    stack<int> S;
    vector<vector<int>> sccs;
    vector<int> scc_id(n, -1);

    // Encontrando os CFCs usando o algoritmo de Tarjan
    for (int v = 0; v < n; ++v) {
        if (indices[v] == -1) {
            strongconnect(v, index, indices, lowlink, onStack, S, sccs, scc_id, adj);
        }
    }

    // Construindo o grafo condensado (grafo dos CFCs)
    int num_sccs = sccs.size();
    vector<set<int>> scc_graph(num_sccs);

    for (int v = 0; v < n; ++v) {
        int scc_v = scc_id[v];
        for (int w : adj[v]) {
            int scc_w = scc_id[w];
            if (scc_v != scc_w) {
                scc_graph[scc_v].insert(scc_w);
            }
        }
    }

    // Invertendo o grafo condensado
    vector<vector<int>> rev_scc_graph(num_sccs);
    for (int u = 0; u < num_sccs; ++u) {
        for (int v : scc_graph[u]) {
            rev_scc_graph[v].push_back(u);
        }
    }

    // Identificando o CFC que contém a capital (supondo que a primeira cidade é a capital)
    string capital_name = id_to_name[0];
    int capital_id = name_to_id[capital_name];
    int capital_scc = scc_id[capital_id];

    // Verificando quais CFCs podem alcançar o CFC da capital
    vector<bool> reachable(num_sccs, false);
    queue<int> q;
    q.push(capital_scc);
    reachable[capital_scc] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : rev_scc_graph[u]) {
            if (!reachable[v]) {
                reachable[v] = true;
                q.push(v);
            }
        }
    }

    // Identificando os CFCs que não podem alcançar a capital
    vector<int> battalion_sccs;
    for (int i = 0; i < num_sccs; ++i) {
        if (!reachable[i]) {
            battalion_sccs.push_back(i);
        }
    }

    // Ordenando os CFCs em ordem topológica reversa
    vector<bool> visited(num_sccs, false);
    vector<int> topo_order;

    function<void(int)> dfs = [&](int u) {
        visited[u] = true;
        for (int v : scc_graph[u]) {
            if (!visited[v]) {
                dfs(v);
            }
        }
        topo_order.push_back(u);
    };

    for (int i = 0; i < num_sccs; ++i) {
        if (!visited[i]) {
            dfs(i);
        }
    }

    // Revertendo a ordem topológica para obter a ordem correta
    reverse(topo_order.begin(), topo_order.end());

    // Selecionando os centros urbanos para instalar os batalhões na ordem correta
    vector<string> battalion_centers;

    for (int scc_idx : topo_order) {
        if (!reachable[scc_idx]) {
            // Selecionamos o centro urbano com o nome lexicograficamente menor no CFC
            vector<int>& scc_vertices = sccs[scc_idx];
            string center_name = id_to_name[scc_vertices[0]];
            for (int v : scc_vertices) {
                if (id_to_name[v] < center_name) {
                    center_name = id_to_name[v];
                }
            }
            battalion_centers.push_back(center_name);
            reachable[scc_idx] = true; // Marca como alcançado para evitar repetição
        }
    }

    // Saída dos resultados
    cout << battalion_centers.size() << endl;
    for (const string& center_name : battalion_centers) {
        cout << center_name << endl;
    }

    return 0;
}
