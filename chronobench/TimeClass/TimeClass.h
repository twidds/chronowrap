/*
This is a utility class for tracking time.  This is just a wrapper for the built in ctime type tm with some functionality and support for millisecond
resolution.  May continue to add to as more functionality is needed.
*/

#pragma once
#include <ctime>
#include <string>
#include <vector>
//#include <limits>

//using namespace std;

namespace tmwrap {

	const double MS_IN_SECOND = 1000.0;
	const double MS_IN_MINUTE = 60000.0;
	const double MS_IN_HOUR = 3600000.0;
	const double MS_IN_DAY = 86400000.0;

	class Time
	{
		double milliseconds;

	public:
		Time();
		bool IsValid() const;
		friend Time Milliseconds(double ms);
		friend Time Seconds(double s);
		friend Time Minutes(double min);
		friend Time Hours(double hrs);
		friend Time Days(double days);

		double asMilliseconds() const;
		double asSeconds() const;
		double asMinutes() const;
		double asHours() const;
		double asDays() const;

		Time& operator= (const Time& other);
		Time& operator+= (const Time& other);
		Time& operator*= (const double factor);
		Time operator+ (const Time& other) const;
		Time operator- (const Time& other) const;
		Time operator* (const double factor);

		static const Time BAD_TIME;
	};


	enum TimeIndices
	{
		YEAR = 0,
		MONTH,
		DAY,
		HOUR,
		MINUTE,
		SECOND,
		MILLISECOND,
		END
	};

	struct Timeformat {
		//Vectors of positions? Pairs of Positions with keyLetters?
		std::vector<size_t> TimePositions[TimeIndices::END];
		char TimeChars[TimeIndices::END];

		Timeformat();

		void Clear_Values();
	};

	class Timestamp {
	private:
		mutable tm time;
		mutable int milliseconds;
		Timeformat t_format;

		time_t FixTime() const;
		Timestamp(bool makebad);
		//Timestamp MakeBad();

	public:
		Timestamp();
		Timestamp(std::string timestring, std::string Format);
		void SetFormat(const std::string); //Sets format using Y,M,D,H,m,s,x for letter designations
		void SetTimeFromString(const std::string);
		void SetTimeFromString(const std::string Input, const std::string Format);
		void AddMsToTime(const int ms); //will restructure time variable.  Can be negative
		void StoreCurrentTime();
		bool isValid() const;

		friend int TimeCompare(Timestamp& t1, Timestamp& t2); //returns -1 if t1 larger, 0 if same, 1 if t2 larger. Also fixes time in t1 and t2
		friend int64_t MsTimeDiff(Timestamp& t1, Timestamp& t2); //Returns difference between t1 and t2 in milliseconds (>0 if t2 > t1).  Also fixes time in t1 and t2
		
		void SetTimeFromParams(int year, int month, int day, int hour, int minute, int second, int milliseconds);

		std::string TimeToString() const; //Uses default format of YYYY/MM/DD HH:mm:ss.xxx
		int year() const { return this->time.tm_year + 1900; }
		int month() const { return this->time.tm_mon + 1; }
		int day() const { return this->time.tm_mday; }
		int weekday() const { return this->time.tm_wday; }
		int hour() const { return this->time.tm_hour; }
		int minute() const { return this->time.tm_min; }
		int second() const { return this->time.tm_sec; }
		int millisecond() const { return this->milliseconds; }


		friend std::ostream& operator<< (std::ostream& out, const Timestamp& t);

		bool operator<(const Timestamp& rhs) const;
		Timestamp& operator=(const Timestamp& other);
		Timestamp& operator+=(const Time& time);
		Timestamp& operator-=(const Time& time);
		Timestamp operator+(const Time& time);
		Timestamp operator-(const Time& time);

		static const Timestamp BAD_TIMESTAMP;
	};

	std::string pad0Left(const std::string in, const int paddedLength);


}

