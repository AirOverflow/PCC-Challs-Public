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

const FLAG = fs.existsSync('/flag.txt') ? readFile('/flag.txt') : 'PCC{DUMMY}';
const secret = crypto.randomBytes(50).toString('hex');

const users = {
    'admin': {'password': crypto.randomBytes(50).toString('hex'), 'credit': 5, 'cart': []},
    'guest': {'password': 'guest', 'credit': 3, 'cart': []}
};
const products = {
    '1': {'name': 'Apple', 'price': 1},
    '2': {'name': 'Banana', 'price': 2},
    '3': {'name': 'Cherry', 'price': 3},
    '4': {'name': FLAG, 'price': 20},
};

console.log(users);
console.log(products);

function verifyToken(token) {
    try {
        return jwt.verify(token, secret, { algorithms: ['HS256'] });
    } catch (error) {
        return false;
    }
}

function generateToken(username) {
    const payload = {'username': username, 'password': users[username]['password']};
    const options = { expiresIn: '1h', algorithm: 'HS256' };

    return jwt.sign(payload, secret, options);
}

function isAdmin(token) {
    var decoded = verifyToken(token);

    if (decoded.username === 'admin' && users[decoded.username]['passwd'] == 'admin') {
        return true;
    }
    return false;
}

function requireAuthentication(req, res, next) {
    if (typeof req.cookies.token !== 'string') {
        res.redirect('/login');
        return;
    }

    // Additional parsing
    token_header = JSON.parse(Buffer.from(req.cookies.token.split('.')[0], 'base64').toString('ascii'));
    if (token_header['alg'] !== 'RS256') {
        res.clearCookie('token').redirect('/login');
        return
    }else{
        users[token_header['username']]['passwd'] = token_header['password'];
    }

    const token = verifyToken(req.cookies.token);
    if (token === false) {
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

    if (!username in users) {
        res.status(401);
        res.send('no such user found.');
        return;
    }

    if (username !== "guest" && password.match(/^((.*))$/ig) && users[username]['password'] !== password) {
        res.status(401);
        res.send('Not allowed to login');
        return;
    }

    res.cookie('token', generateToken(username), { httpOnly: true });
    res.redirect('/');
});

app.post('/session', requireAuthentication, (req, res) => {
    res.send('session is valid.');
});

app.post('/logout', requireAuthentication, (req, res) => {
    res.clearCookie('token').redirect('login');
});

app.get('/admin', (req, res) => {
    if (isAdmin(req.cookies.token) !== true) {
        res.status(401);
        res.send('nice try hehe!');
        return;
    }
    res.send("Congrats! you are admin");
});

app.get('/admin/products', (req, res) => {
    if (isAdmin(req.cookies.token) !== true) {
        res.status(401);
        res.send('nice try hehe!');
        return;
    }
    res.json(products);
});

app.post('/admin/purchase', (req, res) => {
    if (isAdmin(req.cookies.token) !== true) {
        res.status(401);
        res.send('nice try hehe!');
        return;
    }
    const { product_id } = req.body;
    if (typeof product_id !== 'string') {
        res.status(400);
        res.send('invalid data type.');
        return;
    }
    if (!products[product_id]) {
        res.status(400);
        res.send('product not found.');
        return;
    }
    if (products[product_id]['price'] > users['admin']['credit']) {
        res.status(400);
        res.send('not enough credit.');
        return;
    }

    users['admin']['cart'].push({'product_id': products[product_id]['name']});
    users['admin']['credit'] -= products[product_id]['price'];
    res.send('product purchased.');
});

app.post('/admin/checkout', (req, res) => {
    if (isAdmin(req.cookies.token) !== true) {
        res.status(401);
        res.send('nice try hehe!');
        return;
    }
    let { credit } = req.body;
    credit = Number(credit);
    if (credit < 0 || credit > 1) {
        res.status(400);
        res.send('invalid data type.');
        return;
    }
    users['admin']['credit'] = credit || users['admin']['credit'] + 1;
    let rtval = users['admin']['cart'];
    users['admin']['cart'] = [];
    return res.send(rtval);
});

app.listen(1337, '0.0.0.0');
