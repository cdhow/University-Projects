// Canvas settings
const CANVAS_HEIGHT = 810;
const CANVAS_WIDTH = 810;
let CUSTOM_FONT;

// Ghost settings
let GHOST_SCARED_TIME = 2000;
let GHOST_SPEED = 2;

// Game Objects
let pacman;
let ghosts = [];
let gameBoard;
let hud;
let main_menu;
let scoreboard;
let ghost_sprites;
let hexagon;

// Game states
let gameover = true;
let display_login = true;
let display_scoreboard = false;

// Username and highscore (retrieved from database on login)
let highscore = 0;
let username = "";

/**
 * P5.js function - called before the p5.js setup() function to load the ghost sprite image.
 */
function preload() {
  ghost_sprites = loadImage("img/ghost_sprite.png");
  hexagon = loadImage("img/Hexagon.png");
  CUSTOM_FONT = loadFont('fonts/Winear.otf');
}

/**
 * P5.js function - initialises the canvas, GameBoard, Menu and HUD. 
 */
function setup() {
  textFont(CUSTOM_FONT);

  angleMode(DEGREES);
  rectMode(CENTER);
  imageMode(CENTER);

  createCanvas(CANVAS_WIDTH, CANVAS_HEIGHT);
  background(0);

  main_menu = new MainMenu();
  login_menu = new LoginMenu();
  hud = new HUD();
  scoreboard = new Scoreboard();
  gameBoard = new GameBoard();
  gameBoard.buildBoard();

}

/**
 * P5.js function - Continuous game loop, two states defined by the gameover boolean.
 * If gameover is true, draw the menu, else draw the gameBoard and update the game objects as the user plays the game.
 */
function draw() {
  // Pause loop until user has logged in
  if (display_login) {
    login_menu.show();
    noLoop();
    return;
  }
  background(0);

  // Game is not over, run game operations
  if (!gameover) 
  {

    // Draw the game board objects
    gameBoard.drawBoard();

    // Pacman
    if (!pacman.is_dying) {
      pacman.collides(gameBoard.pellets);
      pacman.collides(gameBoard.power_pellets);
      pacman.collides(ghosts);
      pacman.move();
    }
    pacman.show();

    // Ghosts
    for (i = 0; i < ghosts.length; i++) {
      if (pacman.is_moving)
        ghosts[i].move();
      ghosts[i].show();
    }

    // If lives equal 0, then change to the gameover state for,
    // the next iteration of draw()
    if (hud.lives === 0)
      gameover = true;
  } 
  else {
    // Game over, show the main menu
    main_menu.show();
  }

  // Always show the HUD
  hud.show();

  if (display_scoreboard)
    scoreboard.show();
}

/** P5.js function - Constantly listens for and handles key presses. If the user presses the ENTER key 
 * when the gameover equals true (when the menu is displayed), then start the game.
 * If the game is started listen for the arrow keys and update pacmans direction accordingly.
 */
function keyPressed(e) {
  if (display_login)
    return;

  e.preventDefault();


  // Press "s" to show the scoreboard
  if (keyCode == 83)
    display_scoreboard = !display_scoreboard;

  if (!gameover) {
    pacman.handleDirChange(keyCode) // handle direction changes

    if (keyCode == ESCAPE) {
      // Escape exits game (gameover screen)
      gameover = true;
    }

  } else if ((keyCode === ENTER)) {
      // If it is game over and the user presses ENTER,
    // Then start new game
    setup();
    gameover = false;
    // Else continue current game
  }
}
