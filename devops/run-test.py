#!/usr/bin/env python
import os
import subprocess


if __name__  ==  '__main__':
     os.chdir("test")
     subprocess.check_call('make test'.split())
