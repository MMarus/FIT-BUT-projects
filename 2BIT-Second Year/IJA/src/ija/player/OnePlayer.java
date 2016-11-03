package ija.player;

import ija.treasure.*;

/**
 * Predstavuje prave jedneho hraca
 *
 * @author xjezov01
 * @author xmarus05
 */
public class OnePlayer {

    String Name;
    int foundCards;
    TreasureCard myCard;
    // pozicia v hre
    int x;
    int y;

    /**
     * Konstruktor pre triedu Players - nastavi hracovi meno, ...
     *
     * @param Name meno hraca
     * @param foundCards pocet najdenych kariet
     * @param myCard karta hraca
     */
    public OnePlayer(String Name, int foundCards, TreasureCard myCard) {
        this.Name = Name;
        this.foundCards = foundCards;
        this.myCard = myCard;

        //pozicia v hre
        this.x = 0;
        this.y = 0;
    }

    /**
     * Nastavi hracovi jeho suradnice (kde sa vyskytuje na hracej ploche)
     *
     * @param x x-ova suradnica hraca
     * @param y y-ova suradnica hraca
     */
    public void setXY(int x, int y) {
        this.x = x;
        this.y = y;
    }

    /**
     * Vrati X-ovu suradnicu v hre
     *
     * @return int x-ova suradnica
     */
    public int getX() {
        return this.x;
    }

    /**
     * Vrati Y-novu suradnicu v hre
     *
     * @return int Y-suradnica
     */
    public int getY() {
        return this.y;
    }

    /**
     * Ziska meno hraca
     *
     * @return String meno hraca
     */
    public String getName() {
        return Name;
    }

    /**
     * Ziska pocet najdenych pokladov
     *
     * @return int pocet najdenych pokladov
     */
    public int getFoundCards() {
        return foundCards;
    }

    /**
     * Nastavi pocet najdenych pokladov
     *
     * @param i pocet najdenych pokladov
     */
    public void setFoundCards(int i) {
        this.foundCards = i;
    }

    /**
     * Vola sa ked hrac najde poklad - prida mu do jeho najdenych
     */
    public void addFoundCards() {
        this.foundCards++;
    }

    /**
     * Vytiahne hracovu kartu
     *
     * @return TreasureCard vracia kartu hraca
     */
    public TreasureCard getMyCard() {
        return myCard;
    }

    /**
     * Nastavi hracovi jeho kartu
     *
     * @param myCard karta hraca
     */
    public void setMyCard(TreasureCard myCard) {
        this.myCard = myCard;
    }
}
