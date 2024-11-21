const parse = require('url-parse');
const express = require('express');
const bot = require('./bot');
const fs = require('fs');
const router = express();

router.get('/', (req, res) => {
    const link = req.query.link;
    if (link === undefined) {
        res.render('index', {url: '#'});
        return;
    }

    const data = JSON.parse(`{"username":"guest", "link":"${link}"}`);
    const url = parse(data['link'], true);
    const hostname = url.hostname;

    if ((hostname === 'admin.pk' && data['username'] === 'admin')) {
        if(data['pass'] == 'P@ss'){
            res.render('index', {url:url});
        }
    }

    res.render('index', {url:'#'});
});

router.get('/flag', (req, res) => {
    const session = req.cookies.session;
    const cookie = fs.readFileSync('/tmp/cookie', 'utf8');
    if (session === cookie) {
        result = fs.readFileSync('/flag.txt', 'utf8') || 'PCC{TEST}';
    } else {
        result = 'Nice try!';
    }
    res.render('flag.ejs', {result: result});
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