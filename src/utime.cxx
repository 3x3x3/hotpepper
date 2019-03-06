// utime.cxx

#include <chrono>
#include "utime.h"
#include "datetime.h"

long long get_cur_ts_10d() {
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

long long get_cur_ts_16d() {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

double get_cur_ts() {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() * 0.000001;
}

long long get_ts_10d(int year, int month, int day, int hour, int minute, int second) {
	std::tm tm_obj;
	tm_obj.tm_year = year - 1900;
	tm_obj.tm_mon = month - 1;
	tm_obj.tm_mday = day;
	tm_obj.tm_hour = hour;
	tm_obj.tm_min = minute;

	time_t t = mktime(&tm_obj);

	const auto tp = std::chrono::system_clock::from_time_t(t);
	const auto dr = tp.time_since_epoch();
	const long long ts = std::chrono::duration_cast<std::chrono::seconds>(dr).count();

	return ts;
}

long long get_ts_16d(int year, int month, int day, int hour, int minute, int second, int micro_sec) {
	std::tm tm_obj;
	tm_obj.tm_year = year - 1900;
	tm_obj.tm_mon = month - 1;
	tm_obj.tm_mday = day;
	tm_obj.tm_hour = hour;
	tm_obj.tm_min = minute;

	time_t t = mktime(&tm_obj);

	const auto tp = std::chrono::system_clock::from_time_t(t);
	const auto dr = tp.time_since_epoch() + std::chrono::microseconds(micro_sec);
	const long long ts = std::chrono::duration_cast<std::chrono::microseconds>(dr).count();

	return ts;
}

double get_ts(int year, int month, int day, int hour, int minute, int second, int micro_sec) {
	return get_ts_16d(year, month, day, hour, minute, second, micro_sec) * 0.000001;
}

template <class T>
void get_tm_from_ts(std::tm& tm_obj, const long long& timestamp) {
	PyDateTime_IMPORT;
	T dur(timestamp);
	std::chrono::time_point<std::chrono::system_clock> tp(dur);
	time_t t = std::chrono::system_clock::to_time_t(tp);
	localtime_s(&tm_obj, &t);
}

PyObject* get_datetime_from_ts_10d(long long timestamp) {
	std::tm tm_obj;
	get_tm_from_ts<std::chrono::seconds>(tm_obj, timestamp);
	return PyDateTime_FromDateAndTime(tm_obj.tm_year+1900, tm_obj.tm_mon+1, tm_obj.tm_mday, tm_obj.tm_hour, tm_obj.tm_min, tm_obj.tm_sec, 0);
}

PyObject* get_datetime_from_ts_16d(long long timestamp) {
	std::tm tm_obj;
	get_tm_from_ts<std::chrono::microseconds>(tm_obj, timestamp);
	const int micro_sec = (int)(timestamp % 1000000);
	return PyDateTime_FromDateAndTime(tm_obj.tm_year+1900, tm_obj.tm_mon+1, tm_obj.tm_mday, tm_obj.tm_hour, tm_obj.tm_min, tm_obj.tm_sec, micro_sec);
}

PyObject* get_datetime_from_ts(double timestamp) {
	const long long ts_16d = (long long)(timestamp * 1000000.0);
	return get_datetime_from_ts_16d(ts_16d);
}
