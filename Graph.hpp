#pragma once

#include <vector>
#include <fstream>
#include <functional>
#include <algorithm>
#include <random>
#include <set>
#include <numeric>
#include <functional>
#include <iterator>
#include <list>
#include <iostream>
using namespace std;


vector<size_t> inverse(const vector<size_t>& p);
list<std::set<size_t>>::iterator findSetContainingNumber(list<set<size_t>>& vectorOfSets, size_t number);
vector<size_t> getMaxSubSeq(const vector<size_t>& seq, bool reverse);
template <typename T>
void delVecElem(vector<T>& v, T elem);

class Graph
{
private:
	vector<vector<size_t>> adjLists;
	vector<size_t> perm;
	bool isPerm = false;
	bool indepSetsReady = false;
	bool cliqueSetsReady = false;
	void _BaronKerboshIndSet(std::set<size_t>& res, std::set<size_t>& comp, std::set<size_t>& viewed);
	void _BaronKerboshClique(std::set<size_t>& res, std::set<size_t>& comp, std::set<size_t>& viewed);
	vector<size_t> transitiveOrientation();
	vector<size_t> orderedVertexOrientation(size_t v);
	std::vector<std::set<size_t>> indepSets, cliqueSets;
	bool connectWithOne(std::set<size_t>& comp, std::set<size_t>& viewed);
	bool notConnectWithAll(std::set<size_t>& comp, std::set<size_t>& viewed);
	//vector<vector<size_t>> pathDec();
	bool connectExist(size_t u, size_t v);
public:
	vector<vector<size_t>> pathDec();
	Graph(const vector<size_t>& p);
	Graph(const std::string& filePath);
	Graph(const vector<vector<size_t>>& adjL);
	bool pRecognition();
	size_t pathWidth();
	std::vector<std::set<size_t>> getIndepSets();
	std::vector<std::set<size_t>> getCliqueSet();
	std::vector<size_t> getMaxClique();
	std::vector<size_t> getMaxIndepSet();
	size_t numOfTranspozition()
	{
		size_t num = 0;
		for (size_t i = 0; i < adjLists.size(); i++)
			num += adjLists[i].size();
		return num / 2;
	}
};


std::vector<size_t> Graph::getMaxClique()
{
	return getMaxSubSeq(perm, true);
}

std::vector<size_t> Graph::getMaxIndepSet()
{
	return getMaxSubSeq(perm, false);
}
bool Graph::connectExist(size_t u, size_t v)
{
	return std::find(adjLists[u].begin(), adjLists[u].end(), v) != adjLists[u].end();
}
std::vector<std::set<size_t>> Graph::getIndepSets()
{
	if (indepSetsReady)
		return indepSets;
	std::set<size_t> comp, viewed, res;
	for (size_t i = 0; i < adjLists.size(); i++)
		comp.insert(i);
	_BaronKerboshIndSet(res, comp, viewed);
	indepSetsReady = true;
	return indepSets;
}

bool Graph::connectWithOne(std::set<size_t>& comp, std::set<size_t>& viewed)
{

	for (auto i : viewed)
	{
		bool res = false;
		for (auto j : comp)
			if (connectExist(i, j))
			{
				res = true;
				break;
			}
		if (!res)
			return false;
	}
	return true;
}
void Graph::_BaronKerboshIndSet(std::set<size_t>& res, std::set<size_t>& comp, std::set<size_t>& viewed)
{

	while (!comp.empty() && connectWithOne(comp, viewed))
	{
		size_t u = *(comp.begin());
		comp.erase(u);
		res.insert(u);
		auto newComp = comp;
		auto newViewed = viewed;
		for (auto i : adjLists[u])
		{
			newComp.erase(i);
			newViewed.erase(i);
		}
		if (newComp.empty() && newViewed.empty())
			indepSets.push_back(res);
		else
			_BaronKerboshIndSet(res, newComp, newViewed);
		res.erase(u);
		comp.erase(u);
		viewed.insert(u);
	}
	return;
}

Graph::Graph(const vector<size_t>& p)
{
	bool isPerm = true;
	perm = p;
	auto inv = inverse(p);
	adjLists.resize(p.size());
	for (size_t i = 0; i < p.size(); i++)
		for (size_t j = 0; j < p.size(); j++)
		{
			long long int d = int(i - j) * int(inv[i] - inv[j]);
			if (d < 0)
				adjLists[i].push_back(j);
		}
			

}

