#pragma once
#include <vector>
#define inputsSizeH 11
#define inputsSizeW 17
#define inputsSize 187
#define outputsSize 5

enum class NodeType
{
	input,output,hiden
};

struct Node {
	int ID;
	float value;
	NodeType type;
	int x, y;
};

struct Conection {
	int from, to;
	float w;
	bool active;
};

class NN
{
	const int cellSize = 16;
	std::vector<Node> nodes;
	std::vector<Conection> conections;
public:
	NN();

	void mutate();

	Node findNodeToConect(Node from);

	void addConection();

	void addNode();

	void setInputs(int* inputs);

	bool* getOutputs();

	void draw(int x, int y);
};

