package ija.controller;

import ija.ui.Images;
import ija.ui.Gui;
import ija.board.*;
import ija.player.*;
import ija.treasure.*;
import java.awt.Dimension;
import static java.lang.Math.abs;

/**
 * Logika celej hry, obsluhuje a obsahuje vsetky potrebne info
 *
 * @author xjezov01
 * @author xmarus05
 */
public class Logic {

    private MazeBoard gameBoard;
    private CardPack items;
    private Players players;
    private Images images;

    int gameState = 1;
    Dimension insertLast = new Dimension(0, 0);
    Dimension insertCannot = new Dimension(0, 0);

    public Gui windowGui;

    /**
     * Konstruktor inicializuje premenne
     */
    public Logic() {
        this.gameBoard = null;
        this.items = null;
        this.players = null;
        this.images = new Images();
    }

    /**
     * Zapnutie GUI 
     */
    public void startGui() {
        windowGui = new Gui();
        windowGui.setVisible(true);
    }

    /**
     * Pri nacitavani hry, nastavi vsetky premenne
     * @param newGamestate novy status hry 
     * @param newLast novy kamen kam sa posledny krat vkladalo
     * @param newCannot kam nemozeme vkladat
     * @param newGameBoard nova hracia plocha
     * @param newItems novy hraci balicek
     * @param newPlayers novy hraci
     */
    public void setLogic(int newGamestate, Dimension newLast, Dimension newCannot, MazeBoard newGameBoard, CardPack newItems, Players newPlayers) {
        this.gameBoard = newGameBoard;
        this.items = newItems;
        this.players = newPlayers;
        this.gameState = newGamestate;
        this.insertLast = newLast;
        this.insertCannot = newCannot;
    }

    /**
     * vrat stav hry
     * @return int stav hry (0-5)
     */
    public int getGameState() {
        return this.gameState;
    }

    /**
     * Vytvor novu hru a napln premenne
     * @param sizeCnt velkost hracej plochy
     * @param cardsCnt velkost balicku
     * @param playersCnt pocet hracov
     * @return int 0: OK
     */
    public int NewGame(int sizeCnt, int cardsCnt, int playersCnt) {
        // vytvorime balicek kariet + zamiesame
        gameState = 1;
        items = new CardPack(cardsCnt, cardsCnt);
        items.shuffle();

        // vytvorime hracie pole
        gameBoard = MazeBoard.createMazeBoard(sizeCnt);
        gameBoard.newGame();

        // vytvorime pole hracov, neskor sa bude naplnat
        //plus pridame im X a Y
        this.players = new Players();

        return 0;//OK
    }

    /**
     * vrati hraca ktory je na rade
     * @return OnePlayer hrac ktory je na rade
     */
    public OnePlayer getPlayer() {
        return players.getPlayerOnTurn();
    }

    /**
     * Vrati vestkych hracov
     * @return Players vsetci hraci
     */
    public Players getPlayers() {
        return players;
    }

    /**
     * Vrati hraca s cislom i
     * @param i cislo hraca
     * @return OnePlayer najdeny hrac
     */
    public OnePlayer getOnePlayer(int i) {
        return players.getPlayer(i);
    }

    /**
     * Vloz hraca s menom
     * @param name meno hraca
     */
    public void insertPlayer(String name) {
        //tu bude case atd
        int ID = players.getCount();
        int x = 0, y = 0;
        switch (ID) {
            case 0:
                x = 1;
                y = 1;
                break;
            case 1:
                x = 1;
                y = gameBoard.getN();
                break;
            case 2:
                x = gameBoard.getN();
                y = 1;
                break;
            case 3:
                x = gameBoard.getN();
                y = gameBoard.getN();
                break;
        }

        //26 is EMPTY CARD
        TreasureCard CARD = new TreasureCard(26);
        this.players.createPlayer(name, 0, CARD);
        this.players.getPlayer(ID).setXY(x, y);
    }

    /**
     * Dalsi na rade
     */
    public void nextTurn() {
        this.players.nextPlayer();
    }

