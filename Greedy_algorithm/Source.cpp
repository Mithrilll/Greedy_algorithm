#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <time.h>
#include <iomanip>

int sum(std::vector<int> vec, std::vector<std::vector<int>> G)
{
	int s = 0;

	for (int i = 0; i < vec.size() - 1; i++)
		s += G[vec[i]][vec[i + 1]];
	s += G[vec[vec.size() - 1]][vec[0]];

	return s;
}

void TSP(std::vector<std::vector<int>> G, int start, std::vector<int>& best, std::vector<int>& worst)
{
	std::vector<int> curr;

	int sum_curr = 0;
	int sum_best = 0;
	int sum_worst = 0;

	curr.push_back(start);

	for (int i = 0; i < G.size(); i++)
	{
		if (i != start)
			curr.push_back(i);
	}

	best = curr;
	worst = curr;

	sum_curr = sum(curr, G);

	sum_best = sum_curr;

	sum_best = sum_curr;

	int i = G.size() - 2;
	int j = G.size() - 1;

	while (1)
	{
		while (i > 0 && curr[i] > curr[i + 1])
			i--;

		if (i == 0)
			break;

		while (curr[j] < curr[i] && j > i)
			j--;

		std::swap(curr[i], curr[j]);

		std::sort(curr.begin()+i+1, curr.end());

		sum_curr = sum(curr, G);

		if (sum_curr <= sum_best)
		{
			sum_best = sum_curr;
			best = curr;
		}

		if (sum_curr >= sum_worst)
		{
			sum_worst = sum_curr;
			worst = curr;
		}

		i = G.size() - 2;
		j = G.size() - 1;
	}

	return;
}

bool check(std::vector<int> ans, int j)
{
	for (int i = 0; i < ans.size(); i++)
		if(ans[i] == j)
			return false;

	return true;
}

void print(std::vector<std::vector<int>> G)
{
	for (int i = 0; i < G.size(); i++)
	{
		for (int j = 0; j < G[i].size(); j++)
			std::cout << std::setw(5) << G[i][j] << " ";
		std::cout << std::endl;
	}
}

std::vector<int> Heuristic(std::vector<std::vector<int>> G, int start)
{
	std::vector<int> ans;
	ans.push_back(start);

	for (int i = 0; i < G.size()-1; i++)
	{
		/*std::cout << "-----" << ans[i] << "-----" << std::endl;
		print(G);*/

		int minj = 0;
		for (int j = 0; j < G[ans[i]].size(); j++)
			if (G[ans[i]][j] != 0 && check(ans, j))
				minj = j;

		for (int j = 0; j < G[ans[i]].size(); j++)
			if (G[ans[i]][j] < G[ans[i]][minj] && G[ans[i]][j] != 0 && check(ans, j))
				minj = j;

		ans.push_back(minj);

		for (int j = 0; j < G[ans[i]].size(); j++)
			G[ans[i]][j] = 0;

		for (int j = 0; j < G.size(); j++)
			G[j][minj] = 0;
		
		G[minj][ans[i]] = 0;
	}
	
	return ans;
}

int main()
{
	int n;
	int s;
	//std::cin >> n;
	n = 10;
	srand(time(0));

	int r = 10000;

	std::vector<std::vector<int>> Mat(n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			if (i == j)
				Mat[i].push_back(0);
			else
				Mat[i].push_back(rand() % r + 1);
		}

	//std::cin >> s;
	s = 0;
	print(Mat);

	std::vector<int> best;
	std::vector<int> worst;

	std::chrono::nanoseconds start = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
	TSP(Mat, s, best, worst);
	std::chrono::nanoseconds finish = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());

	for (int i = 0; i < best.size() - 1; i++)
		std::cout << best[i] << "->";
	std::cout << best[best.size() - 1] << "->" << best[0] << " : " << sum(best, Mat) << std::endl;

	for (int i = 0; i < worst.size() - 1; i++)
		std::cout << worst[i] << "->";
	std::cout << worst[worst.size() - 1] << "->" << worst[0] << " : " << sum(worst, Mat) << std::endl;

	double full = (finish.count() - start.count()) / 1000000000.0;

	std::cout << "Full solution time:       " << std::fixed << std::setw(10) << (finish.count()-start.count())/1000000000.0 << std::endl;
	
	start = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
	std::vector<int> so = Heuristic(Mat, s);
	finish = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());

	for (int i = 0; i < so.size() - 1; i++)
		std::cout << so[i] << "->";
	std::cout << so[so.size() - 1] << "->" << so[0] << " : " << sum(so, Mat) << std::endl;

	std::cout << "Heuristic algorithm time: " << std::fixed << std::setw(10) << (finish.count() - start.count()) / 1000000000.0 << std::endl;

	return 0;
}