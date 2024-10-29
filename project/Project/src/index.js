import dotenv from 'dotenv';
dotenv.config();
import express, { json, query } from 'express';
const app = express();
import { createConnection } from 'mysql2';


const database = createConnection({
    host:process.env.DB_HOST,
    user:process.env.DB_NAME,
    password:process.env.DB_PASSWORD,
    database: 'solvrorecrutation'
});

database.connect((err)=>{
    if(err){
        console.error("Nie udalo sie polaczyc z baza danych, " + err);
        return;
    }
    console.log("Polaczona z baza danych");
})

app.use(json());
export default app;

const PORT = process.env.PORT || '8080';
app.listen(PORT, ()=>console.log('Server is running ' + PORT));

function queryConstructor(category, order, direction, query, WhereExists){
    if(category){
        if(!WhereExists){
            query+=" WHERE category=? ";
        }
        else{
            query+=" AND category=? "
        }
    }
    if(order){
        query+=" ORDER BY "+order;
    }
    if(direction){
        query+=" "+direction;
    }
    return(query);
}

function NumberOfCocktails(query, category){
    return new Promise ((resolve,reject)=>{
        database.query(query, category,(err,result)=>{
            if(err){
                console.error("query is wrong: ",err);
                reject(['query is wrong',"500"]);
                return;
            }
            if(result.length==0){
                console.error("The table is empty");
                reject(['No such cocktails have been created',"404"]);
                return;
            }
            resolve(result.length);
    });
})
}

app.get('/cocktails', (req,res)=>{
    const { category, order, direction } = req.query;

(async () => {
    try{
    var konkatenacja=[];
    var i =1;
    var Id = 1;
    var query = 'SELECT * from cocktails '
    if(category||order||direction){
        query = queryConstructor(category,order,direction,query,false);
    }
    const CountCocktails = await NumberOfCocktails(query,category);
    while(i<=CountCocktails){
        query = queryConstructor(category,order,direction,'SELECT * from cocktails WHERE cocktail_id=? ',true);
        if(await doesCocktailExistID(Id,query, category)){
            const cocktailResult = await getCocktailOfId(Id);
            const ingredient_cocktailResult = await getCocktail_IngredientOfId(Id);
            const result = {
                cocktail_id:cocktailResult[0],
                name: cocktailResult[1],
                category: cocktailResult[2],
                instructions: cocktailResult[3],
                ingredients: [ingredient_cocktailResult] 
            }
            konkatenacja.push(result);
            i++;
        }
        Id++;
    }
    res.status(200).send(konkatenacja);}
    catch(error){
        console.error(error[0]);
        res.status(parseInt(error[1])).send(error[0]);
    }
})();
});

function getCocktailOfId(Id){
    return new Promise((resolve,reject)=>{

        database.query('SELECT cocktail_id,name,category,instructions from cocktails WHERE cocktail_id=?',Id ,(err,result)=>{
            if(err){
                console.error("query is wrong: ",err);
                reject(['database error',"500"]);
                return;
            }
            if(result.length==0){
                console.error("Cocktail wasnt found");
                reject(['There isnt any ingredient_cocktail with the requested id',"404"]);
                return;
            }
            console.log(result[0].cocktail_id);
            resolve([result[0].cocktail_id,result[0].name,result[0].category,result[0].instructions]);
            return;
    });
})
}

function getCocktail_IngredientOfId(Id){
    return new Promise((resolve,reject)=>{
        database.query('SELECT name, ml from cocktails_ingredients WHERE cocktail_id=?',Id ,(err,result)=>{
            if(err){
                console.error("query is wrong: ",err);
                reject(['database error',"500"]);
                return;
            }
            if(result.length==0){
                console.error("ingredient_cocktail wasnt found");
                reject(['There isnt any ingredient_cocktail with the requested id',"404"]);
                return;
            }
            resolve(result);
            return;
    });
})
}

