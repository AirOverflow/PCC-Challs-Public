#!/usr/bin/env python3

import socket
from rc4 import RC4

"""
This was the very first Raidware prototype I had setup just to check if I could easily make the communication work and use RC4.

I had to make a few changes to the code to make it a more CTF style challenge.
"""

PREPEND = "||RAIDWARE-SoM||"
POSTPEND = "||RAIDWARE-EoM||"

cmds = {
    "ping": "PONG",
    "flag": open("/flag", "r").read(),
    "help": "Commands: ping, flag, help",
    "init_raidware": "Hash?"
}

init = 0

class Raidware:
    """
    TCP Server for Raidware C2
    """

    def __init__(self, PORT):
        self.PORT = PORT

    def setup(self, msg):
        """
        Setup the message to be sent
        """
        return RC4(b"r41dw4re_k3y_f0r_c0mm5").crypt(f"{PREPEND}{msg}{POSTPEND}".encode()) + b"\n"

    def start(self):
        """
        Allow the server to listen for incoming connections
        and make the server handle multiple clients
        """
        global init
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        
        s.bind(("0.0.0.0", self.PORT))
        s.listen()

        print(f"[*] Listening on {self.PORT}")

        while True:
            conn, addr = s.accept()
            print(f"[*] Connection from {addr[0]}:{addr[1]}")

            while True:
                data = conn.recv(1024)
                if not data:
                    break

                print(f"[*] Encrypted: {data}")
                data = RC4(b"r41dw4re_k3y_f0r_c0mm5").crypt(data)
                print(f"[*] Decrypted: {data}")

                if not PREPEND.encode() in data or not POSTPEND.encode() in data:
                    print("[!] Invalid data received")
                    conn.close()
                    break

                data = data.replace(PREPEND.encode(), b"")
                data = data[:data.index(POSTPEND.encode())]

                print(f"[*] Received: {data.decode('latin-1')}")

                if data.decode("latin-1").lower() in cmds.keys():
                    resp = cmds[data.decode("latin-1").lower()]
                    if init == 0 and resp != cmds["init_raidware"]:
                        print("[!] Raidware not initialized")
                        conn.send(self.setup("Please initialize Raidware"))
                    else:
                        print(f"Current init: {init}")
                        print(f"[*] Sending: {resp}")
                        conn.send(self.setup(resp))

                else:
                    if data[:5] == b"Hash|":
                        hash = data.split(b"|")[1]
                        print(f"[*] Hash: {hash.decode('latin-1')}")
                        if hash == b"b7259e73a31fdb3fb50bd752c423eb86":
                            conn.send(self.setup("OK. Raidware Initialized. Please send \"help\" to get a list of commands."))
                            init = 1
                        else:
                            conn.send(self.setup("Invalid agent hash."))

                    else:
                        print("[!] Invalid command")
                        conn.send(self.setup("Invalid command."))

            conn.close()


if __name__ == "__main__":

    while 1:
        try:
            Raidware(1337).start()
        except KeyboardInterrupt:
            print("[!] Exiting")
            break
        except:
            pass
