# TASK 4 - CROSS PLATFORM FILE TRANSFER </pre>
## DESKRIPSI </pre>
TASK 4 - CROSS PLATFORM FILE TRANSFER merupakan sistem pengiriman file .json dengan alur sebagai berikut: </pre>

laptop: </pre>
1. Buat file sesuai ketentuan. </pre>
2. Pecah file tersebut menjadi beberapa bagian kecil . </pre>
3. Kirim tiap bagian ke esp-bridge melalui komunikasi serial. </pre>

esp-bridge: </pre>
1. Terima data dari laptop via serial. </pre>
2. Teruskan data ke esp-receiver menggunakan ESP-NOW. </pre>

esp-receiver: </pre>
1. Terima semua potongan data dari esp-bridge. </pre>
2. Susun kembali potongan-potongan tersebut menjadi file utuh. </pre>
3. Simpan hasilnya ke SPIFFS . </pre>
4. Setelah selesai, tampilkan hasil akhir di serial monitor sesuai format yang ditentukan. </pre>
## STRUKTUR FOLDER </pre>
<pre>TASK 4/
├──build/ 
├──data/ 
│  ├── halo.json
├──lib/ 
│  ├── serial/
├──src/
│  ├── main.cpp
|
├──CMakeLists.txt
|
├──ESP/ 
  ├──── ESP BRIDGE/
  |    ├── Bridge/
  |       ├── .pio/
  |        ├── .vscode/
  |        ├── include/
  |        ├── lib/
  |        ├── test/
  |        ├── src/
  |            ├── main.cpp/
  |        ├── .gitinore
  |        ├── platformio
  |
  ├──── ESP RECEIVER/
      ├── Receiver/
          ├── .pio/
          ├── .vscode/
          ├── include/
          ├── lib/
          ├── test/
          ├── src/
               ├── main.cpp/
          ├── .gitinore
          ├── platformio </pre>

#### CMakeLists.txt </pre>
<pre># Menentukan versi minimum CMake yang diperlukan  cmake_minimum_required(VERSION 3.10)
project(FileTransferLaptop)
 
set(CMAKE_CXX_STANDARD 17)

# Tambahkan include dan library path untuk serial
include_directories("C:/Users/asusv/serial/include")
link_directories("C:/Users/asusv/serial/build/Release")

add_executable(FileTransferLaptop src/main.cpp)

# Menautkan link library "serial" ke executable "FileTransferLaptop"
target_link_libraries(FileTransferLaptop serial)</pre>
## LAPTOP </pre>
#### Main.cpp </pre>

<pre>
#include <iostream>
#include <fstream>
#include <vector>
#include <serial/serial.h>  
#include <thread>
#include <chrono>

using namespace std;

//membuka dan membaca file dalam mode biner
vector<uint8_t> readFileBinary(const string &filename) {
    ifstream file(filename, ios::binary);
    vector<uint8_t> buffer((istreambuf_iterator<char>(file)), {});
    return buffer;
}

//memecah data menjadi potongan-potongan kecil
vector<vector<uint8_t>> splitData(const vector<uint8_t> &data, size_t chunkSize) {
    vector<vector<uint8_t>> chunks; //chunks -> vektor untuk menyimpan potongan-potongan file .json
    for (size_t i = 0; i < data.size(); i += chunkSize) {
        //Menghitung indeks akhir dari potongan agar tidak overflow
        size_t end = min(i + chunkSize, data.size());
        //Memasukkan potongan data ke dalam chunks
        chunks.emplace_back(data.begin() + i, data.begin() + end);
    }
    return chunks;
}

int main() {
    string filePath = "../../../data/halo.json";  // path file JSON
    string port = "COM7";                   
    unsigned long baud = 115200;

    try {
        // Buka serial port
        serial::Serial esp(port, baud, serial::Timeout::simpleTimeout(1000));

        if (!esp.isOpen()) {
            cerr << " Gagal membuka port " << port << endl;
            return -1;
        }

        cout << " Port " << port << " terbuka!\n";

        // Baca file JSON dalam mode biner
        auto fileData = readFileBinary(filePath);
        cout << " File dibaca: " << fileData.size() << " byte\n";

        // Misal bagi file jadi potongan 50 byte
        auto chunks = splitData(fileData, 50);

        cout << " Total potongan: " << chunks.size() << endl;

        // Kirim tiap potongan dengan header sederhana
        for (size_t i = 0; i < chunks.size(); ++i) {
            vector<uint8_t> packet; packet-> menampung header + isi data dari satu potongan

            // header sederhana [index][total][ukuran_data]
            packet.push_back(static_cast<uint8_t>(i)); // index potongan
            packet.push_back(static_cast<uint8_t>(chunks.size())); //total potongan
            packet.push_back(static_cast<uint8_t>(chunks[i].size())); // ukuran data

            // Menambahkan isi potongan data ke belakang header
            packet.insert(packet.end(), chunks[i].begin(), chunks[i].end());

            // kirim ke serial
            esp.write(packet);

            cout << " Mengirim potongan ke-" << i + 1 << " dari " << chunks.size()
                 << " (" << chunks[i].size() << " byte)" << endl;

            // delay kecil supaya tidak menumpuk
            this_thread::sleep_for(chrono::milliseconds(100));
        }

        cout << "\n Semua potongan telah dikirim!\n";

        esp.close();
    } catch (const exception &e) {
        cerr << " Error: " << e.what() << endl;
    }

    return 0;
} </pre>
### FORMAT FILE .JSON </pre>
File .json harus menggunakan setiap key yang ditentukan sedangkan untuk value setiap key dibebaskan. Khusus Value dari deskripsi harus memiliki kata **setidaknya 25 kata** </pre>
<pre>
  {
    "nama": "Revalinda Bunga Nayla Laksono",
    "jurusan": "Teknologi Informasi",
    "umur": 18,
    "deskripsi": "Saya mahasiswi Teknologi Informasi ITS angkatan 2025, asli Ponorogo, pencinta kucing yang rela digigit asal jangan disuruh makan atau minum coklat, dan pejuang anti tidur siang.  "
}
</pre>
### FORMAT OUTPUT HASIL DATA YANG SUDAH DISUSUN KE SERIAL MONITOR </pre>
## ESP BRIDGE </pre>
## ESP RECEIVER </pre>
