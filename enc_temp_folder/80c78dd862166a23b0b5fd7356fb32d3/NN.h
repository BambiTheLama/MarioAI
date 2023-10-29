#pragma once
#include <vector>
#include "json.hpp"
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
		return n.x == x && n.y == y;
	}
	void saveToFile(nlohmann::json& j)
	{
		j["ID"] = ID;
		j["x"] = std::round(100 * x) / (100.0f);
		j["y"] = std::round(100 * y) / (100.0f);
	}
	void readFromFile(nlohmann::json& j)
	{
		ID = j["ID"];
		x = j["x"];
		y = j["y"];
	}
};

struct Connection {
	int from, to;
	float w;
	bool active;
	
	bool compare(Connection c)
	{
		return c.from == from && c.to == to;
	}
	void saveToFile(nlohmann::json& j)
	{
		j["from"] = from;
		j["to"] = to;
		j["w"] = std::round(w * 10000) / 10000.0f;
		j["active"] = active;
	}
	void readFromFile(nlohmann::json& j)
	{
		from = j["from"];
		to = j["to"];
		w = j["w"];
		active = j["active"];
	}
};

class NN
{
	const int cellSize = 16;
	std::vector<Node> nodes;
	std::vector<Connection> connections;
public:
	NN();

	NN(nlohmann::json& j);

	void mutate();

	Node findNodeToConect(Node from);

	void addConnection();

	NN* combineNNs(NN *n);

	void addNode();

	void sortConnections();

	void generateOutput();

	void changeValue();

	void setInputs(int* inputs);

	bool* getOutputs();

	void draw(int x, int y);

	void saveToFile(nlohmann::json &j);

	bool hasConnection(Connection c);

};

