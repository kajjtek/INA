import request from 'supertest';
import app from '../src/index.js';
import { expect } from 'chai';
import { expect as chai } from 'chai';
import { response } from 'express';

describe('POST /ingredients/create', function(){
    it('should create a new ingredient', async function(){
        try{
     const response = await request(app).post('/ingredients/create')
            .send({
                name:'TEST',
                description:'TEST',
                alcohol:'true',
                image:'',
                amount:50
            });
            expect(response.status).to.eql(201);
            expect(response.text).to.eql('Ingredient '+'TEST'+ ' was successfully created');
        }catch(error){
            console.error(error);
        }
    });

    it('responses to two ingredients with the same name being created by adding their volumes', async function () {
        try{
            const response = await request(app).post('/ingredients/create')
                   .send({
                       name:'TEST',
                       description:'TEST',
                       alcohol:'true',
                       image:'',
                       amount:50
                   });
                   expect(response.status).to.eql(201);
                   expect(response.text).to.include('Ingredient already exists - it was added to the already existing inventory');
               }catch(error){
                   console.error(error);
               }
    });

    it('should send an error that name cannot be a null', async function () {
        try{
            const response = await request(app).post('/ingredients/create')
                   .send({
                       name:'',
                       description:'TEST',
                       alcohol:'true',
                       image:'',
                       amount:50
                   });
                   expect(response.ok).to.be.false;
                   expect(response.status).to.eql(400);
                   expect(response.text).to.eql('Name cannot be a null');
               }catch(error){
                   console.error(error);
               }
    });

    it('should send an error that description cannot be a null', async function () {
        try{
            const response = await request(app).post('/ingredients/create')
                   .send({
                       name:'TEST',
                       description:'',
                       alcohol:'true',
                       image:'',
                       amount:'50'
                   });
                   expect(response.ok).to.be.false;
                   expect(response.status).to.eql(400);
                   expect(response.text).to.eql('Description cannot be a null');
               }catch(error){
                   console.error(error);
               }
    });

    it('should send an error that alcohol cannot be a null', async function () {
        try{
            const response = await request(app).post('/ingredients/create')
                   .send({
                       name:'TEST',
                       description:'TEST',
                       alcohol:'',
                       image:'',
                       amount:50
                   });
                   expect(response.ok).to.be.false;
                   expect(response.status).to.eql(400);
                   expect(response.text).to.eql('Alcohol cannot be a null');
               }catch(error){
                   console.error(error);
               }
    });
});

describe('POST /cocktails/create', function () {
    it('should create a new cocktail', async function(){
        try{
            const response = await request(app).post('/cocktails/create')
            .send({
                name:'TEST',
                category:'TEST',
                instructions:'TEST',
                ingredients:[
                    {name:'Test', ml:10}
                ]
            });
            expect(response.status).to.eql(201);
            expect(response.text).to.eql('Cocktail sucessfully created');
        }catch(error){
            console.error(error);
        }
    });

    it('should react to the cocktails of the same name being created', async function () {
        try{
            const response = await request(app).post('/cocktails/create')
            .send({
                name:'TEST',
                category:'TEST',
                instructions:'TEST',
                ingredients:[
                    {name:'Test', ml:10}
                ]
            });
            expect(response.status).to.eql(400);
            expect(response.text).to.include('Cocktail with this name already exists - try using a different name or use /cocktails/edit/:name to edit the cocktail');
        }catch(error){
            console.error(error);
        }
    })

    it('should send an error that name cannot be a null', async function () {
        try{
            const response = await request(app).post('/cocktails/create')
            .send({
                name:'',
                category:'TEST',
                instructions:'TEST',
                ingredients:[
                    {name:'Test', ml:10}
                ]
            });
                   expect(response.ok).to.be.false;
                   expect(response.status).to.eql(400);
                   expect(response.text).to.eql('query wrong - name cannot be a null ');
               }catch(error){
                   console.error(error);
               }
    });

    it('should send an error that category cannot be a null', async function () {
        try{
            const response = await request(app).post('/cocktails/create')
                   .send({
                    name:'TEST',
                    category:'',
                    instructions:'TEST',
                    ingredients:[
                        {name:'Test', ml:10}
                    ]
                   });
                   expect(response.ok).to.be.false;
                   expect(response.status).to.eql(400);
                   expect(response.text).to.eql('query wrong - category cannot be a null ');
               }catch(error){
                   console.error(error);
               }
    });

    it('should send an error that instruction cannot be a null', async function () {
        try{
            const response = await request(app).post('/cocktails/create')
                   .send({
                    name:'TEST',
                    category:'TEST',
                    instructions:'',
                    ingredients:[
                        {name:'Test', ml:10}
                    ]
                   });
                   expect(response.ok).to.be.false;
                   expect(response.status).to.eql(400);
                   expect(response.text).to.eql('query wrong - instructions cannot be a null ');
               }catch(error){
                   console.error(error);
               }
    });

    it('should send an error that the ingredients\' name cannot be a null', async function () {
        try{
            const response = await request(app).post('/cocktails/create')
                   .send({
                    name:'TEST',
                    category:'TEST',
                    instructions:'TEST',
                    ingredients:[
                        {name:'', ml:10}
                    ]
                   });
                   expect(response.ok).to.be.false;
                   expect(response.status).to.eql(404);
                   expect(response.text).to.include.oneOf(['The ingredient required to creata a cocktail was not found','Database error during the ingredient searching phase']);
               }catch(error){
                   console.error(error);
               }
    });

    it('should send an error that the ingredients\' ml cannot be a null', async function () {
        try{
            const response = await request(app).post('/cocktails/create')
                   .send({
                    name:'TEST',
                    category:'TEST',
                    instructions:'TEST',
                    ingredients:[
                        {name:'TEST', ml:''}
                    ]
                   });
                   expect(response.ok).to.be.false;
                   expect(response.status).to.eql(500);
                   expect(response.text).to.include('Dodawanie do tabeli pomocniczej nie dziala');
               }catch(error){
                   console.error(error);
               }
            });
});

