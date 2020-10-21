/** Class for the game Menu */
class MainMenu {
    /**
   * Initialises all the Menu attributes.
   */
    constructor() {
        // Game settings inputs TODO: ADD MORE
        this.ghost_scared_dur_input;
        this.ghost_speed_input;

        // Apply changes button
        this.button;
    }


    /** Displays the Menu. */
    show() {
        background("#414142");
        fill("#fc2050");
        textSize(64);
        text("GAME OVER!", width / 2 - 200, height / 2 - 240);

        fill("#22f7b0");
        textSize(32);
        text("PRESS ENTER TO START GAME!", width / 2 - 240, height / 2 - 150);
        textSize(20);
        text("PRESS S TO OPEN SCOREBOARD", width / 2 - 160, height / 2 - 100);
        text("Pacman remake ~ Caleb Howard", width / 2 - 140, height - 10);
    }

    /** Draws input text boxes for the user to update the game parameters, and an "Apply" button for
     * finalising the updates.
     */
    gameSettingsMenu() {
        textSize(15);
        // text("Settings", width / 2 - 30, height / 2 - 75);

        // Ghost scared time
        this.ghost_scared_dur_input = createInput(GHOST_SCARED_TIME);
        this.ghost_scared_dur_input.position(width / 2 - 70, height / 2 - 50);
        text("Ghost Scared Duration", width / 2 - 67, height / 2 - 65)

        // Ghost Speed
        this.ghost_speed_input = createInput(GHOST_SPEED);
        this.ghost_speed_input.position(width / 2 - 70, height / 2 + 10);
        text("Ghost Speed", width / 2 - 37, height / 2 - 5)


        // Handle apply button
        this.button = createButton('Apply');
        this.button.position(width / 2 - 7, this.ghost_speed_input.x + 120);
        this.button.mousePressed(this.updateConfig);
    }


    /**
     * Called when the apply button is pressed, once pressed this method will update all the game settings.
     */
    updateConfig() {
        GHOST_SCARED_TIME = this.ghost_scared_dur_input.value();
        GHOST_SPEED = this.ghost_speed_input.value();
    }
}