/** Class for the GameBoard, this class when initiated creates and draws Wall, Pellet,
 *  Power Pellet, Pacman, and Ghost objects on the p5.js canvas.  */
class GameBoard {
  /**
   * Initialises all the GameBoard attributes.
   */
  constructor() {
    this.walls = [];
    this.hex_walls = [];
    this.pellets = [];
    this.power_pellets = [];
    this.cell_w = Math.ceil(width / 27); //30
    this.cell_h = Math.ceil(height / 27); //30
    this.row_total = Math.floor(height / this.cell_h);
    this.column_total = Math.floor(width / this.cell_w);
    this.board = [];
    this.board_num = 0;



    /**
    * @property {Array.<String>} this.board_layout -
    * Board layout for the GameBoard, listed below are the keys and their meaning:
    * <pre>
    *   W : wall.
    *   G : ghost starting position.
    *   0 : powerPellet.
    *   P : pacman starting position.
    *   H : hall to transfer to the other side of maze.
    *   WhiteSpace : path.
    * </pre>
    */

    this.board_layouts =
      [
        [
          "WWWWWWWWWWWWWWWWWWWWWWWWWWW",
          "W0    W      0      W    0W",
          "W WWW W WWW WWW WWW W WWW W",
          "W W G        W        G W W",
          "W W WWW WWWW W WWWW WWW W W",
          "W W          W          W W",
          "W WW WW WWWW W WWWW WW WW W",
          "W            W            W",
          "W WW WWWW W WWW W WWWW WW W",
          "W  W    W W W0  W W    W  W",
          "WW    W W W W WWW W W    WW",
          "WWWW  W W         W W  WWWW",
          "WWWW WW W WWW W W W WW WWWW",
          "HHHH      W     W      HHHH",
          "WWWW WW W W W WWW W WW WWWW",
          "WWWW  W W P       W W  WWWW",
          "WW    W W W WWW W W W    WW",
          "W   W   W         W   W   W",
          "W WWWWW W WWWWWWW W WWWWW W",
          "W0                       0W",
          "W W W W W W W WW WW W   W W",
          "W W   W W W W W   W W W W W",
          "W W W W W W W W W W   W W W",
          "W W W W   G W   W W WWWGW W",
          "W W W W W W WWW W W W   W W",
          "W         W  0        W   W",
          "WWWWWWWWWWWWWWWWWWWWWWWWWWW",
        ],
        [
          "WWWWWWWWWWWWWWWWWWWWWWWWWWW",
          "W0          WWW          0W",
          "W W WWWWWWW WWW WWWWWWW W W",
          "W W G        0        G W W",
          "W W WWW WWWW W WWWW WWW W W",
          "W W          W          W W",
          "W WWWWW WWWW W WWWW WWWWW W",
          "W       W    W    W       W",
          "W WWWWW W WWWWW W W WWWWW W",
          "W       W W  0  W W       W",
          "WWWWW W W W WWWWW W W WWWWW",
          "WWWWW W W         W W WWWWW",
          "WWWWW W W WWWWWWW W W WWWWW",
          "HHHHH                 HHHHH",
          "WWWWW W W WWWWWWW W W WWWWW",
          "WWWWW W W  P   P  W W WWWWW",
          "WWWWW W W WWWWWWW W W WWWWW",
          "W       W    W    W       W",
          "W WWWWW W WW W WW W WWWWW W",
          "W0                       0W",
          "W WWWWW W W W WWWWW W W W W",
          "W W   W W W W W   W W W W W",
          "W W W W W W W W W W W W W W",
          "W W W W W G W W W W W G W W",
          "W W W W WWWWW W W W WWWWW W",
          "W            0            W",
          "WWWWWWWWWWWWWWWWWWWWWWWWWWW"
        ],
        [
          "WWWWWWWWWWWWWWWWWWWWWWWWWWW",
          "W0WW G WW  W  WW    WW    W",
          "W  W W WW  W   W  W WW WW W",
          "W  W W W       W  W    G  W",
          "W  W   W W  W    W  WWWWWWW",
          "W  WWW   W  WWW  W 0     WW",
          "W W  WW  WW   WWW  WW  W  W",
          "W    WWW  W       WWW WW  W",
          "W WW WWWW   WWWWW     WW  W",
          "W WW WWWWWWWWWWWWWWWWWWWW W",
          "W     W   W   W   W  WW   W",
          "WWW W W W W W W W W  W  WWW",
          "WWW W W W W W W W W  W  WWW",
          "HHH W W W 0 W W W  W    HHH",
          "WWW     WWWWW   W  W  W WWW",
          "WWW WWWW    W W  WWWW W WWW",
          "W    W    W W WW         WW",
          "W  W W WW   W   WWW WW    W",
          "WW W W WW W W W WWW  WW W W",
          "WW          P             W",
          "WW   W W W  WWWWW W WWWW  W",
          "W  WWW W W  WW  W W W  W  W",
          "W        W  WW  W W W  W  W",
          "W WW  WW W  WW  W W W  W  W",
          "W  WWW   W  WWW       WW  W",
          "WW0  G   WW     W  W   G WW",
          "WWWWWWWWWWWWWWWWWWWWWWWWWWW",
        ],
      ];
    console.log(this.board_layouts[0].length, this.board_layouts[0][1].length);
  }

