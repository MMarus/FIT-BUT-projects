/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.controller;

import ija.board.MazeBoard;
import static ija.Labyrinth.LOG;
import ija.player.Players;
import ija.treasure.CardPack;
import ija.treasure.TreasureCard;
import java.awt.Dimension;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStreamWriter;

import java.io.Writer;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * trieda na ukladanie a nacitavanie hier
 *
 * @author xjezov01
 * @author xmarus05
 */
public class GameIO {

    /**
     * String, pouzite pri ukladani hry, sluzia aj ako check ci je spravny subor
     */
    public static final String[] MSGS = {
        "###### MEGA MEGA SUPER LABIRINT SAVE ######\n",
        "### GAMEINFO\n",
        "### ALL PLAYERS\n",
        "#PLAYER ",
        "### ALL ITEMS\n",
        "#ITEM",
        "### ALL BOARD\n",
        "##FREEFIELD\n",
        "#FIELD >> X:",
        ">> Y:",
        "###### END OF SAVE ######\n"
    };

    /**
     * Ulozenie hry
     *
     * @param GAMENAME meno suboru kam ulozime hru
     * @param ULOZ parameter ci chceme prepisat hru s rovnakym nazvom
     * @return int exit kod
     */
    public static int save(String GAMENAME, boolean ULOZ) {
        String DATA = MSGS[0];

        DATA += saveInfo();
        DATA += savePlayer();
        DATA += saveItems();
        DATA += saveBoard();

        DATA += MSGS[10];

        return write(DATA, GAMENAME, ULOZ);
    }

    /**
     * Ulozi premenne gameState insertLast insertCannot do stringu
     *
     * @return String obsahuje informacie o hre gameState insertLast
     * insertCannot
     */
    public static String saveInfo() {
        String DATA = MSGS[1];

        //GAMESTATE
        DATA += LOG.gameState + "\n";
        // insertLast
        DATA += LOG.insertLast.width + "\n";
        DATA += LOG.insertLast.height + "\n";
        //CANNOT
        DATA += LOG.insertCannot.width + "\n";
        DATA += LOG.insertCannot.height + "\n";

        return DATA;
    }

    /**
     * Uloz hracov do string
     *
     * @return String info o hracoch
     */
    public static String savePlayer() {
        String DATA = MSGS[2];
        // COUNT + ONTURN
        DATA += LOG.getPlayers().getCount() + "\n";
        DATA += LOG.getPlayers().getOnTurn() + "\n";

        //ONEPLAYER
        for (int i = 0; i < LOG.getPlayers().getCount(); i++) {
            DATA += MSGS[3] + (i + 1) + "\n";
            DATA += LOG.getOnePlayer(i).getName() + "\n";
            DATA += LOG.getOnePlayer(i).getFoundCards() + "\n";
            DATA += LOG.getOnePlayer(i).getX() + "\n";
            DATA += LOG.getOnePlayer(i).getY() + "\n";
            DATA += LOG.getOnePlayer(i).getMyCard().getItem() + "\n";
        }

        return DATA;

    }

    /**
     * Uloz balicek pokladov
     *
     * @return String info o balicku pokladov
     */
    public static String saveItems() {
        String DATA = MSGS[4];
        //MAXSIZE
        DATA += LOG.getItemsPack().maxSize() + "\n";
        //ACTUAL SIZE
        DATA += LOG.getItemsPack().size() + "\n";

        for (int i = 0; i < LOG.getItemsPack().maxSize(); i++) {
            DATA += MSGS[5] + (i + 1) + "\n";
            DATA += LOG.getItemsPack().getTreasure(i).getItem() + "\n";
        }

        return DATA;
    }

