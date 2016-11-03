package ija.ui;

import ija.board.*;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Textove rozhranie hry (nie vsetky moznosti) Vsetko sa vypisuje na *STDOUT
 *
 * @author xjezov01
 * @author xmarus05
 */
public class Tui {

    MazeBoard hraciaPlocha;

    /**
     * Konstruktor pre triedu MazeBoard - vstupom je hracia plocha, nasledne
     * akcie su vykonavane na nej
     *
     * @param hraciaPlocha hracia plocha hry
     */
    public Tui(MazeBoard hraciaPlocha) {
        this.hraciaPlocha = hraciaPlocha;
        System.out.println("Welcome! Labirint v0.3 in TUI\n");
    }

    /**
     * Metoda spusta textove rozhranie hry, ocakava od uzivatela prikazy pomocou
     * CLI
     */
    public void startTUI() {
        BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
        boolean end = false;

        while (!end) {
            System.out.print("(Game)#");
            String instruction;
            try {
                instruction = in.readLine();
                end = oneStep(instruction);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * Podla vstupneho prikazu sa vykona urcita akcia, vrati ci bol prikaz
     * validny
     *
     * @param str ovladaci string
     * @return boolean
     */
    private boolean oneStep(String str) {
        switch (str) {
            case "q":
                return true; // koniec hry

            //case "n":	hraciaPlocha.newGame(); break; // nova hra
            case "p":
                showBoard();
                break; // zobrazi hraciu dosku

            default:
                if (!str.matches("[s][0-9][0-9]")) {
                    System.err.print("Zla volba\n");
                } else {
                    int row = Integer.parseInt((str.substring(1, 2)));
                    int col = Integer.parseInt((str.substring(2, 3)));

                    hraciaPlocha.shiftFIELD(row, col);
                }
                break;
        }
        return false;
    }

    /**
     * Vykresli do CLI pomocou ASCII cele hracie pole iba s kamenmi (v0.3 - bez
     * hracov, pokladov,...)
     */
    public void showBoard() {
        int n = this.hraciaPlocha.getN();
        System.out.println("-----------------------GameBoard-start----------------------------------\n");

        char ll = 0x03A6;
        System.out.print(ll + "|");// zaciatok
        for (int j = 1; j <= n; j++) {
            System.out.print(j);// index stlpca
            System.out.print("|");//posuv
        }

        System.out.println();//nic
        for (int i = 1; i <= n; i++) {
            System.out.print(i);// index riadku
            for (int j = 1; j <= n; j++) {
                MazeField field = hraciaPlocha.get(i, j);

                System.out.print("|" + (char) dajCestu(field.getCard()));

            }
            System.out.println("|");
        }

        System.out.println("\nFree: " + dajCestu(this.hraciaPlocha.getFreeField().getCard()));

        System.out.println("\n-----------------------GameBoard-end----------------------------------");
    }

    /**
     * Vrati hodnotu ASCII pre metodu showBoard() - ASCII predstavuje KAMEN
     *
     * @param card kamen kt sa ma zobrazovat
     * @return char
     */
    public char dajCestu(MazeCard card) {
        if (card.up != null) {
            if (card.down != null) {
                if (card.left != null) {
                    // hore dole dolava
                    return 0x2563;
                }

                if (card.right != null) {
                    // hore dole doprava
                    return 0x2560;
                }

                // hore dole
                return 0x2551;
            }

            if (card.left != null) {
                if (card.right != null) {
                    //hore dolava doprava
                    return 0x2569;
                }

                //hore dolava
                return 0x255D;
            }

            if (card.right != null) {
                //hore doprava
                return 0x255A;
            }
        }

        if (card.down != null) {
            if (card.left != null) {
                if (card.right != null) {
                    // dole dolava doprava
                    return 0x2566;
                }

                // dole dolava
                return 0x2557;
            }

            if (card.right != null) {
                // dole doprava
                return 0x2554;
            }
        }

        if (card.left != null && card.right != null) {
            // dolava doprava
            return 0x2550;
        }

        return 0; // ERR
    }
}
