// /home/ed/bin/bin/g++ -std=c++1z -O3 -o queens3 queens3.cpp

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./queens3

/*
    Some more deep changes.

    Changes include:
    -Returning as early as possible. As soon as the queen can not be placed.
    -using std::make_pair (probably don't gain much here, but its' recommended)
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;

typedef std::pair<int, int> Point;

#define LARGEUR_GRILLE 22
vector<Point> positions;

bool verifierNonPrise(const Point& emplacement) {
    for (const auto& p : positions) {
        if (p.first != emplacement.first) {
            if (p.second == emplacement.second) {
                return false;
            }
            if (abs(p.second - emplacement.second) ==
                abs(p.first - emplacement.first)) {
                return false;
            }
        }
    }

    return true;
}

bool placerDame(int i) {
    for (int j = 1; j <= LARGEUR_GRILLE; j++) {
        auto emplacement = std::make_pair(i, j);
        positions.push_back(emplacement);
        if (verifierNonPrise(emplacement) &&
            (i == LARGEUR_GRILLE || placerDame(i + 1))) {
            return true;
        } else {
            positions.pop_back();
        }
    }

    return false;
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