  /**
   * Takes this.board_layout and builds the board,
   * placing all the object types and their positions
   */
  buildBoard() {
    this.walls = [];
    this.hex_walls = [];
    this.pellets = [];
    this.power_pellets = [];
    if (this.board_num == this.board_layouts.length)
      this.board_num = 0;
    this.board = this.board_layouts[this.board_num];
    ghosts = [];
    let wall_color = this.getRandomColor();
    let wall_color2 = wall_color;

    for (let y = 0; y < this.board.length; y++) {
      for (let x = 0; x < this.board[y].length; x++) {
        let cell = this.board[y].slice(x, x + 1);

        // Pathway, place pellets
        if (cell == " ") {
          this.pellets.push(
            new Pellet(
              x * this.cell_w,
              y * this.cell_h,
              this.cell_w,
              this.cell_h
            )
          );

          // Place Power Pellets
        } else if (cell == "0") {
          this.power_pellets.push(
            new PowerPellet(
              x * this.cell_w,
              y * this.cell_h,
              this.cell_w,
              this.cell_h
            )
          );

          // Place wall
        } else if (cell == "W") {
          if (hud.level_num > 3)
              wall_color = "#000000";
          this.hex_walls.push(
            new HexagonWall(
              x * this.cell_w,
              y * this.cell_h,
              this.cell_w,
              this.cell_h,
              wall_color2
            )
          );
          this.walls.push(
            new SquareWall(
              x * this.cell_w,
              y * this.cell_h,
              this.cell_w,
              this.cell_h,
              wall_color
            )
          );
          // Pacman starting position
        } else if (cell == "P") {
          pacman =
            new Pacman(
              x * this.cell_w + this.cell_w / 2,
              y * this.cell_h + this.cell_h / 2,
              this.cell_w
            );

          // Ghost starting position
        } else if (cell == "G") {
          ghosts.push(
            new Ghost(
              ghosts.length,
              x * this.cell_w + this.cell_w / 2,
              y * this.cell_h + this.cell_h / 2,
            )
          );
        }
      }
    }
  }

  /** Constantly looped in the p5js draw() function to draw the current state of the board */
  drawBoard() {

    // Draw pellets
    let pellets_remaining = false;
    for (var i = 0; i < this.pellets.length; i++) {
      this.pellets[i].show();
      if (!this.pellets[i].empty)
        pellets_remaining = true;
    }

    // Draw powerPellets
    let power_pellets_remaining = false;
    for (let i = 0; i < this.power_pellets.length; i++) {
      this.power_pellets[i].show();
      if (!this.power_pellets[i].empty)
        power_pellets_remaining = true
    }

    // Draw walls
    for (let i = 0; i < this.walls.length; i++) {
      if (hud.level_num > 3) {
        this.walls[i].show();
        this.hex_walls[i].show();
      } else {
        this.walls[i].show();
      }

    }

    if (!power_pellets_remaining && !pellets_remaining) {
      // Level completed
      this.board_num++;
      hud.incrementLevelNum();
      this.buildBoard();
    }

  }

  generateRandomMap() {
    let new_board = [];
    backdrop = " W"
    for (let i = 0; i < 27; i++) {
      for (let j = 0; j < 27; j++) {
        if (i == 13 && (j < 4 || j > 4)) {
          new_board[i][j] = 'H' // Hall
        }
        else {
          new_board[i][j] = backdrop[Math.floor(Math.random() * 2)]; // random either Wall or Pellet
        }
      }
    }
  }

  getRandomColor() {
    let letters = '0123456789ABCDEF';
    let color = '#';
    for (let i = 0; i < 6; i++) {
      color += letters[Math.floor(Math.random() * 16)];
    }
    return color;
  }
}
