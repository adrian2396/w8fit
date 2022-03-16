const express = require('express');
const router = express.Router();

const Cards = require('../models/cards');

router.get('/', async (req, res) => {
    const cards = await Cards.find();
    res.json(cards);
    console.log(cards);
});

router.post('/', async (req, res) => {
    const {name, eatAt, mealTime, state, measures} = req.body;
    const card =  new Cards({
        name,
        eatAt,
        mealTime,
        state,
        measures
    });
    await card.save();
    res.json({status: 'card saved'});
});

router.put('/:id', async (req, res) => {
    const {name, eatAt, mealTime, state, measures} = req.body;
    const newCard = {
        name,
        eatAt,
        mealTime,
        state,
        measures
    }
    await Cards.findByIdAndUpdate(req.params.id, newCard);
    res.json({status: 'Card updated'});
});

router.delete('/:id', async (req, res) => {
    await Cards.findByIdAndRemove(req.params.id);
    res.json({status: 'Card deleted'});
});

module.exports = router;