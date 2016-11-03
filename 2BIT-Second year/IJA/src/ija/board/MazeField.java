package ija.board;

import ija.treasure.*;

/**
 * trieda reprezentujuca hracie policko obsahuje kamen a poklad
 *
 * @author xjezov01
 * @author xmarus05
 */
public class MazeField {

    MazeCard kamen;
    TreasureCard item;

    /**
     * Konstruktor, inicializuje kamen a poklad
     */
    public MazeField() {

        this.kamen = null;
        this.item = null;
    }

    /**
     * vrati kamen
     *
     * @return MazeCard kamen
     */
    public MazeCard getCard() {
        return this.kamen;
    }

    /**
     * vrati poklad
     *
     * @return TreasureCard poklad
     */
    public TreasureCard getTreasure() {
        return this.item;
    }

    /**
     * vymaz poklad z policka
     */
    public void removeTreasure() {
        this.item = null;
    }

    /**
     * vloz kamen do policka
     *
     * @param c typ karty
     */
    public void putCard(MazeCard c) {
        this.kamen = c;
    }

    /**
     * Napln policko s kamenom a pokladom
     *
     * @param kamen urcity kamen
     * @param item urcity poklad
     */
    public void createField(MazeCard kamen, TreasureCard item) {
        this.kamen = kamen;
        this.item = item;
    }

}