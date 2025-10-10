#ifndef MOTION_MOVIE_H //HEADER GUARD, agar file tidak dimasukkan dua kali
#define MOTION_MOVIE_H 

#include <string>
#include <iostream>
#include <rapidjson/document.h> // untuk membaca dan memproses data JSON di memori
#include <rapidjson/istreamwrapper.h>

class MotionMovie {
private:
    int id;
    std::string name;
    int unit_id;

public:
    MotionMovie() : id(0), unit_id(0) {}
    /*nilai awal id 0
    nilai awal unit_id 0
    name {}*/

    // membuka dan membaca file JSON di [path]nya
    bool loadFromFile(const std::string& path);
    int getUnitId() const { return unit_id; }
    //menampilkan isi data motion_movie dari motion_movie.cpp (src)
    void printInfo() const;
};

#endif
