const jwt = require('jsonwebtoken');
const path = require('path');
const cookieParser = require('cookie-parser');
const crypto = require('crypto');
const express = require('express');
const fs = require('node:fs');

const app = express();

app.set('view engine', 'ejs');
app.set('views', path.join(__dirname, 'static'));

app.use(cookieParser());
app.use(express.urlencoded({ extended: true }));

function readFile(filePath) {
    try {
        return fs.readFileSync(filePath, { encoding: 'utf8' });
    } catch (err) {
        console.error(err);
        process.exit(1);
    }
}

const FLAG = readFile('/flag.txt');
const key_data = readFile('/priv.pem');
const public_key = readFile('/pub.crt');

const users = {
    'admin': {'password': crypto.randomBytes(50).toString('hex')},
    'guest': {'password': 'guest'}
};
console.log(users);

function verifyToken(token) {
    try {
        const alg = jwt.decode(token, { complete: true }).header.alg;
        if (alg === 'RS256') {
            return jwt.verify(token, public_key);
        } else if (alg === 'HS256') {
            return jwt.verify(token, public_key, { algorithms: ['HS256'] });
        } else if (alg === 'ES256') {
            return jwt.verify(token, public_key, { algorithms: ['ES256'] });
        } else {
            return false;
        }
    } catch (error) {
        return false;
    }
}

function generateToken(username) {
    const payload = {'username': username};
    const options = { expiresIn: '1h', algorithm: 'RS256' };

    return jwt.sign(payload, key_data, options);
}

function isAdmin(token) {
    var decoded = verifyToken(token);

    if (decoded.username === 'admin') {
        return true;
    }
    return false;
}

function requireAuthentication(req, res, next) {
    if (typeof req.cookies.token !== 'string') {
        res.redirect('/login');
        return;
    }
    if (verifyToken(req.cookies.token) === false) {
        res.clearCookie('token').redirect('/login');
        return;
    }

    next();
}

function requireNoAuthentication(req, res, next) {
    if (typeof req.cookies.token === 'string') {
        res.status(403);
        res.send('you are already logged in.');
        return;
    }

    next();
}

function checkDataTypes(req, res, next) {
    if (typeof req.body.username !== 'string' || typeof req.body.password !== 'string') {
        res.status(400);
        res.send('username or password is not a string.');
        return;
    }

    next();
}

app.post('/login', requireNoAuthentication, checkDataTypes, (req, res) => {
    const username = req.body.username;
    const password = req.body.password;
    if (!username || !password) {
        res.status(400);
        res.send('username or password is empty.');
        return;
    }

    console.log(username, password);
    if (!username in users) {
        res.status(401);
        res.send('no such user found.');
        return;
    }

    if (username !== "guest" && users[username]['password'] !== password) {
        res.status(401);
        res.send('username or password is wrong.');
        return;
    }

    res.cookie('token', generateToken(username), { httpOnly: true });
    res.redirect('/');
});

app.post('/logout', (req, res) => {
    res.clearCookie('token').redirect('login');
});

app.get('/admin', (req, res) => {
    if (isAdmin(req.cookies.token) !== true) {
        res.status(401);
        res.send('nice try hehe!');
        return;
    }
    res.send(FLAG);
});

app.listen(1337, '0.0.0.0');
