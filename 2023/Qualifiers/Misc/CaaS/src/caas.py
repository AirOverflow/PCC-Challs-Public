#!/usr/bin/env python3
# Author: @TheFlash2k

'''
Description:
	This Compiler as a Service is a production grade application that will simply parse a C-code and then execute it.

Changelog(s):
	v0.1 - Validated existence of sockets
	v0.2 - Validated existence of system function and halted program if it exists
	v0.3 - Validated existence of word "PCC" and "FLAG" in the program output.
'''
DEBUG = False
MAX_CONTENT_LEN = 130
MAX_OUTPUT_LEN = 10

import subprocess
from tempfile import NamedTemporaryFile
from random import choice
from string import ascii_letters, digits
import os

def debug(msg: str) -> None:
	if DEBUG: print(f"[DEBUG] {msg}")

def get_random_string(length: int = 10) -> str:
	return ''.join(choice(ascii_letters + digits) for _ in range(length))

def validate(content: str, output=False) -> str:
	''' Check if invalid keywords exist. '''
	keywords = [
		"sock",
		"inet",
		"bind",
		"listen",
		"accept",
		"connect",
		"getaddrinfo",
		"gethostbyname",
		"system",
		"execve",
		"exec",
		"fork",
		"pcc",
		"ctf",
		"freopen",
		"flag",
		"whoami",
		"cat",
		"ls",
		"pwd",
		"grep",
		"find",
		"chmod",
		"chown",
		"string",
		"stdlib"
	]

	for keyword in keywords:
		if keyword in content.lower():
			msg = "Invalid keyword found"
			if output: msg += " in output of command"
			raise Exception(msg)
		
	return content

def compile(content: str) -> str:

	name = compiled_bin = ""
	try:
		if len(content) > MAX_CONTENT_LEN:
			raise Exception("Content too long.")
		
		content = validate(content)

		with NamedTemporaryFile(suffix='.c', delete=False) as f:
			f.write(content.encode())
		name = f.name

		compiled_bin = get_random_string(10)

		debug(f"STORED AT: {name}")
		os.environ['PATH']="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" # Manually adding the path to fix cc1 issue.
		output = subprocess.run(['gcc', f.name, '-w', '-o', compiled_bin], stderr=subprocess.PIPE, stdout=subprocess.PIPE)
		if output.returncode != 0:
			debug(f"stdout: {output.stdout.decode()}")
			debug(f"stderr: {output.stderr.decode()}")
			raise Exception("[ERROR] Compilation failed.")

		del os.environ['PATH']
		output = subprocess.run([f"./{compiled_bin}"], stderr=subprocess.PIPE, stdout=subprocess.PIPE)
		if output.returncode != 0:
			debug(f"stdout: {output.stdout.decode()}")
			debug(f"stderr: {output.stderr.decode()}")
			raise Exception("[ERROR] Execution failed.")
		
		try:
			output = output.stdout.decode()	
		except:
			raise Exception("[ERROR] Invalid output.")
		
		output = validate(output, output=True)
		print("[OUTPUT]")
		print(output[:MAX_OUTPUT_LEN])
		print("*" * 10)

	except Exception as E:
		print(E)
	finally:
		subprocess.run(["rm", "-f", name, compiled_bin])

if __name__ == "__main__":

	data = ""
	print("Enter the C code to compile (Type word 'END-OF-CODE' to submit OR press enter (empty newline)):")
	while True:
		try:
			curr_d = input()
			if curr_d == "" or curr_d.lower() == "end-of-code":
				break
			data += curr_d + "\n"

		except Exception as E:
			print(E)
			break
	
	debug(f"Passed data: {data}")
	print(f"Received {len(data)} bytes.")
	compile(data)
