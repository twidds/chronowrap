/*
//This is just a benchmark against one of my old TimeClass libraries.
//This will eventually be removed.
*/

#include "chronowrap.hpp"
#include "TimeClass\TimeClass.h"
#include <iostream>

#pragma optimize("", off)

template <class T>
void doNotOptimizeAway(T&& datum) {
	datum = datum;
}

#pragma optimize("", on)


const char CHRONOFORMAT[] = "%Y/%M/%d %H:%m:%s.%x";
const char TIMECLASSFORMAT[] = "YYYY/MM/DD HH:mm:ss.xxx";
const char TIMESTAMP[] = "2018/07/14 22:14:35.243";

static inline double benchmarkchronocreate(size_t nruns)
{
	timestamp t;
	auto t_start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < nruns; ++i)
	{
		t = timestamp();
	}
	doNotOptimizeAway(t);
	return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t_start).count();
}

static inline double benchmarkchronowrite(size_t nruns)
{
	timestamp t;
	auto t_start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < nruns; ++i)
	{
		t.fromstring(TIMESTAMP, CHRONOFORMAT);
	}
	doNotOptimizeAway(t);
	return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t_start).count();
}

static inline double benchmarkchronoread(size_t nruns)
{
	timestamp t;
	std::string out;
	t.fromstring(TIMESTAMP, CHRONOFORMAT);
	auto t_start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < nruns; ++i)
	{
		out = t.tostdstring(CHRONOFORMAT);
	}
	doNotOptimizeAway(t);
	doNotOptimizeAway(out);
	//std::cout << out;
	return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t_start).count();
}

static inline double benchmarktimeclasscreate(size_t nruns)
{
	tmwrap::Timestamp t;
	auto t_start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < nruns; ++i)
	{
		t = tmwrap::Timestamp();
	}
	doNotOptimizeAway(t);
	return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t_start).count();
}

static inline double benchmarktimeclasswrite(size_t nruns)
{
	tmwrap::Timestamp t;
	auto t_start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < nruns; ++i)
	{
		t.SetTimeFromString(TIMESTAMP, TIMECLASSFORMAT);
	}
	return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t_start).count();
}

static inline double benchmarktimeclassread(size_t nruns)
{
	tmwrap::Timestamp t;
	t.SetTimeFromString(TIMESTAMP, TIMECLASSFORMAT);
	std::string out;
	auto t_start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < nruns; ++i)
	{
		out = t.TimeToString();
	}
	return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t_start).count();
}



int main()
{

	const size_t nruns = 1000000;
	//Split size into 4. That's how many in each row

	std::cout << "For " << nruns << " iterations:\n";
	//std::cout << "Size\tVectorIter\tMapIter\t\tVectorInsert\tMapInsert\n";

	auto t_start = std::chrono::high_resolution_clock::now();


	double chronoread;
	double chronowrite;
	double chronocreate;
	double timeclassread;
	double timeclasswrite;
	double timeclasscreate;

	//Creation
	chronocreate = benchmarkchronocreate(nruns);
	timeclasscreate = benchmarktimeclasscreate(nruns);

	//Writing
	chronowrite = benchmarkchronowrite(nruns);
	timeclasswrite = benchmarktimeclasswrite(nruns);

	//Reading
	chronoread = benchmarkchronoread(nruns);
	timeclassread = benchmarktimeclassread(nruns);

	std::chrono::duration<double> t_taken = std::chrono::high_resolution_clock::now() - t_start;


	std::cout << "Test\t\tTimeclass\t\tChronowrap\n";
	std::cout << "Create\t\t" << timeclasscreate << "\t\t" << chronocreate << '\n';
	std::cout << "Read\t\t" << timeclassread << "\t\t" << chronoread << '\n';
	std::cout << "Write\t\t" << timeclasswrite << "\t\t" << chronowrite << '\n';

	std::cout << "\n\nTotal runtime: " << t_taken.count() << " seconds.\n";

	//We shouldn't ever have duplicate units in a tray. That's important. Maybe use a set?
	//Split it half across columns, half across rows?


	////Times to test:

	////Insertion:

	////Iteration:

	////Retrieval


	std::cin.get();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
