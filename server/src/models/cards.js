const mongoose = require('mongoose');
const { Schema } = mongoose;

const CardsSchema = new Schema({
    name: {type: String, require: true},
    mealTime: {type: String, require: true},
    eatAt: {type: String, require: true},
    state: {type: String, require: true},
    measures: [{
        w1: {type: Number}, 
        w2: {type: Number}, 
        w3: {type: Number}, 
        w4: {type: Number},  
    }]
});

module.exports = mongoose.model('Cards', CardsSchema);