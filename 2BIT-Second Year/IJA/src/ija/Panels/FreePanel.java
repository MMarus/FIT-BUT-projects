package ija.Panels;

import ija.ui.Images;
import static ija.Labyrinth.LOG;
import ija.treasure.TreasureCard;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;

/**
 * Panel, ktory zobrazuje celu hraciu plochu
 *
 * @author xjezov01
 * @author xmarus05
 */
public class FreePanel extends JPanel implements ActionListener {

    private JButton FREE;

    /**
     * Konstruktor, nastavi layout na null
     */
    public FreePanel() {
        setLayout(null);
    }

    /**
     * Funkcia na vykreslenie panelu
     */
    public void draw() {
        removeAll();

        setLayout(new GridLayout(1, 1, 0, 0));

        //Vytvorime novy button pre free kamen
        //pri kliknuti na button sa kamen natoci 
        FREE = new JButton();
        FREE.addActionListener(this);
        add(FREE);
        FREE.setMargin(new Insets(0, 0, 0, 0));
        FREE.setLayout(new GridLayout(3, 3, 0, 0));
        JLabel ButtonLabels[][] = createLabels();

        //vybereme obrazok, kt sedi s danou kartou
        ImageIcon icon = new ImageIcon(LOG.getImages().getMazeCardImg(LOG.showBoard().getFreeField().getCard()));

        FREE.setIcon(new ImageIcon(icon.getImage()));

        //Ak je nejaky item na policku, zobrazime ho
        TreasureCard tr = LOG.showBoard().getFreeField().getTreasure();
        if (tr != null) {
            Image item = LOG.getImages().getTreasureImg(tr);
            icon = new ImageIcon(Images.resize(item, 200 / 3, 200 / 3));
            ButtonLabels[1][1].setIcon(icon);
        }

        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                FREE.add(ButtonLabels[x][y]);
            }
        }
    }

    /**
     * Vytvori prazdne policka pre vyplnenie mriezky na polickach
     *
     * @return labels prazdne jlabels v mriezke 9x9
     */
    private JLabel[][] createLabels() {
        JLabel[][] labels = new JLabel[3][3];
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                labels[i][j] = new JLabel();
            }
        }
        return labels;
    }

    /**
     * pri kliknuti na button sa kamen natoci
     *
     * @param e Event listener
     */
    public void actionPerformed(ActionEvent e) {
        /**
         * kliknutie na free kamen, button cislo 12 ( free kamen)
         */
        //Volame logiku s dotazom na otocenie freee kamenu
        LOG.GameProcess(12, 0, 0);

        removeAll();
        LOG.windowGui.drawAll();
    }
}