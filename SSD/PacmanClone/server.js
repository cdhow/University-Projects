const express = require("express");
const Datastore = require("nedb");

const app = express();
app.listen(3000, () => console.log("listening at 3000.."));
app.use(express.static("src"));
app.use(express.json());

const db = new Datastore("highscores.db");
db.loadDatabase();

// Used to send all the user data to the client (used for scoreboard).
app.get("/scoreboard", (req, res) => {
    // Get all the data from the database
    db.find({}, (err, data) => {
        res.json(data);
    });
});

// Used at login to get the user details
// or to add a new user to the database
app.post("/user", (req, res) => {
    // Find the user in the database
    let data = req.body;
    db.find(data, (err, docs) => {
        if (docs.length == 0) {
            // new user
            data.highscore = "0";
            db.insert(data);
            res.json(data);
        } else {
            // User exists
            res.json(docs[0]);
        }
    });
});

// Used to update the user
app.post("/update", (req, res) => {
    // Get the {username, highscore}
    const data = req.body;

    // Update the user
    const query = {username: data.username};
    db.update(query, data, {}, (err, numReplaced) => {
        if (numReplaced == 1) {
            res.json({
                state:"success"
            });
        } else {
            res.json({
                state:"failed",
                err
            });
        }
    });
});
