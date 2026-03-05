#ifndef MOTION_UNIT_H
#define MOTION_UNIT_H

#include <string>
#include <vector>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

class MotionUnit {
private:
    int id;
    std::string name;
    int total_frame;
    std::vector<int> time;                          // array waktu
    std::vector<std::vector<int>> motion_frames;    // array 2D dari motion_frame

public:
// membuka dan membaca file JSON di [path]nya
    bool loadFromFile(const std::string& path);
    void printFrames() const; // untuk menampilkan hasil parsing ke terminal

    int getId() const { return id; }
};

#endif //HEADER GUARD, agar file tidak dimasukkan dua kali
