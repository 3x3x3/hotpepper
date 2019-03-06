# -*- coding: utf-8 -*-
import os

from setuptools import setup, Extension

packages = [
	"hotpepper",
]

ext_modules = [
	Extension(
		name="hotpepper.time",
		sources=[
			os.path.join("src", "cpp", "hp_time.cpp"),
		],
	),
]

setup(
	name="hotpepper",
	version="0.0.1",
	description="hotpepper",
	ext_modules=ext_modules,
	packages=packages,
	package_dir={"hotpepper": "src"},
)
