#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Robot.hpp>
#include <json/json.h>

namespace wb = webots;

// Membaca file JSON pose
std::vector<std::vector<double>> loadPoseFile(const std::string &path) {
    std::ifstream file(path);
    std::vector<std::vector<double>> poses;

    if (!file.is_open()) {
        std::cerr << "[ERROR] Tidak bisa membuka file pose: " << path << std::endl;
        return poses;
    }

    Json::Value root; //Membuat objek root dari library JSON untuk menyimpan isi file
    file >> root; // Membaca isi file JSON ke root

    //Mengambil bagian "pose_group" dari JSON (array grup pose)
    const Json::Value &groups = root["pose_group"];
    for (const auto &g : groups) {
        const Json::Value &poseList = g["pose"];
        for (const auto &p : poseList) {
            std::vector<double> pos; //Buat vector pos untuk satu pose.
            const Json::Value &arr = p["posisi"]; // Ambil array "posisi" dari pose p.
            for (const auto &val : arr)
                pos.push_back(val.asDouble()); // Loop untuk setiap nilai val di array, konversi ke double, dan tambahkan ke pos.
            if (!pos.empty()) poses.push_back(pos);
        }
    }

    std::cout << "[INFO] Total pose terbaca dari " << path << ": " << poses.size() << std::endl;
    return poses;
}

// Menjalankan urutan pose
void executePoseSequence(wb::Robot *robot, std::map<std::string, wb::Motor *> &motors,
                         const std::vector<std::vector<double>> &poses,
                         const std::vector<std::string> &motorNames, int step) {
    for (const auto &frame : poses) {
        if (frame.size() != motorNames.size()) {
            std::cerr << "[WARN] Ukuran posisi tidak cocok dengan jumlah motor!" << std::endl;
            continue;
        }

        for (size_t i = 0; i < motorNames.size(); ++i)
            motors[motorNames[i]]->setPosition(frame[i]);

        robot->step(step * 8);
    }
}

int main(int argc, char **argv) {
    wb::Robot *robot = new wb::Robot();
    const int timeStep = static_cast<int>(robot->getBasicTimeStep());

    std::map<std::string, wb::Motor *> motors;
    std::map<std::string, wb::PositionSensor *> positionSensors;

    const char *motorNamesArray[] = {
        "Head", "Neck",
        "ShoulderL", "ShoulderR",
        "ArmUpperL", "ArmUpperR",
        "ArmLowerL", "ArmLowerR",
        "PelvYL", "PelvYR",
        "PelvL", "PelvR",
        "LegUpperL", "LegUpperR",
        "LegLowerL", "LegLowerR",
        "AnkleL", "AnkleR",
        "FootL", "FootR"
    };

    std::vector<std::string> motorNames(std::begin(motorNamesArray), std::end(motorNamesArray));

    // Inisialisasi motor dan sensor
    for (const auto &name : motorNames) {
        wb::Motor *motor = robot->getMotor(name);
        if (motor) {
            motors[name] = motor;
            std::string sensorName = name + std::string("S");
            wb::PositionSensor *sensor = robot->getPositionSensor(sensorName);
            if (sensor) {
                sensor->enable(timeStep);
                positionSensors[name] = sensor;
            }
        }
    }

    std::cout << "[INFO] Semua motor berhasil diinisialisasi.\n";

    // === Posisi awal: berdiri stabil ===
    std::string standPosePath = "C:/Users/asusv/OneDrive/Dokumen/UKM ROBOTIK/VI-ROSE/FinalProjek/Webots/poses/pose-berdiri.json";

    std::vector<std::vector<double>> standPose = loadPoseFile(standPosePath);
    if (!standPose.empty()) {
        executePoseSequence(robot, motors, standPose, motorNames, timeStep);
        std::cout << "[INFO] Robot dalam posisi awal: BERDIRI STABIL.\n";
    } else {
        std::cout << "[WARN] File pose berdiri tidak ditemukan.\n";
    }

    // === Loop utama: kontrol interaktif ===
    while (robot->step(timeStep) != -1) {
        if (!std::cin.rdbuf()->in_avail())
            continue;

        char key;
        std::cin >> key;
        std::string filePath;

        switch (key) {
            case 'W': filePath = "C:/Users/asusv/OneDrive/Dokumen/UKM ROBOTIK/VI-ROSE/FinalProjek/Webots/poses/pose-jalan-maju.json"; break;
            case 'S': filePath = "C:/Users/asusv/OneDrive/Dokumen/UKM ROBOTIK/VI-ROSE/FinalProjek/Webots/poses/pose-jalan-mundur.json"; break;
            case 'A': filePath = "C:/Users/asusv/OneDrive/Dokumen/UKM ROBOTIK/VI-ROSE/FinalProjek/Webots/poses/pose-geser-kiri.json"; break;
            case 'D': filePath = "C:/Users/asusv/OneDrive/Dokumen/UKM ROBOTIK/VI-ROSE/FinalProjek/Webots/poses/pose-geser-kanan.json"; break;
            case 'Q': filePath = "C:/Users/asusv/OneDrive/Dokumen/UKM ROBOTIK/VI-ROSE/FinalProjek/Webots/poses/pose-belok-kiri.json"; break;
            case 'E': filePath = "C:/Users/asusv/OneDrive/Dokumen/UKM ROBOTIK/VI-ROSE/FinalProjek/Webots/poses/pose-belok-kanan.json"; break;
            case 'Z': filePath = "C:/Users/asusv/OneDrive/Dokumen/UKM ROBOTIK/VI-ROSE/FinalProjek/Webots/poses/pose-berdiri.json"; break;
            case 'C': filePath = "C:/Users/asusv/OneDrive/Dokumen/UKM ROBOTIK/VI-ROSE/FinalProjek/Webots/poses/pose-jongkok.json"; break;
            default:
                std::cout << "[INFO] Tombol tidak dikenali, abaikan.\n";
                continue;
        }

        std::cout << "[CMD] Menjalankan: " << filePath << std::endl;
        std::vector<std::vector<double>> poseData = loadPoseFile(filePath);
        if (!poseData.empty()) {
            executePoseSequence(robot, motors, poseData, motorNames, timeStep);
        } else {
            std::cout << "[WARN] Pose tidak valid.\n";
        }
    }
}