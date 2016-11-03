package ija.board;

import ija.board.MazeCard.CANGO;
import static ija.Labyrinth.LOG;
import ija.treasure.TreasureCard;
import java.awt.Dimension;
import java.util.Random;

/**
 * trieda hracej plochy
 *
 * @author xjezov01
 * @author xmarus05
 */
public class MazeBoard {

    //Kolko je aktualne Tciek, Liek Iciek (typy policok 1:1:1)
    int TS;
    int LS;
    int IS;
    //Max jedneho typu
    int MaxS;
    int n;

    //hracia plocha
    MazeField BOARD[][];
    //policko pre volny kamen
    MazeField FREEFIELD = null;

    /**
     * konstruktor, inicializuje hraciu plochu plus vypocita kolko akych policok
     * moze byt maximalne
     *
     * @param n velkost hracej plochy
     */
    public MazeBoard(int n) {
        // velkost hracej plochy
        this.n = n;
        TS = 0;
        LS = 4;
        IS = 0;
        //Get max count of one type button, +1
        MaxS = n * n / 3 + 1;
        this.BOARD = new MazeField[n + 1][n + 1];
    }

    /**
     * Vytvor hraciu plochu s velkostou n*n a napln ju inicializovanymi
     * polickami
     *
     * @param n velkost hracej plochy
     * @return MazeBoard hracia plochu s velkostou n*n
     */
    public static MazeBoard createMazeBoard(int n) {
        MazeBoard nove = new MazeBoard(n);

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                nove.BOARD[i][j] = new MazeField();
            }
        }

        return nove;
    }

    /**
     * Vygeneruju sa vsetky potrebne policka a vsetko potrebne
     */
    public void newGame() {
        int turns = 0;
        // generujem a umiestnim kamene do policok
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                turns = 0;
                //generacia T kamenov na neparnych poliach
                if ((i % 2) == 1 && (j % 2) == 1) {
                    this.BOARD[i][j].kamen = MazeCard.create("T");
                    if (i == 1) {
                        turns = 2;
                    } else if (j == 1) {
                        turns = 1;
                    } else if (j == n) {
                        turns = 3;
                    } else if (i == n) {
                        turns = 0;
                    } else {
                        turns = getIntRand(3);
                    }
                    this.BOARD[i][j].kamen.turn(turns);
                    TS += 1;
                } else {
                    //random generacia kamena, plus random otacanie
                    this.BOARD[i][j].kamen = MazeCard.create(myRAND());
                    this.BOARD[i][j].kamen.turn(getIntRand(3));
                }
            }
        }

        //vytvaranie rohovych kamenov
        this.BOARD[1][1].kamen = MazeCard.create("L");
        this.BOARD[1][1].kamen.turn(2);

        this.BOARD[1][n].kamen = MazeCard.create("L");
        this.BOARD[1][n].kamen.turn(3);

        this.BOARD[n][1].kamen = MazeCard.create("L");
        this.BOARD[n][1].kamen.turnRight();

        this.BOARD[n][n].kamen = MazeCard.create("L");

        // vytvorime volny kamen
        MazeCard FREECARD = MazeCard.create(myRAND());
        FREECARD.turn(getIntRand(3));
        this.FREEFIELD = new MazeField();
        this.FREEFIELD.kamen = FREECARD;

        //vygeneruj treasurky
        generateTreasures();

    }

    private void generateTreasures() {
        int i, x, y;

        for (i = 0; i < LOG.getItemsPack().maxSize(); i++) {
            while (true) {
                x = getIntRand(n);
                x++;
                y = getIntRand(n);
                y++;
                if (this.BOARD[x][y].getTreasure() == null) {
                    this.BOARD[x][y].item = new TreasureCard(i);
                    break;
                }
            }
        }
    }

    /**
     * vrati policko na pozicii r*c
     *
     * @param r row
     * @param c col
     * @return MazeField
     */
    public MazeField get(int r, int c) {
        if (r > this.n || c > this.n) {
            return null;
        }

        return this.BOARD[r][c];
    }

    /**
     * Vrati prazdne policko
     *
     * @return MazeField
     */
    public MazeField getFreeField() {
        return this.FREEFIELD;
    }

    /**
     * Vygeneruje random typ policka
     *
     * @return String L/T/I
     */
    public String myRAND() {
        int mojRAND;
        String type;
        if (TS >= MaxS && LS >= MaxS && IS >= MaxS) {
            return "I";
        }

        Random randcislo = new Random();
        // nextInt robi rozmedzie random int cisel
        mojRAND = randcislo.nextInt(3);
        if (mojRAND == 0 && TS <= MaxS) {
            TS += 1;
            type = "T";
        } else if (mojRAND == 1 && LS <= MaxS) {
            LS += 1;
            type = "L";
        } else if (mojRAND == 2 && IS <= MaxS) {
            IS += 1;
            type = "I";
        } else {
            type = myRAND();
        }

        return type;
    }

    /**
     * generacia cisla do 0 po n
     *
     * @param n maximum daneho intervalu
     * @return nahodne vygenerovane cislo do 0 po n
     */
    public int getIntRand(int n) {
        Random randcislo = new Random();
        int num = randcislo.nextInt(n);
        return num;
    }

    /**
     * vrat velkost hracej plochy
     *
     * @return Velkost hracej plochy
     */
    public int getN() {
        return n;
    }

    /**
     * Nastav kamen na dane hodnoty
     *
     * @param X suradnica X
     * @param Y suradnica Y
     * @param item karta
     * @param DOWN smer kam moze ist
     * @param LEFT smer kam moze ist
     * @param RIGHT smer kam moze ist
     * @param UP smer kam moze ist
     * @return boolean spravne vykonanie - true nespravne - false
     */
    public boolean setField(int X, int Y, int item, int DOWN, int LEFT, int RIGHT, int UP) {
        if (X > n || X < 0 || Y > n || Y < 0) {
            return false;
        }

        CANGO down = null;
        CANGO left = null;
        CANGO right = null;
        CANGO up = null;

        if (DOWN == 1) {
            down = CANGO.DOWN;
        } else if (DOWN != 0) {
            return false;
        }
        if (LEFT == 1) {
            left = CANGO.LEFT;
        } else if (LEFT != 0) {
            return false;
        }
        if (RIGHT == 1) {
            right = CANGO.RIGHT;
        } else if (RIGHT != 0) {
            return false;
        }
        if (UP == 1) {
            up = CANGO.UP;
        } else if (UP != 0) {
            return false;
        }

        if (item == -1) {
            BOARD[X][Y].createField(new MazeCard(left, up, right, down), null);
        } else {
            BOARD[X][Y].createField(new MazeCard(left, up, right, down), new TreasureCard(item));
        }

        return true;
    }

    /**
     * Nastva volny kamen
     *
     * @param item aky ma poklad
     * @param DOWN ci moze ist dole
     * @param LEFT ci moze ist dolava
     * @param RIGHT ci moze ist doprava
     * @param UP ci moze ist hore
     * @return boolean spravne vykonanie - true nespravne - false
     */
    public boolean setFreeField(int item, int DOWN, int LEFT, int RIGHT, int UP) {
        CANGO down = null;
        CANGO left = null;
        CANGO right = null;
        CANGO up = null;

        if (DOWN == 1) {
            down = CANGO.DOWN;
        }
        if (LEFT == 1) {
            left = CANGO.LEFT;
        }
        if (RIGHT == 1) {
            right = CANGO.RIGHT;
        }
        if (UP == 1) {
            up = CANGO.UP;
        }

        FREEFIELD = new MazeField();

        if (item == -1) {
            FREEFIELD.createField(new MazeCard(left, up, right, down), null);
        } else {
            FREEFIELD.createField(new MazeCard(left, up, right, down), new TreasureCard(item));
        }

        return true;
    }

    /**
     * Posun policko na danu poziciu
     *
     * @param TMProw riadok
     * @param TMPcol stlpec
     * @return Dimension insertCannot kam sa nabuduce nemoze vkladat kament
     */
    public Dimension shiftFIELD(int TMProw, int TMPcol) {

        MazeField TEMP;
        Dimension CannotGO;
        CannotGO = new Dimension();

        if (TMProw == 1 && TMPcol % 2 == 0) //kamene sa posunu DOLE v stlpci col
        {
            TEMP = this.BOARD[this.n][TMPcol]; // posledny

            for (int i = this.n; i > 1; i--) {
                // col sa nemeni

                this.BOARD[i][TMPcol] = this.BOARD[i - 1][TMPcol];
            }

            this.BOARD[TMProw][TMPcol] = this.FREEFIELD;

            this.FREEFIELD = TEMP;

            //System.out.println("POSUV>" + TMPcol);
            for (int k = 0; k < LOG.getPlayers().getCount(); k++) {
                if (LOG.getOnePlayer(k).getY() == TMPcol) {
                    int POSUV = LOG.getOnePlayer(k).getX() + 1;
                    if (POSUV > this.n) {
                        POSUV = 1;
                    }

                    LOG.getOnePlayer(k).setXY(POSUV, TMPcol);
                }
            }
            CannotGO.width = this.n;
            CannotGO.height = TMPcol;
            return CannotGO;
        }

        //kamene sa posunu HORE v stlpci col
        if (TMProw == this.n && TMPcol % 2 == 0) {
            TEMP = this.BOARD[1][TMPcol]; // posledny

            for (int i = 1; i < this.n; i++) {

                this.BOARD[i][TMPcol] = this.BOARD[i + 1][TMPcol];
            }

            this.BOARD[TMProw][TMPcol] = this.FREEFIELD;

            this.FREEFIELD = TEMP;

            for (int k = 0; k < LOG.getPlayers().getCount(); k++) {
                if (LOG.getOnePlayer(k).getY() == TMPcol) {
                    int POSUV = LOG.getOnePlayer(k).getX() - 1;
                    if (POSUV < 1) {
                        POSUV = this.n;
                    }

                    LOG.getOnePlayer(k).setXY(POSUV, TMPcol);
                }
            }

            CannotGO.width = 1;
            CannotGO.height = TMPcol;
            return CannotGO;
        }

        //kamene sa posunu DOPRAVA v riadku row
        if (TMProw % 2 == 0 && TMPcol == 1) {
            TEMP = this.BOARD[TMProw][this.n]; // posledny

            for (int i = this.n; i > 1; i--) {

                this.BOARD[TMProw][i] = this.BOARD[TMProw][i - 1];
            }

            this.BOARD[TMProw][TMPcol] = this.FREEFIELD;

            this.FREEFIELD = TEMP;

            for (int k = 0; k < LOG.getPlayers().getCount(); k++) {
                if (LOG.getOnePlayer(k).getX() == TMProw) {
                    int POSUV = LOG.getOnePlayer(k).getY() + 1;
                    if (POSUV > this.n) {
                        POSUV = 1;
                    }

                    LOG.getOnePlayer(k).setXY(TMProw, POSUV);
                }
            }
            CannotGO.width = TMProw;
            CannotGO.height = this.n;
            return CannotGO;
        }

        //kamene sa posunu DOLAVA v riadku row
        if (TMProw % 2 == 0 && TMPcol == this.n) {

            TEMP = this.BOARD[TMProw][1]; // posledny

            for (int i = 1; i < this.n; i++) {
                this.BOARD[TMProw][i] = this.BOARD[TMProw][i + 1];
            }

            this.BOARD[TMProw][TMPcol] = this.FREEFIELD;

            this.FREEFIELD = TEMP;

            for (int k = 0; k < LOG.getPlayers().getCount(); k++) {
                if (LOG.getOnePlayer(k).getX() == TMProw) {
                    int POSUV = LOG.getOnePlayer(k).getY() - 1;
                    if (POSUV < 1) {
                        POSUV = this.n;
                    }

                    LOG.getOnePlayer(k).setXY(TMProw, POSUV);
                }
            }
            CannotGO.width = TMProw;
            CannotGO.height = 1;
            return CannotGO;
        }
        return null;
    }

}