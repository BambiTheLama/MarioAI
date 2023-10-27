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
	float x, y;

	bool compare(Node n)
	{
		return n.x == x && n.y == y && n.ID == ID;
	}
};

struct Conection {
	int from, to;
	float w;
	bool active;
	
	bool compare(Conection c)
	{
		return c.from == from && c.to == to;
	}

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

	NN combineNNs(NN n);

	void addNode();

	void sortConnections();

	void generateOutput();

	void changeValue();

	void setInputs(int* inputs);

	bool* getOutputs();

	void draw(int x, int y);

};

