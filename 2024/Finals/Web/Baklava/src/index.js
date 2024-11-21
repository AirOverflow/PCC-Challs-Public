const express = require("express");
const crypto = require('node:crypto');
const fs = require("fs")
const cookieParser = require("cookie-parser");
const jwt = require('jsonwebtoken');
const { readFile, readFileSync } = require("node:fs");

const app = express();
const secret = crypto.randomBytes(32).toString('hex');

app.use(express.json());
app.use(cookieParser());
app.use(express.urlencoded({ extended: false }));

let flag = fs.existsSync("/flag.txt") ? fs.readFileSync("/flag.txt", "utf-8") : 'PCC{this_is_a_fake_flag}';

let db = {
    admin: {
        uid: "a04ac86d71e84874820bd36c5823364c",
        password: crypto.randomBytes(16).toString('hex'),
        name: "admin",
    }
}
console.log(db.admin.password)

function verifyToken(token) {
    try {
        return jwt.verify(token, secret, { algorithms: ['HS256'] });
    } catch (error) {
        return false;
    }
}

function generateToken(uid) {
    const payload = {'uid': uid};
    const options = { expiresIn: '1h', algorithm: 'HS256' };

    return jwt.sign(payload, secret, options);
}

function parseBody(body) {
    const parsed = {}
    for (let key in body) {
        parsed[key] = (key == "password" || key == "id") ? body[key].toString() : body[key]
    }

    return parsed;
}

function findUser(id, password) {
    const vv =  db[id];
    if (vv && (vv.password == password || vv.password.match(password.value))) {
        return vv;
    }
    return null;
}

app.post("/login", async (req, res) => {
    try{
        const { id, password } = req.body
        const body = parseBody(JSON.parse(`{"id": "${id}", "password": "${password}"}`))        

        if (!id || !password) {
            res.json({ message: "Invalid Request" })
            return
        }

        if(typeof id !== "string" || typeof password !== "string"){
            res.json({ message: "Invalid Request" })
            return
        }

        if (!(Object.keys(db).includes(body.id))) {
            res.json({ message: "Invalid Credentials" })
            return
        }

        const user = findUser(body.id, body.password)
        if (!user){
            res.json({ message: "Invalid Credentials" })
            return
        }
    
        if (user["password"] == password) {
            let token = await generateToken(db[id]["uid"])
            res
                .cookie("auth", token, {
                    maxAge: 30000,
                }, { algorithm: 'HS256' }).json({ message: "success" })
            return
        } else {
            res.json({ message: "Invalid Credentials" }, 401)
            return
        }
    }catch(e){
        res.json({ message: "Invalid Credentials" })
    }
})

app.post("/flag", async (req, res) => {
    const token = req.cookies.auth ?? "";
    let data = ""
    try {
        data = await verifyToken(token)
    } catch (e) {
        res.json({ message: "Invalid Token" })
        return
    }

    if (db.admin.uid != data.uid) {
        res.json({ message: "You don't have permission to access this API" })
        return
    }

    res.json({ flag: flag || "PCC{this_is_a_fake_flag}" })
    return
})

app.use(express.urlencoded({ extended: false }));
app.listen(1337);