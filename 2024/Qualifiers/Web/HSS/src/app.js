const express = require('express');
const cookieParser = require('cookie-parser');
const path = require('path');
const app = express();

app.set("views", path.join(__dirname, "views"));
app.set('view engine', 'ejs');
app.use(cookieParser());
app.use('/static', express.static('static'))
app.use(express.urlencoded(
    {extended: false}
));

app.use('/', require('./app/index.js'));

app.listen(process.env.PORT || 1337 , () => {
    console.log('Running...');
});