describe('GET /cocktails', function(){
it('shows all cocktails', async function(){
    const response = await request(app).get('/cocktails');
    expect(response.status).to.be.eql(200);
    expect(response.type).to.be.eql('application/json');
    expect(response.body).to.be.an('array').with.length.greaterThan(0);
});

it('responses to no cocktails in the database', async function () {
    await request(app).delete('/cocktails/deleteAll');
    const response = await request(app).get('/cocktails');
    expect(response.status).to.be.eql(404);
})
});

describe('GET /ingredients', function(){
    it('shows all ingredients', async function(){
        const response = await request(app).get('/ingredients');
        expect(response.status).to.be.eql(200);
        expect(response.type).to.be.eql('application/json');
        expect(response.body).to.be.an('array').with.length.greaterThan(0);
        // expect(response.body).to.be.greaterThan(0);
        // expect(response.body.category).to.be.greaterThan(0);
        // expect(response.body.instructions).to.be.greaterThan(0);
    });

    it('responses to no ingredients in the database', async function () {
        await request(app).delete('/ingredients/deleteAll');
        const response = await request(app).get('/ingredients');
        expect(response.status).to.be.eql(404);
    })

    });

describe('GET /ingredients/:id', function(){
it('succeds in getting the given ingredient', async function () {
    await request(app).post('/ingredients/create')
    .send({
        name:'TEST',
        description:'TEST',
        alcohol:'true',
        image:'',
        amount:50});
    const response = await request(app).get('/ingredients/1');
    expect(response.status).to.be.eql(200);
    expect(response.type).to.be.eql('application/json');
});

it('responses to ingredient not found',async function(){
    const response = await request(app).get('/ingredients/256');
    expect(response.status).to.be.eql(404);
});
});

describe('GET /cocktails/:id', function(){
    it('succeds in getting the given cocktail', async function () {
        await request(app).post('/cocktails/create')
        .send({
            name:'TEST',
                category:'TEST',
                instructions:'TEST',
                ingredients:[
                    {name:'TEST', ml:1}
                ]});
        const response = await request(app).get('/cocktails/1');
        expect(response.status).to.be.eql(200);
        expect(response.type).to.be.eql('application/json');
    });
    
    it('responses to cocktail not found',async function(){
        const response = await request(app).get('/cocktails/256');
        expect(response.status).to.be.eql(404);
    });
    });

