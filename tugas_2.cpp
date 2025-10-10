#include <iostream>
#include "motion_movie.h"
#include "motion_unit.h"
using namespace std;

int main() {
    //MASUKKAN ID MOVIE
    int movieId;
    cout << "Masukkan ID movie: ";
    cin >> movieId;

    //MEMBACA FILE MOTION MOVIE
    string moviePath = "../../XL/XL/motion_movie/" + to_string(movieId) + ".json";
    MotionMovie movie;
    if (!movie.loadFromFile(moviePath)) {
        return 1;
    }

    movie.printInfo();

    //MEMBACA FILE MOTION UNIT SESUAI UNIT ID YANG DIRUJUK 
    string unitPath  = "../../XL/XL/motion_unit/" + to_string(movie.getUnitId()) + ".json";
    MotionUnit unit;
    if (!unit.loadFromFile(unitPath)) {
        return 1;
    }

    unit.printFrames();

    return 0;
}
