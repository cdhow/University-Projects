class Scoreboard {

    constructor() { 
        this.users = [];
        getUsersFromDB();
    }


    /** Displays the Menu. */
    show() {
        push();
        background("#fcf56f");
        fill("#727573");
        textSize(30);
        let x_pos = 20;
        for (let y_pos=0; y_pos<this.users.length; y_pos++) {
            let name = this.users[y_pos].username;
            let hs = this.users[y_pos].highscore;
            text(name + " : " + hs, x_pos, 30 * (y_pos+1));
            // Return print to top and across
            if (y_pos > CANVAS_HEIGHT - 20) {
                x_pos += 20;
                y_pos = 0;
            }
        }
        pop();
    }
}

/** Gets all user information from the server **/
async function getUsersFromDB() {
    const res = await fetch("/scoreboard");
    const data = await res.json();
    scoreboard.users = data;
}