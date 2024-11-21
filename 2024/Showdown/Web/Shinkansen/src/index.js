const express = require('express');
const path = require('path');
const util = require('util');
const crypto = require('crypto');
const exec = util.promisify(require('child_process').exec);

const superSecret = crypto.randomBytes(32).toString('hex');
const app = express();

app.use(express.urlencoded({ extended: false }));

app.post('/disclose', (req, res) => {
    const { number1, number2 } = req.body;

    if (isNaN(number1) || isNaN(number2)) {
        res.send("Invalid boarding Numbers for Shinkansen");
        return;
    }

    const check1 = Number.parseInt(number1);
    const check2 = Number.parseInt(number2);

    if(check1 < 0 || check1 > 2 || check2 < 0 || check2 > 2){
        res.send("You are unable to board the Shinkansen. Wromg Boarding Numbers");
        return;
    }

    const calculation = 1+(parseInt(number1 - parseFloat(number2)))
    if(calculation >= 10){
        res.send(superSecret);
        return;
    }

    res.send("You are unable to board the Shinkansen");
    return;
});

app.post('/execute', async (req, res) => {
    const secret = req.query.secret;
    const { choice,ㅤ} = req.body;
    const integerChoice = +choice;

    if(secret !== superSecret) {
        return res.status(403).send('Invalid secret');
    }
    
    if (isNaN(integerChoice)) {
        return res.status(400).send('Invalid choice: must be a number');
    }

    const commands = ['free -m', 'uptime', 'iostat', 'mpstat', 'netstat', 'ps aux',ㅤ];

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
    console.log('Dammnnnn, you just sat on Shinkansen. The fastest train that will take to you the Showdown');
});
