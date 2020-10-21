/** Class for a Wall object, a wall item is a simple coloured rectangle. */
class SquareWall extends Wall {
    /**
     * Creates a Wall.
     * @param {number} x - x coordinate of where to draw wall.
     * @param {number} y - y coordinate of where to draw wall.
     * @param {number} w - wall width.
     * @param {number} h - wall height.
     */
    constructor(x, y, w, h, color="#03b72d") {
      super(x, y, w, h, color);
      this.type = "wall";
    }
  
    /**
     * Displays the Wall.
     */
    show() {
      fill(this.colour);
      rect(this.x + this.w / 2, this.y + this.h / 2, this.w, this.h);
    }
  }