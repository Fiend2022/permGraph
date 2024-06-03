#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <random>
#include <chrono>
#include "Graph.hpp"
using namespace std;


void cliqueTest(size_t n)
{
	vector<size_t> perm(n);
	for (size_t i = 0; i < n; i++)
		perm[i] = i;

	std::random_device dev;
	std::mt19937 rng(dev());
	shuffle(perm.begin(), perm.end(), rng);

	cout << "permutation" << endl;

	for (size_t i = 0; i < n; i++)
		cout << perm[i] << " ";
	cout << endl;
	Graph gpi(perm);
	auto res = gpi.getMaxClique();
	auto sets = gpi.getCliqueSet();
	cout << "Max Clique:" << endl;
	for (auto& i : res)
		cout << i << " ";
	cout << endl;
	sort(res.begin(), res.end(), greater<int64_t>());
	set<size_t> resS(res.begin(), res.end());
	cout << "all max  Cliques:" << endl;
	bool match = false;
	for (auto& i : sets)
	{
		if (i.size() == res.size())
		{
			for (auto j = prev(i.end()); j != i.begin(); j--)
				cout << *j << " ";
			cout << *(i.begin()) << " ";
			cout << endl;
		}
	}
	//cout << "all indepSet:" << endl;
	//for (auto& i : sets)
	//{
	//	for (auto& j : i)
	//		cout << j << " ";
	//	cout << endl;

	//}
}


void indepSetTest(size_t n)
{
	vector<size_t> perm(n);
	for (size_t i = 0; i < n; i++)
		perm[i] = i;

	std::random_device dev;
	std::mt19937 rng(dev());
	shuffle(perm.begin(), perm.end(), rng);

	cout << "permutation" << endl;

	for (size_t i = 0; i < n; i++)
		cout << perm[i] << " ";
	cout << endl;
	Graph gpi(perm);
	auto res = gpi.getMaxIndepSet();
	auto sets = gpi.getIndepSets();
	cout << "Max indepSet:" << endl;
	for (auto& i : res)
		cout << i << " ";
	cout << endl;
	cout << "all max indepSet:" << endl;
	for (auto& i : sets)
	{
		if (i.size() == res.size())
		{
			for (auto& j : i)
				cout << j << " ";
			cout << endl;
		}
	}
	//cout << "all indepSet:" << endl;
	//for (auto& i : sets)
	//{
	//	for (auto& j : i)
	//		cout << j << " ";
	//	cout << endl;

	//}
}
int main()
{
	size_t n = 6;
	vector<size_t> perm(n);
	for (size_t i = 0; i < n; i++)
		perm[i] = i;


	std::random_device dev;
	std::mt19937 rng(dev());
	shuffle(perm.begin(), perm.end(), rng);


	cout << "permutation" << endl;

	for (size_t i = 0; i < n; i++)
		cout << perm[i] << " ";
	cout << endl;

	//Graph gpi(vector<size_t>({4, 5, 2, 0, 3 ,1}));
	//gpi.pRecognition();
}