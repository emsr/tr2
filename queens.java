// /usr/java/jdk1.6.0_26/bin/javac -encoding ISO-8859-1 queens.java

import java.awt.Point;
import java.util.ArrayList;
import java.util.List;

public class HuitDames {

/**
 * La liste des coordnnées des dames.
 */
private static List<Point> positions = new ArrayList<>();

/**
 * Largeur de la grille.
 */
private static final int LARGEUR_GRILLE = 22; 


/**
 * @param args the command line arguments
 */
public static void main(String[] args) {
    int i = 1;
    placerDame(i);
    for (Point point : positions) {
        System.out.println("(" + point.x + "; " + point.y + ")");
    }
}

/**
 * Place une dame et return true si la position est bonne.
 * @param i le numéro de la dame.
 * @return si la position est bonne.
 */
private static boolean placerDame(int i) {

    boolean bonnePosition = false;
    for (int j = 1; j <= LARGEUR_GRILLE && bonnePosition == false; j++) {
        Point emplacement = new Point(i, j);
        positions.add(emplacement);
        if (verifierPrise(emplacement) && (i == LARGEUR_GRILLE || placerDame(i + 1))) {
            bonnePosition = true;
        }
        else {
            positions.remove(i - 1);
        }
    }

    return bonnePosition;
}

/**
 * Vérifie que la nouvelle position n'est pas en prise avec une position déjà présente.
 * @param position la position de la nouvelle dame.
 * @return Si la position convient par rapport aux positions des autres dames.
 */
private static boolean verifierPrise(Point position) {
    boolean nonPrise = true;
    for (Point point : positions) {
        if (!point.equals(position)) {
            // Cas où sur la même colonne.
            if (position.y == point.y) {
                nonPrise = false;
            }
            // Cas où sur même diagonale.
            if (Math.abs(position.y - point.y) == Math.abs(position.x - point.x)) {
                nonPrise = false;
            }
        }
    }

    return nonPrise;
}
