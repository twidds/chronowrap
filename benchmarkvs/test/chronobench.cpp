#include "benchmark/benchmark.h"
#include "chronowrap.hpp"
#include "TimeClass.h"

#include <sstream>
#include <string>

//#define BASIC_BENCHMARK_TEST(x) BENCHMARK(x)->Arg(8)->Arg(512)->Arg(8192)

const char CHRONOFORMAT[] = "%Y/%M/%d %H:%m:%s.%x";
const char TIMECLASSFORMAT[] = "YYYY/MM/DD HH:mm:ss.xxx";
const char TIMESTAMP[] = "2018/07/14 22:14:35.243";


void BM_Empty(benchmark::State &state)
{
	for (auto _ : state);
}

void BM_chrono_tstampcreate(benchmark::State &state)
{
	timestamp t;
	for (auto _ : state)
		benchmark::DoNotOptimize(t = timestamp());

	std::stringstream ss;
	ss << t.isvalid();
	state.SetLabel(ss.str());
}

void BM_chrono_fromstring(benchmark::State &state)
{
	timestamp t;
	for (auto _ : state)
		t.fromstring(TIMESTAMP, CHRONOFORMAT);

	//Avoid optimizations
	std::stringstream ss;
	ss << t.tostdstring(CHRONOFORMAT);
	state.SetLabel(ss.str());
}

void BM_chrono_tostring(benchmark::State &state)
{
	std::string result;
	timestamp t;
	t.fromstring(TIMESTAMP, CHRONOFORMAT);
	for (auto _ : state) result = t.tostdstring(CHRONOFORMAT);

	std::stringstream ss;
	ss << t.tostdstring(CHRONOFORMAT);
	state.SetLabel(ss.str());
}

void BM_chrono_tdiffcreate(benchmark::State &state)
{
	//timediff t(std::chrono::seconds(state.range(0)));
	timediff t = std::chrono::seconds(state.range(0));
	for (auto _ : state)
		benchmark::DoNotOptimize(t = std::chrono::seconds(state.range(0)));

	std::stringstream ss;
	ss << t.asseconds<double>();
	state.SetLabel(ss.str());
}

void BM_chrono_tstampsubtract(benchmark::State &state)
{
	timestamp tstamp1, tstamp2;
	tstamp1.fromstring(TIMESTAMP, CHRONOFORMAT);
	tstamp2 = tstamp1 - std::chrono::seconds(state.range(0));
	timediff t = tstamp2 - tstamp1;

	for (auto _ : state)
		benchmark::DoNotOptimize(t = tstamp1 - tstamp2);

	std::stringstream ss;
	ss << t.asseconds<float>();
	state.SetLabel(ss.str());
}


void BM_timeclass_create(benchmark::State &state) {
	tmwrap::Timestamp t;
	for (auto _ : state)
		t = tmwrap::Timestamp();

	std::stringstream ss;
	ss << t.isValid();
	state.SetLabel(ss.str());
}

void BM_timeclass_fromstring(benchmark::State &state) {
	tmwrap::Timestamp t;
	for (auto _ : state)
		t.SetTimeFromString(TIMESTAMP, TIMECLASSFORMAT);

	std::stringstream ss;
	ss << t.TimeToString();
	state.SetLabel(ss.str());
}

void BM_timeclass_tostring(benchmark::State &state) {
	tmwrap::Timestamp t;
	t.SetTimeFromString(TIMESTAMP, TIMECLASSFORMAT);
	std::string res;
	for (auto _ : state)
		res = t.TimeToString();

	std::stringstream ss;
	ss << res;
	state.SetLabel(ss.str());
}

void BM_timeclass_tdiffcreate(benchmark::State &state) {
	tmwrap::Time t;
	for (auto _ : state)
		benchmark::DoNotOptimize(t = tmwrap::Time(tmwrap::Seconds(state.range(0))));

	std::stringstream ss;
	ss << t.asSeconds();
	state.SetLabel(ss.str());
}









//Uncomment once we get the correct implementation for this in TimeClass
//void BM_timeclass_tstampsubtract(benchmark::State &state) {
//	tmwrap::Time t;
//	tmwrap::Timestamp tstamp1, tstamp2;
//	tstamp1.SetTimeFromString(TIMESTAMP, TIMECLASSFORMAT);
//	tstamp2 = tstamp1 - tmwrap::Seconds(state.range(0));
//
//	for (auto _ : state) {
//		benchmark::DoNotOptimize(t = tstamp2 - tstamp1);
//	}
//	std::stringstream ss;
//
//	state.SetLabel(ss.str());
//}


BENCHMARK(BM_Empty);
BENCHMARK(BM_chrono_tstampcreate);
BENCHMARK(BM_timeclass_create);

BENCHMARK(BM_chrono_fromstring);
BENCHMARK(BM_timeclass_fromstring);

BENCHMARK(BM_chrono_tostring);
BENCHMARK(BM_timeclass_tostring);

BENCHMARK(BM_chrono_tdiffcreate)->Arg(50);
BENCHMARK(BM_timeclass_tdiffcreate)->Arg(50);

BENCHMARK(BM_chrono_tstampsubtract)->Arg(50);
//BENCHMARK(BM_timeclass_tstampsubtract)->Arg(50);

BENCHMARK_MAIN();
