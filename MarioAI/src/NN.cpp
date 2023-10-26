#include "NN.h"

NN::NN()
{

	for (int i = 0; i < inputsSize; i++)
	{
		Node n;
		n.ID = nodes.size();
		n.type = NodeType::input;
		n.value = -1;
		n.x = (i % inputsSizeW) * cellSize;
		n.y = (i / inputsSizeW) * cellSize;
		nodes.push_back(n);
	}
	{
		Node n;
		n.ID = nodes.size();
		n.type = NodeType::input;
		n.value = 1;
		n.x = ((inputsSize - 1) % inputsSizeW) * cellSize;
		n.y = (inputsSize / inputsSizeW + 1) * cellSize;
		nodes.push_back(n);
	}
	for (int i = 0; i < outputsSize; i++)
	{
		Node n;
		n.ID = nodes.size();
		n.type = NodeType::output;
		n.value = 0;
		n.x = 64 * cellSize;
		n.y = i * cellSize*1.5;
		nodes.push_back(n);
	}

}

void NN::addConection()
{
	int n = inputsSize + 1;
	int hidenStart = inputsSize + outputsSize + 1;
	for (int i = hidenStart; i < nodes.size(); i++)
	{
		n++;
	}
	int value = rand() % n;
	Node from;
	if (value < inputsSize + 1)
	{
		from = nodes[value];
	}
	else
	{
		from = nodes[value + outputsSize];
	}
	Node to = findNodeToConect(from);
	Conection c;
	c.from = from.ID;
	c.to = to.ID;
	c.w = ((rand() % 20000001) - 10000000.0f)/ 10000000.0f;
	c.active = true;
	conections.push_back(c);
}

void NN::addNode()
{
	int v = rand() % conections.size();
	Conection c = conections[v];
	Node from = nodes[c.from];
	Node to = nodes[c.to];
	Node newNode;
	newNode.x = (from.x + to.x) / 2;
	newNode.y = (from.y + to.y) / 2;
	newNode.ID = nodes.size();
	c.active = false;
	Conection newC1;
	newC1.from = from.ID;
	newC1.to = newNode.ID;
	newC1.w = c.w;
	newC1.active = true;
	Conection newC2;
	newC2.from = newNode.ID;
	newC2.to = to.ID;
	newC2.w = c.w;
	newC2.active = true;
	nodes.push_back(newNode);
	conections.push_back(newC1);
	conections.push_back(newC2);
}

Node NN::findNodeToConect(Node from)
{
	std::vector<Node> nodesToConect;
	for (int i = 0; i < outputsSize; i++)
	{
		nodesToConect.push_back(nodes[i + inputsSize + 1]);
	}
	for (int i = inputsSize + 1 + outputsSize; i < nodes.size(); i++)
	{
		Node n = nodes[i];
		if (n.x > from.x)
		{
			nodesToConect.push_back(n);
		}
	}
	int v = rand() % nodesToConect.size();
	return nodesToConect[v];
}

void NN::mutate()
{
	bool addC = rand() % 2;
	if (conections.size() <= 0 || addC)
	{
		addConection();
	}
	else
	{
		addNode();
	}
}

void NN::setInputs(int* inputs)
{
	for (int i = 0; i < inputsSize; i++)
	{
		nodes[i].value = inputs[i];
	}
}

bool* NN::getOutputs()
{
	bool outputs[outputsSize];
	for (int i = 0; i < outputsSize; i++)
	{
		outputs[i] = (nodes[i + inputsSize + 1].value > 0.5f);
	}
	return outputs;
}

#include "raylib.h"

Color getColorFromValue(float v)
{
	if (v == 0)
		return { 0,0,0,0 };
	if (v >= 0)
		return { 255,255,255,255 };
	return { 0,0,0,255 };
}

Color getColorLineFromValue(float v)
{
	if (v >= 0)
		return { 0,(unsigned char)(255 * (v > 1 ? 1 : v)),0,255 };
	return { (unsigned char)(255 * (v > 1 ? 1 : v)),0,0,255 };
}

void NN::draw(int x, int y)
{
	for (auto n : nodes)
	{
		DrawRectangleLines(n.x + x, n.y + y, cellSize, cellSize, BLACK);
		DrawRectangle(n.x + x, n.y + y, cellSize, cellSize, getColorFromValue(n.value));
	}
	int size = cellSize / 2;
	for (auto c : conections)
	{
		if (!c.active)
			continue;
		Node form = nodes[c.from];
		Node to = nodes[c.to];
		DrawLine(form.x + size, form.y + size, to.x + size, to.y + size, getColorLineFromValue(c.w));
	}
}