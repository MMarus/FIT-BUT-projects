package ija.Panels;

import static ija.Labyrinth.LOG;
import java.awt.GridLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;


/**
 * panel GUI, kde sa vypisuje stav hry
 * 
 * @author xjezov01
 * @author xmarus05
 */
public class StatsPanel extends JPanel {

    private JLabel statDATA;

    /**
     * Konstruktor pre triedu StatsPanel - nastavi NULL layout
     */
    public StatsPanel() {
        setLayout(null);

    }

    /**
     * Prekresluje tento panel, ziska nove statistiky
     */
    public void draw() {
        removeAll();

        setLayout(new GridLayout(1, 1, 0, 0));
        
        // SET STATS MESAGE
        String TXT;
        // hrac ktory je na tahu
        TXT = "<html><pre><font size=\"4\">\t\t" + LOG.windowGui.gameTXT[15] + " ";
        TXT += LOG.getPlayer().getName();
        
        // staty vsetkych hracov
        for (int i = 0; i < LOG.getPlayers().getCount(); i++) {

            TXT += "<br>\t\t" + LOG.windowGui.gameTXT[16] + " " + LOG.getOnePlayer(i).getName() + " " + LOG.windowGui.gameTXT[17] + " " + LOG.getOnePlayer(i).getFoundCards();
        }
        TXT += "</font>";
        
        // co ma v hre nasledovat
        TXT += "<br><font size=\"4\" face=\"verdana\" color=\"red\">" + LOG.windowGui.gameTXT[LOG.getGameState()] + "</font>";
        TXT += "</pre></html>";

        statDATA = new JLabel(TXT);
        add(statDATA);

        statDATA.setFont(new java.awt.Font("Tahoma", 0, 15));
    }
}
