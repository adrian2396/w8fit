const express = require("express");
const morgan = require('morgan');
const path = require("path");
const cors = require('cors')

// Initilitaicions
const app = express();
const { mongoose } = require('./database');

// Settings
app.set('port', process.env.PORT || 3001);

// Middlewares
app.use(morgan('dev'));
app.use(express.json());
app.use(cors())

// Global Variables

// Routes
app.use('/api/data', require('./routes/cards.routes'));
app.use('/api/balance', require('./routes/balance.routes'));
//app.use('/api/calibrate', require('./src/routes/tasks.routes'));
//app.use('/api/save', require('./src/routes/tasks.routes'));

// Static Files
app.use(express.static(path.join(__dirname, 'public')));

// Server is listening
app.listen(app.get('port'), () => {
  console.log('Server listening on port', app.get('port'));
});