    /**
     * Kliknutie na balicek kariet, ked nema kartu dostane novu z vrchu balicka
     */
    public void cardClick() {
        OnePlayer HRAC = players.getPlayerOnTurn();

        if (HRAC.getMyCard().getItem() == 26) {
            //System.out.println("HRAC>" + HRAC.getName() + ">>NEMA KARTU");
            TreasureCard item;

            if (items.size() == 0) {
                //System.out.println("BALICEK PRAZNY!!!");
                item = new TreasureCard(25);
            } else {
                item = items.popCard();
            }

            // nastavi hracovi tu novu kartu
            HRAC.setMyCard(item);
        }
    }

    /**
     * Vrat cislo pokladu
     * @return int cislo pokladu
     */
    public int showMyCard() {
        OnePlayer HRAC = players.getPlayerOnTurn();

        TreasureCard KARTA = HRAC.getMyCard();

        return KARTA.getItem();
    }

    /**
     * vrati balicek kariet
     * @return CardPack
     */
    public CardPack getItemsPack() {
        return this.items;
    }

    /**
     * Vrati meno hraca ktory je na rade
     * @return String meno hraca ktory je na rade
     */
    public String showOnTurn() {
        OnePlayer HRAC = players.getPlayerOnTurn();
        String name = HRAC.getName();

        return name;
    }

    /**
     * Vrati hraciu plochu
     * @return hraciu plochu
     */
    public MazeBoard showBoard() {
        return this.gameBoard;
    }

    /**
     * vrati Obrazky
     * @return Images Obrazky
     */
    public Images getImages() {
        return this.images;
    }

    /**
     * spracovava kliknutia + priebeh hry uchovava krok hry/ pozera sa nan Ak je
     * x, y vecsie ako nula bolo kliknute na BOARD, inak len obyc klik
     * @param Code cislo buttona ktory bol kliknuty
     * @param x suradnice buttona ktory bol kliknuty
     * @param y suradnice buttona ktory bol kliknuty
     */
    public void GameProcess(int Code, int x, int y) {

        //over karrtu ci nevyhravam...
        int plTreas = players.getPlayerOnTurn().getMyCard().getItem();
        TreasureCard mfTreas = gameBoard.get(players.getPlayerOnTurn().getX(), players.getPlayerOnTurn().getY()).getTreasure();

        if (mfTreas != null && plTreas == mfTreas.getItem()) {
            // NASIEL KARTU pripocitame mu ,  a nastavime ze nema kartu!
            players.getPlayerOnTurn().addFoundCards();
            gameBoard.get(players.getPlayerOnTurn().getX(), players.getPlayerOnTurn().getY()).removeTreasure();
            players.getPlayerOnTurn().setMyCard(new TreasureCard(26));

            gameState = 5;

        }

        if ((items.maxSize() / players.getCount()) <= players.getPlayerOnTurn().getFoundCards()) {
            // HRA KONCI
            gameState = 0;
        }

        switch (gameState) {
            //1 krok, zober kartu
            case 1:
                if (Code == 11) {
                    cardClick();
                    gameState = 2;
                }

                break;
            //2 krok, natoc kamen + vloz
            case 2:
                if (Code == 1) {
                    gameState = 4; // ACCEPT NEVLOZILI SME NIC --> STATE -> 4
                    break;
                }

                if (Code == 12) {
                    gameBoard.getFreeField().getCard().turnRight();
                }

                if (Code == 13) {
                    //System.out.println("CHCEM>x = "+insertCannot.width+" y = "+ insertCannot.height);
                    if (insertCannot.width != x || insertCannot.height != y) {
                        Dimension shifting = gameBoard.shiftFIELD(x, y);
                        if (shifting != null) {
                            insertLast = insertCannot;
                            insertCannot = shifting;
                            //LAST.width = x;
                            //LAST.height = y;
                            gameState = 3;
                        }

                    }
                    //System.out.println("CANTGO>x = "+insertCannot.width+" y = "+ insertCannot.height);
                }

                break;

            case 3:
                // prisiel som zo stavu 2 >> MOZEM DAT UNDO BUTTON ALEBO ACCEPT!
                if (Code == 1) {
                    //ACCEPT --> STAV 3
                    gameState = 4;
                }

                if (Code == 2) {
                    // UNDO BUTTON

                    Dimension shifting = gameBoard.shiftFIELD(insertCannot.width, insertCannot.height);
                    if (shifting != null) {
                        insertCannot = insertLast;
                        //insertLast.width = x;
                        //insertLast.height = y;
                        gameState = 2;
                    }
                }
                break;

            //4 krok posuv hraca po doske + accept
            case 4:

                if (Code == 13) {
                    posuvHraca(x, y);
                }

                if (Code == 1) {
                    nextTurn();
                    gameState = 1;
                    if (players.getPlayerOnTurn().getMyCard().getItem() != 26) {
                        gameState = 2;
                    }
                }
                break;

            //NASIEL SOM KARTU!, moze si vypytat dalsiu, a znova na krok 4
            case 5:
                if (Code == 11) {
                    cardClick();
                    gameState = 4;
                }

                if (Code == 1) {
                    nextTurn();
                    gameState = 1;
                    if (players.getPlayerOnTurn().getMyCard().getItem() != 26) {
                        gameState = 2;
                    }
                }
                break;

            //KONIEC HRY -- NIEKTO VYHRAL!!
            case 0:
                break;
        }

    }

