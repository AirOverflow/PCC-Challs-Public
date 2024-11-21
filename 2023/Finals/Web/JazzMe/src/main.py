# main.py

from fastapi import FastAPI, Request
from fastapi.responses import HTMLResponse
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates
import os
import subprocess

app = FastAPI(docs_url=None, redoc_url=None)

#app.mount("/static", StaticFiles(directory="static"), name="static")
templates = Jinja2Templates(directory="templates")

@app.get("/", response_class=HTMLResponse)
async def root(req: Request):
    return templates.TemplateResponse("index.html", {"request": req, "id": id})

invalid_chars = [
    " ",
    "less",
    "more",
    "head",
    "tail",
    "grep",
    "awk",
    "sed",
    "flag",
    "txt",
    "base",
    "*",
]

@app.post("/submit")
async def submit(req: Request):
    data = await req.json()
    name = data.get("name")

    for char in invalid_chars:
        if char in name:
            return {
                "success": False,
                "response": "Invalid Characters"
            }

    try:
        get_output = subprocess.check_output(f"echo " + name, shell=True)
    except:
        return {
            "success": False,
            "response": "Something went wrong on our side."
        }
    return {
        "success": True,
        "response": get_output
    }    