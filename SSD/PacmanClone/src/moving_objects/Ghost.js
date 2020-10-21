/** Class for a Ghost object. */
class Ghost {
  /**
   * Creates a Ghost.
   * @param {number} ghost_idx - the ghost sprite sheet contains 4 different ghosts in a column,
   *  this is the index for a single sprite in that sheet.
   * @param {number} x - x coordinate of ghost.
   * @param {number} y - y coordinate of ghost.
   */
  constructor(ghost_idx, x, y) {
    this.type = "ghost";
    // Position and Diameter
    this.starting_x = x;
    this.starting_y = y;
    this.x = x;
    this.y = y;
    this.d = 32;
    this.r = this.d/2;
    // Specific which ghost on the ghost_sprite.png this ghost is
    // (counting vertically)
    this.ghost_idx = ghost_idx;

    // This ghost's corner cell that it will run to when pacman eats
    // a power pellet, calculated using the ghost's index.
    this.corner = () => {
      return [
        [30, 30],
        [30, height],
        [width, 30],
        [width, height],
      ][ghost_idx];
    };

    // Speed and direction
    this.speed = GHOST_SPEED;
    this.previous_speed = this.speed;
    this.move_count = 30 / this.speed;
    this.dir = [0, 0];
    this.next_dir = [0, 0];

    // State
    this.is_scared = false;
    this.scared_time = GHOST_SCARED_TIME;
    // This attribute holds the counter of how many power pellets
    // pacman has eaten that are still currently active,
    // it is incremented and decremented in the getScared() method
    this.active_power_pellets = 0;

    // Position
    this.current_cell = [Math.floor(x / 30), Math.floor(y / 30)];
    this.previous_cell = this.current_cell.slice();

    // Sprite source horizontal position
    // Currently the horizontal source position is statically set to the 3rd image (2 index)
    // Once I update the animation I will incorporate all the sprite states
    this.horiz_img_idx = 2;
  }

  /** Displays the ghost at the current this.x and this.y. */
  show() {
    push();

    // Change vertical source image index to the blue scared ghost if it is currently scared
    // Otherwise it will be the ghost index
    let vert_img_idx = this.is_scared ? 4 : this.ghost_idx;

    /* This Translate to:
     - Place the image on the canvas at x,y with dimension d by d.
     - Grab the source image from  x-axis position 16 * horizontal image index and
        y-axis position 16 * vertical image index. (units in pixel).
     - Grab the and slice of the source image that is 16 by 16 in size.
     Note: The sprite image consists of 16 by 16 images. Therefore, and example
        for grabbing the second ghost would be at x-axis pos 16*1 and y-axis pos 16*2.
    */
    image(
    ghost_sprites,
      this.x, // canvas x pos
      this.y, // canvas y pos
      this.d, // image width
      this.d, // image height
      16 * this.horiz_img_idx, // x source image pos
      16 * vert_img_idx, // y source image pos
      16, // source image slice width size
      16 // source image slice height size
    );
    pop();
  }

  /** Handles the movement of the ghost through the gameBoard. */
  move() {
    if (!this.is_dying) {
      if (this.move_count > 0) {
        this.x += this.dir[0] * this.speed; // moving in x-direction
        this.y += this.dir[1] * this.speed; // moving in y-direction

        this.move_count--;
      } else {
        this.previous_cell = this.current_cell.slice();
        this.current_cell = [
          this.current_cell[0] + this.dir[0],
          this.current_cell[1] + this.dir[1],
        ];
        this.dir = this.chooseNextDirection();
        this.move_count = 30 / this.speed;
      }
    }
  }

