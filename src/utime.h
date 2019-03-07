// utime.h
#pragma once

#include <Python.h>

long long get_cur_ts_10d();
long long get_cur_ts_13d();
long long get_cur_ts_16d();
double get_cur_ts();

long long get_ts_10d(int year, int month, int day, int hour, int minute, int second);
long long get_ts_13d(int year, int month, int day, int hour, int minute, int second, int milli_sec);
long long get_ts_16d(int year, int month, int day, int hour, int minute, int second, int micro_sec);
double get_ts(int year, int month, int day, int hour, int minute, int second, int micro_sec);

PyObject* get_datetime_from_ts_10d(long long timestamp);
PyObject* get_datetime_from_ts_13d(long long timestamp);
PyObject* get_datetime_from_ts_16d(long long timestamp);
PyObject* get_datetime_from_ts(PyObject* timestamp);
