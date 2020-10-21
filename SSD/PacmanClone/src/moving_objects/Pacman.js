/** Class for Pacman object. */
class Pacman {
    /**
     * Creates a Pacman object.
    * @param {number} x - x coordinate of pacman.
    * @param {number} y - y coordinate of pacman.
    * @param {number} d - y coordinate of diameter of pacman.
     */
    constructor(x, y, d) {
        // Position and diameter
        this.starting_x = x;
        this.starting_y = y;
        this.x = x;
        this.y = y;
        this.d = d;

        // Features: colour, radius, etc
        this.colour = "yellow";
        this.r = this.d / 2;
        this.mouth_bottom = 9
        this.mouth_top = -9;

        // Direction. position, speed and state
        this.dir = [0, -1];
        this.next_dir = [0, -1];
        this.speed = 3;
        this.mouth_speed = 5
        this.facing_angle = 90;
        this.next_facing_angle = 90;
        this.mouth_dir = 1; //opening or closing (-1)
        this.move_count = 30 / this.speed;
        this.current_cell = this.getCurrentCell();
        this.is_path_ahead_valid = false;
        this.pellets_eaten = 0;
        this.is_dying = false;
        this.is_moving = false;

    }

    /** Returns the current cell where pacman is located. */
    getCurrentCell() {
        return [Math.floor(this.x / 30), Math.floor(this.y / 30)]
    }

    /** Mouth opening and closing animation for pacman. */
    animateMouth() {
        this.mouth_bottom += (this.mouth_speed * this.mouth_dir);
        this.mouth_top -= (this.mouth_speed * this.mouth_dir);

        // Open and close mouth within pixel range (5-50)
        if (this.mouth_bottom > 50 || this.mouth_bottom < 5)
            this.mouth_dir *= -1;

    }


    /* Displays pacman. */
    show() {
        push();
        if (this.is_dying) {
            this.facing_angle += 10;
            this.d > 0 ? this.d -= .5 : this.d = 0;
        }
        translate(this.x, this.y);
        fill(this.colour);
        rotate(this.facing_angle);
        arc(0, 0, this.d, this.d, this.mouth_bottom, this.mouth_top, PIE);
        pop();
    }


    /** Handles the movement of pacman through the gameboard. */
    move() {

        // If pacman is in the middle of a cell block, check if
        // user has specified a valid direction change and update
        // direction if valid
        if ((this.x - 15) % 30 === 0 && (this.y - 15) % 30 === 0) {
            // If next direction is valid
            if (this.isDirValid(this.next_dir))
                this.updateDir();

            // Check the path ahead is valid
            this.is_path_ahead_valid = this.isDirValid(this.dir);
        }

        if (this.is_path_ahead_valid) {
            this.x += this.dir[0] * this.speed;
            this.y += this.dir[1] * this.speed;
            this.animateMouth();

            // Boundaries
            if (this.x > width - this.r)
                this.x = this.d;
            if (this.x < this.r)
                this.x = width - this.d;
        }
    }

    /**
     * Takes a direction vector and returns true if the direction
     * does not contain a wall.
     * @param {Array} dir - direction vector
     * @return {boolean} next_direction != "W"
     */
    isDirValid(dir) {

        let cc = this.current_cell = this.getCurrentCell();

        // If wanting to change directions, look that way

        let next_dir_cell = [cc[0] + dir[0], cc[1] + dir[1]];

        // If the turn is valid then turn
        let next_dir_cell_content = gameBoard.board[next_dir_cell[1]].slice(next_dir_cell[0], next_dir_cell[0] + 1); // map symbol

        // "W" is a wall (View the GameBoard class to see the game layout)
        // If the next direction is not a wall then the path is valid
        return next_dir_cell_content != "W";
    }


    /** Updates pacman's current direction vector. */
    updateDir() {
        this.dir = this.next_dir;
        this.facing_angle = this.next_facing_angle;
    }


    /** Sets the next direction vector that pacman will change to on the next p5js draw() loop. */
    updateNextDir(dir, next_facing_angle) {
        this.next_dir = dir;
        this.next_facing_angle = next_facing_angle;

        // Turn immediately if the turn is 180 degrees
        if (Math.max(this.facing_angle, this.next_facing_angle) - Math.min(this.facing_angle, this.next_facing_angle) === 180)
            this.updateDir();
    }

    /** Takes a KeyCode as a paramter and updates pacmans direction accordingly */
    handleDirChange(key_code) {
        pacman.is_moving = true;

        switch (key_code) {
            case 38: // up
                pacman.updateNextDir([0, -1], 270);
                break;
            case 39: // right
                pacman.updateNextDir([1, 0], 0);
                break;
            case 40: // down
                pacman.updateNextDir([0, 1], 90);
                break;
            case 37: // left
                pacman.updateNextDir([-1, 0], 180);
                break;
        }
    }

    /** Handles all pacman's collision types.*/
    collides(arr) {
        // Loop through the solid types and check if pacman
        // has collide with any
        for (let i = arr.length - 1; i > -1; i--) {

            // Distance from pacman;s centre to objects centre
            let d = dist(this.x, this.y, arr[i].x, arr[i].y);

            // If the distance is less than pacman's and the object's 
            // radii combined then they have collided
            if (d < this.r + arr[i].r) {

                // If pacman hits a pellet
                if (arr[i].type === "pellet" && !arr[i].empty) {
                    arr[i].empty = true;
                    this.pellets_eaten++;
                    hud.addToScore(1);
                    // Extra life added
                    if (this.pellets_eaten % 400 == 0)
                        hud.updateLives(1);
                }

                // If pacman hits a power pellet
                if (arr[i].type === "powerPellet" && !arr[i].empty) {
                    arr[i].empty = true;
                    hud.addToScore(50);

                    // Trigger the ghost scared functionality
                    ghosts.forEach((ghost) => {
                        ghost.getScared();
                    });

                }
                // If pacman hits a ghost
                if (arr[i].type === "ghost" && !arr[i].is_dying && !this.is_dying) {

                    // If the ghost is in a scared state, then kill the ghost
                    if (arr[i].is_scared) {
                        arr[i].kill(); // kill ghost
                        hud.addToScore(100);


                    } else {
                        this.kill(); // kill pacman
                    }


                }
            }

        }
    }

    /** Kills pacman (called in the collides() method if pacman collides with a ghost that is not in a scared state). */
    kill() {
        hud.updateLives(-1);
        this.is_dying = true;
        this.is_moving = false;

        let pacman = this;
        setTimeout(function () {
            pacman.restart();

            ghosts.forEach((ghost) => {
                ghost.restart();
            });

        }, 2000);
    }

    /** Restarts all pacman's attribute such as position, direction vector, etc (called in the kill() method). */
    restart() {
        // If pacman still has lives restart his position, etc.
        if (hud.lives > 0) {
            this.d = 30;
            this.x = this.starting_x;
            this.y = this.starting_y;
            this.dir = [0, -1];
            this.next_dir = [0, -1];
            this.facing_angle = 90;
            this.next_facing_angle = 90;
            this.is_dying = false;
        }
    }
}