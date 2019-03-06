// utime.cxx

#include <Python.h>
#include <chrono>
#include "datetime.h"
#include <iostream>

auto get_timepoint(const int& year, const int& month, const int& day, const int& hour, const int& min, const int& sec) {
	std::tm tm_obj;
	tm_obj.tm_year = year - 1900;
	tm_obj.tm_mon = month - 1;
	tm_obj.tm_mday = day;
	tm_obj.tm_hour = hour;
	tm_obj.tm_min = min;
	tm_obj.tm_sec = sec;

	time_t t = mktime(&tm_obj);

	return std::chrono::system_clock::from_time_t(t);
}

static PyObject* get_timestamp(PyObject* self, PyObject* args) {
	PyObject* ret = NULL;
    const Py_ssize_t args_len = PyObject_Length(args);

	if ( 0 == args_len ) {
		const double ts = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1000000.0;
		ret = PyFloat_FromDouble(ts);
	}
	else if ( 7 == args_len ) {
		int year, month, day, hour, min, sec, micro_sec;

		if ( !PyArg_ParseTuple(args, "iiiiiii", &year, &month, &day, &hour, &min, &sec, &micro_sec) ) {
			return NULL;
		}

		const auto tp = get_timepoint(year, month, day, hour, min, sec);
		const auto dr = tp.time_since_epoch() + std::chrono::microseconds(micro_sec);
		const double ts = std::chrono::duration_cast<std::chrono::microseconds>(dr).count() / 1000000.0;
		ret = PyFloat_FromDouble(ts);
	}
	else if ( 6 == args_len ) {
		int year, month, day, hour, min, sec;

		if ( !PyArg_ParseTuple(args, "iiiiii", &year, &month, &day, &hour, &min, &sec) ) {
			return NULL;
		}

		const auto tp = get_timepoint(year, month, day, hour, min, sec);
		const auto dr = tp.time_since_epoch();
		const long long ts = std::chrono::duration_cast<std::chrono::seconds>(dr).count();
		ret = PyLong_FromLongLong(ts);
	}
	else {
		return NULL;
	}

	return ret;
}

template <class T>
void get_tm_from_ts(std::tm& tm_obj, const long long& timestamp) {
	PyDateTime_IMPORT;
	T dur(timestamp);
	std::chrono::time_point<std::chrono::system_clock> tp(dur);
	time_t t = std::chrono::system_clock::to_time_t(tp);
	localtime_s(&tm_obj, &t);
}

static PyObject* get_datetime_from_ts(PyObject* self, PyObject* args) {
	double ts = 0.0;

	if ( !PyArg_ParseTuple(args, "d", &ts) ) {
		return NULL;
	}

	const long long ts_16d = (long long)(ts * 1000000.0);

	std::tm tm_obj;
	get_tm_from_ts<std::chrono::microseconds>(tm_obj, ts_16d);
	const int micro_sec = (int)(ts_16d % 1000000);

	return PyDateTime_FromDateAndTime(tm_obj.tm_year+1900, tm_obj.tm_mon+1, tm_obj.tm_mday, tm_obj.tm_hour, tm_obj.tm_min, tm_obj.tm_sec, micro_sec);
}

static PyMethodDef timeMethods[] = {
	{"get_timestamp", get_timestamp, METH_VARARGS, "Get TimeStamp"},
	{"get_datetime_from_ts", get_datetime_from_ts, METH_VARARGS, "Get DateTime From TimeStamp"},
	{NULL, NULL, 0, NULL}
};

struct PyModuleDef timeModule = {
	PyModuleDef_HEAD_INIT,
	"time",
	NULL,
	-1,
	timeMethods
};

PyMODINIT_FUNC
PyInit_time(void) {
    return PyModule_Create(&timeModule);
}