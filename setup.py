# -*- coding: utf-8 -*-
import os

from setuptools import setup, Extension, find_packages


ext_modules = [
	Extension(
		name="hotpepper._utime",
		sources=[
			os.path.join("hotpepper", "utime.cxx"),
			os.path.join("hotpepper", "utime_wrap.cxx"),
		],
	),
]

classifiers = [
	"License :: OSI Approved :: MIT License",
	"Programming Language :: Python :: 3 :: Only",
]

setup(
	name="hotpepper",
	version="0.0.2",
	description="useful utils",
	author="3x3x3",
	author_email="ivmivm001@gmail.com",
	url="https://github.com/3x3x3/hotpepper",
	license="MIT",
	python_requires=">=3.6",
	ext_modules=ext_modules,
	packages=find_packages(),
	classifiers=classifiers,
	zip_safe=False
)
