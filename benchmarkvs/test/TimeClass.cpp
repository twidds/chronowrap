#include "TimeClass.h"

namespace tmwrap {


	//Random initialization to the year 1970.  This is to help mitigate issues with pre-1970 timestamps that occur when using the tm struct.

	Timeformat::Timeformat()
	{
		TimeChars[TimeIndices::YEAR] = 'Y';
		TimeChars[TimeIndices::MONTH] = 'M';
		TimeChars[TimeIndices::DAY] = 'D';
		TimeChars[TimeIndices::HOUR] = 'H';
		TimeChars[TimeIndices::MINUTE] = 'm';
		TimeChars[TimeIndices::SECOND] = 's';
		TimeChars[TimeIndices::MILLISECOND] = 'x';
	}

	void Timeformat::Clear_Values()
	{
		for (auto& it : TimePositions)
		{
			it.clear();
		}
	}

	const Timestamp Timestamp::BAD_TIMESTAMP = {Timestamp(true)};

	//Random initialization to the year 1970.  This is to help mitigate issues with pre-1970 timestamps that occur when using the tm struct.
	Timestamp::Timestamp()
	{
		time.tm_year = 70;
		time.tm_mon = 0;
		time.tm_mday = 0;
		time.tm_hour = 0;
		time.tm_min = 0;
		time.tm_sec = 1;
		milliseconds = 0;
	}

	Timestamp::Timestamp(std::string timestring, std::string Format)
	{
		SetFormat(Format);
		SetTimeFromString(timestring);
	}

	//Fills the Timeformat struct instance based on the positions of the letters inside of the format string provided.
	//Clears the format struct, then pushes the new positions into the respective vectors.
	void Timestamp::SetFormat(const std::string Format)
	{
		t_format.Clear_Values();
		const char* const front_char = &Format.front();


		for (int refPos = 0; refPos < TimeIndices::END; ++refPos)
		{
			for (const char& it : Format)
				if (t_format.TimeChars[refPos] == it)
					t_format.TimePositions[refPos].push_back(&it - front_char);

		}
	}

	//NOTE: Accurate only to within 1 second.  This is because we're using the built in ctime implementation.
	void Timestamp::StoreCurrentTime()
	{
		time_t cur_time;
		std::time(&cur_time);

		//time = *localtime(&cur_time);
		localtime_s(&time, &cur_time);
	}

	bool Timestamp::isValid() const
	{
		if (time.tm_year == std::numeric_limits<int>::min())
			return false;
		return true;
	}

	//This currently only works for one format: 2017/09/28 00:13:37.694.  May add support for multiple formats in the future if needed.
	void Timestamp::SetTimeFromString(const std::string timestring)
	{
		//Strings for each parameter
		std::string partStrings[TimeIndices::END];
		for (auto p_string : partStrings)
			p_string = "0";

		//Timepositions is array of vectors of integers of indices. WOOF!
		//We need to get each index from the timestring and copy it into the correct partstring.
		for (int it = 0; it < TimeIndices::END; ++it)
		{
			for (auto s_index : t_format.TimePositions[it])
				partStrings[it].push_back(timestring[s_index]);
		}

		time.tm_year = std::stoi(partStrings[TimeIndices::YEAR]) - 1900;
		time.tm_mon = std::stoi(partStrings[TimeIndices::MONTH]) - 1;
		time.tm_mday = std::stoi(partStrings[TimeIndices::DAY]);
		time.tm_hour = std::stoi(partStrings[TimeIndices::HOUR]);
		time.tm_min = std::stoi(partStrings[TimeIndices::MINUTE]);
		time.tm_sec = std::stoi(partStrings[TimeIndices::SECOND]);
		time.tm_isdst = -1;
		milliseconds = std::stoi(partStrings[TimeIndices::MILLISECOND]);

		if (time.tm_year < 0)
			time.tm_year = 0;
		if (time.tm_mon < 0)
			time.tm_mon = 0;

		//May change this in the future to support sub-milliseconds, but currently the milliseconds will be concatenated if >1000
		while (milliseconds > 1000)
			milliseconds /= 10;


		//FixTime();
	}

