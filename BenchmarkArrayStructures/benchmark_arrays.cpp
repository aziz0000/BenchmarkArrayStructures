#include <chrono>
#include <vector>
#include <numeric>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <string>

// Random numbers (float, double, and int)
std::mt19937 mt(10000);
template <typename T> T randNum(T min, T max) {
	std::uniform_real_distribution<T> dist(min, max);
	return dist(mt);
}
int randInt(int min, int max) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(mt);
}

// ###########################################


using cputime = std::chrono::duration<double, std::micro>;
struct vec3 { double x, y, z; };

// ###########################################


// double[] of size 3N
struct DoubleArray {
	const std::string name = "double[] (size: 3N)";
	int n;
	double *p;
	DoubleArray(int size) { n = size; p = new double[3 * n]; }
	~DoubleArray() { delete[] p; }
	cputime run(double x, double y, double z) {
		auto t0 = std::chrono::steady_clock::now();
		for (auto i = 0; i < n; i++)
		{
			p[3 * i + 0] += x;
			p[3 * i + 1] += y;
			p[3 * i + 2] += z;
		}
		return std::chrono::steady_clock::now() - t0;
	}
};
// ###########################################


// vec3[] of size N
struct Vec3Array {
	const std::string name = "vec3[] (size: N)";
	int n;
	vec3 *p;
	Vec3Array(int size) { n = size; p = new vec3[n]; }
	~Vec3Array() { delete[] p; }
	cputime run(double x, double y, double z) {
		auto t0 = std::chrono::steady_clock::now();
		for (auto i = 0; i < n; i++)
		{
			p[i].x += x;
			p[i].y += y;
			p[i].z += z;
		}
		return std::chrono::steady_clock::now() - t0;
	}
};
// ###########################################


// std::vector<double> of size 3N
struct DoubleVector {
	const std::string name = "std::vector<double> (size: 3N)";
	int n;
	std::vector<double> p;
	DoubleVector(int size) { n = size; p.resize(3 * n, 0.0); }
	cputime run(double x, double y, double z) {
		auto t0 = std::chrono::steady_clock::now();
		for (auto i = 0; i < n; i++)
		{
			p[3 * i + 0] += x;
			p[3 * i + 1] += y;
			p[3 * i + 2] += z;
		}
		return std::chrono::steady_clock::now() - t0;
	}
};
// ###########################################

// std::vector<double> of size 3N
struct Vec3Vector {
	const std::string name = "std::vector<vec3> (size: N)";
	int n;
	std::vector<vec3> p;
	Vec3Vector(int size) { n = size; p.resize(n); }
	cputime run(double x, double y, double z) {
		auto t0 = std::chrono::steady_clock::now();
		for (auto i = 0; i < n; i++)
		{
			p[i].x += x;
			p[i].y += y;
			p[i].z += z;
		}
		return std::chrono::steady_clock::now() - t0;
	}
};
// ###########################################





void benchmark(int n) {
	const auto S = 30; // number of steps (each step is a larger benchmark size)
	const auto R = 3;  // number of repeated benchmarks per point (averaged)
	const auto N = 4;  // number of different array structures

	// different benchmark sizes
	std::vector<int> sizes(S);
	std::generate(sizes.begin(), sizes.end(), [n, m = n]() mutable { auto v = m; m = m + n; return v; });

	// keep benchmark results
	std::vector<std::vector<cputime>> times(N, std::vector<cputime>(S,cputime(0)));
	
	
	double x = randNum(0.0, 1.0);
	double y = randNum(0.0, 1.0);
	double z = randNum(0.0, 1.0);

	// DoubleArray
	for (auto s = 0; s < S; s++) {
		DoubleArray a(sizes[s]);
		std::vector<cputime> b;
		for (auto r = 0; r < R; r++)
			b.push_back(a.run(x, y, z));
		times[0][s] = std::accumulate(b.begin(), b.end(), cputime(0)) / b.size();
	}

	// DoubleVector
	for (auto s = 0; s < S; s++) {
		DoubleVector a(sizes[s]);
		std::vector<cputime> b;
		for (auto r = 0; r < R; r++)
			b.push_back(a.run(x, y, z));
		times[1][s] = std::accumulate(b.begin(), b.end(), cputime(0)) / b.size();
	}

	// Vec3Array
	for (auto s = 0; s < S; s++) {
		Vec3Array a(sizes[s]);
		std::vector<cputime> b;
		for (auto r = 0; r < R; r++)
			b.push_back(a.run(x, y, z));
		times[2][s] = std::accumulate(b.begin(), b.end(), cputime(0)) / b.size();
	}


	// Vec3Vector
	for (auto s = 0; s < S; s++) {
		Vec3Vector a(sizes[s]);
		std::vector<cputime> b;
		for (auto r = 0; r < R; r++)
			b.push_back(a.run(x, y, z));
		times[3][s] = std::accumulate(b.begin(), b.end(), cputime(0)) / b.size();
	}


	// write result to file
	std::ofstream fn("bm_names.txt");
	fn << DoubleArray(0).name << '\n';
	fn << DoubleVector(0).name << '\n';
	fn << Vec3Array(0).name << '\n';
	fn << Vec3Vector(0).name << '\n';
	fn.flush();
	fn.close();


	std::ofstream f("bm.txt");
	// write sizes
	std::copy(sizes.begin(), sizes.end(), std::ostream_iterator<int>(f, "\t"));
	f << '\n';

	// write each structure's benchmark results
	for (auto n = 0; n < N; n++)
	{
		std::transform(times[n].begin(), times[n].end(), std::ostream_iterator<double>(f, "\t"), [](cputime t) {return t.count(); });
		f << '\n';
	}
	f.flush();
	f.close();
}

int main() {
	int startLength;
	std::cin >> startLength;
	benchmark(startLength);

//	std::cin >> startLength;
}