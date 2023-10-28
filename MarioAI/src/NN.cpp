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
NN::NN(nlohmann::json& j):NN()
{
	for (int i = 0; i < j["NODES"].size(); i++)
	{
		Node n;
		n.readFromFile(j["NODES"][i]);
		nodes.push_back(n);
	}
	for (int i = 0; i < j["Connections"].size(); i++)
	{
		Connection c;
		c.readFromFile(j["Connections"][i]);
		connections.push_back(c);
	}
}

void NN::addConnection()
{
	int n = inputsSize + 1;
	int hidenStart = inputsSize + outputsSize + 1;

	int value = rand() % (nodes.size() - outputsSize);
	Node from;
	if (value < inputsSize + 1)
	{
		from = nodes[value];
	}
	else
	{
		from = nodes[value - outputsSize + hidenStart];
	}
	Node to = findNodeToConect(from);
	Connection c;
	c.from = from.ID;
	c.to = to.ID;
	c.w = (rand() % 20000) / 10000.0f - 1.0f;
	c.active = true;
	connections.push_back(c);
}

struct NodesID {
	int ID1, ID2;
};

int getNewID(int lastID, std::vector<NodesID> newIDs)
{
	for (int i = 0; i < newIDs.size(); i++)
		if (lastID == newIDs[i].ID2)
			return newIDs[i].ID1;
	return lastID;
}
bool randomSwapConnetions(Connection c, std::vector<Connection>& connections)
{
	for(int i=0;i< connections.size();i++)
		if (c.compare(connections[i]))
		{
			if (c.active == connections[i].active)
			{
				bool swap = rand() % 2;
				if (swap)
					connections[i] = c;
			}
			else
			{
				if (!c.active)
					connections[i] = c;
			}
			return true;
		}
	return false;
}
NN* NN::combineNNs(NN *n)
{
	NN* toRet=new NN();
	std::vector<NodesID> doubleNodes = std::vector<NodesID>();
	std::vector<Node> nnnodes = std::vector<Node>();
	for (int i = inputsSize + 1 + outputsSize; i < nodes.size(); i++)
	{
		nnnodes.push_back(nodes[i]);
	}
	for (int i = inputsSize + 1 + outputsSize; i < n->nodes.size(); i++)
	{
		nnnodes.push_back(n->nodes[i]);
	}
	for (int i = 0; i < nnnodes.size(); i++)
	{
		for (int j = nnnodes.size() - 1; j > i; j--)
		{
			if (nnnodes[i].compare(nnnodes[j]))
			{
				NodesID s;
				s.ID1 = nnnodes[i].ID;
				s.ID2 = nnnodes[j].ID;
				doubleNodes.push_back(s);
				nnnodes.erase(nnnodes.begin()+j);
			}
		}

	}
	if (nnnodes.size() > 0)
	{
		int ID = toRet->nodes.size();
		for (int i = 0; i < nnnodes.size(); i++)
		{
			if (nnnodes[i].ID != ID + i)
			{
				NodesID s;
				s.ID1 = ID + i;
				s.ID2 = nnnodes[i].ID;
				doubleNodes.push_back(s);
				nnnodes[i].ID = ID + i;
			}
			toRet->nodes.push_back(nnnodes[i]);
		}

	}
	
	for (int i = 0; i < connections.size(); i++)
	{
		toRet->connections.push_back(connections[i]);
	}
	for (int i = 0; i < n->connections.size(); i++)
	{
		Connection con;
		con.from = getNewID(n->connections[i].from, doubleNodes);
		con.to = getNewID(n->connections[i].to, doubleNodes);
		con.active = n->connections[i].active;
		con.w = n->connections[i].w;
		if (!randomSwapConnetions(con, toRet->connections) && con.from < toRet->nodes.size() && con.to < toRet->nodes.size())
			toRet->connections.push_back(con);
	}

	return toRet;
}

void NN::addNode()
{
	int v = rand() % connections.size();
	Connection c = connections[v];
	connections[v].active = false;
	Node from = nodes[c.from];
	Node to = nodes[c.to];
	Node newNode;
	newNode.x = (from.x + to.x) / 2;
	newNode.y = (from.y + to.y) / 2;
	newNode.ID = nodes.size();
	Connection newC1;
	newC1.from = from.ID;
	newC1.to = newNode.ID;
	newC1.w = c.w;
	newC1.active = true;
	Connection newC2;
	newC2.from = newNode.ID;
	newC2.to = to.ID;
	newC2.w = c.w;
	newC2.active = true;
	nodes.push_back(newNode);
	connections.push_back(newC1);
	connections.push_back(newC2);
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
		for (int i = k; i < connections.size(); i++)
		{
			if (connections[i].to == ID)
			{
				if (k != i)
				{
					Connection c = connections[i];
					connections[i] = connections[k];
					connections[k] = c;
				}
				k++;
				if (k >= connections.size())
					return;
			}
		}
	}


}

void NN::generateOutput()
{
	for (int i = inputsSize + 1; i < nodes.size(); i++)
		nodes[i].value = 0;
	for (auto c : connections)
	{
		if(c.active)
			nodes[c.to].value += nodes[c.from].value * c.w;
	}
}

void NN::changeValue()
{
	int i = rand() % connections.size();
	connections[i].w *= ((rand() % 4001) / 1000) - 1;
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
	int mutationType = rand() % 200;
	if (connections.size() <= 0 || mutationType <= 55)
	{
		addConnection();
	}
	else if(mutationType <= 85)
	{
		addNode();
		sortConnections();
	}
	else if(mutationType<=100)
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
	if (v == 1)
		return { 255,255,255,255 };
	if (v == -1)
		return { 255,0,0,255 };
	if (v == 2)
		return { 0,255,0,255 };
	if (v < 1 && v>-1)
		return { (unsigned char)(255 * (-v)),(unsigned char)(255 * v),0,255 };
	if(v>1)
		return { 0,(255),0,255 };
	if (v < -1)
		return { 255,0,0,255 };
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
	for (auto c : connections)
	{
		if (!c.active)
			continue;
		Node form = nodes[c.from];
		Node to = nodes[c.to];
		DrawLine(form.x + size + x, form.y + size + y, to.x + size + x, to.y + size + y, getColorLineFromValue(c.w));
	}
	for (auto n : nodes)
	{
		DrawRectangle(n.x + x, n.y + y, cellSize, cellSize, getColorFromValue(n.value));
		DrawRectangleLines(n.x + x, n.y + y, cellSize, cellSize, BLACK);

	}

}

void NN::saveToFile(nlohmann::json &j)
{
	int k = 0;
	for (int i = inputsSize + 1 + outputsSize; i < nodes.size(); i++)
	{
		nodes[i].saveToFile(j["NODES"][k]);
		k++;
	}
	k = 0;
	for (auto c:connections)
	{
		c.saveToFile(j["Connections"][k]);
		k++;
	}
}