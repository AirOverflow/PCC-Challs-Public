from flask import Flask, render_template, render_template_string
from flask import request

app = Flask(__name__)

data = """
<!DOCTYPE html>
<html>
<head>
    <title>Lama</title>
</head>
<body>
    <h1>Flag</h1>
    <p>Flag is: <code>{}</code></p>
</body>
</html>
"""

#app.mount("/static", StaticFiles(directory="static"), name="static")
# templates = Jinja2Templates(directory="templates")

@app.route("/robots.txt")
def robots():
    return "Disallow: /flag/*"

@app.route("/")
def root():
    return render_template("index.html")

invalid_chars = [
    " ",
    "base",
    "read",
    "popen",
    "request",
    "flag"
]

@app.route("/flag/<flager>", methods=["GET", "POST"])
def flag(flager):
    for char in invalid_chars:
        if char in flager.lower():
            return render_template_string(data.format("Invalid Flag!"))
    rtval = render_template_string(data.format(flager))
    if "PCC{" in rtval:
        return render_template_string(data.format("Invalid Flag!"))
    return rtval