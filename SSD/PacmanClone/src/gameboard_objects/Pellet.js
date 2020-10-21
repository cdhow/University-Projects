/** Class for a Pellet object with a black square background (for the pathway),
 *  a pellet is a simple coloured circle. */
class Pellet {
    /**
     * Creates a Pellet.
     * @param {number} x - x coordinate of where to draw pellet.
     * @param {number} y - y coordinate of where to draw pellet.
     * @param {number} w - width of the black square that is positioned behind the pellet to give the illusion of a pathway.
     * @param {number} h - height of the black square that is positioned behind the pellet to give the illusion of a pathway.
     * @param {number} d_divisor - how much we want to divide the width by to get the pellet diameter.
     */
    constructor(x, y, w, h, d_divisor=5) {
      this.type = "pellet";
      this.empty = false;
      this.w = w;
      this.h = h;
      this.x = x + this.w / 2;
      this.y = y + this.h / 2;
      this.d = this.w / d_divisor;
      this.r = this.d / 2;
      this.colour = "#ffb8ae";
    }
  
    /** Displays the black square at (x,y) with dimensions (w,h) with a pellet of colour #ffb8ae in front with a diameter of d. 
     * If this.empty is true fill the pellet with black to make it invisible.
    */
    show() {
      fill("black");
      rect(this.x, this.y, this.w, this.h);
  
      // If it is not empty (has not been eaten) give it colour
      // otherwise it will default to black (not visible)
      if (!this.empty) {
        noStroke();
        fill(this.colour);
        ellipse(this.x, this.y, this.d, this.d);
      }
    }
  }