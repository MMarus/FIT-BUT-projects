package ija.ui;

import ija.board.MazeCard;
import ija.treasure.TreasureCard;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;
import javax.swing.ImageIcon;

/**
 * Trieda uchovavajuca vsetky obrazky
 * 
 * @author xjezov01
 * @author marek
 */
public class Images {
    Image items[];
    Image rocks[];
    Image players[];
    
    
    /**
     * Konstruktor nacita vsetky obrazky do premennych
     */
    public Images() {
        //for a nacitaj obrazecky
        int i;
        ImageIcon icon;
        rocks = new Image[5];
        items = new Image[40];
        players = new Image[4];
        
        for( i = 0; i < 4; i++){
            icon = new ImageIcon(getClass().getResource("/resources/images/rocks/rock"+i+".png"));           
            rocks[i] = icon.getImage();
        }
        
        
        for( i = 0; i <= 26; i++){
            icon = new ImageIcon(getClass().getResource("/resources/images/items/"+i+".png"));            
            items[i] = icon.getImage();
        }
        
        for( i = 0; i < 4; i++){
            icon = new ImageIcon(getClass().getResource("/resources/images/players/player"+i+".png"));            
            players[i] = icon.getImage();
        }
        
    }

    /**
     * Vrati obrazok pokladu
     * 
     * @param item TreasureCard hracia karta
     * @return Image obrazok pokladu
     */
        public Image getTreasureImg( TreasureCard item) {
        return items[item.getItem()];
    }
    
    /**
     * Vrati Obrazok hraca cislo n
     * 
     * @param n cislo hraca 
     * @return Image obrazok
     */
    public Image getPlayerImg(int n) {
        return players[n];
    }

    /**
     * Vrati Obrazok danej kary
     * 
     * @param card MazeCard karta 
     * @return Image Obrazok danej kary
     */
        public Image getMazeCardImg(MazeCard card) {
        //GOD_MODE KAMEN
        if( card.up != null && card.down != null && card.left != null && card.right != null)
            return rocks[3];
        if (card.up != null) {
            if (card.down != null) {
                if (card.left != null) {
                    // hore dole dolava
                    return Images.rotate(rocks[1], -90);
                }

                if (card.right != null) {
                    // hore dole doprava
                    return Images.rotate(rocks[1], 90);
                }

                // hore dole
                return Images.rotate(rocks[2], 0);
            }

            if (card.left != null) {
                if (card.right != null) {
                    //hore dolava doprava
                    return Images.rotate(rocks[1], 0);
                }

                //hore dolava
                return Images.rotate(rocks[0], 90);
            }

            if (card.right != null) {
                //hore doprava
                return Images.rotate(rocks[0], 180);
            }
        }

        if (card.down != null) {
            if (card.left != null) {
                if (card.right != null) {
                    // dole dolava doprava
                    return Images.rotate(rocks[1], 180);
                }

                // dole dolava
                return Images.rotate(rocks[0], 0);
            }

            if (card.right != null) {
                // dole doprava
                return Images.rotate(rocks[0], -90);
            }
        }

        if (card.left != null && card.right != null) {
            // dolava doprava
            return Images.rotate(rocks[2], 90);
        }

        //TODO: ERR zavolaj exception
        return Images.rotate(rocks[2], 90);
    }
    
    
    
    
    /**
     * Converts a given Image into a BufferedImage
     *
     * @param img The Image to be converted
     * @return The converted BufferedImage
     */
    public static BufferedImage toBufferedImage(Image img) {
        if (img instanceof BufferedImage) {
            return (BufferedImage) img;
        }
        // Create a buffered image with transparency
        BufferedImage bimage = new BufferedImage(img.getWidth(null), img.getHeight(null), BufferedImage.TYPE_INT_ARGB);
        // Draw the image on to the buffered image
        Graphics2D bGr = bimage.createGraphics();
        bGr.drawImage(img, 0, 0, null);
        bGr.dispose();
        // Return the buffered image
        return bimage;
    }

    /**
     * Converts a given BufferedImage into an Image
     *
     * @param bimage The BufferedImage to be converted
     * @return The converted Image
     */
    public static Image toImage(BufferedImage bimage) {
        // Casting is enough to convert from BufferedImage to Image
        Image img = (Image) bimage;
        return img;
    }

    /**
     * Creates an empty image with transparency
     *
     * @param width The width of required image
     * @param height The height of required image
     * @return The created image
     */
    public static Image getEmptyImage(int width, int height) {
        BufferedImage img = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
        return toImage(img);
    }

    /**
     * Rotates an image. Actually rotates a new copy of the image.
     *
     * @param img The image to be rotated
     * @param angle The angle in degrees
     * @return The rotated image
     */
    public static Image rotate(Image img, double angle) {
        double sin = Math.abs(Math.sin(Math.toRadians(angle))), cos = Math.abs(Math.cos(Math.toRadians(angle)));
        int w = img.getWidth(null), h = img.getHeight(null);
        int neww = (int) Math.floor(w * cos + h * sin), newh = (int) Math.floor(h
                * cos + w * sin);
        BufferedImage bimg = toBufferedImage(getEmptyImage(neww, newh));
        Graphics2D g = bimg.createGraphics();
        g.translate((neww - w) / 2, (newh - h) / 2);
        g.rotate(Math.toRadians(angle), w / 2, h / 2);
        g.drawRenderedImage(toBufferedImage(img), null);
        g.dispose();
        return toImage(bimg);
    }

    /**
     * Resizes a given image to given width and height
     *
     * @param img The image to be resized
     * @param width The new width
     * @param height The new height
     * @return The resized image
     */
    public static Image resize(Image img, int width, int height) {
        // Create a null image
        Image image = null;
        // Resize into a BufferedImage
        BufferedImage bimg = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
        Graphics2D bGr = bimg.createGraphics();
        bGr.drawImage(img, 0, 0, width, height, null);
        bGr.dispose();
        // Convert to Image and return it
        image = toImage(bimg);
        return image;
    }

}
