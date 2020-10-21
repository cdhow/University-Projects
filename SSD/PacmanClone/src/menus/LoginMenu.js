// Input fields for username
let input_elem;
let btn_elem;

/** Class for the Login Menu, gets the user's username from input before the 
 * game begins and loads their highscore from the database
 */
class LoginMenu {
    /**
   * Initialises all the Menu attributes.
   */
    constructor() {}

    show() {
        background("#ccff00");

        fill("#382f2b");
        textSize(64);
        text("PACMAN", width / 2 - 150, height / 2 - 120);

        fill("#382f2b");
        textSize(32);
        text("Please enter your username..", width / 2 - 215, height / 2 - 80);
        
        input_elem = createInput();
        input_elem.position(width / 2 - 135, height / 2 - 50);

        btn_elem = createButton('PLAY!');
        btn_elem.position(input_elem.x + input_elem.width, input_elem.y);
        btn_elem.mousePressed(this.handleUser);


        textSize(20);
        text("Pacman remake ~ Caleb Howard", width / 2 - 160, height - 10);
    }

    /**
     * Queries the database for the user's highscore, if the user is new, add them to
     * the database with highscore = 0
     * https://github.com/louischatriot/nedb/wiki
     * https://www.youtube.com/watch?v=q-lUgFxwjEM&list=PLRqwX-V7Uu6YxDKpFzf_2D84p0cyk4T7X&index=13
     */
    handleUser() {
        // Get username from input field
       const user = input_elem.value();  
        
        getUserFromDB(user);

        // Remove input fields
        input_elem.remove();
        btn_elem.remove();
        // Continue program loop
        display_login = false;
        loop();
    }
}

async function getUserFromDB(user) {
    // Send the username to the server
    // and read the user object response to
    // load current user
    const data = {username:user};
    const options = {
        method:"POST",
        headers:{
            "Content-Type": "application/json"
        },
        body: JSON.stringify(data)
    }
    const res = await fetch("/user", options);
    const user_res = await res.json();
    // Update user global variables
    username = user_res.username;
    highscore = Number(user_res.highscore);
}

// const response = await fetch("/api");
// const data = await response.json();
// console.log(data);