#include <vector>

struct ivec2
{
	int x = 0, y = 0;
};
struct No
{
	ivec2 posicao;
	float g{0}, h{0};
	std::vector<No*> vizinhos;
	No* pai {nullptr};
	No(){};
	No(const ivec2& pos) : posicao(pos){};
	void adi_vizinho(const ivec2& pos)
	{
		vizinhos.push_back(new No(pos));
	};
};