// /home/ed/bin/bin/g++ -std=c++11 -O3 -o queens2 queens2.cpp

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./queens2

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;

typedef std::pair<int, int> Point;

#define LARGEUR_GRILLE 22
vector<Point> positions;

bool verifierNonPrise(const Point& emplacement) {
    bool nonPrise = true;
    for (const auto& p : positions) {
        if (p.first != emplacement.first) {
            if (p.second == emplacement.second) {
                nonPrise = false;
            }
            if (abs(p.second - emplacement.second) ==
                abs(p.first - emplacement.first)) {
                nonPrise = false;
            }
        }
    }

    return nonPrise;
}

bool placerDame(int i) {
    bool bonnePosition = false;
    for (int j = 1; j <= LARGEUR_GRILLE && !bonnePosition; j++) {
        Point emplacement(i, j);
        positions.emplace_back(emplacement);
        if (verifierNonPrise(emplacement) &&
            (i == LARGEUR_GRILLE || placerDame(i + 1))) {
            bonnePosition = true;
        } else {
            positions.pop_back();
        }
    }

    return bonnePosition;
}

int main(int argc, char* argv[]) {
    std::chrono::system_clock::time_point begin_time =
        std::chrono::system_clock::now();

    positions.reserve(LARGEUR_GRILLE);

    placerDame(1);
    for (const auto& p : positions) {
        cout << "(" << p.second << "; " << p.second << ")" << endl;
    }

    std::chrono::system_clock::time_point end_time =
        std::chrono::system_clock::now();
    long long elapsed_milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            end_time - begin_time).count();
    std::cout << "Duration (milliseconds): " << elapsed_milliseconds
              << std::endl;

    return 0;
}
