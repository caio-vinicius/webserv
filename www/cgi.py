#!/usr/bin/python3

import os
import sys

name = os.environ['QUERY_STRING']
splitted = name.split('=')
print("<html><head><title>Hello!</title></head><body>")
print(f"<h1>Hello, {splitted[1]}!</h1>")
print("</body></html>")
