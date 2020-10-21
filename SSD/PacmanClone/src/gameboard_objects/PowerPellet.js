/** 
 * Class for a PowerPellet 
 * @extends Pellet
*/
class PowerPellet extends Pellet {
    /**
     * Creates a PowerPellet.
     * @param {number} x - x coordinate of where to draw power pellet.
     * @param {number} y - y coordinate of where to draw power pellet.
     * @param {number} w - width of the black square that is positioned behind the power pellet to give the illusion of a pathway.
     * @param {number} h - height of the black square that is positioned behind the power pellet to give the illusion of a pathway.
     */
    constructor(x, y, w, h) {
      super(x, y, w, h, 2)
      this.type = "powerPellet";
    }
  }