Graph::Graph(const std::string& filePath)
{
	std::ifstream file(filePath);
	int dist;
	size_t numOfvert;
	file >> numOfvert;
	size_t currentVert = 0;
	adjLists.resize(numOfvert);
	while (currentVert < numOfvert)
	{
		std::vector<size_t> list;
		for (size_t i = 0; i < numOfvert; i++)
		{
			file >> dist;
			if (dist != 0)
				list.push_back(i);
		}
		sort(list.begin(), list.end());
		adjLists[currentVert++] = list;
	}
}
Graph::Graph(const vector<vector<size_t>>& adjL)
{
	this->adjLists = adjL;
}






vector<size_t> getMaxSubSeq(const vector<size_t>& seq, bool reverse = false)
{
	vector<int64_t> d(seq.size()), pos(seq.size()), prev(seq.size());
	function<bool(int64_t, int64_t)> comp;
	if (!reverse)
	{
		fill(d.begin(), d.end(), INT32_MAX);
		d[0] = INT32_MIN;
		comp = less<int64_t>();
	}
	else
	{
		fill(d.begin(), d.end(), INT32_MIN);
		d[0] = INT32_MAX;
		comp = greater<int64_t>();
	}
	
	size_t len = 0;
	pos[0] = -1;
	for (size_t i = 0; i < seq.size(); i++)
	{
		size_t j = int(upper_bound(d.begin(), d.end(), seq[i], comp) - d.begin());
		d[j] = seq[i];
		pos[j] = i;
		prev[i] = pos[j - 1];
		len = max(len, j);

	}
	vector<size_t> answer;
	int64_t p = pos[len];
	while (p != -1)
	{
		answer.push_back(static_cast<size_t>(seq[p]));
		p = prev[p];
	}
	std::reverse(answer.begin(), answer.end());
	return answer;
}

bool Graph::pRecognition()
{
	/*if (isPerm)
		return true;*/
	auto v = transitiveOrientation();

	return false;
}


vector<size_t> Graph::transitiveOrientation()
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, adjLists.size() - 1);
	auto l = this->orderedVertexOrientation(2);
	return this->orderedVertexOrientation(l[l.size() - 1]);
}



//
//vector<size_t> Graph::orderedVertexOrientation(size_t v)
//{
//	list<set<size_t>> l;
//	l.push_back({ v });
//	vector<size_t> V(adjLists.size());
//	auto adjCopy = adjLists;
//
//	iota(V.begin(), V.end(), 0);
//	iter_swap(find(V.begin(), V.end(), v), prev(V.end()));
//	V.pop_back();
//	l.push_back(set<size_t>(V.begin(), V.end()));
//	auto x = v;
//	while (find_if(l.begin(), l.end(),
//		[](const set<size_t>& V) {return V.size() > 1; }) != l.end())
//	{
//		auto X = findSetContainingNumber(l, x);
//		size_t xPos = distance(l.begin(), X);
//		vector<size_t> alpha
//			0;
//
//
//	}
//	vector<size_t> res(adjLists.size());
//	auto j = res.begin();
//	for (auto& k : l)
//		*j++ = *(k.begin());
//	return res;
//}


