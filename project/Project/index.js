const express = require('express');
const app = express();
const mysql = require('mysql2');
const cocktail_ingredient = require('./cocktail_ingredient');

const database = mysql.createConnection({
    host:'localhost',
    user:'root',
    password:'0606',
    database: 'solvrorecrutation'
});

database.connect((err)=>{
    if(err){
        console.error("Nie udalo sie polaczyc z baza danych, " + err);
        return;
    }
    console.log("Polaczona z baza danych");
})

app.use(express.json());

const PORT = process.env.PORT || '8080';
app.listen(PORT, ()=>console.log('Server is running ' + PORT));

app.get('/cocktails', (req,res)=>{
database.query('SELECT * from cocktails', (err,result)=>{
    if(err){
        console.error("query is wrong: ",err);
        return res.status(500).send("database error");
    }
    if(result.length==0){
        console.error("The table is empty");
        res.status(404).send('No cocktails have been created');
        return;
    }
    res.status(200).json(result);
})
});

app.get('/cocktails/:id', (req,res)=>{
    database.query('SELECT * from cocktails WHERE cocktail_id=?',parseInt(req.params.id) ,(err,result)=>{
        if(err){
            console.error("query is wrong: ",err);
            return res.status(500).send("database error");
        }
        if(result.length==0){
            console.error("Cocktail wasnt found");
            res.status(404).send('There isnt any cocktail with the requested id: '+parseInt(req.params.id));
            return;
        }
        res.status(200).json(result);
    })
});

app.get('/ingredients', (req,res)=>{
    database.query('SELECT * from ingredients', (err,result)=>{
        if(err){
            console.error("query is wrong: ",err);
            return res.status(500).send("database error");
        }
        if(result.length===0){
            console.error("The ingredients table is empty");
            res.status(404).send('No ingredient has been created');
            return;
        }
        res.status(200).json(result);
    })
    });
    
app.get('/ingredients/:id', (req,res)=>{
        database.query('SELECT * from ingredients WHERE ingredient_id=?',parseInt(req.params.id) ,(err,result)=>{
            if(err){
                console.error("query is wrong: ",err);
                return res.status(500).send("database error");
            }
            if(result.length==0){
                console.error("Ingredient wasnt found");
                res.status(404).send('There isnt any ingredient with the requested id: '+parseInt(req.params.id));
                return;
            }
            res.status(200).json(result);
        })
    });

function ingredientSearching(name,ml){
    return new Promise((resolve,reject)=>{
        
            const query2 = 'SELECT name, ingredient_id from ingredients WHERE name=?';
            database.query(query2,name, (err,result)=>{
                if(err){
                    reject(["Database error during the ingredient searching phase "+name,"500"]);
                    return;
                }
                if(result.length===0){
                    reject(["The ingredient required to creata a cocktail was not found "+name,"404"]);
                    return;
                }
                if(result.amount<ml){
                    reject(["Nie wystarczajaco skladnika "+name,"400"]);
                    return;
                }
                resolve(["Ingredient "+name+ " is correct",result[0].ingredient_id]);
                //moze zakodowac jako array z tym tekstem i id wyszukanym
            });
       
    });
}

function cocktailSearching(name){
    return new Promise((resolve,reject)=>{
        
            const query2 = 'SELECT name, amount, ingredient_id from cocktails WHERE name=?';
            database.query(query2,name, (err,result)=>{
                if(err){
                    reject(["Database error during the ingredient searching phase "+name,"500"]);
                    return;
                }
                if(result.length===0){
                    reject(["The ingredient required to creata a cocktail was not found "+name,"404"]);
                    return;
                }
                resolve(result[0].ingredient_id);
                //moze zakodowac jako array z tym tekstem i id wyszukanym
            });
       
    });
}

function insertingCocktail(name,category,instructions){
    return new Promise((resolve,reject)=>{
        
        const query = 'INSERT INTO cocktails(name,category,instructions) VALUES (?,?,?)';
        database.query(query,[name,category,instructions],(err,result2)=>{
            if(err){
            if(err.message.includes("Column 'name' cannot be null")){
                reject(["query wrong - name cannot be a null "+err,"500"]);
                return;
            }
            if(err.message.includes("Column 'category' cannot be null")){
                reject(["query wrong - category cannot be a null "+err,"500"]);
                return;
            }
            if(err.message.includes("Column 'alcohol' cannot be null")){
                reject(["query wrong - alcohol cannot be a null "+err,"500"]);
                return;
            }}
            resolve(["Koktajls stworzony",result2.insertId])
        });
    
    });
}

function insertingSupportingTable(insertedID,name,ml,ingID){
    return new Promise((resolve,reject)=>{
        database.query('INSERT INTO cocktails_ingredients VALUES(?,?,?,?)',[insertedID,name,ml,ingID],(err,result3)=>{
            if(err){
                reject(['Dodawanie do tabeli pomocniczej nie dziala '+err,'500']);
                return;
            }
            resolve('Prawidlowe dodanie do wspierajacej tabeli');
            // res.status(200).send('Child table filled');
        });
});

}

