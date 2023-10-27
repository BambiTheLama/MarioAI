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
		n.x = 96 * cellSize;
		n.y = i * cellSize*2;
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
	c.w = (rand() % 20000) / 10000.0f - 1.0f;
	c.active = true;
	conections.push_back(c);
}
NN NN::combineNNs(NN n)
{
	NN toRet=NN();
	for (int i = inputsSize + 1 + outputsSize; i < nodes.size(); i++)
	{
		toRet.nodes.push_back(nodes[i]);
	}
	for (int i = inputsSize + 1 + outputsSize; i < n.nodes.size(); i++)
	{
		if (toRet.nodes.size() <= i)
		{
			toRet.nodes.push_back(n.nodes[i]);
		}
		else if (!n.nodes[i].compare(toRet.nodes[i]))
		{
			Node node = n.nodes[i];
			node.ID = toRet.nodes.size();
			toRet.nodes.push_back(node);
		}
	}

	for (auto c1 : conections)
	{
		bool breaked = false;
		for (auto c2 : n.conections)
		{
			if (c1.compare(c2) && c1.active == c2.active)
			{
				breaked = true;
				if (rand() % 2)
					toRet.conections.push_back(c1);
				else
					toRet.conections.push_back(c2);
				break;
			}
			else if (c1.active != c2.active)
			{
				if(c1.active)
					toRet.conections.push_back(c2);
				else
					toRet.conections.push_back(c1);
				breaked = true;
				break;
			}
		}
		if(!breaked)
			toRet.conections.push_back(c1);
	}
	for (auto c1 : n.conections)
	{
		bool breaked = false;
		for (auto c2 : toRet.conections)
		{
			if (c1.compare(c2))
			{
				breaked = true;
				break;
			}
		}
		if (!breaked);

	}

	return toRet;
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

void NN::sortConnections()
{
	std::vector<Node> nodes;
	for (int i = inputsSize + 1 + outputsSize; i < nodes.size(); i++)
		nodes.push_back(this->nodes[i]);
	for (int j = 0; j < nodes.size(); j++)
	{
		for (int i = 0; i < nodes.size() - 1; i++)
		{
			if (nodes[i].x > nodes[i + 1].x)
			{
				Node n = nodes[i];
				nodes[i] = nodes[i + 1];
				nodes[i + 1] = n;
			}
		}
	}
	for (int i = 0; i < outputsSize; i++)
		nodes.push_back(this->nodes[i + inputsSize + 1]);
	int k = 0;
	for (int j = 0; j < nodes.size(); j++)
	{
		int ID = nodes[j].ID;
		for (int i = k; i < conections.size(); i++)
		{
			if (conections[i].to == ID)
			{
				if (k != i)
				{
					Conection c = conections[i];
					conections[i] = conections[k];
					conections[k] = c;
				}
				k++;
				if (k >= conections.size())
					return;
			}
		}
	}


}

void NN::generateOutput()
{
	for (int i = inputsSize + 1; i < nodes.size(); i++)
		nodes[i].value = 0;
	for (auto c : conections)
	{
		nodes[c.to].value += nodes[c.from].value * c.w;
	}
}

void NN::changeValue()
{
	int i = rand() % conections.size();
	conections[i].w *= ((rand() % 4001) / 1000) - 1;
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
	int mutationType = rand() % 100;
	if (conections.size() <= 0 || mutationType <= 55)
	{
		addConection();
	}
	else if(mutationType <= 85)
	{
		addNode();
		sortConnections();
	}
	else
	{
		changeValue();
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
	Color c;
	c.r = 255 * (v > 1 ? 0 : (v < -1) ? 1 : (2-v) / 2);
	c.g = 255 * (v > 1 ? 1 : (v < -1) ? 0 : v / 2);
	c.b = 0;
	c.a = 255;
	return c;
}

void NN::draw(int x, int y)
{
	int size = cellSize / 2;
	for (auto c : conections)
	{
		if (!c.active)
			continue;
		Node form = nodes[c.from];
		Node to = nodes[c.to];
		DrawLine(form.x + size, form.y + size, to.x + size, to.y + size, getColorLineFromValue(c.w));
	}
	for (auto n : nodes)
	{
		DrawRectangleLines(n.x + x, n.y + y, cellSize, cellSize, BLACK);
		DrawRectangle(n.x + x, n.y + y, cellSize, cellSize, getColorFromValue(n.value));
	}

}