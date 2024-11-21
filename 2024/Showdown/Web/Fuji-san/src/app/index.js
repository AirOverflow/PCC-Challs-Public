const parse = require('url-parse');
const express = require('express');
const bot = require('./bot');
const fs = require('fs');
const crypto = require('crypto');
const router = express();

const supersecret = crypto.randomBytes(32).toString('hex');

router.get('/', (req, res) => {
    const link = req.query.link;
    if (link === undefined) {
        res.render('index', {path: '#'});
        return;
    }

    let data = JSON.parse(`{"username":"guest", "link":"${link}"}`);
    const url = parse(data['link'], true);
    const hostname = url.hostname;
    const scheme = url.protocol;

    if ((scheme === 'http:' && hostname === 'localhost' && data['username'] === 'hashi-san')) {
        if(data['pass'] == 'sh0wd0wn'){
            res.render('index', {path: url.host.indexOf('1337') ? url.pathname : '#'});
            return;
        }
    }

    res.render('index', {path:'#'});
});

router.get('/secret', (req, res) => {
    const session = req.cookies.session;
    const cookie = fs.existsSync('/tmp/cookie') ? fs.readFileSync('/tmp/cookie', 'utf8') : 'PCC{DUMMY}';
    if (session === cookie) {
        result = supersecret;
    } else {
        result = 'Nice try!';
    }
    res.send(result);
});

router.post('/execute', (req, res) => {
    let {secret, command} = req.body;

    if (secret !== supersecret) {
        res.send('<script>alert("Invalid Secret!");history.go(-1);</script>');
        return;
    }

    try {
        const exec = require('child_process').exec;
        exec(command, (err, stdout, stderr) => {
            if (err) {
                res.send('Error executing command');
                return;
            }
            res.send(stdout);
        });
    } catch (err) {
        res.send('Error executing command');
    }
});

router.get('/report', (req, res) => {
    res.render('report.ejs');
});

router.post('/report', (req, res) => {
    try{
        if (req.body.parameters === undefined) {
            res.send('<script>alert("Some Error!");history.go(-1);</script>');
            return;
        }
        bot(req.body.parameters);
        res.send('<script>alert("Your page will be reiewed soon thanks!");history.go(-1);</script>');
    } catch (err) {
        res.send('<script>alert("Some Error!");history.go(-1);</script>');
    }
});

module.exports = router;