    /**
     * Uloz hraciu plochu
     *
     * @return String info o hracej ploche( aky poklad, ake smery)
     */
    public static String saveBoard() {
        String DATA = MSGS[6];
        // GAME N x N
        DATA += LOG.showBoard().getN() + "\n";

        //FREEFIELD
        DATA += MSGS[7];
        //ITEM OF FREEFIELD
        if (LOG.showBoard().getFreeField().getTreasure() == null) {
            DATA += "-1\n";
        } else {
            DATA += LOG.showBoard().getFreeField().getTreasure().getItem() + "\n";
        }

        //ROCK DOWN
        if (LOG.showBoard().getFreeField().getCard().down != null) {
            DATA += "1\n";
        } else {
            DATA += "0\n";
        }
        //ROCK LEFT
        if (LOG.showBoard().getFreeField().getCard().left != null) {
            DATA += "1\n";
        } else {
            DATA += "0\n";
        }
        //ROCK RIGHT
        if (LOG.showBoard().getFreeField().getCard().right != null) {
            DATA += "1\n";
        } else {
            DATA += "0\n";
        }
        //ROCK UP
        if (LOG.showBoard().getFreeField().getCard().up != null) {
            DATA += "1\n";
        } else {
            DATA += "0\n";
        }

        // ALL FIELDS ON BOARD
        for (int x = 1; x <= LOG.showBoard().getN(); x++) {
            for (int y = 1; y <= LOG.showBoard().getN(); y++) {
                DATA += MSGS[8] + (x) + MSGS[9] + (y) + "\n";

                //ITEM
                if (LOG.showBoard().get(x, y).getTreasure() == null) {
                    DATA += "-1\n";
                } else {
                    DATA += LOG.showBoard().get(x, y).getTreasure().getItem() + "\n";
                }

                //ROCK DOWN
                if (LOG.showBoard().get(x, y).getCard().down != null) {
                    DATA += "1\n";
                } else {
                    DATA += "0\n";
                }
                //ROCK LEFT
                if (LOG.showBoard().get(x, y).getCard().left != null) {
                    DATA += "1\n";
                } else {
                    DATA += "0\n";
                }
                //ROCK RIGHT
                if (LOG.showBoard().get(x, y).getCard().right != null) {
                    DATA += "1\n";
                } else {
                    DATA += "0\n";
                }
                //ROCK UP
                if (LOG.showBoard().get(x, y).getCard().up != null) {
                    DATA += "1\n";
                } else {
                    DATA += "0\n";
                }

            }
        }

        return DATA;
    }

    /**
     * Zapis informacie do suboru
     *
     * @param DATA info na zapisanie
     * @param GAMENAME meno suboru
     * @param ULOZ default - false, ak existuje subor s danym menom opyta sa ci
     * chceme prepisat, ak druhy raz pride true, chceme prepisat existujuci
     * subor
     * @return int exit kod 0 je spravne vykonane
     */
    public static int write(String DATA, String GAMENAME, boolean ULOZ) {

        Writer writer = null;
        if ("".equals(GAMENAME)) {
            java.util.Date date = new java.util.Date();
            SimpleDateFormat simpleDateFormat = new SimpleDateFormat("YYYY-MM-dd---HH-mm-ss");
            
            GAMENAME = simpleDateFormat.format(date).toUpperCase();
        }

        //CREATE FOLDER FOR SAVE
        File folder = new File("savedGames");
        if (!folder.exists()) {
            //dont exist , i create
            if (folder.mkdir() == false) {
                return 10; // NEDA SA
            }
        }

        if (ULOZ == false) {
            File f = new File("savedGames/" + GAMENAME);
            if (f.exists()) {
                //existuje
                return 5;
            }

        }

        try {
            writer = new BufferedWriter(new OutputStreamWriter(
                    new FileOutputStream("savedGames/" + GAMENAME), "utf-8"));
            writer.write(DATA);
        } catch (IOException ex) {
            return 10;
        }
        try {
            writer.close();
        } catch (IOException ex) {
            Logger.getLogger(GameIO.class.getName()).log(Level.SEVERE, null, ex);
        }

        return 0;
    }

