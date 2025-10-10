#include "motion_movie.h"
#include <fstream>

using namespace std;
namespace rj = rapidjson;

bool MotionMovie::loadFromFile(const string& path) {
    ifstream ifs(path); //Membuka file berdasarkan path (lokasi file JSON)

    //Mengecek apakah file berhasil dibuka
    if (!ifs.is_open()) {
        cout << "Gagal membuka file movie: " << path << endl;
        return false;
    }

    rj::IStreamWrapper isw(ifs);
    rj::Document doc;
    doc.ParseStream(isw);

    if (doc.HasParseError()) {
        cout << "JSON parse error di file movie." << endl;
        return false;
    }

    id = doc["id"].GetInt(); // Ambil nilai dari key "id" → simpan ke variabel id
    name = doc["name"].GetString(); // Ambil nilai dari key "name" → simpan ke variabel name

    if (doc.HasMember("motion_unit") && doc["motion_unit"].IsArray() && doc["motion_unit"].Size() > 0) {
        const rj::Value& firstUnit = doc["motion_unit"][0];
        if (firstUnit.HasMember("id") && firstUnit["id"].IsInt()) {
            unit_id = firstUnit["id"].GetInt();
        } else {
            unit_id = 0;
        }
    } else {
        unit_id = 0;
    }

    return true;
}

//menampilkan isi data motion_movie 
void MotionMovie::printInfo() const {
    cout << "Movie ID: " << id << endl;
    cout << "Nama Movie: " << name << endl;
    cout << "Unit ID: " << unit_id << endl;
}