	//Sets the values inside of the struct based off of the time given in the input string along with the format specified in the Format string.
	//Returns false if the assignment couldn't be made (e.g. formats don't match).  Returns true if the assignment worked.
	//For the format string, the current structure only accepts 24 hour format.  Letters are defined as follows:
	//Y -> year(1970-). M -> Month(1-12). D -> day(1-31). H -> hour(0-23). m -> minute(0-59).  s -> second (0-59). x -> millisecond(0-1000).
	void Timestamp::SetTimeFromString(const std::string Input, const std::string Format)
	{
		SetFormat(Format);
		SetTimeFromString(Input);
	}


	void Timestamp::AddMsToTime(const int ms)
	{
		milliseconds += ms;
		FixTime();
	}

	//Internal function used to bring all of the values within their correct ranges (e.g. resets milliseconds to be <1000 and seconds <60 etc)
	//Returns same time_t returned by mktime(), so it doesn't include ms.  This function is currently private and only used internally.
	time_t Timestamp::FixTime() const
	{
		//Manual millisecond adjustments.
		int msAdjust = milliseconds / 1000;

		if (milliseconds < 0)
			msAdjust--;

		milliseconds -= msAdjust * 1000;
		time.tm_sec += msAdjust;

		//Repair the tm structure if needed.
		return std::mktime(&time);
	}

	Timestamp::Timestamp(bool makebad)
	{
		Timestamp();

		if (makebad)
		{
			time.tm_year = std::numeric_limits<int>::min();
		}
	}

	//Timestamp Timestamp::MakeBad()
	//{
	//	Timestamp bad;
	//	bad.time.tm_year = std::numeric_limits<int>::min(); //invalid.
//		return bad;
//	}

	void Timestamp::SetTimeFromParams(int year, int month, int day, int hour, int minute, int second, int milliseconds)
	{
		time.tm_year = year - 1900;
		time.tm_mon = month - 1;
		time.tm_mday = day;
		time.tm_hour = hour;
		time.tm_min = minute;
		time.tm_sec = second;
		this->milliseconds = milliseconds;

		FixTime();
	}

	std::string Timestamp::TimeToString() const
	{
		std::string ret =
			std::to_string(time.tm_year + 1900) + '/' +
			pad0Left(std::to_string(time.tm_mon + 1), 2) + '/' +
			pad0Left(std::to_string(time.tm_mday), 2) + ' ' +
			pad0Left(std::to_string(time.tm_hour), 2) + ':' +
			pad0Left(std::to_string(time.tm_min), 2) + ':' +
			pad0Left(std::to_string(time.tm_sec), 2) + '.' +
			pad0Left(std::to_string(milliseconds), 3);

		return ret;
	}

	bool Timestamp::operator<(const Timestamp & rhs) const
	{
		int64_t t_diff = std::mktime(&rhs.time) - std::mktime(&time);
		int ms_diff = rhs.milliseconds - milliseconds; //Difference between millisecond values

		return (t_diff * 1000 + ms_diff) > 0;
		//return (t_diff * 1000) + ms_diff; //Can represent differences of up to 2.9E8 years in milliseconds.

		//return false;
	}

	Timestamp & Timestamp::operator=(const Timestamp & other)
	{
		t_format = other.t_format;
		time = other.time;
		milliseconds = other.milliseconds;
		return *this;
	}

	Timestamp & Timestamp::operator+=(const Time & time)
	{
		milliseconds += time.asMilliseconds();
		FixTime();

		return *this;
	}

	Timestamp & Timestamp::operator-=(const Time & time)
	{
		milliseconds -= time.asMilliseconds();
		FixTime();

		return *this;
	}

	Timestamp Timestamp::operator+(const Time & time)
	{
		Timestamp ret = *this;
		ret.milliseconds += time.asMilliseconds();
		ret.FixTime();
		return ret;
	}