    /**
     * Zisti dostupne ulozene hry na nacitanie
     * @return ArrayList of String zoznam vsetky hier v zlozke
     */
    public static String[] getSavedGames() {
        ArrayList<String> games = new ArrayList<>();

        File folder = new File("savedGames");
        File[] listOfFiles = folder.listFiles();
        games.add("Zvolte hru");

        if (folder.exists()) {
            for (File listOfFile : listOfFiles) {
                if (listOfFile.isFile()) {
                    //System.out.println("File " + listOfFile.getName());
                    games.add(listOfFile.getName());
                } else if (listOfFile.isDirectory()) {
                    //System.out.println("Directory " + listOfFile.getName());
                }
            }
        } else {
            System.out.println("Priecinok neexistuje");
        }

        return games.toArray(new String[games.size()]);
    }  
    
    /**
     * Nacitaj hru zo suboru
     * @param GameName Meno suboru odkial nacitavat
     * @return int return code - 1: chyba 0: ok
     */
    public static int LoadGame(String GameName) {

        File file = new File("savedGames/" + GameName);

        if (!file.exists() || !file.canRead()) {
            return 1;
        }
        
        //docasne ulozenie informacii o nacitanej hre
        int NEW_GAMESTATE;
        Dimension NEW_LAST;
        Dimension NEW_CANNOT;
        int NEW_PlayerCount;
        MazeBoard NEW_hraciaPlocha;
        CardPack NEW_items;
        Players NEW_hraci;

        //Najskor si nacitaj subor
        int data, i, width, height;
        String l, m;
        try (BufferedReader br = new BufferedReader(new FileReader("savedGames/" + GameName))) {

            //###### MEGA MEGA SUPER LABIRINT SAVE ######
            if (!MSGS[0].equals(br.readLine() + "\n")) {
                return 1;
            }

            //### GAMEINFO
            if (!MSGS[1].equals(br.readLine() + "\n")) {
                return 1;
            }

            //GameState
            try {
                data = Integer.parseInt(br.readLine());
            } catch (NumberFormatException e) {
                return 1;
            }

            if (data < 0 || data > 5) {
                return 1;
            }
            NEW_GAMESTATE = data;

            //LAST
            try {
                width = Integer.parseInt(br.readLine());
            } catch (NumberFormatException e) {
                return 1;
            }
            try {
                height = Integer.parseInt(br.readLine());
            } catch (NumberFormatException e) {
                return 1;
            }
            NEW_LAST = new Dimension(width, height);

            //CANNOT
            try {
                width = Integer.parseInt(br.readLine());
            } catch (NumberFormatException e) {
                return 1;
            }
            try {
                height = Integer.parseInt(br.readLine());
            } catch (NumberFormatException e) {
                return 1;
            }
            NEW_CANNOT = new Dimension(width, height);

            //### ALL PLAYERS
            if (!MSGS[2].equals(br.readLine() + "\n")) {
                return 1;
            }

            try {
                data = Integer.parseInt(br.readLine());
            } catch (NumberFormatException e) {
                return 1;
            }
            //Pocet palyerov
            NEW_PlayerCount = data;
            if (NEW_PlayerCount < 1 || NEW_PlayerCount > 4) {
                return 1;
            }

            //vytvorime novych hracov
            NEW_hraci = new Players();

            try {
                data = Integer.parseInt(br.readLine());
            } catch (NumberFormatException e) {
                return 1;
            }
            int onTurn = data;
            //On turn - ak je onTurn vecsi ako pocet hracov, vrat chybu.
            if (NEW_PlayerCount < onTurn || false == NEW_hraci.setOnTurn(onTurn)) {
                return 1;
            }

            //Players - po jednom nacitame
            for (i = 0; i < NEW_PlayerCount; i++) {

                l = br.readLine();
                //System.out.println(l);
                m = MSGS[3] + (i + 1);
                if (!l.equals(m)) {
                    return 1;
                }

                String name = br.readLine();

                try {
                    data = Integer.parseInt(br.readLine());
                } catch (NumberFormatException e) {
                    return 1;
                }
                int foundCards = data;

                try {
                    data = Integer.parseInt(br.readLine());
                } catch (NumberFormatException e) {
                    return 1;
                }
                int playX = data;

                try {
                    data = Integer.parseInt(br.readLine());
                } catch (NumberFormatException e) {
                    return 1;
                }
                int playY = data;

                try {
                    data = Integer.parseInt(br.readLine());
                } catch (NumberFormatException e) {
                    return 1;
                }
                int ItemNum = data;

                NEW_hraci.createPlayer(name, foundCards, new TreasureCard(ItemNum));
                NEW_hraci.getPlayer(i).setXY(playX, playY);
                //System.out.println("Player" + i + " added");
            }

            l = br.readLine() + "\n";
            m = MSGS[4];
            if (!l.equals(m)) {
                return 1;
            }

            //CARD PACK
            try {
                data = Integer.parseInt(br.readLine());
            } catch (NumberFormatException e) {
                return 1;
            }
            int maxSize = data;
            if (maxSize != 12 && maxSize != 24) {
                return 1;
            }

            for (i = 0; i < NEW_PlayerCount; i++) {
                int players_card = NEW_hraci.getPlayer(i).getMyCard().getItem();
                if (players_card > maxSize && players_card != 25 && players_card != 26) {
                    return 1;
                }
            }
            //skontrolovat ci playerova karta neni prilis velke cislo 

            try {
                data = Integer.parseInt(br.readLine());
            } catch (NumberFormatException e) {
                return 1;
            }
            if (data > maxSize) {
                return 1;
            }
            NEW_items = new CardPack(maxSize, data);

            //Po jednom vlozime itemy
            for (i = 0; i < maxSize; i++) {
                l = br.readLine();
                m = MSGS[5] + (i + 1);
                if (!l.equals(m)) {
                    return 1;
                }
                try {
                    data = Integer.parseInt(br.readLine());
                } catch (NumberFormatException e) {
                    return 1;
                }
                int item = data;
                if (data > maxSize) {
                    return 1;
                }

                NEW_items.balicek[i] = new TreasureCard(item);
                //System.out.println("item" + i + " added");
            }

            if (!MSGS[6].equals(br.readLine() + "\n")) {
                return 1;
            }

            try {
                data = Integer.parseInt(br.readLine());
            } catch (NumberFormatException e) {
                return 1;
            }
            int allBoard = data;
            if (allBoard < 5 || allBoard > 11) {
                return 1;
            }
            NEW_hraciaPlocha = MazeBoard.createMazeBoard(allBoard);

            //FREEFIELD
            if (!MSGS[7].equals(br.readLine() + "\n")) {
                return 1;
            }

            try {
                data = Integer.parseInt(br.readLine());
            } catch (NumberFormatException e) {
                return 1;
            }
            int item = data;

            try {
                data = Integer.parseInt(br.readLine());
            } catch (NumberFormatException e) {
                return 1;
            }
            int down = data;

            try {
                data = Integer.parseInt(br.readLine());
            } catch (NumberFormatException e) {
                return 1;
            }
            int left = data;

            try {
                data = Integer.parseInt(br.readLine());
            } catch (NumberFormatException e) {
                return 1;
            }
            int right = data;

            try {
                data = Integer.parseInt(br.readLine());
            } catch (NumberFormatException e) {
                return 1;
            }
            int up = data;

            if (!NEW_hraciaPlocha.setFreeField(item, down, left, right, up)) {
                return 1;
            }

            for (i = 1; i <= allBoard; i++) {
                for (int j = 1; j <= allBoard; j++) {
                    //FIELDS on board
                    l = br.readLine();
                    m = MSGS[8] + (i) + MSGS[9] + (j);
                    if (!l.equals(m)) {
                        return 1;
                    }

                    try {
                        data = Integer.parseInt(br.readLine());
                    } catch (NumberFormatException e) {
                        return 1;
                    }
                    item = data;

                    try {
                        data = Integer.parseInt(br.readLine());
                    } catch (NumberFormatException e) {
                        return 1;
                    }
                    down = data;

                    try {
                        data = Integer.parseInt(br.readLine());
                    } catch (NumberFormatException e) {
                        return 1;
                    }
                    left = data;

                    try {
                        data = Integer.parseInt(br.readLine());
                    } catch (NumberFormatException e) {
                        return 1;
                    }
                    right = data;

                    try {
                        data = Integer.parseInt(br.readLine());
                    } catch (NumberFormatException e) {
                        return 1;
                    }
                    up = data;

                    if (!NEW_hraciaPlocha.setField(i, j, item, down, left, right, up)) {
                        return 1;
                    }
                    //System.out.println("FIELD x=" + (i + 1) + " y=" + (j + 1) + " added");
                }
            }
            if (!MSGS[10].equals(br.readLine() + "\n")) {
                return 1;
            }
            //ak last je vecsie alebo cannot vecsie ako velkost hracej plochy return 1
            if (NEW_LAST.width > allBoard || NEW_LAST.height > allBoard) {
                return 1;
            }
            if (NEW_CANNOT.width > allBoard || NEW_CANNOT.height > allBoard) {
                return 1;
            }

            //Ak vsetko preslo v pohode tak mozme prepisat aktualnu hru
            LOG.setLogic(NEW_GAMESTATE, NEW_LAST, NEW_CANNOT, NEW_hraciaPlocha, NEW_items, NEW_hraci);

        } catch (FileNotFoundException ex) {
            //Logger.getLogger(Logic.class.getName()).log(Level.SEVERE, null, ex);
            return 1;
        } catch (IOException ex) {
            //Logger.getLogger(Logic.class.getName()).log(Level.SEVERE, null, ex);
            return 1;
        }

        return 0;
    }

