package ija.treasure;

import java.util.Random;

/**
 * Predstavuje balicek kariet
 *
 * @author xjezov01
 * @author xmarus05
 */
public class CardPack {

    public TreasureCard balicek[];
    int maxSizee;
    int actualSize;

    /**
     * Konstruktor vytvory balicek kariet s velkostou maxSize a nastavi aktualny
     * pocet kariet v balicku na intiSize
     *
     * @param maxSize max pocet kariet v balicku
     * @param initSize inicializacny pocet kariet v balicku
     */
    public CardPack(int maxSize, int initSize) {
        if (maxSize >= initSize) {
            // vytvorime novy balicek kariet s velkostou maxSize
            balicek = new TreasureCard[maxSize];
            this.maxSizee = maxSize;
            // treba vediet kolko kariet je aktualne v balicku
            this.actualSize = initSize;

            // tu sa naplnaju karty do balicku ( na vrchu balicku musi byt karta s pokladom 0 )
            for (int i = 0; i < maxSize; i++) {
                balicek[i] = new TreasureCard(i);
            }
        }
    }

    /**
     * Zober kart z vrchu balicku, zniz pocet kariet v balicku o 1
     * 
     * @return TreasureCard Vracia kartu z vrchu balicku musi aj znizit pocet kariet v
     * balicku
     */
    public TreasureCard popCard() {
        return balicek[--this.actualSize];
    }

    /**
     * vracia aktualny pocet kariet v balicku
     * 
     * @return int aktualny pocet kariet v balicku
     */
    public int size() {
        return this.actualSize;
    }

    /**
     * Navracia MAXIMALNY POCET KARIET V BALICKU
     * 
     * @return int maximalny pocet kariet baliku
     */
    public int maxSize() {
        return this.maxSizee;
    }

    /**
     * Nastav max velkost balicku 
     * @param num Maximalna velkost balicku
     * @return boolean true: podarilo sa nastavit, false: chyba
     */
    public boolean setMaxSize(int num) {
        if (num != 12 && num != 24) {
            return false;
        }
        this.maxSizee = num;
        return true;
    }

    /**
     * Vrat poklad karty s cislom n
     * @param n cislo pokladu
     * @return TreasureCard vracia poklad
     */
    public TreasureCard getTreasure(int n) {
        if (n < 0 || n > maxSizee) {
            return null;
        }
        return balicek[n];
    }
    //

    /**
     * Nastav kartu s cislom n aby obsahoval poklad cislo item
     * @param n cislo karty balicku
     * @param item cislo pokladu
     * @return boolean boolean true: podarilo sa nastavit, false: chyba
     */
    public boolean setTreasure(int n, int item) {
        if (n < 0 || n > maxSizee) {
            return false;
        }
        balicek[n].setItem(item);
        return true;
    }

    /**
     * Zamiesaj balicek
     */
    public void shuffle() {

        TreasureCard zaloha; // zaloha pre SWAP
        int mojRAND;

        for (int i = 0; i < this.actualSize; i++) {
            // urobi random cislo ale este ho bude treba dat aby to bolo int + rozmedzie
            Random randcislo = new Random();

            mojRAND = randcislo.nextInt(this.actualSize); // nextInt robi rozmedzie random int cisel

            // SWAP
            zaloha = this.balicek[i];
            this.balicek[i] = this.balicek[mojRAND];
            this.balicek[mojRAND] = zaloha;
        }
    }
}
