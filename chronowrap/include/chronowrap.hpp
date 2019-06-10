#pragma once

#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
//#include <cstdlib>

const int S_IN_MINUTE = 60;
const int S_IN_HOUR = S_IN_MINUTE * 60;
const int S_IN_DAY = S_IN_HOUR * 24;
const int S_IN_WEEK = S_IN_DAY * 7;

inline std::vector<std::string> splitstring(const std::string& input, char delim)
{
	std::vector<std::string> ret;
	ret.reserve(std::count(input.begin(), input.end(), delim) + 1);
	size_t pos = 0;
	size_t next = input.find_first_of(delim);

	while (next != std::string::npos)
	{
		ret.push_back(input.substr(pos, next - pos));
		pos = ++next;
		next = input.find_first_of(delim, next);
	}
	ret.push_back(input.substr(pos));
	return ret;
}

inline std::string pad0left(const std::string& in, size_t length)
{
	if (in.length() >= length)
		return in;
	return std::string(length - in.length(), '0') + in;
}

inline std::string pad0right(const std::string& in, size_t length)
{
	if (in.length() >= length)
		return in;
	return in + std::string(length - in.length(), '0');
}

inline bool isinteger(const std::string& in)
{
	if (!in.size()) return false;
	for (auto c : in)
		if (!isdigit(c)) return false;
	return true;
}


//TIMEDIFF type.  Class for representing durations in time.
class timediff
{
	using t_nsec = std::chrono::nanoseconds;
	using t_sec = std::chrono::seconds;
	using t_msec = std::chrono::milliseconds;

	template<class T, class R = std::ratio<1>>
	using durtype = std::chrono::duration<T, R>;

	t_sec sec;
	t_nsec nsec;

	//Wrapper for the chrono duration cast function
	template<class TT, class FT> static TT durcast(FT rhs) { return std::chrono::duration_cast<TT>(rhs); }

public:
	constexpr timediff(const timediff& rhs) : sec(rhs.sec), nsec(rhs.nsec) {}
	//Simplified constructor if the type is in seconds
	constexpr timediff(t_sec s) : nsec(0), sec(s) {}
	//Templated constructor for other types
	template<class T> constexpr timediff(T dur)
	{
		sec = durcast<t_sec>(dur);
		nsec = durcast<t_nsec>(dur - sec);
	}

	//Methods to retrieve the duration:
	template<class rtype> constexpr rtype asnanoseconds() { return durcast<durtype<rtype, std::nano>>(sec + nsec).count(); }
	template<class rtype> constexpr rtype asmicroseconds() { return durcast<durtype<rtype, std::micro>>(sec + nsec).count(); }
	template<class rtype> constexpr rtype asmilliseconds() { return durcast<durtype<rtype, std::milli>>(sec + nsec).count(); }
	template<class rtype> constexpr rtype asseconds() { return durcast<durtype<rtype>>(sec + nsec).count(); }
	template<class rtype> constexpr rtype asminutes() { return durcast<durtype<rtype, std::ratio<S_IN_MINUTE>>>(sec + nsec).count(); }
	template<class rtype> constexpr rtype ashours() { return durcast<durtype<rtype, std::ratio<S_IN_HOUR>>>(sec + nsec).count(); }
	template<class rtype> constexpr rtype asdays() { return durcast<durtype<rtype, std::ratio<S_IN_DAY>>>(sec + nsec).count(); }
	template<class rtype> constexpr rtype asweeks() { return durcast<durtype<rtype, std::ratio<S_IN_WEEK>>>(sec + nsec).count(); }

	//Get underlying data
	constexpr std::pair<t_sec, t_nsec> data() const { return { sec, nsec }; }
};

