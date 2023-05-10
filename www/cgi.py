#!/usr/bin/python3

import os
import sys

name = os.environ['QUERY_STRING']
str_list = name.split("=")
print("<html><head><title>Hello!</title></head><body>")
print(f"<h1>Hello, {str_list[1]}!</h1>")
print("</body></html>")
