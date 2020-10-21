/** Base class for a Wall object, a wall item is a simple coloured shape. */
class Wall {
    /**
     * Creates a Wall.
     * @param {number} x - x coordinate of where to draw wall.
     * @param {number} y - y coordinate of where to draw wall.
     * @param {number} w - wall width.
     * @param {number} h - wall height.
     */
    constructor(x, y, w, h, color="#03b72d") {
        this.x = x;
        this.y = y;
        this.w = w;
        this.h = h;
        this.colour = color;
    }
  
    /**
     * Displays the Wall.
     */
    show() {}
  }