vector<size_t> Graph::orderedVertexOrientation(size_t v)
{
	list<set<size_t>> l;
	l.push_back({ v });
	vector<size_t> V(adjLists.size());
	auto adjCopy = adjLists;
	
	iota(V.begin(), V.end(), 0);
	iter_swap(find(V.begin(), V.end(), v), prev(V.end()));
	V.pop_back();
	l.push_back(set<size_t>(V.begin(), V.end()));
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, adjLists.size() - 1); 
	
	
	while (find_if(l.begin(), l.end(),
	[](const set<size_t>& V) {return V.size() > 1; }) != l.end())
	{
		size_t x;
		std::cin >> x;
		auto X = findSetContainingNumber(l, x);
		vector<list<set<size_t>>::iterator> alpha(l.size() - 1);
		auto it1 = l.begin();
		auto it2 = alpha.begin();
		while (it2 != alpha.end())
		{
			if (it1 != X)
			{
				*it2 = it1;
				it2++;
			}
			it1++;
		}

		size_t lSize = l.size();
		for (auto& j : alpha)
		{
			auto Y = j;
			if (Y != X)
			{

				set<size_t> Ya, Yn;
				set_intersection((*Y).begin(), (*Y).end(),
					adjCopy[x].begin(), adjCopy[x].end(), inserter(Ya, Ya.end()));
				set_difference((*Y).begin(), (*Y).end(),
					Ya.begin(), Ya.end(), inserter(Yn, Yn.end()));
				*Y = Yn;
				auto yaPos = Y;
				if (distance(l.begin(),Y) > distance(l.begin(), X))
					l.insert(++yaPos, Ya);
				else
					l.insert(yaPos, Ya);
				if (Ya.empty()) l.remove(Ya);
				if (Yn.empty()) l.remove(*Y);
				for (auto y : Ya)
				{
					delVecElem(adjCopy[x], y);
					delVecElem(adjCopy[y], x);
				}
			}
		}
	}
	vector<size_t> res(adjLists.size());
	auto j = res.begin();
	for (auto& k : l)
		*j++ = *(k.begin());
	return res;
}
vector<size_t> inverse(const vector<size_t>& p)
{
	vector<size_t> arr2(p.size());
	for (int i = 0; i < arr2.size(); i++)
		arr2[p[i]] = i;
	return arr2;
}

vector<vector<size_t>> Graph::pathDec()
{
	auto invertPi = inverse(perm);
	auto n = perm.size();
	vector<vector<size_t>> X(perm.size());
	for (size_t i = 0; i < n; i++)
	{
		if (invertPi[i] == i)
			X[i].push_back(i);
		else if(invertPi[i] > i)
			for (size_t k = i; k < invertPi[i]; k++)
				X[k].push_back(i);
		else
			for (size_t k = invertPi[i]; k < i; k++)
				X[k].push_back(i);
	}
	return X;
}

size_t Graph::pathWidth()
{
	vector<vector<size_t>> path = pathDec();
	auto m = *(std::max_element( path.begin(), path.end(),
		[](const std::vector<size_t>& a, const std::vector<size_t>& b) {
			return a.size() < b.size();
		}));
	return m.size()-1;
}

void Graph::_BaronKerboshClique(std::set<size_t>& res, std::set<size_t>& comp, std::set<size_t>& viewed)
{

	while (!comp.empty() && notConnectWithAll(comp, viewed))
	{
		size_t u = *(comp.begin());
		comp.erase(u);
		res.insert(u);
		set<size_t> newComp = comp, newViewed = viewed;
		for (auto i = 0; i < adjLists.size();i++)
		{
			if (!connectExist(u, i))
			{
				newComp.erase(i);
				newViewed.erase(i);
			}
		}
		if (newComp.empty() && newViewed.empty())
			cliqueSets.push_back(res);
		else
			_BaronKerboshClique(res, newComp, newViewed);
		res.erase(u);
		comp.erase(u);
		viewed.insert(u);
	}
	return;
}

bool Graph::notConnectWithAll(std::set<size_t>& comp, std::set<size_t>& viewed)
{

	for (auto i : viewed)
	{
		bool res = false;
		for (auto j : comp)
			if (!connectExist(i, j))
			{
				res = true;
				break;
			}
		if (!res)
			return false;
	}
	return true;
}


std::vector<std::set<size_t>> Graph::getCliqueSet()
{
	if (cliqueSetsReady)
		return cliqueSets;
	std::set<size_t> comp, viewed, res;
	for (size_t i = 0; i < adjLists.size(); i++)
		comp.insert(i);
	_BaronKerboshClique(res, comp, viewed);
	cliqueSetsReady = true;
	return cliqueSets;
}

list<set<size_t>>::iterator findSetContainingNumber(list<set<size_t>>& vectorOfSets, size_t number) {
	for (auto it = vectorOfSets.begin(); it != vectorOfSets.end(); it++)
		if ((*it).find(number) != (*it).end())
			return it;

}

template <typename T>
void delVecElem(vector<T>& v, T elem)
{
	auto it = find(v.begin(), v.end(), elem);
	if (it != v.end())
	{
		iter_swap(it, prev(v.end()));
		v.pop_back();
	}
	sort(v.begin(), v.end());
}