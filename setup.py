# -*- coding: utf-8 -*-
import os

from setuptools import setup, Extension

packages = [
	"hotpepper",
]

ext_modules = [
	Extension(
		name="hotpepper._utime",
		sources=[
			os.path.join("src", "utime.cxx"),
			os.path.join("src", "utime_wrap.cxx"),
		],
	),
]

classifiers = [
	"License :: OSI Approved :: MIT License",
	"Programming Language :: Python :: 3 :: Only",
]

setup(
	name="hotpepper",
	version="0.0.1",
	description="useful utils",
	author="3x3x3",
	author_email="ivmivm001@gmail.com",
	url="https://github.com/3x3x3/hotpepper",
	license="MIT",
	python_requires=">=3",
	ext_modules=ext_modules,
	packages=packages,
	package_dir={"hotpepper": "src"},
	classifiers=classifiers,
	zip_safe=False
)