//TimeDiff functions for integral types:
template <class I> timediff weeks(typename std::enable_if<std::_Is_integral<I>::value, I>::type t) { return timediff(std::chrono::seconds(t * S_IN_WEEK)); }
template <class I> timediff days(typename std::enable_if<std::_Is_integral<I>::value, I>::type t) { return timediff(std::chrono::seconds(t * S_IN_DAY)); }
template <class I> timediff hours(typename std::enable_if<std::_Is_integral<I>::value, I>::type t) { return timediff(std::chrono::seconds(t * S_IN_HOUR)); }
template <class I> timediff minutes(typename std::enable_if<std::_Is_integral<I>::value, I>::type t) { return timediff(std::chrono::seconds(t * S_IN_MINUTE)); }
template <class I> timediff seconds(typename std::enable_if<std::_Is_integral<I>::value, I>::type t) { return timediff(std::chrono::seconds(t)); }
template <class I> timediff milliseconds(typename std::enable_if<std::_Is_integral<I>::value, I>::type t) { return timediff(std::chrono::milliseconds(t)); }
template <class I> timediff microseconds(typename std::enable_if<std::_Is_integral<I>::value, I>::type t) { return timediff(std::chrono::microseconds(t)); }
template <class I> timediff nanoseconds(typename std::enable_if<std::_Is_integral<I>::value, I>::type t) { return timediff(std::chrono::nanoseconds(t)); }


//Convenience wrappers for non integral types:
template <class T> timediff weeks(typename std::enable_if<!std::_Is_integral<T>::value, T>::type t) { return timediff(std::chrono::duration<T>(t * S_IN_WEEK)); }
template <class T> timediff days(typename std::enable_if<!std::_Is_integral<T>::value, T>::type t) { return timediff(std::chrono::duration<T>(t * S_IN_DAY)); }
template <class T> timediff hours(typename std::enable_if<!std::_Is_integral<T>::value, T>::type t) { return timediff(std::chrono::duration<T>(t * S_IN_HOUR)); }
template <class T> timediff minutes(typename std::enable_if<!std::_Is_integral<T>::value, T>::type t) { return timediff(std::chrono::duration<T>(t * S_IN_MINUTE)); }
template <class T> timediff seconds(typename std::enable_if<!std::_Is_integral<T>::value, T>::type t) { return timediff(std::chrono::duration<T>(t)); }
template <class T> timediff milliseconds(typename std::enable_if<!std::_Is_integral<T>::value, T>::type t) { return timediff(std::chrono::duration<T, std::milli>(t)); }
template <class T> timediff microseconds(typename std::enable_if<!std::_Is_integral<T>::value, T>::type t) { return timediff(std::chrono::duration<T, std::micro>(t)); }
template <class T> timediff nanoseconds(typename std::enable_if<!std::_Is_integral<T>::value, T>::type t) { return timediff(std::chrono::duration<T, std::nano>(t)); }

//TIMESTAMP TYPE
class timestamp
{
	using t_nsec = std::chrono::nanoseconds;
	using t_sec = std::chrono::seconds;
	using t_msec = std::chrono::milliseconds;
	using t_sysclock = std::chrono::system_clock;
	using t_hiresclock = std::chrono::high_resolution_clock;
	template<class T> using t_timepoint = std::chrono::time_point<T>;

	//This is just a wrapper around a couple of chrono classes.
	t_timepoint<t_sysclock> time;
	bool hastime = false;

	//private functions
	template<class TT, class FT> static constexpr TT durcast(FT rhs) { return std::chrono::duration_cast<TT>(rhs); }

public:
	constexpr timestamp(const t_timepoint<t_sysclock>& t, bool ht) : time(t), hastime(ht) {}
	constexpr timestamp() : hastime(false), time(t_sec(0)) {};
	constexpr timestamp(const timestamp& rhs) : hastime(rhs.hastime), time(rhs.time) {}

	constexpr static timestamp now() { return timestamp(t_sysclock::now(), true); }
	constexpr t_timepoint<t_sysclock> astimepoint() const { return time; }
	constexpr bool isvalid() const { return hastime; }

	timediff operator-(const timestamp& rhs) const { return timediff((time.time_since_epoch() - rhs.time.time_since_epoch())/* + (nano - rhs.nano)*/); }
	constexpr timestamp operator+(const timediff& rhs) const { return timestamp(time + rhs.data().first + durcast<std::chrono::system_clock::duration>(rhs.data().second), true); }
	constexpr timestamp operator-(const timediff& rhs) const { return timestamp(time - rhs.data().first - durcast<std::chrono::system_clock::duration>(rhs.data().second), true); }

