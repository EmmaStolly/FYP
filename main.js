const express = require('express')
const app = express()
const exphbs = require('express-handlebars');

app.engine('handlebars', exphbs());
app.set('view engine', 'handlebars');

//Allows handlebars to find my stylesheet
app.use(express.static(__dirname + '/public'));

app.get('/', function (req, res) {
    res.render('home');
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log(`Our app is running on port ${ PORT }`);
});
