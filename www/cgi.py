#!/usr/bin/python3

import os
import sys

name = os.environ['QUERY_STRING']
print("<html><head><title>Hello!</title></head><body>")
print(f"<h1>Hello, {name}!</h1>")
print("</body></html>")