	constexpr timestamp& operator+=(const timediff& rhs) { return *this = *this + rhs; }
	constexpr timestamp& operator-=(const timediff& rhs) { return *this = *this - rhs; }

	bool fromstring(const char* tstamp, const char* format, size_t tlen, size_t flen);
	bool fromstring(const std::string& tstamp, const std::string& format) { return fromstring(tstamp.c_str(), format.c_str(), tstamp.size(), format.size()); }
	std::string tostdstring(const std::string& format);
};


//Modifies the start pointer and advances it by the number of characters read in.
inline bool assigntmparam(const char ** start, const char* end, int mincount, int maxcount, int& param, int paramlow, int paramhigh)
{
	//Assert that the user isn't stupid:
	_ASSERT(start);
	_ASSERT(end);
	const char* s_ptr = *start;
	_ASSERT(s_ptr);
	_ASSERT(end > s_ptr);

	int temp;
	int count = std::min(int(end - s_ptr), maxcount);
	if (count < mincount)
		return false;

	//should we do a heap allocation to store the string? might be faster....
	//char digits[maxcount];

	//Is this faster? I don't like this... Need to do a speed comparison.
	try {
		const std::string tstring = pad0right(std::string(s_ptr, count), maxcount);
		temp = std::stoi(tstring);
	}
	catch (std::exception&)
	{
		return false;
	}

	if (temp < paramlow || temp > paramhigh)
		return false;
	param = temp;
	*start += count; //advance the pointer
	return true;
}

inline constexpr int lowIndex(const char* s, const char lowchar)
{
	int i = 0;
	while (*s && *s != lowchar)
	{
		++s;
		++i;
	}
	return i;
}

//Uses lookup table, only works for values under 10^10
inline int smallatoi(const char* const snum, int slen)
{
	if (slen > 10)
		return 0;
	const int powten[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };
	int ret = 0;

	for (int i = 0; i < slen; ++i)
	{
		ret += (snum[i] - '0') * powten[slen - i - 1];
	}
	return ret;
}

inline time_t time_to_epoch(const struct tm *ltm, int utcdiff) {
	const int mon_days[] =
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	long tyears, tdays, leaps, utc_hrs;
	int i;

	tyears = ltm->tm_year - 70; // tm->tm_year is from 1900.
	leaps = (tyears + 2) / 4; // no of next two lines until year 2100.
	//i = (ltm->tm_year – 100) / 100;
	//leaps -= ( (i/4)*3 + i%4 );
	tdays = 0;
	for (i = 0; i < ltm->tm_mon; i++) tdays += mon_days[i];

	tdays += ltm->tm_mday - 1; // days of month passed.
	tdays = tdays + (tyears * 365) + leaps;

	utc_hrs = ltm->tm_hour + utcdiff; // for your time zone.
	return (tdays * 86400) + (utc_hrs * 3600) + (ltm->tm_min * 60) + ltm->tm_sec;
}

