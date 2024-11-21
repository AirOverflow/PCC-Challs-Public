#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# -*- author: @TheFlash2k -*-

from collections import defaultdict
import re

FLAG = ""
with open("/flag.txt", "r") as f:
    FLAG = f.read()

class JailDict(defaultdict):
    ''' A dictionary that returns None if the key is not found. '''
    def __init__(self, d):
        super().__init__(lambda: None, d)

banned = JailDict({
    'import': True,
    'eval': True,
    'exec': True,
    'open': True,
    'input': True,
    'read': True,
    'write': True,
    'os': True,
    'system': True,
    'subprocess': True,
    'popen': True,
    'start': True,
    'call': True,
    'spawn': True,
    'fork': True,
    'pty': True
})

# Ban all uppercase, lowercase, numbers and _, {}, [], () and . using regex:
ban_regex = r'[a-zA-Z0-9_{}()\[\]\.]+'

class User:
    def __init__(self, name):
        self.name = name

    def update(self, name):
        old = self.name
        self.name = name
        print("Your name has been changed from {} to {}".format(old, self.name))

class Prompts:
    welcome = """Welcome to Yet Another Python Jail!
    No one has been able to escape this jail yet. Do you think you have what it takes to escape?
    """
    ask_name = """Before continuing, please identify yourself? """

def is_logged_in(self: User):
    _ = JailDict({self.name: JailDict({})}) # So you can never really escape my jail.
    if self.name:
        print(f"Logged in as: {name}".format(name=_[self.name]))
        return True
    else:
        print("Please login first!")
        return False


if __name__ == '__main__':

    print(Prompts.welcome)
    name = input(Prompts.ask_name)

    for key in banned.keys():
        if key in name.lower():
            print("You are not allowed to use that name!")
            exit(1)

    user = User(name)
    while True:
        try:
            if not is_logged_in(user):
                print("Man, you are not even logged in!")
                exit()
            cmd = input(">>> ")
            if cmd == 'exit':
                break
            elif cmd == 'help':
                print("You can't escape this jail! Try harder!")
            else:
                if re.match(ban_regex, cmd):
                    print("You are not allowed to use that command!")
                    continue
                exec(cmd)
        except Exception as e:
            print(e)
            exit()