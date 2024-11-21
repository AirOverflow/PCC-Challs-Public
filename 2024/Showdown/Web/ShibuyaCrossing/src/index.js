const express = require('express');
const util = require('util');
const crypto = require('crypto');
const jwt = require('jsonwebtoken');
const exec = util.promisify(require('child_process').exec);

const secretkey = crypto.randomBytes(32).toString('hex');
const superSecret = crypto.randomBytes(32).toString('hex');
const app = express();

app.use(express.urlencoded({ extended: false }));
Object.freeze(Object.prototype)

function verifyToken(token) {
    try {
        return jwt.verify(token, secretkey, { algorithms: ['HS256'] });
    } catch (error) {
        return false;
    }
}

function generateToken(uid) {
    const payload = {'uid': uid};
    const options = { expiresIn: '1h', algorithm: 'HS256' };

    return jwt.sign(payload, secretkey, options);
}

const users = {}

app.post('/register', (req, res) => {
    const { username, password, message } = req.body;

    if (typeof username !== 'string' || typeof password !== 'string' || typeof message !== 'string' || message.length > 100) {
        return res.status(400).send('Invalid fields');
    }

    if (users[username]) {
        return res.status(400).send('User already exists');
    }

    users[username] = {
        uid: crypto.randomBytes(16).toString('hex'),
        password: password,
        message: message.toLowerCase()
    };

    res.status(200).send('User registered successfully');
});

app.post('/login', (req, res) => {
    const { username, password } = req.body;

    if (typeof username !== 'string' || typeof password !== 'string') {
        return res.status(400).send('Invalid fields');
    }

    const user = users[username];

    if (!user || user.password !== password) {
        return res.status(401).send('Invalid credentials');
    }

    const token = generateToken(user.uid);
    res.status(200).send(token);
});

app.post('/disclose', (req, res) => {
    const token = req.headers.authorization;

    if (!token) {
        return res.status(401).send('Unauthorized');
    }

    const payload = verifyToken(token);

    if (!payload) {
        return res.status(401).send('Invalid token');
    }

    const user = Object.values(users).find(user => user.uid === payload.uid);

    if (!user) {
        return res.status(404).send('User not found');
    }

    if(!user.message.match('^.*$') && user.message.length >= 180){
        return res.status(200).send(superSecret);
    }

    res.status(401).send("You can't cross Shibuya Crossing Yet");
});

app.post('/execute', async (req, res) => {
    const secret = req.query.secret;
    const { choice , _ } = req.body;
    const integerChoice = +choice;

    if(secret !== superSecret) {
        return res.status(403).send('Invalid secret');
    }
    
    if (isNaN(integerChoice)) {
        return res.status(400).send('Invalid choice: must be a number');
    }

    const commands = ['free -m', 'uptime', 'iostat', 'mpstat', 'netstat', 'ps aux', _ ];

    if (integerChoice < 0 || integerChoice >= commands.length) {
        return res.status(400).send('Invalid choice');
    }

    exec(commands[integerChoice], (error, stdout) => {
        if (error) {
            return res.status(500).send('Error executing command');
        }

        res.status(200).send(stdout);
    });
});

app.listen(1337, () => {
    console.log('This is Shibuya Crossing. You can cross it if you want to');
});
