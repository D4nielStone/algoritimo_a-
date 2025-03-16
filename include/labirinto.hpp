#include <vector>
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <map>
#include <cmath>  // Para cálculo da distância (heurística)
#include <functional>
#include "no.hpp"

class Labirinto
{
private:
    std::priority_queue<No*, std::vector<No*>, std::function<bool(No*, No*)>> fila;
    std::map<std::pair<int, int>, bool> visitados;
    int num_visitados = 0;
    std::vector<std::vector<char>> layout;
    std::string nome_arquivo;
    int max_width = 0, height = 0;
    No* ponto_B = nullptr;

    // Função para ler o arquivo e preencher a matriz
    void ler_arquivo()
    {
        std::ifstream file(nome_arquivo);
        if (!file) {
            std::cerr << "Erro ao abrir o arquivo!" << std::endl;
            return;
        }

        std::string line;

        // Primeira leitura para determinar dimensões
        while (std::getline(file, line)) {
            height++;
            if (line.length() > max_width) {
                max_width = line.length();
            }
        }

        // Redimensionar a matriz
        layout.resize(height, std::vector<char>(max_width, ' ')); // Preencher com espaços

        // Reposicionar o ponteiro do arquivo para o início
        file.clear();
        file.seekg(0);

        // Segunda leitura para preencher a matriz
        for (int i = 0; i < height; ++i) {
            std::getline(file, line);
            for (int j = 0; j < line.length(); ++j) {
                layout[i][j] = line[j]; // Preencher a matriz com os caracteres da linha
                if (layout[i][j] == 'B') {
                    ponto_B = new No(ivec2{j, i});
                }
            }
        }

        file.close();
    }

    // Método para exibir a matriz
    void exibir() const {
        if (layout.empty()) return;
        for (const auto& linha : layout) {
            for (char c : linha) {
                std::cout << c;
            }
            std::cout << std::endl;
        }
    }

    No descobrir_a()
    {
        for (int i = 0; i < layout.size(); ++i)
        {
            for (int j = 0; j < layout[i].size(); ++j)
            {
                if (layout[i][j] == 'A')
                {
                    std::cout << "Posicao do ponto A: " << i + 1 << " e " << j + 1 << "\n";
                    return No({j, i});
                };
            }
        }
        std::cerr << "Não foi possível achar o ponto A\n";
        return {};
    }

    // Função heurística: Distância de Manhattan
    int heuristica(No* no)
    {
        return std::abs(no->posicao.x - ponto_B->posicao.x) + std::abs(no->posicao.y - ponto_B->posicao.y);
    }

    // Método para descobrir possíveis movimentos
    void descobrir_vizinhos(No* no)
    {
        // verifica vizinho à baixo
        if (no->posicao.y + 1 < layout.size() && layout[no->posicao.y + 1][no->posicao.x] != '#') 
            no->adi_vizinho({no->posicao.x, no->posicao.y + 1});

        // verifica vizinho à cima
        if (no->posicao.y - 1 >= 0 && layout[no->posicao.y - 1][no->posicao.x] != '#') 
            no->adi_vizinho({no->posicao.x, no->posicao.y - 1});

        // verifica vizinho à esquerda
        if (no->posicao.x - 1 >= 0 && layout[no->posicao.y][no->posicao.x - 1] != '#') 
            no->adi_vizinho({no->posicao.x - 1, no->posicao.y});

        // verifica vizinho à direita
        if (no->posicao.x + 1 < layout[0].size() && layout[no->posicao.y][no->posicao.x + 1] != '#') 
            no->adi_vizinho({no->posicao.x + 1, no->posicao.y});
    }

public:
    Labirinto(std::string path) : nome_arquivo(path), fila([](No* a, No* b) {
        return (a->g + a->h) > (b->g + b->h);  // Prioriza os nós com menor custo
    })
    {
        ler_arquivo();
        std::cout << "Labirinto: \n";
        exibir();
    }

    // Método para solucionar labirinto usando A*
    void resolver()
    {
        // Inicia a fila com o nó A
        No* A = new No(descobrir_a());
        A->g = 0;  // Custo do ponto A até A é 0
        A->h = heuristica(A);  // Heurística para A
        visitados[{A->posicao.x, A->posicao.y}] = true;
        fila.push(A);

        // Enquanto a fila não está vazia
        while (!fila.empty())
        {
            No* u = fila.top();  // u é o nó com menor custo (g + h)
            fila.pop();

            // Se chegamos ao ponto B
            if (layout[u->posicao.y][u->posicao.x] == 'B')
            {
                std::cout << "Num visitados: " << std::to_string(num_visitados) << "\n";
                std::cout << "Solucao:\n";

                // Exibe o caminho percorrido
                while (u->pai)
                {
                    u = u->pai;
                    if (layout[u->posicao.y][u->posicao.x] != 'A')
                        layout[u->posicao.y][u->posicao.x] = '*';
                }
                exibir();
                return;
            }

            descobrir_vizinhos(u);

            // Para cada vizinho de u
            for (auto& vizinho : u->vizinhos)
            {
                if (!visitados[{vizinho->posicao.x, vizinho->posicao.y}])
                {
                    visitados[{vizinho->posicao.x, vizinho->posicao.y}] = true;
                    num_visitados++;
                    vizinho->g = u->g + 1;  // Custo acumulado até o vizinho
                    vizinho->h = heuristica(vizinho);  // Heurística do vizinho
                    vizinho->pai = u;
                    fila.push(vizinho);
                }
            }
        }

        std::cerr << "Ponto B nao encontrado.\n";
    }
};