	Timestamp Timestamp::operator-(const Time & time)
	{
		Timestamp ret = *this;
		ret.milliseconds -= time.asMilliseconds();
		ret.FixTime();
		return ret;
	}

	//Returns effective result of t2 - t1 in milliseconds
	int64_t MsTimeDiff(Timestamp& t1, Timestamp& t2)
	{
		//int64_t t_diff = t2.FixTime() - t1.FixTime(); //difference in seconds.
		int64_t t_diff = std::mktime(&t2.time) - std::mktime(&t1.time);
		int ms_diff = t2.milliseconds - t1.milliseconds; //Difference between millisecond values

		return (t_diff * 1000) + ms_diff; //Can represent differences of up to 2.9E8 years in milliseconds.
	}


	//Returns 1 if t2 > t1, 0 if t2 = t1, -1 if t2 < t1
	int TimeCompare(Timestamp & t1, Timestamp & t2)
	{

		int64_t t_diff = MsTimeDiff(t1, t2);

		if (t_diff)
		{
			return t_diff > 0 ? 1 : -1;
		}
		else
			return 0;
	}

	//Overloading << operator for ostreams.
	std::ostream& operator<< (std::ostream& out, const Timestamp& t)
	{
		out << t.TimeToString();
		return out;
	}



	//Pads left side of string with zeros until the length is equal to paddedLength
	//If string is longer than padded length, just returns the string.
	std::string pad0Left(const std::string in, const int paddedLength)
	{
		size_t l_diff = paddedLength - in.length();

		if (l_diff < 1)
			return in;

		std::string out = "";

		for (size_t i = 0; i < l_diff; ++i)
			out += '0';

		return out + in;
	}


	//
	//
	//
	//Implementations for Time class
	//
	//
	//

	Time::Time() : milliseconds(0.0)
	{
	}

	bool Time::IsValid() const
	{
		if (std::isnan(milliseconds))
			return false;
		return true;
	}


	Time Milliseconds(double ms)
	{
		Time ret;
		ret.milliseconds = ms;
		return ret;
	}
	const Time Time::BAD_TIME = Milliseconds(std::numeric_limits<double>::quiet_NaN());


	Time Seconds(double s)
	{
		Time ret;
		ret.milliseconds = s * MS_IN_SECOND;
		return ret;
	}

	Time Minutes(double min)
	{
		Time ret;
		ret.milliseconds = min * MS_IN_MINUTE;
		return ret;
	}

	Time Hours(double hrs)
	{
		Time ret;
		ret.milliseconds = hrs * MS_IN_HOUR;
		return ret;
	}

	Time Days(double days)
	{
		Time ret;
		ret.milliseconds = days * MS_IN_DAY;
		return ret;
	}


	double Time::asMilliseconds() const
	{
		return milliseconds;
	}

	double Time::asSeconds() const
	{
		return milliseconds / MS_IN_SECOND;
	}

	double Time::asMinutes() const
	{
		return milliseconds / MS_IN_MINUTE;
	}

	double Time::asHours() const
	{
		return milliseconds / MS_IN_HOUR;
	}

	double Time::asDays() const
	{
		return milliseconds / MS_IN_DAY;
	}

	Time & Time::operator=(const Time & other)
	{
		milliseconds = other.milliseconds;
		return *this;
	}

	Time & Time::operator+=(const Time & other)
	{
		milliseconds += other.milliseconds;
		return *this;
	}

	Time & Time::operator*=(const double factor)
	{
		milliseconds *= factor;
		return *this;
	}

	Time Time::operator+(const Time & other) const
	{
		return Milliseconds(milliseconds + other.milliseconds);
	}

	Time Time::operator-(const Time & other) const
	{
		return Milliseconds(milliseconds - other.milliseconds);
	}

	Time Time::operator*(const double factor)
	{
		return Milliseconds(milliseconds * factor);
	}

}