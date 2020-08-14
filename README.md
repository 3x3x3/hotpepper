# HotPepper
C++을 이용하여 Python을 좀 더 잘 활용할 수 있게 도와주는 패키지 입니다.
- hotpepper.utime: 시간관련

## Package 설치
```commandline
pip install hotperpper
```

## Package 삭제
```commandline
pip uninstall hotperpper
```

## hotpepper.utime 설명

### get_cur_ts_10d()
10자리 정수형태의 타임스탬프를 얻어온다. (초)
### get_cur_ts_13d()
13자리 정수형태의 타임스탬프를 얻어온다. (밀리초)
### get_cur_ts_16d()
16자리 정수형태의 타임스탬프를 얻어온다. (마이크로초)
### get_cur_ts()
10자리 정수 + 6자리 소수 형태의 타임스탬프를 얻어온다.

### get_ts_10d(year, month, day, hour, minute, second)
원하는 시간대의 타임스탬프를 생성 (10자리 정수, 초)
### get_ts_13d(year, month, day, hour, minute, second, milli_sec)
원하는 시간대의 타임스탬프를 생성 (13자리 정수, 밀리초)
### get_ts_16d(year, month, day, hour, minute, second, micro_sec)
원하는 시간대의 타임스탬프를 생성 (16자리 정수, 마이크로초)
### get_ts(year, month, day, hour, minute, second, micro_sec)
원하는 시간대의 타임스탬프를 생성 (10자리 정수 + 6자리 소수, 마이크로초)

### get_datetime_from_ts_10d(timestamp)
10자리 정수형태의 타임스탬프를 파이썬의 datetime 형식으로 변환한다.
### get_datetime_from_ts_13d(timestamp)
13자리 정수형태의 타임스탬프를 파이썬의 datetime 형식으로 변환한다.
### get_datetime_from_ts_16d(timestamp)
16자리 정수형태의 타임스탬프를 파이썬의 datetime 형식으로 변환한다.
### get_datetime_from_ts(timestamp)
타임스탬프를 파이썬의 datetime 형식으로 변환한다. (10자리 정수, 13자리 정수, 16자리 정수, 10자리 정수+6자리 소수 형식 모두 지원)
