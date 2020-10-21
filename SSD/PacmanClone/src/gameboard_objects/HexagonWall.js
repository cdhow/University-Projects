// /** Class for a Wall object, a wall item that is a simple coloured hexagon. */

class HexagonWall extends Wall  {
    /**
     * Creates a HexagonWall.
     * @param {number} x - x coordinate of where to draw wall.
     * @param {number} y - y coordinate of where to draw wall.
     * @param {number} w - wall width.
     * @param {number} h - wall height.
     */
    constructor(x, y, w, h, color = "#03b72d") {
        super(x, y, w, h, color);
        this.type = "HexWall";
    }

    /**
     * Displays the Wall.
     */
    show() {
        push();
        image(
            hexagon,
              this.x + this.w / 2, // canvas x pos
              this.y + this.h / 2, // canvas y pos
              this.w, // image width
              this.h, // image height
            );
            pop();
    }
    // this.x + this.w / 2, this.y + this.h / 2, this.w, this.h
}