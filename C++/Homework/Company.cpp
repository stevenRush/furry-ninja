#include <iostream>
#include <vector>

using namespace std;

typedef vector<vector<int> > vector2D;

class DefaultVisitor
{
public:
	void OnNodeExit(int) {	}

};

template<typename Visitor=DefaultVisitor>
class DFSWorker
{
public:
	DFSWorker(const vector2D *graph) :
		m_Graph(graph)
	{
		m_WasHere.assign(m_Graph->size(), false);
	}

	void DFS(int node, Visitor & visitor)
	{
		m_WasHere[node] = true;
		for(vector<int>::const_iterator it = m_Graph->at(node).begin(); it != m_Graph->at(node).end(); ++it)
		{
			if (!m_WasHere[*it])
				DFS(*it, visitor);
		}
		visitor.OnNodeExit(node);
	}

	bool WasHere(int node) const
	{
		return m_WasHere[node];
	}

private:
	const vector2D *m_Graph;
	vector<bool> m_WasHere;
};

void Input(istream & inp, vector2D & graph)
{
	size_t edgeCount, nodeCount;
	inp >> nodeCount;
	inp >> edgeCount;
	graph.assign(nodeCount, vector<int>());
	for(size_t i = 0; i < edgeCount; ++i)
	{
		int player1, player2, result;
		inp >> player1;
		inp >> player2;
		inp >> result;
		--player1;
		--player2;
		switch(result)
		{
			case 1:
				graph[player1].push_back(player2);
				break;
			case 2:
				graph[player2].push_back(player1);
				break;
		}
	}
}

void TransposeGraph(const vector2D & graph, vector2D & transposedGraph)
{
	transposedGraph.assign(graph.size(), vector<int>());
	for(size_t i = 0; i < graph.size(); ++i)
	{
		for(vector<int>::const_iterator it = graph[i].begin(); it != graph[i].end(); ++it)
		{
			transposedGraph[*it].push_back(i);
		}
	}
}

class TopSortVisitor 
{
public:
	void OnNodeExit(int node)
	{
		m_Order.push_back(node);
	}

	vector<int> GetOrder() const
	{
		return m_Order;
	}

private:
	vector<int> m_Order;
};

class ComponentSizeVisitor
{
public:
	ComponentSizeVisitor() :
		m_Size(0)
	{   }

	void OnNodeExit(int node)
	{
		++m_Size;
	}

	size_t GetSize() const
	{
		return m_Size;
	}

private:
	size_t m_Size;
};

int main()
{
	//freopen("C:\\temp\\input.txt", "r", stdin);
	vector2D graph;	
	Input(cin, graph);
	vector2D transposeGraph;
	TransposeGraph(graph, transposeGraph);
	
	DFSWorker<TopSortVisitor> topSorter(&graph);
	TopSortVisitor topSortVisitor;
	for(size_t i = 0; i < graph.size(); ++i)
		if (!topSorter.WasHere(i))
			topSorter.DFS(i, topSortVisitor);
	
	size_t minCompSize = graph.size();
	vector<int> order = topSortVisitor.GetOrder();
	DFSWorker<ComponentSizeVisitor> compSizeCounter(&transposeGraph);
	DFSWorker<> defaultDFS(&graph);
	for(vector<int>::const_reverse_iterator it = order.rbegin(); it != order.rend(); ++it)
	{
		if (!defaultDFS.WasHere(*it) && !compSizeCounter.WasHere(*it))
		{
			ComponentSizeVisitor compSizeVisitor;
			DefaultVisitor defaultVisitor;
			compSizeCounter.DFS(*it, compSizeVisitor);
			if (compSizeVisitor.GetSize() < minCompSize)
				minCompSize = compSizeVisitor.GetSize();
			defaultDFS.DFS(*it, defaultVisitor);
		}
	}

	cout << graph.size() - minCompSize + 1 << endl;

	return 0;
}
