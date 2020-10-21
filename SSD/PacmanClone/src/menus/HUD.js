/** Class for the HUD object. */
class HUD {
    /**
   * Initialises all the HUD attributes.
   */
    constructor() {
        this.colour = "yellow";
        this.level_num = 1;
        this.score = 0;
        this.lives = 3;
        // This is used when in the gameover state,
        // it specified whether the database has been updated
        // with the new highscore
        this.is_db_updated = false;
    }


    /** Displays the HUD. Includes lives remaining, player score, and high score. */
    show() {
        
        textSize(20);
        fill(this.colour);
        text("lives: " + this.lives, 20, height -5);
        text("user: " + username, width - 200, height-5);
        text("score: " + this.score, 20, 20);
        text("highscore: " + highscore, width/2 - 55, 20);
        text("level: " + this.level_num, (width) - 100, 20);

        this.updateHighscore();

    }

    /** Updates level number */
    incrementLevelNum() {
        this.level_num++;
    }

    /** Updates Score */
    addToScore(num) {
        this.score += num;
    }

    updateLives(num) {
        this.lives += num;
    }

    // Updates the HUD highscore and if the game is over, updates
    // the database highscore for the current user
    updateHighscore() {
        if (this.score > 0 && this.score >= highscore) {
            highscore = this.score;
            
            // If it is gameover and the database
            // has yet to be updated, then update database.
            if (gameover && !this.is_db_updated) {
                this.is_db_updated = true;
                const hs = "" + highscore;
                const data = {username, highscore: hs}
                const options = {
                    method:"POST",
                    headers:{
                        "Content-Type": "application/json"
                    },
                    body: JSON.stringify(data)
                }
                fetch("/update", options);
            }
        }
            
    }
}