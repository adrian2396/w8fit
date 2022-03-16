const mongoose = require('mongoose');
const { Schema } = mongoose;

const Cards = require('../models/cards');

const BalanceSchema = new Schema({
    actualCardId: {type: String, require: true},
    name: {type: String, require: true},
    eatAt: {type: String, require: true},
    state: {type: String, require: true},
    measures: [{
        w1: {type: Number, require: true}, 
        w2: {type: Number, require: true}, 
        w3: {type: Number, require: true}, 
        w4: {type: Number, require: true},  
    }],
    weigth: {type: String, require: true},
    cards:[
        {
            type: mongoose.Schema.Types.ObjectId,
            ref: Cards,
        }
    ]
});

module.exports = mongoose.model('Balance', BalanceSchema);