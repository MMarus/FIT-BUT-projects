package ija.board;

/**
 * trieda reprezentujuca hraciu kartu
 *
 * @author xjezov01
 * @author xmarus05
 */
public class MazeCard {

    //smery kam sa moze hrac hybat

    public CANGO left;
    public CANGO up;
    public CANGO right;
    public CANGO down;

    public static enum CANGO {

        LEFT,
        UP,
        RIGHT,
        DOWN
    }

    /**
     * konstruktor vytvori policko, s danymi hodnotami smerov kam moze ist
     *
     * @param left ci moze ist dolava
     * @param up ci moze ist hore
     * @param right ci moze ist vpravo
     * @param down ci moze ist dole
     */
    public MazeCard(CANGO left, CANGO up, CANGO right, CANGO down) {
        this.left = left;
        this.up = up;
        this.right = right;
        this.down = down;
    }

    /**
     * vytvor kamen s tvarom podla typu
     *
     * @param type L / T / I -typ kamena
     * @return MazeCard vytvoreny kamen
     */
    public static MazeCard create(String type) {
        MazeCard NEWW;

        switch (type) {
            case "L": // L rohova cesta -- zlava hore
                NEWW = new MazeCard(CANGO.LEFT, CANGO.UP, null, null);
                break;

            case "I": // ROVNA CESTA -- zlava doprava
                NEWW = new MazeCard(CANGO.LEFT, null, CANGO.RIGHT, null);
                break;

            case "T": // T cesta -- zlava hore aj doprava
                NEWW = new MazeCard(CANGO.LEFT, CANGO.UP, CANGO.RIGHT, null);
                break;

            default:
                throw new IllegalArgumentException();
        }

        return NEWW;

    }

    /**
     * Otoc kamen o 90 stupnov
     */
    public void turnRight() {
        CANGO TEMP;

        TEMP = this.left;

        this.left = this.down;
        this.down = this.right;
        this.right = this.up;
        this.up = TEMP;
    }

    /**
     * Otoc kamenom o 90 stupnov * n
     *
     * @param n kolko krat treba otocit
     */
    public void turn(int n) {
        int i;
        for (i = 0; i < n; i++) {
            turnRight();
        }
    }

    /**
     * vracia ci je mozne ist do daneho smeru
     *
     * @param dir smer kam chcem ist
     * @return boolean true -moze ist false - nemoze ist
     */
    public boolean canGo(MazeCard.CANGO dir) {
        if (dir == CANGO.LEFT) {
            if (this.left != null) {
                return true;
            } else {
                return false;
            }
        } else if (dir == CANGO.UP) {
            if (this.up != null) {
                return true;
            } else {
                return false;
            }
        } else if (dir == CANGO.RIGHT) {
            if (this.right != null) {
                return true;
            } else {
                return false;
            }
        } else if (dir == CANGO.DOWN) {
            if (this.down != null) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }

    }
}