function updatingTable(type,ArrayOfEdits, ObjectName, ObjectID){
    var ToBeEdited=[];
    var NamesOfEdits=[]
    for(let y of ArrayOfEdits){
            if(y!=null){
                ToBeEdited.push(y[1]);
                NamesOfEdits.push(y[0]);
            }
    }
    const CountOfEdits = ToBeEdited.length;
    if(type.localeCompare('cocktails')===0){
        var query ='UPDATE cocktails SET ';
        var wherestatement =' WHERE '+ObjectName+"=name";
    }else if(type.localeCompare('ingredients')===0){
        var query ='UPDATE ingredients SET ';
        var wherestatement=' WHERE '+ObjectName+"=ingredients.name";
    }else if(type.localeCompare('cocktails_ingredients')===0){
        var query ='UPDATE cocktails_ingredients SET ';
        var wherestatement=" WHERE "+ObjectName+"="+type+".name AND "+ObjectID+"="+type+".cocktail_id";
    }
    for(let i=0;i<CountOfEdits;i++){
        query+= NamesOfEdits[i]+'=?';
        if(i<CountOfEdits-1){
            query+=" , ";
        }
    }

        query+=wherestatement;

        return new Promise((resolve,reject)=>{
            database.query(query,ToBeEdited,(err,result)=>{
                if(err){
                    reject(["Database error - updating","500"]);
                    return;
                }
                if(result.length===0){
                    reject(["Not found","404"]);
                    return;
                }
                else{
                    resolve(["Successful update","200"]);
                }
            })
        });
    
}


app.post('/cocktails/create', (req,res)=>{
        const name = req.body.name||null;
        const category = req.body.category||null;
        const ingredients = req.body.ingredients||null;
        const instructions = req.body.instructions||null;
        var insertedID;
        var ingID=[];

        async function createCocktail(){
            try{
                for(const x of ingredients){
                    const searchingResult = await ingredientSearching(x.name,x.ml);
                    console.log(searchingResult[0]);
                    ingID.push(searchingResult[1]);
                }
                const insertingCocktailResult = await insertingCocktail(name,category,instructions);
                console.log(insertingCocktailResult[0]);
                insertedID=insertingCocktailResult[1];
                for(let i=0;i<ingredients.length;i++){
                    const insertingSupportingTableResult = await insertingSupportingTable(insertedID,ingredients[i].name,ingredients[i].ml,ingID[i]);
                    console.log(insertingSupportingTableResult);
                }
                res.status(200).send("Cocktail sucessfully created");
                console.log("Cocktail successfully created");
            }
            catch(error){
                console.error(error[0]);
                res.status(parseInt(error[1])).send(error[0]);
            }
        }

        createCocktail();
        
    });


app.post('/ingredients/create', (req,res)=>{
        const name = req.body.name||null;
        const description = req.body.description||null;
        var alcohol = req.body.alcohol||null;
        const amount = req.body.amount||null;
        const image = req.body.image||null;

        if(alcohol.localeCompare("true")||alcohol.localeCompare("True")){
            alcohol=true;
        }else{
            alcohol=false;
        }

        const query = 'INSERT INTO ingredients(name,description,alcohol,image,amount) VALUES (?,?,?,?,?)';

        database.query(query,[name,description,alcohol, image, amount], (err,result)=>{
            if(err){
            if(err.message.includes("Column 'name' cannot be null")){
                console.error("query wrong - name cannot be a null");
                res.status(500).send("Name cannot be a null");
                return;
            }
            if(err.message.includes("Column 'description' cannot be null")){
                console.error("query wrong - description cannot be a null");
                res.status(500).send("Description cannot be a null");
                return;
            }
            if(err.message.includes("Column 'alcohol' cannot be null")){
                console.error("query wrong - alcohol cannot be a null");
                res.status(500).send("Alcohol cannot be a null");
                return;
            }
        }
            console.log('Ingredient successfuly created');
            res.status(200).send('Ingredient '+name+' was successfully created');
        })
    });

app.put('/ingredients/edit/:name', (req,res)=>{
    const name = req.body.name||null;
        const description = req.body.description||null;
        var alcohol = req.body.alcohol||null;
        const amount = req.body.amount||null;
        const image = req.body.image||null;

        if(alcohol.localeCompare("true")||alcohol.localeCompare("True")){
            alcohol=true;
        }else{
            alcohol=false;
        }

        async function updatingIngredient() {
            try{
            const upgradeResult = await updatingTable("ingredients",[['name',name],['description',description],['alcohol',alcohol],['amount',amount],['image',image]],name,0);
            res.status(parseInt(upgradeResult[1])).send(upgradeResult[0]);
            }catch(error){
                console.error(error[0]);
                res.status(parseInt(error[1])).send(error[0]);
            }
        }

        updatingIngredient();

});

app.put('/cocktails/edit/:name', (req,res)=>{
        const name = req.body.name||null;
        const category = req.body.category||null;
        const ingredients = req.body.ingredients||null;
        const instructions = req.body.instructions||null;
        var CocktailId;


        async function updatingCocktail(table,tab,ObjectID) {
            try{
            const upgradeResult = await updatingTable(table,tab,name,ObjectID);
            res.status(parseInt(upgradeResult[1])).send(upgradeResult[0]);
            }catch(error){
                console.error(error[0]);
                res.status(parseInt(error[1])).send(error[0]);
            }
        }

        updatingCocktail('cocktails',[["name",name],['category',category],['instructions',instructions]],0);
        (async () => {
            try{
            CocktailId = await cocktailSearching(name);
            }catch(error){
                console.error(error[0]);
                res.status(parseInt(error[1])).send(error[0]);
            }
        })();

        for(let x of ingredients){
            updatingCocktail('cocktails_ingredients',[['name',x.name],['ml',x.ml]],CocktailId);
        }

});

app.delete('/cocktails/delete/:id',(req,res)=>{

});