  /**
   * Checks all cells surrounding the ghost (up,down,left,right),
   * confirms which directions can be taken, and returns a random cell of those possible cells.
   */
  chooseNextDirection() {
    let cc = this.current_cell;
    // All the surround cells (valid and invalid)
    let next_cell_options = [
      [cc[0], cc[1] - 1], // up
      [cc[0] + 1, cc[1]], // right
      [cc[0], cc[1] + 1], // down
      [cc[0] - 1, cc[1]], // left
    ];

    // Get the possible (valid) cells
    // The filter method returns the elements of the next_cell_options array
    // based on the condition below
    let possible_cells = next_cell_options.filter((possible) => {
      let cell = gameBoard.board[possible[1]].slice(
        possible[0],
        possible[0] + 1
      ); //map symbol

      // Filter out the cells that are Walls or Halls
      return (
        cell != "W" &&
        cell != "H" &&
        this.previous_cell.join(",") != possible.join(",")
      );
    }, this);

    // Choose a random cell of the possible_cells

    // If there is only one option
    if (possible_cells.length === 1) {
      return possible_cells[0].map(function (cell_index, i) {
        return cell_index - this.current_cell[i];
      }, this);
    }

    // The ghosts will move in a random direction on the first level
    // at each new level another ghost will begin to follow the target
    // instead of moving randomly

    if (this.ghost_idx == 0) {
      if (hud.level_num < 2)
        return this.chooseRandomCell(possible_cells);
      else
        return this.followPacman(possible_cells);
    }

    if (this.ghost_idx == 1) {
      if (hud.level_num < 3)
        return this.chooseRandomCell(possible_cells);
      else
        return this.followPacman(possible_cells);
    }

    if (this.ghost_idx == 2) {
      if (hud.level_num < 4)
        return this.chooseRandomCell(possible_cells);
      else
        return this.followPacman(possible_cells);
    }

    if (this.ghost_idx == 3) {
      if (hud.level_num < 5)
        return this.chooseRandomCell(possible_cells);
      else
        return this.followPacman(possible_cells);
    }
  }


  /**
   * Given an array of cells it will return a random single cell.
   * @param {cell[]} cells
   * @return {cell} random(cells).
   */
  chooseRandomCell(cells) {
    return random(cells).map((cell_idx, i) => {
      return cell_idx - this.current_cell[i];
    }, this);
  }

    /**
   * Given an array of cells it will return a cell that is closest to pacman.
   * @param {cell[]} cells
   * @return {cell} 
   */
  followPacman(possible_cells) {

    let pos = this.is_scared ? this.corner : [pacman.x, pacman.y];

    // Distances
    let distances_from_target = possible_cells.map(function (possible, i) {
        return dist(possible[0] * 30, possible[1] * 30, pos[0], pos[1]);
    }, this);

    let shortest_choice_indx = distances_from_target.indexOf(Math.min.apply(null, distances_from_target));

    return possible_cells[shortest_choice_indx].map(function (cell_index, i) {

        return cell_index - this.current_cell[i];

    }, this);
}


  /** Handles the ghost's scared animation and functionality. */
  getScared() {
    // Set is scared to true (the show method will now display the ghost as scared)
    this.is_scared = true;
    // Every time this method is called pacman eats a power pellet, there
    // this counter is the number of power pellets that pacman has eaten
    // that are still active
    this.active_power_pellets++;

    let ghost = this;
    // For a scared_time duration, perform the scared functionality
    setTimeout(() => {
      ghost.active_power_pellets--; // Power pellet is now being used

      // If the pacman does not eat another power pellet
      // i.e if the active power pellets == 0, set is_scared to false
      if (ghost.active_power_pellets === 0) {
        setTimeout(() => {
          ghost.is_scared = false;
        }, 1000);
      }
    }, ghost.scared_time);
  }


  /** Kills the ghost and triggers the restart method to default all the attributes. */
  kill() {
    this.is_dying = true;

    let ghost = this;
    setTimeout(() => {
      ghost.restart();
    }, 500); // 1 second death period
  }

  /** Restarts all the attributes (called in the kill() method). */
  restart() {
    this.d = 33;
    this.x = this.starting_x;
    this.y = this.starting_y;
    this.current_cell = [Math.floor(this.x / 30), Math.floor(this.y / 30)];
    this.dir = [0, 0];
    this.next_dir = [0, 0];
    this.move_count = 30 / this.speed;
    this.is_dying = false;
    this.is_scared = false;
  }
}
