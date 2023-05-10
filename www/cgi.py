#!/usr/bin/python3

import os
import sys

print("Content-Type: text/html\r\n\r\n")

name = os.environ['QUERY_STRING']
print(f"<html><head><title>Hello, {name}!</title></head><body>")
print("<h1>Hello, World!</h1>")
print("</body></html>")
