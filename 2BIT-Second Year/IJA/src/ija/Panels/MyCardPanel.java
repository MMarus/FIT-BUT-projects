package ija.Panels;

import static ija.Labyrinth.LOG;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JPanel;

/**
 * panel GUI, kde sa vykresluje aktualna karta hraca + event listenery
 *
 * @author xjezov01
 * @author xmarus05
 */
public class MyCardPanel extends JPanel implements ActionListener {

    private JButton button;

    /**
     * Konstruktor pre triedu MyCardPanel - nastavi NULL layout
     */
    public MyCardPanel() {
        setLayout(null);

    }

    /**
     * Prekresluje tento panel, nacitava hracovu kartu
     */
    public void draw() {
        removeAll();

        setLayout(new GridLayout(1, 1, 0, 0));

        button = new JButton();
        button.addActionListener(this);
        add(button);
        
        // nastavime novy obrazok karty
        ImageIcon icon = new ImageIcon(LOG.getImages().getTreasureImg(LOG.getPlayer().getMyCard()));

        button.setIcon(new ImageIcon(icon.getImage()));

    }

    /**
     * Pri kliknuti na kartu posiela logike signal 11, prekresli cely GAME Panel
     * @param e Action listener
     */
    @Override
    public void actionPerformed(ActionEvent e) {

        //kliknutie na kartu, button cislo 11
        LOG.GameProcess(11, 0, 0);
        //prekresli cely GAME Panel
        LOG.windowGui.drawAll();
    }
}