package ija.player;

import ija.treasure.TreasureCard;

/**
 * Predstavuje vsetkych hracov hry
 *
 * @author xjezov01
 * @author xmarus05
 */
public class Players {

    int count;
    OnePlayer playersField[];
    int onTurn;
    final int MAXPLAY = 4;

    /**
     * Konstruktor pre triedu Players - nastavi pocet aktualnych hracov na 0,...
     */
    public Players() {
        this.count = 0;
        this.playersField = new OnePlayer[MAXPLAY];
        this.onTurn = 0;
    }

    /**
     * Vytvori jedneho hraca, nastavi mu meno, jeho kartu a pocet najdenych
     * pokladov
     *
     * @param name Meno hraca
     * @param foundCards pocet najdenych kariet
     * @param card karta ktoru prave hlada
     */
    public void createPlayer(String name, int foundCards, TreasureCard card) {
        this.playersField[this.count] = new OnePlayer(name, foundCards, card);

        this.count++;
    }

    /**
     * Posunie hru o hraca dalej
     */
    public void nextPlayer() {
        this.onTurn++;
        if (this.count == this.onTurn) {
            this.onTurn = 0;
        }
    }

    /**
     * Vrati kolko hracov je vytvorenych
     *
     * @return int pocet
     */
    public int getCount() {
        return count;
    }

    /**
     * Nastavi pocet vlozenych hracov (vyuzitelne pri obnove hry)
     *
     * @param num pocet hracov
     * @return boolean exit code
     */
    public boolean setCount(int num) {
        if (num < 0 || num > MAXPLAY) {
            return false;
        }

        count = num;
        return true;
    }

    /**
     * Vrati cislo hraca ktory je na tahu
     *
     * @return int cislo hraca kt je na rade
     */
    public int getOnTurn() {
        return onTurn;
    }

    /**
     * Nastavi ktory hrac je na tahu podla ID
     *
     * @param num cislo hraca, kt chceme aby bol na rade
     * @return boolean exit code
     */
    public boolean setOnTurn(int num) {
        if (num < 0 || num > MAXPLAY) {
            return false;
        }

        onTurn = num;
        return true;
    }

    /**
     * Vrati hraca, ktory je prave na tahu (vrati cely objekt)
     *
     * @return OnePlayer vracia hraca
     */
    public OnePlayer getPlayerOnTurn() {
        return this.playersField[this.onTurn];
    }

    /**
     * Vrati hraca podla zadaneho ID
     *
     * @param i cislo hraca, kt. chceme vratit
     * @return OnePlayer vracia hraca
     */
    public OnePlayer getPlayer(int i) {
        return this.playersField[i];
    }
}