    /**
     * Posuv hraca na policko x y
     * @param x suradnice
     * @param y suradnice
     */
    public void posuvHraca(int x, int y) {
        // ak je policko o 1 vzdialene mozeme tam hraca posunut
        //zisti o kolko sa posuvame
        OnePlayer player = getPlayer();
        //Ideme len o jedno ??? 
        int shiftCount = abs(player.getX() - x) + abs(player.getY() - y);

        //Mozem ist do daneho smeru ?? 
        // Idem na druhu straunu 
        int playerX = player.getX();
        int playerY = player.getY();

        int horizontal = player.getY() - y;
        int vertical = player.getX() - x;

        if (abs(player.getX() - x) == (gameBoard.getN() - 1) && abs(player.getY() - y) == 0) {
            shiftCount = 1;
        }
        if (abs(player.getY() - y) == (gameBoard.getN() - 1) && abs(player.getX() - x) == 0) {
            shiftCount = 1;
        }

        //Mozem ist Do prava??
        if (horizontal == -1 || horizontal == (gameBoard.getN() - 1)) {
            //System.out.println("chcem ist do prava?? player " + (getPlayer().getX() - x) + ""            );
            //Ak nemozeme doprava, return
            if (!gameBoard.get(player.getX(), player.getY()).getCard().canGo(MazeCard.CANGO.RIGHT)) {
                return;
            }
            if (!gameBoard.get(x, y).getCard().canGo(MazeCard.CANGO.LEFT)) {
                return;
            }
        }
        //Mozem ist Do lava??
        if (horizontal == 1 || horizontal == (gameBoard.getN() - 1) * (-1)) {
            if (!gameBoard.get(player.getX(), player.getY()).getCard().canGo(MazeCard.CANGO.LEFT)) {
                return;
            }
            if (!gameBoard.get(x, y).getCard().canGo(MazeCard.CANGO.RIGHT)) {
                return;
            }
        }
        //Mozem ist Dole??
        if (vertical == -1 || vertical == (gameBoard.getN() - 1)) {
            if (!gameBoard.get(player.getX(), player.getY()).getCard().canGo(MazeCard.CANGO.DOWN)) {
                return;
            }
            if (!gameBoard.get(x, y).getCard().canGo(MazeCard.CANGO.UP)) {
                return;
            }
        }
        //Mozem ist Hore??
        if (vertical == 1 || vertical == (gameBoard.getN() - 1) * (-1)) {
            if (!gameBoard.get(player.getX(), player.getY()).getCard().canGo(MazeCard.CANGO.UP)) {
                return;
            }
            if (!gameBoard.get(x, y).getCard().canGo(MazeCard.CANGO.DOWN)) {
                return;
            }
        }

        if (shiftCount == 1) {
            //posuneme
            getPlayer().setXY(x, y);
        }

    }

    /**
     * Funkcia ovladajuca cheaty (zatial len god mode) v buducnosti mozno viac
     * @param n cislo cheatu
     */
    public void cheats(int n) {

        //GODMODE
        if (n == 1) {
            int item;
            for (int x = 1; x <= gameBoard.getN(); x++) {
                for (int y = 1; y <= gameBoard.getN(); y++) {
                    if (gameBoard.get(x, y).getTreasure() == null) {
                        item = -1;
                    } else {
                        item = gameBoard.get(x, y).getTreasure().getItem();
                    }
                    gameBoard.setField(x, y, item, 1, 1, 1, 1);
                }
            }
        }
    }
}
