const express = require('express');
const router = express.Router();

const Balance = require('../models/balance');

router.get('/', async (req, res) => {
    const balance = await Balance.find();
    res.json(balance);
});

router.post('/', async (req, res) => {
    const {name, eatAt, description, state, measures, cards} = req.body;
    const card =  new Balance({
        name,
        eatAt,
        description,
        state,
        measures,
        cards
    });
    await card.save();
    res.json({status: 'card saved'});
});

module.exports = router;