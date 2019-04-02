// $HOME/bin/bin/g++ -O3 -o queens1 queens1.cpp

// LD_LIBRARY_PATH=$HOME/bin/lib64:$LD_LIBRARY_PATH ./queens1

#include <iostream>
#include <list>
#include <math.h>
#include <stdlib.h>

using namespace std;


// Class to represent points.
class Point {
private:
    double xval, yval;
public:
    // Constructor uses default arguments to allow calling with zero, one,
    // or two values.
    Point(double x = 0.0, double y = 0.0) {
            xval = x;
            yval = y;
    }

    // Extractors.
    double x() { return xval; }
    double y() { return yval; }

};

#define LARGEUR_GRILLE 22
list<Point> positions;


bool verifierNonPrise(Point emplacement) {
    bool nonPrise = true;
    for (list<Point>::iterator it = positions.begin(); it!= positions.end(); it++) {
        if (it->x() != emplacement.x()) {
            if (it->y() == emplacement.y()) {
                nonPrise = false;
            }
            if (abs(it->y() - emplacement.y()) == abs(it->x() - emplacement.x())) {
                nonPrise = false;
            }
        }
    }

    return nonPrise;
}

bool placerDame(int i) {
    bool bonnePosition = false;
for (int j = 1; j <= LARGEUR_GRILLE && !bonnePosition; j++) {
    Point emplacement(i,j);
    positions.push_back(emplacement);
    if (verifierNonPrise(emplacement) && (i == LARGEUR_GRILLE || placerDame(i + 1))) {
        bonnePosition = true;
    }
    else {
        positions.pop_back();
    }
}

return bonnePosition;
}

int main()
{
    int i = 1;
    placerDame(i);
    for (list<Point>::iterator it = positions.begin(); it!= positions.end(); it++) {
        cout << "(" << it->x() << "; " << it->y() << ")" << endl;
    }
    return 0;
}
