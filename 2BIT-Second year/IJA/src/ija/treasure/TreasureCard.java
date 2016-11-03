package ija.treasure;

/**
 * Trieda reprezentujuca poklad
 *
 * @author xjezov01
 * @author xmarus05
 */
public class TreasureCard {
    int item;

    /**
     * Konstruktor nastavi item v poklade na cislo item
     * @param item cislo pokladu
     */
    public TreasureCard(int item) {
        this.item = item;
    }

    /**
     * Vrat c. pokladu
     * @return int vrati cislo pokladu
     */
    public int getItem() {
        return item;
    }

    /**
     * Nastav poklad na cislo n
     * @param n cislo pokladu
     */
    public void setItem(int n){
        this.item = item;
    }
}
