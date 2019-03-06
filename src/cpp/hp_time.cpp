// hp_time.cxx

#include <Python.h>
#include <chrono>
#include "datetime.h"

auto _get_timepoint(const int& year, const int& month, const int& day, const int& hour, const int& min, const int& sec) {
	std::tm tm_obj{sec, min, hour, day, month-1, year-1900, 0, 0, -1};
	const time_t t = mktime(&tm_obj);
	return std::chrono::system_clock::from_time_t(t);
}

long long _get_timestamp_10d() {
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

long long _get_timestamp_10d(const int& year, const int& month, const int& day, const int& hour, const int& min, const int& sec) {
	const auto tp = _get_timepoint(year, month, day, hour, min, sec);
	return std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
}

long long _get_timestamp_16d() {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

long long _get_timestamp_16d(const int& year, const int& month, const int& day, const int& hour, const int& min, const int& sec, const int& micro_sec) {
	const auto tp = _get_timepoint(year, month, day, hour, min, sec);
	const auto dr = tp.time_since_epoch() + std::chrono::microseconds(micro_sec);
	return std::chrono::duration_cast<std::chrono::microseconds>(dr).count();
}

static PyObject* get_timestamp(PyObject* self, PyObject* args) {
	PyObject* ret = NULL;
    const Py_ssize_t args_len = PyObject_Length(args);

	if ( 0 == args_len ) {
		const double ts = _get_timestamp_16d() / 1000000.0;
		ret = PyFloat_FromDouble(ts);
	}
	else if ( 7 == args_len ) {
		int year, month, day, hour, min, sec, micro_sec;

		if ( !PyArg_ParseTuple(args, "iiiiiii", &year, &month, &day, &hour, &min, &sec, &micro_sec) ) {
			return NULL;
		}

		const double ts = _get_timestamp_16d(year, month, day, hour, min, sec, micro_sec) / 1000000.0;
		ret = PyFloat_FromDouble(ts);
	}
	else if ( 6 == args_len ) {
		int year, month, day, hour, min, sec;

		if ( !PyArg_ParseTuple(args, "iiiiii", &year, &month, &day, &hour, &min, &sec) ) {
			return NULL;
		}

		const long long ts = _get_timestamp_10d(year, month, day, hour, min, sec);
		ret = PyLong_FromLongLong(ts);
	}
	else {
		return NULL;
	}

	return ret;
}

static PyObject* get_timestamp_10d(PyObject* self, PyObject* args) {
	PyObject* ret = NULL;
    const Py_ssize_t args_len = PyObject_Length(args);

	if ( 0 == args_len ) {
		const double ts = _get_timestamp_10d();
		ret = PyLong_FromLongLong(ts);
	}
	else if ( 6 == args_len ) {
		int year, month, day, hour, min, sec;

		if ( !PyArg_ParseTuple(args, "iiiiii", &year, &month, &day, &hour, &min, &sec) ) {
			return NULL;
		}

		const long long ts = _get_timestamp_10d(year, month, day, hour, min, sec);
		ret = PyLong_FromLongLong(ts);
	}
	else {
		return NULL;
	}

	return ret;
}

static PyObject* get_timestamp_16d(PyObject* self, PyObject* args) {
	PyObject* ret = NULL;
    const Py_ssize_t args_len = PyObject_Length(args);

	if ( 0 == args_len ) {
		const double ts = _get_timestamp_16d();
		ret = PyLong_FromLongLong(ts);
	}
	else if ( 7 == args_len ) {
		int year, month, day, hour, min, sec, micro_sec;

		if ( !PyArg_ParseTuple(args, "iiiiii", &year, &month, &day, &hour, &min, &sec, &micro_sec) ) {
			return NULL;
		}

		const long long ts = _get_timestamp_16d(year, month, day, hour, min, sec, micro_sec);
		ret = PyLong_FromLongLong(ts);
	}
	else {
		return NULL;
	}

	return ret;
}

template <class T>
void _get_tm_from_ts(std::tm& tm_obj, const long long& timestamp) {
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
	_get_tm_from_ts<std::chrono::microseconds>(tm_obj, ts_16d);
	const int micro_sec = (int)(ts_16d % 1000000);

	return PyDateTime_FromDateAndTime(tm_obj.tm_year+1900, tm_obj.tm_mon+1, tm_obj.tm_mday, tm_obj.tm_hour, tm_obj.tm_min, tm_obj.tm_sec, micro_sec);
}

static PyObject* get_datetime_from_ts_10d(PyObject* self, PyObject* args) {
	long long ts_10d = 0;

	if ( !PyArg_ParseTuple(args, "L", &ts) ) {
		return NULL;
	}

	std::tm tm_obj;
	_get_tm_from_ts<std::chrono::seconds>(tm_obj, ts_10d);

	return PyDateTime_FromDateAndTime(tm_obj.tm_year+1900, tm_obj.tm_mon+1, tm_obj.tm_mday, tm_obj.tm_hour, tm_obj.tm_min, tm_obj.tm_sec);
}

static PyObject* get_datetime_from_ts_16d(PyObject* self, PyObject* args) {
	long long ts_16d = 0;

	if ( !PyArg_ParseTuple(args, "L", &ts) ) {
		return NULL;
	}

	std::tm tm_obj;
	_get_tm_from_ts<std::chrono::microseconds>(tm_obj, ts_16d);
	const int micro_sec = (int)(ts_16d % 1000000);

	return PyDateTime_FromDateAndTime(tm_obj.tm_year+1900, tm_obj.tm_mon+1, tm_obj.tm_mday, tm_obj.tm_hour, tm_obj.tm_min, tm_obj.tm_sec, micro_sec);
}

static PyMethodDef timeMethods[] = {
	{"get_timestamp", get_timestamp, METH_VARARGS, "Get TimeStamp (micro second)"},
	{"get_timestamp_10d", get_timestamp_10d, METH_VARARGS, "Get 10d TimeStamp (second)"},
	{"get_timestamp_16d", get_timestamp_16d, METH_VARARGS, "Get 16d TimeStamp (micro second)"},
	{"get_datetime_from_ts", get_datetime_from_ts, METH_VARARGS, "Get DateTime From TimeStamp (micro second)"},
	{"get_datetime_from_ts_10d", get_datetime_from_ts_10d, METH_VARARGS, "Get DateTime From 10d TimeStamp (second)"},
	{"get_datetime_from_ts_16d", get_datetime_from_ts_16d, METH_VARARGS, "Get DateTime From 16d TimeStamp (micro second)"},
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