    /**
     * Zisti ake su dostupne jazykove balicky
     * @return ArrayList of String vrati vsetky dostupne balicky zo zlozky
     */
    public static String[] getLangs() {
        ArrayList<String> langs = new ArrayList<>();

        File folder = new File("customLangs");
        File[] listOfFiles = folder.listFiles();
        langs.add("EN-ORIGINAL");

        if (folder.exists()) {
            for (File listOfFile : listOfFiles) {
                if (listOfFile.isFile()) {
                    langs.add(listOfFile.getName());
                }
            }
        }

        return langs.toArray(new String[langs.size()]);
    }

    /**
     * Nacitaj zvoleny jazyk
     * @param lang nazov jazykoveho balicku (subor)
     * @return String[] pole textov v danom jazyku
     */
    public static String[] loadLang(String lang) {
        String TXT[] = new String[100];
        TXT[99] = "EN-ORIGINAL";//INIT
        File file = new File("customLangs/" + lang);

        if (!file.exists() || !file.canRead()) {
            return TXT; // error --> load ORIGINAL
        }

        try {
            // OK MOZME CITAT
            BufferedReader br = new BufferedReader(new FileReader("customLangs/" + lang));

            TXT[99] = br.readLine(); // NAZOV LANGU

            TXT[0] = "<font size=\"5\" color=\"green\">" + br.readLine() + "</font><br>";
            TXT[1] = br.readLine();
            TXT[2] = br.readLine();
            TXT[3] = br.readLine();
            TXT[4] = br.readLine();
            TXT[5] = "<font color=\"green\">" + br.readLine() + "</font><br>";

            TXT[15] = br.readLine(); // ON TURN IS PLAYER:
            TXT[16] = br.readLine(); // NAME:
            TXT[17] = br.readLine(); //...FOUND ITEMS

            //EXIT? SURE?
            TXT[10] = "<html>" + br.readLine() + "</html>"; // EXIT? ARE YOU SURE?

            //HOW TO PLAY
            TXT[12] = "<html>" + br.readLine() + "</html>";//"<html>HRA SA HRA TAKTO A TAKTO A HENTKA<br>NO A TAK</html>";
            
            br.readLine();
            br.readLine();
            
            //TU BUTTONY A TEXTY HRY!
            for(int i = 20; i <= 71; i++){
                //System.out.println(i);
                TXT[i] = br.readLine();   
            }

        } catch (FileNotFoundException ex) {
            Logger.getLogger(GameIO.class.getName()).log(Level.SEVERE, null, ex);
            TXT[99] = "EN-ORIGINAL";
        } catch (IOException ex) {
            Logger.getLogger(GameIO.class.getName()).log(Level.SEVERE, null, ex);
            TXT[99] = "EN-ORIGINAL";
        }

        return TXT;// OK
    }

}
