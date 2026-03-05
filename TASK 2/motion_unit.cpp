#include "motion_unit.h"
#include <fstream>
#include <iostream>

using namespace std;
namespace rj = rapidjson;

bool MotionUnit::loadFromFile(const string& path) {
    ifstream ifs(path);////Membuka file berdasarkan path (lokasi file JSON)
    //Mengecek apakah file berhasil dibuka
    if (!ifs.is_open()) {
        cout << "Gagal membuka file unit: " << path << endl;
        return false;
    }

    rj::IStreamWrapper isw(ifs);
    rj::Document doc;
    doc.ParseStream(isw);

    if (doc.HasParseError()) {
        cout << "JSON parse error di file unit." << endl;
        return false;
    }

    // Ambil properti dasar
    id = doc["id"].GetInt(); // Mengambil nilai integer dari key "id"
    name = doc["name"].GetString(); //Mengambil nilai string dari key "name".
    total_frame = doc["total_frame"].GetInt(); // Mengambil jumlah frame dari key "total_frame".

    // Ambil array waktu
    const auto& timeArr = doc["time"].GetArray();
    for (auto& t : timeArr) {
        time.push_back(t.GetInt());
    }

    // Ambil motion_frame (array 2D)
    const auto& framesArr = doc["motion_frame"].GetArray();
    for (auto& frameSet : framesArr) {
        vector<int> row;
        for (auto& val : frameSet.GetArray()) {
            row.push_back(val.GetInt());
        }
        motion_frames.push_back(row);
    }

    return true;
}

void MotionUnit::printFrames() const {
    cout << "Unit ID: " << id << endl;
    cout << "Nama Unit: " << name << endl;
    cout << "Total Frame: " << total_frame << endl;

    cout << "Time: ";
    for (auto t : time) cout << t << " ";
    cout << endl;

    cout << "\nMotion Frames:" << endl;
    for (size_t i = 0; i < motion_frames.size(); ++i) {
        cout << "  Frame " << i + 1 << " -> ";
        for (auto val : motion_frames[i]) {
            cout << val << " ";
        }
        cout << endl;
    }
}
