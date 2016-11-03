package ija.Panels;

import ija.ui.Images;
import ija.board.*;
import ija.treasure.*;
import static ija.Labyrinth.LOG;
import java.awt.Insets;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Image;
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
public class BoardPanel extends JPanel implements ActionListener {
    private int size;
    private JButton button;
    int width, height;

    /**
     * Konstruktor pre triedu BoardPanel
     */
    public BoardPanel() {
        size = 0;
        setLayout(null);
    }

    /**
     * Funkcia na vykreslovanie hracej plochy
     */
    public void draw() {
        removeAll();
        size = LOG.showBoard().getN();

        //Tui TxtUI = new Tui(LOG.showBoard());
        //TxtUI.showBoard();
        setLayout(new GridLayout(size, size, 0, 0));

        //hracia plocha sa rozdeli na sizexsize velku mriezku
        //pre kazdy prvok sa vygeneruje policko
        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                //Vyberieme data z hracej plochy
                MazeField field = LOG.showBoard().get(i, j);
                //vygenerujeme prislusny obrazok
                Image img = Images.resize(LOG.getImages().getMazeCardImg(field.getCard()), 660/size, 660/size);

                //vytvorenie noveho policka
                button = new JButton();
              
                button.addActionListener(this);
                add(button);
                button.setIcon(new ImageIcon(img));
                button.setMargin(new Insets(0,0,0,0));
                button.setLayout(new GridLayout(3, 3, 0, 0));
                
                TreasureCard tr = field.getTreasure();
                
                
                JLabel ButtonLabels[][] = createLabels();//new JLabel[3][3];
                
                //Pridame treasure
                if (tr != null) {
                    Image item = LOG.getImages().getTreasureImg(tr);
                    ImageIcon icon = new ImageIcon(Images.resize(item, (660/size/3 -3 ), (660/size/3 -3)));
                    ButtonLabels[1][1].setIcon(icon);
                }
                
                //Pridame hracov
                for( int player = 0 ; player < LOG.getPlayers().getCount(); player++){

                    if( LOG.getOnePlayer(player).getX() == i && LOG.getOnePlayer(player).getY() == j){
                        Image item = LOG.getImages().getPlayerImg(player);
                        ImageIcon icon = new ImageIcon(Images.resize(item, (660/size/3 -3), (660/size/3 -3)));                    
                        //vykreslovanie hracov na tahu plus hracov ostatnych na rovnakom policku
                        if( ButtonLabels[0][0].getIcon() == null){
                            ButtonLabels[0][0].setIcon(icon);
                            if(LOG.getOnePlayer(player) == LOG.getPlayers().getPlayerOnTurn() ){
                                ButtonLabels[0][0].setBackground(Color.BLACK);
                                ButtonLabels[0][0].setOpaque(true);
                            }
                        }
                        else if( ButtonLabels[0][2].getIcon() == null){
                            ButtonLabels[0][2].setIcon(icon);
                            if(LOG.getOnePlayer(player) == LOG.getPlayers().getPlayerOnTurn() ){
                                ButtonLabels[0][2].setBackground(Color.BLACK);
                                ButtonLabels[0][2].setOpaque(true);
                            }
                        }
                        else if( ButtonLabels[2][0].getIcon() == null){
                            ButtonLabels[2][0].setIcon(icon);
                            if(LOG.getOnePlayer(player) == LOG.getPlayers().getPlayerOnTurn() ){
                                ButtonLabels[2][0].setBackground(Color.BLACK);
                                ButtonLabels[2][0].setOpaque(true);
                            }
                        }
                        else{
                            ButtonLabels[2][2].setIcon(icon);
                            if(LOG.getOnePlayer(player) == LOG.getPlayers().getPlayerOnTurn() ){
                                ButtonLabels[2][2].setBackground(Color.BLACK);
                                ButtonLabels[2][2].setOpaque(true);
                            }
                        }
                    }
                    
                }
                
                for(int x = 0; x < 3; x++){
                    for(int y = 0; y < 3; y++ ){
                        button.add(ButtonLabels[x][y]);
                    }
                }

            }
        }

    }
    
    /**
     * Vytvori prazdne policka pre vyplnenie mriezky na polickach
     *
     * @return labels prazdne jlabels v mriezke 9x9
     */
    private JLabel[][] createLabels(){
        JLabel[][] labels = new JLabel[3][3];
        for ( int i = 0; i < 3; i++){
            for( int j = 0; j< 3; j++){
                labels[i][j]=new JLabel();
            }
        }
        return labels;
    }
    
    /**
     * Pri vykonani akcie, vratime poziciu kliknuteho policka
     * @param e Event Listener
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        //System.out.println("Klik");

        JButton button = (JButton) e.getSource();
        Dimension size = button.getSize();

        //System.out.println("size " + size);

        int buttonX = button.getX();
        int buttonY = button.getY();
        int buttonPosX = buttonX / size.width;
        int buttonPosY = buttonY / size.height;

        //System.out.println("KLIK x = " + (buttonPosY+1) + "y= " + (buttonPosX+1) );
        
        /**
         * kliknutie na boardku, button cislo 13
         */
        LOG.GameProcess(13,buttonPosY + 1, buttonPosX + 1);

        LOG.windowGui.drawAll();

    }

}