describe('PUT /ingredients/edit/:name', function(){
    it('edit successful', async function(){
        const response = await request(app).put('/ingredients/edit/TEST')
        .send({
            name: 'TEST2',
            description:'TEST2',
            alcohol:'false',
            amount:100
        });
        expect(response.status).to.be.eql(200);
        expect(response.text).to.include('Successful update');
    });

    it('couldnt find the ingredient to edit',async function () {
        const response = await request(app).put('/ingredients/edit/ASDAFAWQAFAQ')
        .send({
            name: 'TEST2',
            description:'TEST2',
            alcohol:'false',
            amount:100
        });
        expect(response.status).to.be.eql(404);
        expect(response.text).to.include('Not found');
    })
});

describe('PUT /cocktails/edit/:name', function(){
    it('edit cocktail successful', async function(){
        const response = await request(app).put('/cocktails/edit/TEST')
        .send({
            name:'TEST2',
                category:'TEST2',
                ingredients:[
                    {originalname:'TEST',name:'Test', ml:10}
                ]
        });
        expect(response.status).to.be.eql(200);
        expect(response.text).to.include('Successful update');
    });

    it('couldnt find the cocktail to edit',async function () {
        const response = await request(app).put('/cocktails/edit/ASDAFAWQAFAQ')
        .send({
            name:'TEST2',
                category:'TEST',
                ingredients:[
                    {originalname:'Test', name:'Test2', ml:10}
                ]
        });
        expect(response.status).to.be.eql(404);
        expect(response.text).to.include('Not found');
    });

    it('couldnt fint the ingredient in the supporting table', async function () {
        const response = await request(app).put('/cocktails/edit/TEST2')
        .send({
            name:'TEST',
                category:'TEST',
                instructions:'TEST',
                ingredients:[
                    {originalname:'afasfaffaaf',name:'Test', ml:10}
                ]
        });
        expect(response.status).to.be.eql(404);
        expect(response.text).to.include('Not found');
    });
});


describe('DELETE /cocktails/delete/:id', function(){
    it('Deletes the required cocktail correctly', async function () {
        const response = await request(app).delete('/cocktails/delete/1');
        expect(response.text).to.include('Delete process completed');
        expect(response.status).to.be.eql(200);
    });

    it('doesnt delete the cocktail that doesnt exist', async function(){
        const response = await request(app).delete('/cocktails/delete/256');
        expect(response.text).to.include('Not Found');
        expect(response.status).to.be.eql(404);
    });
});

describe('DELETE /ingredients/delete/:id', function(){
    it('Deletes the required ingredient correctly', async function () {
        const response = await request(app).delete('/ingredients/delete/1');
        expect(response.text).to.include('Delete process completed');
        expect(response.status).to.be.eql(200);
    });

    it('doesnt delete the ingredient that doesnt exist', async function(){
        const response = await request(app).delete('/ingredients/delete/256');
        expect(response.text).to.include('Not Found');
        expect(response.status).to.be.eql(404);
    });
});

describe('DELETE /ingredients/deleteAll',function(){
    it('responds to no ingredients to delete',async function () {
        const response = await request(app).delete('/ingredients/deleteAll');
        expect(response.text).to.include('Not Found');
        expect(response.status).to.be.eql(404);
    });
    it('deletes all', async function () {
        await request(app).post('/ingredients/create')
            .send({
                name:'TEST',
                description:'TEST',
                alcohol:'true',
                image:'',
                amount:50
            });
        const response = await request(app).delete('/ingredients/deleteAll');
        expect(response.status).to.be.eql(200);
        expect(response.text).to.include('Delete ALL');
    });
});

describe('DELETE /cocktails/deleteAll',function(){
    it('responds to no cocktails to delete',async function () {
        const response = await request(app).delete('/ingredients/deleteAll');
        expect(response.text).to.include('Not Found');
        expect(response.status).to.be.eql(404);
    });
    it('deletes all', async function () {
        await request(app).post('/ingredients/create')
            .send({
                name:'TEST',
                description:'TEST',
                alcohol:'true',
                image:'',
                amount:50
            });
            await request(app).post('/cocktails/create')
            .send({
                name:'TEST',
                category:'TEST',
                instructions:'TEST',
                ingredients:[
                    {name:'Test', ml:10}
                ]
            });    
        const response = await request(app).delete('/ingredients/deleteAll');
        expect(response.status).to.be.eql(200);
        expect(response.text).to.include('Delete ALL');
    });
});
//pomyslec nad tym co sie stanie jak dodajemy ten sam koktajl + dodac testowanie tego
//POPRAWIC GET
//FILTRY I SORTOWANIE
//DOKUMENTACJA
//USPRAWNIC KOD