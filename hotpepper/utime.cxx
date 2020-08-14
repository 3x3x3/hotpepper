// utime.cxx

#include <chrono>
#include "utime.h"
#include "datetime.h"

long long get_cur_ts_10d() {
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

long long get_cur_ts_13d() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

long long get_cur_ts_16d() {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

double get_cur_ts() {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1000000.0;
}

auto get_timepoint(const int& year, const int& month, const int& day, const int& hour, const int& minute, const int& second) {
	std::tm tm_obj{second, minute, hour, day, month-1, year-1900, 0, 0, -1};
	const time_t t = mktime(&tm_obj);
	return std::chrono::system_clock::from_time_t(t);
}

long long get_ts_10d(int year, int month, int day, int hour, int minute, int second) {
	const auto tp = get_timepoint(year, month, day, hour, minute, second);
	const auto dr = tp.time_since_epoch();
	return std::chrono::duration_cast<std::chrono::seconds>(dr).count();
}

long long get_ts_13d(int year, int month, int day, int hour, int minute, int second, int milli_sec) {
	const auto tp = get_timepoint(year, month, day, hour, minute, second);
	const auto dr = tp.time_since_epoch() + std::chrono::milliseconds(milli_sec);
	return std::chrono::duration_cast<std::chrono::milliseconds>(dr).count();
}

long long get_ts_16d(int year, int month, int day, int hour, int minute, int second, int micro_sec) {
	const auto tp = get_timepoint(year, month, day, hour, minute, second);
	const auto dr = tp.time_since_epoch() + std::chrono::microseconds(micro_sec);
	return std::chrono::duration_cast<std::chrono::microseconds>(dr).count();
}

double get_ts(int year, int month, int day, int hour, int minute, int second, int micro_sec) {
	return get_ts_16d(year, month, day, hour, minute, second, micro_sec) / 1000000.0;
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

PyObject* get_datetime_from_ts_13d(long long timestamp) {
	std::tm tm_obj;
	get_tm_from_ts<std::chrono::milliseconds>(tm_obj, timestamp);
	const int milli_sec = (int)(timestamp % 1000) * 1000;
	return PyDateTime_FromDateAndTime(tm_obj.tm_year+1900, tm_obj.tm_mon+1, tm_obj.tm_mday, tm_obj.tm_hour, tm_obj.tm_min, tm_obj.tm_sec, milli_sec);
}

PyObject* get_datetime_from_ts_16d(long long timestamp) {
	std::tm tm_obj;
	get_tm_from_ts<std::chrono::microseconds>(tm_obj, timestamp);
	const int micro_sec = (int)(timestamp % 1000000);
	return PyDateTime_FromDateAndTime(tm_obj.tm_year+1900, tm_obj.tm_mon+1, tm_obj.tm_mday, tm_obj.tm_hour, tm_obj.tm_min, tm_obj.tm_sec, micro_sec);
}

PyObject* get_datetime_from_ts(PyObject* timestamp) {
	if ( PyFloat_Check(timestamp) ) {
		const double ts_dbl = PyFloat_AsDouble(timestamp);

		if ( 1000000000.0 <= ts_dbl && ts_dbl < 10000000000.0 ) {
			return get_datetime_from_ts_16d((long long)(ts_dbl * 1000000.0));
		}
	}
	else if ( PyLong_Check(timestamp) ) {
		const long long ts_ll = PyLong_AsLongLong(timestamp);

		if ( 1000000000000000 <= ts_ll && ts_ll < 10000000000000000)  {
			return get_datetime_from_ts_16d(ts_ll);
		}
		else if ( 1000000000000 <= ts_ll && ts_ll < 10000000000000 ) {
			return get_datetime_from_ts_13d(ts_ll);
		}
		else if ( 1000000000 <= ts_ll && ts_ll < 10000000000 ) {
			return get_datetime_from_ts_10d(ts_ll);
		}
	}
	
	Py_INCREF(Py_None);
	return Py_None;
}
