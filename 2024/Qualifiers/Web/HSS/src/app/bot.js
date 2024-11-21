const puppeteer = require('puppeteer');
const parse = require('url-parse');
const fs = require('fs');

const cookies = [{
    name: 'session',
    value: fs.readFileSync('/tmp/cookie', 'utf8'),
    domain: "127.0.0.1",
    httpOnly: true
}];

const bot = async function (url){
    const URL = parse(url, true)
    try{
        const browser = await puppeteer.launch({
            headless: true,
            args: [
                '--no-sandbox',
                '--disable-setuid-sandbox',
                '--disable-dev-shm-usage'
            ],
            dumpio: true,
        });
        page = await browser.newPage('http://127.0.0.1:1337');
        await page.setCookie(
            ...cookies
        );

        console.log(URL.href)
        await page.goto(`http://127.0.0.1:1337/?link=${URL.href}`);
        setTimeout(() => {
            browser.close();
        }, 5000);
    } catch (err) {
        console.log(`err : ${err}`);
    }
}

module.exports = bot;