app.get('/cocktails/:id', (req,res)=>{
    (async () => {
        try{
        const cocktailResult = await getCocktailOfId(req.params.id);
        const ingredient_cocktailResult = await getCocktail_IngredientOfId(req.params.id);
        const result = {
            cocktail_id:cocktailResult[0],
                cocktailname: cocktailResult[1],
                cocktail_category: cocktailResult[2],
                cocktail_instructions: cocktailResult[3],
                ingredients: [ingredient_cocktailResult] 
        }
        console.log(result);
        res.status(200).json(result);
    }catch(error){
        console.error(error[0]);
        res.status(parseInt(error[1])).send(error[0]);
    }
    })();
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

function supportingTableIngredientsSearch(Id){
    return new Promise((resolve,reject)=>{
        const query = 'SELECT name from cocktails_ingredients WHERE cocktail_id=?'
        database.query(query,[Id],(err,result)=>{
            if(err){
                reject(["Supporting Table Problem"+query,"500"]);console.error("Supporting Table Problem");
                return;
            }
            if(result.length===0){
                reject(["Not found in cocktails_ingredients table"+query+Id,"404"]);
                return;
            }
            else{
                console.log("Znaleziono w wspierajacej tablicy");
                resolve(result);            }
        })
    });
}

function findIngredientAmount(name){
    return new Promise((resolve,reject)=>{
        
        const query2 = 'SELECT amount from ingredients WHERE name=?';
        database.query(query2,name, (err,result)=>{
            if(err){
                reject(["Database error - findIngredientAmount "+name,"500"]);
                return;
            }
            if(result.length===0){
                reject(["Not found - findIngredientAmount func"+name,"404"]);
                return;
            }
            resolve(result[0].amount);
            return;
        });
   
});
}

function ingredientSearching(name,ml,type){
    return new Promise((resolve,reject)=>{
        
            const query2 = 'SELECT name, ingredient_id, amount from ingredients WHERE name=?';
            database.query(query2,name, (err,result)=>{
                if(err){
                    reject(["Database error during the ingredient searching phase "+name,"500"]);
                    return;
                }
                if(result.length===0){
                    reject(["The ingredient required to creata a cocktail was not found "+name,"404"]);
                    return;
                }
                if(result.amount<ml&&type=='cocktails'){
                    reject(["Nie wystarczajaco skladnika "+name,"400"]);
                    return;
                }
                if(type=='cocktails'){
                resolve(["Ingredient "+name+ " is correct",result[0].ingredient_id]);return;}
                else if(type=='cocktails_create'){
                    resolve(result[0].amount);
                    console.log(result[0].amount);
                    return;
                }else{
                    resolve(undefined,'0');
                    return;
                }
            });
       
    });
}

function doesIngredientExist(name){
    return new Promise((resolve,reject)=>{
        
            const query2 = 'SELECT name, ingredient_id, amount from ingredients WHERE name=?';
            database.query(query2,name, (err,result)=>{
                if(err){
                    reject(["Database error during the ingredient searching phase "+name,"500"]);
                    return;
                }
                if(result.length===0){
                    resolve(false);
                    return;
                }
                else{
                    resolve(true);
                    return;
                }
            });
       
    });
}

function doesCocktailExist(name){
    return new Promise((resolve,reject)=>{
        
        const query2 = 'SELECT name, cocktail_id from cocktails WHERE name=?';
        database.query(query2,name, (err,result)=>{
            if(err){
                reject(["Database error during the cocktails searching phase "+name,"500"]);
                return;
            }
            if(result.length===0){
                resolve(false);
                return;
            }
            resolve(true);
        });
   
});
}

function doesCocktailExistID(Id, query, category){
    return new Promise((resolve,reject)=>{
        let tab = [Id];
        if(category){
            tab.push(category);
        }
        // const query2 = 'SELECT name, cocktail_id from cocktails WHERE cocktail_id=?';
        database.query(query,tab, (err,result)=>{
            if(err){
                reject(["Database error during the cocktails searching phase "+Id,"500"]);
                return;
            }
            if(result.length===0){
                resolve(false);
                return;
            }
            resolve(true);
        });
   
});
}

function cocktailSearching(name){
    return new Promise((resolve,reject)=>{
        
            const query2 = 'SELECT name, cocktail_id from cocktails WHERE name=?';
            database.query(query2,name, (err,result)=>{
                if(err){
                    reject(["Database error during the cocktails searching phase "+name,"500"]);
                    return;
                }
                if(result.length===0){
                    console.error("Couldnt find the cocktail "+name+query2);
                    reject(["Couldnt find the cocktail "+name+query2,"404"]);
                    return;
                }
                resolve(result[0].cocktail_id);
            });
       
    });
}

function insertingCocktail(name,category,instructions){
    return new Promise((resolve,reject)=>{
        
        const query = 'INSERT INTO cocktails(name,category,instructions) VALUES (?,?,?)';
        database.query(query,[name,category,instructions],(err,result2)=>{
            if(err){
            if(err.message.includes("Column 'name' cannot be null")){
                reject(["query wrong - name cannot be a null ","400"]);
                return;
            }
            if(err.message.includes("Column 'category' cannot be null")){
                reject(["query wrong - category cannot be a null ","400"]);
                return;
            }
            if(err.message.includes("Column 'instructions' cannot be null")){
                reject(["query wrong - instructions cannot be a null ","400"]);
                return;
            }
        }
            resolve(["Koktajls stworzony",result2.insertId]);
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
            if(y[1]!=null){
                ToBeEdited.push(y[1]);
                NamesOfEdits.push(y[0]);
            }
    }
    const CountOfEdits = ToBeEdited.length;
    ToBeEdited.push(ObjectName);
    var query;
    var wherestatement;
    if(type.localeCompare('cocktails')===0){
        query ='UPDATE cocktails SET ';
        wherestatement =' WHERE cocktails.name=?';
    }else if(type.localeCompare('ingredients')===0){
        query ='UPDATE ingredients SET ';
        wherestatement =' WHERE ingredients.name=?';
    }else if(type.localeCompare('cocktails_ingredients')===0){
        query ='UPDATE cocktails_ingredients SET ';
        wherestatement =' WHERE cocktails_ingredients.name=? AND cocktails_ingredients.cocktail_id=?';
        ToBeEdited.push(ObjectID);
    }
    for(let i=0;i<CountOfEdits;i++){
        // query+=' cocktails.${NamesOfEdits[i]}=? ';
        query+= NamesOfEdits[i]+' = ? ';
        if(i<CountOfEdits-1){
            query+=" , ";
        }
    }

        query+=wherestatement;

        return new Promise((resolve,reject)=>{
            database.query(query,ToBeEdited,(err,result)=>{
                if(err){
                    reject(["Database error - updating"+err+query+NamesOfEdits+ToBeEdited,"500"]);
                    return;
                }
                if(result.affectedRows===0){
                    reject(["Not found","404"]);
                    return;
                }
                else{
                    resolve(["Successful update+"+query+ToBeEdited+NamesOfEdits,"200"]);
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
                if(await doesCocktailExist(name)){
                    res.status(400).send("Cocktail with this name already exists - try using a different name or use /cocktails/edit/:name to edit the cocktail");
                }else{
                for(const x of ingredients){
                    const searchingResult = await ingredientSearching(x.name,x.ml,'cocktails');
                    console.log(searchingResult[0]);
                    ingID.push(searchingResult[1]);
                }
                const insertingCocktailResult = await insertingCocktail(name,category,instructions);
                console.log(insertingCocktailResult[0]);
                insertedID=insertingCocktailResult[1];
                for(let i=0;i<ingredients.length;i++){
                    const insertingSupportingTableResult = await insertingSupportingTable(insertedID,ingredients[i].name,ingredients[i].ml,ingID[i]);
                    console.log(insertingSupportingTableResult);
                    let ML = await ingredientSearching(ingredients[i].name,0,'cocktails_create');
                    console.log(ML);
                    const ingredientMLEdit = await updatingTable('ingredients',[['amount',ML-ingredients[i].ml]],ingredients[i].name,0);
                    console.log(ingredientMLEdit);
                }
                res.status(201).send("Cocktail sucessfully created");
                console.log("Cocktail successfully created");}
            }
            catch(error){
                console.error(error[0]);
                res.status(parseInt(error[1])).send(error[0]);
            }
        }

        createCocktail();
        
    });


app.post('/ingredients/create', async(req,res)=>{
        const name = req.body.name||null;
        const description = req.body.description||null;
        var alcohol = req.body.alcohol||null;
        const amount = req.body.amount||null;
        const image = req.body.image||null;
        let IngredientExsit = undefined;
        let amountofIngredient=0;

        if(alcohol=="true"||alcohol=="True"){
            alcohol=true;
        }else if(alcohol=="false"||alcohol=="False"){
            alcohol=false;
        }
        try{
        IngredientExsit = await doesIngredientExist(name);
        console.log(IngredientExsit);
        if(!IngredientExsit){
            const query = 'INSERT INTO ingredients(name,description,alcohol,image,amount) VALUES (?,?,?,?,?)';

        database.query(query,[name,description,alcohol, image, amount], (err,result)=>{
            if(err){
            if(err.message.includes("Column 'name' cannot be null")){
                console.error("query wrong - name cannot be a null");
                res.status(400).send("Name cannot be a null");
                return;
            }
            if(err.message.includes("Column 'description' cannot be null")){
                console.error("query wrong - description cannot be a null");
                res.status(400).send("Description cannot be a null");
                return;
            }
            if(err.message.includes("Column 'alcohol' cannot be null")){
                console.error("query wrong - alcohol cannot be a null");
                res.status(400).send("Alcohol cannot be a null");
            }
            return;
        }
            console.log('Ingredient successfuly created');
            res.status(201).send('Ingredient '+name+' was successfully created');
        })
        }else if(IngredientExsit){
            amountofIngredient = await findIngredientAmount(name);
            console.log(amountofIngredient);
            const updateIngredient = await updatingTable('ingredients',[['amount',amount+amountofIngredient]],name,undefined);
            res.status(201).send('Ingredient already exists - it was added to the already existing inventory');
        }
    }
    catch(error){
        console.error(error);
        res.status(parseInt(error[1])).send(error[0]);
    }
    });

app.put('/ingredients/edit/:name', (req,res)=>{
    const name = req.body.name||null;
        const description = req.body.description||null;
        var alcohol = req.body.alcohol||null;
        const amount = req.body.amount||null;
        const image = req.body.image||null;

        if((alcohol=="true")||(alcohol==="True")){
            alcohol=true;
        }else{
            alcohol=false;
        }

        async function updatingIngredient() {
            try{
            const upgradeResult = await updatingTable("ingredients",[['name',name],['description',description],['alcohol',alcohol],['amount',amount],['image',image]],req.params.name,undefined);
            res.status(parseInt(upgradeResult[1])).send(upgradeResult[0]);
            console.log(upgradeResult[0]);
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


        async function updatingCocktail(table,tab,ObjectID) {
            try{
            const updateResult = await updatingTable(table,tab,req.params.name,ObjectID);
            console.log('Update Result:', updateResult);
            const SearchResult = await cocktailSearching(name);
            const NamesOfIngredientsTable = await supportingTableIngredientsSearch(SearchResult);
            for(let x of ingredients){
                const ingredient_cocktailResult = await updatingTable('cocktails_ingredients',[['name',x.name],['ml',x.ml]],x.originalname,SearchResult);
                console.log('Update result: '+ingredient_cocktailResult);
            }
            res.status(parseInt(updateResult[1])).send(updateResult[0]);
            }catch(error){
                console.error(error[0]);
                res.status(parseInt(error[1])).send(error[0]);
            }
        }  
        updatingCocktail('cocktails',[["name",name],['category',category],['instructions',instructions]],0);
});

app.delete('/cocktails/delete/:id',(req,res)=>{
    
    (async () => {
    try{
        const cocktail_ingredientDeleteResult = await (()=>{
            return new Promise((resolve,reject)=>{
                let query = 'DELETE FROM cocktails_ingredients WHERE cocktail_id=?';
                database.query(query,[req.params.id],(err,result)=>{
                    if(err){
                        reject(["Database problem - cocktail_ingredients delete "+err,"500"]);
                        return;
                    }
                    if(result.affectedRows===0){
                        reject(["Cocktails_ingredients Not Found - app.Delete","404"]);
                        return;
                    }
                    resolve(["Cocktails_ingredients successfuly deleted","200"]);
                }) 
            })
        })();
        const cocktailDeleteResult = await (()=>{
            return new Promise((resolve,reject)=>{
                let query = 'DELETE FROM cocktails WHERE cocktail_id=?';
            database.query(query,[req.params.id],(err,result)=>{
                if(err){
                    reject(["Database problem - cocktail delete "+err,"500"]);
                    return;
                }
                if(result.length===0){
                    reject(["Cocktail Not Found - app.Delete","404"]);
                    return;
                }
                resolve(["Cocktail successfuly deleted","200"]);
            })
            })
        })();

        res.status(200).send("Delete process completed");
        console.log("Delete Process completed")
    }catch(error){
        res.status(parseInt(error[1])).send(error[0]);
        console.error(error[0]);
    }
})();
});

app.delete('/ingredients/delete/:id',(req,res)=>{
    (async () => {
    try{
        const cocktailDeleteResult = await (()=>{
            return new Promise((resolve,reject)=>{
                let query = 'DELETE FROM ingredients WHERE ingredient_id=?';
            database.query(query,[req.params.id],(err,result)=>{
                if(err){
                    reject(["Database problem - ingredient delete "+err,"500"]);
                    return;
                }
                if(result.affectedRows===0){
                    reject(["Cocktail Not Found - app.Delete","404"]);
                    return;
                }
                resolve(["Ingredient successfuly deleted","200"]);
            })
            })
        })();

        res.status(200).send("Delete process completed");
        console.log("Delete Process completed")
    }catch(error){
        res.status(parseInt(error[1])).send(error[0]);
        console.error(error[0]);
    }
})();
});

app.delete('/ingredients/deleteAll',(req,res)=>{
    (async () => {
    try{
        const cocktailDeleteResult = await (()=>{
            return new Promise((resolve,reject)=>{
                let query = 'Delete From ingredients';
            database.query(query,(err,result)=>{
                if(err){
                    reject(["Database problem - ingredient deleteALL "+err,"500"]);
                    return;
                }
                if(result.affectedRows===0){
                    reject(["No ingredients to delete Not Found - app.Delete"+query,"404"]);
                    return;
                }
                resolve(["Ingredient successfuly deleted","200"]);
            })
            })
        })();

        await (()=>{
            return new Promise((resolve,reject)=>{
                database.query('ALTER TABLE ingredients AUTO_INCREMENT = 1', (err,result)=>{
                    if (err) {
                        console.error("Error resetting auto-increment on ingredients:", err);
                        reject(["Database problem - resetting auto-increment " + err, "500"]);
                        return;
                    }
                    console.log("Auto-increment reset for ingredients");
                    resolve();
                });
            });
        })();

        res.status(200).send("Delete ALL process completed");
        console.log("Delete ALL Process completed")
    }catch(error){
        res.status(parseInt(error[1])).send(error[0]);
        console.error(error[0]);
    }
})();
});

app.delete('/cocktails/deleteAll',(req,res)=>{
    
    (async () => {
    try{
        const cocktail_ingredientDeleteResult = await (()=>{
            return new Promise((resolve,reject)=>{
                let query = 'DELETE FROM cocktails_ingredients';
                database.query(query,[],(err,result)=>{
                    if(err){
                        reject(["Database problem - cocktail_ingredients deleteALL "+err,"500"]);
                        return;
                    }
                    // if(result.length===0){
                    //     reject(["Cocktails_ingredients Not Found - app.Delete","404"]);
                    //     return;
                    // }
                    // resolve(["Cocktails_ingredients successfuly deleted","200"]);
                    console.log('supporting table ALL deleted');
                    resolve();
                }) 
            })
        })();
        const cocktailDeleteResult = await (()=>{
            return new Promise((resolve,reject)=>{
            database.query('DELETE FROM cocktails',[],(err,result)=>{
                if(err){
                    reject(["Database problem - cocktail delete "+err,"500"]);
                    return;
                }
                // if(result.length===0){
                //     reject(["Cocktails Not Found - app.DeleteALL","404"]);
                //     return;
                // }
                // resolve(["Cocktails successfuly deleted","200"]);
                console.log("all cocktails deleted");
                resolve();
            })
            })
        })();
        await (()=>{
            return new Promise((resolve,reject)=>{
                database.query('ALTER TABLE cocktails AUTO_INCREMENT = 1', (err,result)=>{
                    if (err) {
                        console.error("Error resetting auto-increment on cocktails:", err);
                        reject(["Database problem - resetting auto-increment " + err, "500"]);
                        return;
                    }
                    console.log("Auto-increment reset for cocktails");
                    resolve();
                });
            });
        })();
        res.status(200).send("DeleteALL process completed");
        console.log("DeleteALL Process completed")
    }catch(error){
        res.status(parseInt(error[1])).send(error[0]);
        console.error(error[0]);
    }
})();
});

app.delete('/ingredients/deletename/:name',(req,res)=>{
    (async () => {
    try{
        const cocktailDeleteResult = await (()=>{
            return new Promise((resolve,reject)=>{
                let query = 'DELETE FROM ingredients WHERE name=?';
            database.query(query,[req.params.name],(err,result)=>{
                if(err){
                    reject(["Database problem - ingredient delete "+err,"500"]);
                    return;
                }
                if(result.length===0){
                    reject(["Ingredient Not Found - app.Delete","404"]);
                    return;
                }
                resolve(["Ingredient successfuly deleted","200"]);
            })
            })
        })();

        res.status(200).send("Delete process completed");
        console.log("Delete Process completed")
    }catch(error){
        res.status(parseInt(error[1])).send(error[0]);
        console.error(error[0]);
    }
})();
});

app.delete('/cocktails/deletename/:name',(req,res)=>{
    
    (async () => {
    try{
        const cocktail_ingredientDeleteResult = await (()=>{
            return new Promise(async(resolve,reject)=>{
                const ID = await cocktailSearching(req.params.name);
                console.log(ID);
                let query = 'DELETE FROM cocktails_ingredients WHERE cocktail_id=?';
                database.query(query,[ID],(err,result)=>{
                    if(err){
                        reject(["Database problem - cocktail_ingredients delete "+err+query+ID,"500"]);
                        return;
                    }
                    if(result.affectedRows===0){
                        reject(["Cocktails_ingredients Not Found - app.Delete","404"]);
                        return;
                    }
                    resolve(["Cocktails_ingredients successfuly deleted","200"]);
                }) 
            })
        })();
        const cocktailDeleteResult = await (()=>{
            return new Promise((resolve,reject)=>{
                let query = 'DELETE FROM cocktails WHERE name=?';
            database.query(query,[req.params.name],(err,result)=>{
                if(err){
                    reject(["Database problem - cocktail delete "+err,"500"]);
                    return;
                }
                if(result.affectedRows===0){
                    reject(["Cocktail Not Found - app.Delete","404"]);
                    return;
                }
                resolve(["Cocktail successfuly deleted","200"]);
            })
            })
        })();

        res.status(200).send("Delete process completed");
        console.log("Delete Process completed")
    }catch(error){
        res.status(parseInt(error[1])).send(error[0]);
        console.error(error[0]);
    }
})();
});