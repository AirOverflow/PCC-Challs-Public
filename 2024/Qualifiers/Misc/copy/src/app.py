#!/usr/local/bin/python3

with open("/flag.txt", "r") as f:
    flag = f.read().strip()

usernames = [
    "Abdul-Haseeb2003",
    "CloudMeshSolutions",
]
hashes = [
    "0bd68c8005154d102084895d471d5f3f1899924c",
    "db00854eec5bfd20e2c763598747dde4f39321d4"
]

username = input("Copycat username? ")
if username.lower() not in [u.lower() for u in usernames]:
    print("Nope! Don't blame anyone else, this guy might be good, who knows")
    exit()

print("HMMMMMMMM. Good job!")

hash = input("What was the hash where TheFlash2k was given credits (Atleast first 7 character)? ")
for h in hashes:
    if h[:7] == hash[:7]:
        print(f"Good job! Here's your flag: {flag}")
        exit()
print("Nope! Try again")
exit()
