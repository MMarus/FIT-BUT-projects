package ija;

import ija.controller.*;

//TODO: vygenerovat dokumentaciu, pridat komenty, 
//TODO: dokomentovat about a help
// vytvorit krajsie figurky,
// vytvorit krajsiu cesticku ( 3 obrazky)
//TODO: pomenit nazvy premennych a class 
//TODO: javodc
//TODO: dokomentovat
//VYLEPSENIA:
//TODO: CHEATY, TELEPORT ... 
// vysvietit okienko ktore teraz ma player pouzivat
// vysvietit cesticku playerovi kam moze ist ( iba o 1 krok do predu)

/**
 * Hlavna trieda, ktora spusta cely program
 *
 * @author xjezov01
 * @author xmarus05
 */
public class Labyrinth {

    public static Logic LOG;

    /**
     * Spusta cely program
     *
     * @param argv Argumenty CLI pri spusteni
     */
    public static void main(String argv[]) {

        LOG = new Logic();
        LOG.startGui();
    }
}