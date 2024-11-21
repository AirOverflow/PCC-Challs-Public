const express = require("express");
const crypto = require('node:crypto');
const fs = require("fs")
const cookieParser = require("cookie-parser");
const jwt = require('jsonwebtoken');

const app = express();

app.use(express.json());
app.use(cookieParser());

const flag = fs.readFileSync("/flag.txt", "utf-8");
const PrivateKey = fs.readFileSync("private.pem", "utf-8");
const PublicKey = 'MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEh1pPVTT+jLOJQvFsrArrFIQZ1Yf2FbXuBh7diN6XbaCaxk1NzWRvCFD8waLDQPRNrQcD+Gf2TajOso7b7LR4cg=='

let db = {
    admin: {
        uid: "a04ac86d71e84874820bd36c5823364c",
        password: crypto.randomBytes(16).toString('hex'),
        name: "zoom",
    }
}

Object.freeze(Object.prototype)
let temporaryFileName = crypto.randomBytes(10).toString('hex')

function verifyToken(token) {
    try {
        const alg = jwt.decode(token, { complete: true }).header.alg;
        if (alg === 'RS256') {
            return jwt.verify(token, PublicKey);
        } else if (alg === 'HS256') {
            return jwt.verify(token, PublicKey, { algorithms: ['HS256'] });
        } else if (alg === 'ES256') {
            return jwt.verify(token, PublicKey, { algorithms: ['ES256'] });
        } else {
            return false;
        }
    } catch (error) {
        return false;
    }
}

function generateToken(uid) {
    const payload = {'uid': uid};
    const options = { expiresIn: '1h', algorithm: 'RS256' };

    return jwt.sign(payload, PrivateKey, options);
}

const GenerateSecureRand = () => {
    return new Promise((resolve, _) => {
        let data = ""
        for (var i = 0; i < 1000; i++) {
            data += crypto.randomBytes(1000).toString('hex')[0]
        }
        const hash = crypto.createHash('sha512').update(data)
        let q = hash.digest('hex')

        resolve(q.slice(0, 16))
    })
}

app.post("/login", async (req, res) => {
    try{
        const { id, password } = req.body

        if (!id || !password) {
            res.json({ message: "Invalid Request" })
            return
        }

        if (typeof id !== "string" || typeof password !== "string") {
            res.json({ message: "Invalid Request" })
            return
        }

        if (!(Object.keys(db).includes(id))) {
            res.json({ message: "Already Registered" })
            return
        }
    
        if (db[id]["password"] == password) {
            let token = await generateToken(db[id]["uid"])
            res
                .cookie("auth", token, {
                    maxAge: 30000,
                }, { algorithm: 'HS256' }).json({ message: "success" })
            return
        } else {
            res.json({ message: "Invalid Credentials" })
            return
        }
    }catch(e){
        res.json({ message: "Invalid Credentials" })
    }
})


app.post("/register", async (req, res) => {
    try{
        const { id, password, name } = req.body;

        if (!id || !password || !name) {
            res.json({ message: "Invalid Request" })
            return
        }

        if (typeof id !== "string" || typeof password !== "string" || typeof name !== "string") {
            res.json({ message: "Invalid Request" })
            return
        }

        if (id.toLowerCase() == "admin" || Object.keys(db).includes(id)) {
            res.json({ message: "Not Allowed" })
            return
        }
        
        db[id] = {
            "password": password,
            "name": name
        }
        db[id]["uid"] = crypto.randomBytes(16).toString('hex')
    
        res.json({ message: "success" })
    }catch(e){
        res.json({ message: "failed" })
    }

})


app.post("/generate_token", async (req, res) => {
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

    const targetFile = await fs.promises.open(temporaryFileName, 'w')
    let rand_data = await GenerateSecureRand()
    await new Promise(resolve => setTimeout(resolve, 500));

    await targetFile.write(rand_data)
    await targetFile.close()
    res.json({ message: "success" })

    return
})

app.post("/validate_token", async (req, res) => {
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
    
    try {
        const { data } = req.body
        const targetFile = await fs.promises.readFile(temporaryFileName)
        await fs.promises.unlink(temporaryFileName)
        temporaryFileName = crypto.randomBytes(10).toString('hex')

        if (data == targetFile) {
            res.json({ flag: flag })
            return
        } else {
            res.json({ message: "Invalid 2FA Code" })
            return
        }
    } catch (e) {
        res.json({ message: "Internal Error" })
        return
    }
})

app.use(express.urlencoded({ extended: false }));
app.listen(1337);