inline bool timestamp::fromstring(const char* tstamp, const char* format, size_t tlen, size_t flen)
{
	if (!tstamp || !format)
		return false;

	if (!tlen)
		tlen = strlen(tstamp);
	if (!flen)
		flen = strlen(format);

	const char* tend = tstamp + tlen;
	const char* fend = format + flen;

	tm t = { 0 };
	//t.tm_isdst = 0;
	int ms = 0;
	int ns = 0;
	//I'd rather only go through one string at a time if possible
	//Format string is used to get positions of different info in the time string
	constexpr char symbols[] = "YMdHmsxf";
	constexpr int lowadjust = lowIndex(symbols, 'x');
	constexpr char symlen[] = "42222239";

	int * const tmpos[] = { &(t.tm_year), &(t.tm_mon), &(t.tm_mday), &(t.tm_hour), &(t.tm_min), &(t.tm_sec), &ms, &ns };
	//Buffer to use for int conversions:
	char chBuf[10];

	while (tstamp < tend && format < fend)
	{

		if (*format == '%' && ++format < fend)
		{
			//const char* chPos = strchr(symbols, *format);
			int index = 0;
			for (; index < sizeof(symbols) / sizeof(char); ++index)
				if (symbols[index] == *format)
					break;

			if (index < sizeof(symbols) / sizeof(char))
			{
				//int fIndex = int(chPos - symbols);
				int paramlen = symlen[index] - '0';
				//Checks
				if (tstamp + paramlen > tend)
					return false;

				//For year, month, day, hour, and second, the number lengths are fixed. For ms and ns, the lengths are variable. Two sections to deal with this.
				if (index < lowadjust)
				{
					for (int i = 0; i < paramlen; ++i)
					{
						if (*tstamp - '0' > 9)
							return false;
						chBuf[i] = *tstamp++;
					}
				}
				else
				{
					int i = 0;
					while (i < paramlen)
					{
						if (tstamp[i] - '0' > 9)
							break;
						chBuf[i] = *tstamp++;
						++i;
					}
					while (i < paramlen)
						chBuf[i++] = '0';
				}
				
				chBuf[paramlen] = '\0';
				//*tmpos[fIndex] = std::stoi(chBuf);
				*tmpos[index] = smallatoi(chBuf, paramlen);
			}
		}
		else
			++tstamp;
		++format;
	}

	t.tm_year -= 1900;
	--t.tm_mon;

	//Bounds checking
	if (t.tm_hour < 0 || t.tm_hour > 23)
		return false;
	if (t.tm_mday < 1 || t.tm_mday > 31)
		return false;
	if (t.tm_min < 0 || t.tm_min > 59)
		return false;
	if (t.tm_mon < 0 || t.tm_mon > 11)
		return false;
	if (t.tm_sec < 0 || t.tm_sec > 59)
		return false;
	
	long tz;
	long dstoff;
	_get_timezone(&tz);
	_get_dstbias(&dstoff);
	t.tm_sec += tz + dstoff;
	//time_t tbase = std::mktime(&t);
	time_t tbase = time_to_epoch(&t, 0);
	if (ns)
		time = std::chrono::system_clock::from_time_t(tbase) + durcast<std::chrono::system_clock::duration>(t_nsec(ns));
	else
		time = std::chrono::system_clock::from_time_t(tbase) + durcast<std::chrono::system_clock::duration>(t_msec(ms));

	return true;
}


//Uses input format to transform internal time_point into a formatted std::string. Usable format characters: %Y, %M, %d, %H, %m, %s, %x, %f
inline std::string timestamp::tostdstring(const std::string& format)
{
	std::string ret;
	ret.reserve(format.size());
	auto split = splitstring(format, '%');
	if (split.size() < 2)
		return ret;

	tm t;
	time_t trep = std::chrono::system_clock::to_time_t(time);
	localtime_s(&t, &trep);
	auto tfrac = time.time_since_epoch() - t_sec(trep); //removes seconds from the time since epoch, so we only have the nanoseconds

	ret = split.front();

	for (size_t n = 1; n < split.size(); ++n)
	{
		if (!split[n].size())
		{
			ret.push_back('%');
			continue;
		}

		switch (split[n].front())
		{
		case 'Y':
			ret += std::to_string(t.tm_year + 1900);
			break;
		case 'M':
			ret += pad0left(std::to_string(t.tm_mon + 1), 2);
			break;
		case 'd':
			ret += pad0left(std::to_string(t.tm_mday), 2);
			break;
		case 'H':
			ret += pad0left(std::to_string(t.tm_hour), 2);
			break;
		case 'm':
			ret += pad0left(std::to_string(t.tm_min), 2);
			break;
		case 's':
			ret += pad0left(std::to_string(t.tm_sec), 2);
			break;
		case 'x':
			ret += pad0left(std::to_string(durcast<std::chrono::milliseconds>(tfrac).count()), 3);
			break;
		case 'f':
			ret += pad0left(std::to_string(durcast<std::chrono::nanoseconds>(tfrac).count()), 9);
			break;
		}

		ret += split[n].substr(1, split[n].size() - 1);
	}

	return ret;
}