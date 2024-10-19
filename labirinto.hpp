#include <vector>
#include <iostream>
#include <fstream>
#include <queue>
#include <map>
#include "no.hpp"

class Labirinto
{
private:
    std::queue<No*> fila;
    std::map<std::pair<int, int>, bool> visitados;
	std::vector<std::vector<char>> layout;
	std::string nome_arquivo;
	int max_width = 0, height = 0;

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
            for (size_t j = 0; j < line.length(); ++j) {
                layout[i][j] = line[j]; // Preencher a matriz com os caracteres da linha
            }
        }

        file.close();
}

    // Método para exibir a matriz
    void exibir() const {
    	if(layout.empty())return;
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
    			/* passa a posicao do nó raiz onde é o ponto a */
		    	if(layout[i][j] == 'A')
		    		{
		    			std::cout << "Posicao do ponto A: " << i+1 << " e " << j+1 << "\n";
		    			return No({j, i});
		    		};
    		}
    	}
    	std::cerr << "Não foi possível achar o ponto A\n";
    }
    // Método para descobrir possíveis movimentos
    void descobrir_vizinhos(No* no)
    {
    	//verifica vizinho à baixo
    	if(no->posicao.y + 1 < layout.size() && layout[no->posicao.y + 1][no->posicao.x] != '#') no->adi_vizinho({no->posicao.x, no->posicao.y + 1});

    	//verifica vizinho à cima
    	if(no->posicao.y -1 > 0 && no->posicao.y > 0 && layout[no->posicao.y - 1][no->posicao.x] != '#') no->adi_vizinho({no->posicao.x, no->posicao.y - 1});

    	//verifica vizinho à esquerda
    	if(no->posicao.x -1 > 0 && no->posicao.x > 0 && layout[no->posicao.y][no->posicao.x - 1] != '#') no->adi_vizinho({no->posicao.x - 1, no->posicao.y});

    	//verifica vizinho à direita
    	if(no->posicao.x +1 < layout[0].size() && no->posicao.x < layout[0].size()-1 && layout[no->posicao.y][no->posicao.x + 1] != '#') no->adi_vizinho({no->posicao.x + 1, no->posicao.y});
    }
public:
	Labirinto(std::string path): nome_arquivo(path)
	{
		ler_arquivo();
        std::cout << "Labirinto: \n";
		exibir();
	}
	// Método para solucionar labirinto
    void resolver()
    {
        // Inicia a fila com o nó A
    	No* A = new No(descobrir_a());
        visitados[{A->posicao.x, A->posicao.y}] = true;
        fila.push(A);

        // Enquanto a fila não está vazia
        while(!fila.empty())
        {
            No* u = fila.front(); // u é o Primeiro elemento da fila
            if(layout[u->posicao.y][u->posicao.x] == 'B')
            {
                std::cout << "Solucao:\n";

                while(u->pai)
                {
                    u = u->pai;
                    if(layout[u->posicao.y][u->posicao.x]!='A')layout[u->posicao.y][u->posicao.x] = '*';
                }
                exibir();
                system("pause");

                return;
            }
            fila.pop();           // Remove primeiro elemento da fila
            descobrir_vizinhos(u);
            for(auto &vizinho : u->vizinhos)    // Para cada vizinho de u
            {
                if(!visitados[{vizinho->posicao.x, vizinho->posicao.y}])          // se não foi visitado
                {
                    visitados[{vizinho->posicao.x, vizinho->posicao.y}] = true;
                    vizinho->pai = u;
                    fila.push(vizinho);
                }
            }
        }
        std::cerr << "Ponto B nao encontrado.\n";
        system("pause");
    }
};