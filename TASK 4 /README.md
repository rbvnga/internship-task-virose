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
<pre># Menentukan versi minimum CMake yang diperlukan
cmake_minimum_required(VERSION 3.10)
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
**Keterangan** </pre>
1. membaca file halo.json dalam mode biner </pre>
2. memecah data menjadi potongan-potongan setiap beberapa byte, didalam kode ini file dipecah setiap 50 byte </pre>
3. setiap potongan akan memiliki header sederhana, yaitu index ptongan, total potongan, dan ukuran data </pre>
4. masukkan header dan isi data di setiap potongannya </pre>
5. kirim potongan ke serial
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
### main.cpp </pre>
<pre>
//MAC address ESP penerima 
uint8_t RECEIVER_MAC[] = { 0xD0, 0xEF, 0x76, 0x32, 0x55, 0xB8 };  

#define SERIAL_BAUD 115200

//buffer dinamis untuk menyimpan byte yang diterima dari Serial
std::vector<uint8_t> serialBuf;

// Callback ketika data terkirim via ESP-NOW
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("[ESP-NOW] Kirim ke ");
  for (int i = 0; i < 6; i++) {
    if (i) Serial.print(":");
    Serial.printf("%02X", mac_addr[i]);
  }
  Serial.print(" status = ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "SUKSES" : "GAGAL");
}

// Tambah peer atau perangkat tujuan ke daftar ESP NOW
bool addPeer(const uint8_t *peer_mac) {
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peer_mac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_err_t res = esp_now_add_peer(&peerInfo);
  if (res != ESP_OK && res != ESP_ERR_ESPNOW_EXIST) {
    Serial.printf("Gagal menambahkan peer: %d\n", res);
    return false;
  }
  return true;
}

// Parse frame dari serial buffer
//Mengecek apakah buffer berisi satu frame lengkap 
bool ParseFrame(std::vector<uint8_t> &buf, std::vector<uint8_t> &frame) {
  if (buf.size() < 3) return false; // butuh header 3 byte
  uint8_t payload_len = buf[2]; //->Byte ke 3 (indeks 2) menunjukkan panjang payload
  //Menghitung total panjang frame
  size_t total_len = 3 + payload_len;
  if (buf.size() < total_len) return false;
  // Menyalin total panjang frame/ total_len (byte) pertama dari  ke .
  frame.assign(buf.begin(), buf.begin() + total_len);
  //Menghapus data frame yang sudah diproses dari buf
  buf.erase(buf.begin(), buf.begin() + total_len);
  return true;
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(500);
  Serial.println("=== ESP Bridge Mulai ===");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Inisialisasi ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Gagal inisialisasi ESP-NOW");
    while (true) delay(1000);
  }

  //Mendaftarkan callback untuk status pengiriman
  esp_now_register_send_cb(onDataSent);

  if (addPeer(RECEIVER_MAC)) {
    Serial.println("Peer (receiver) ditambahkan.");
  }

  Serial.println("Menunggu data dari Serial...");
}

void loop() {
  // Baca semua byte yang masuk dari Serial
  while (Serial.available()) {
    int b = Serial.read();
    if (b >= 0) serialBuf.push_back((uint8_t)b);
  }

  // Jika ada frame lengkap → kirim via ESP-NOW
  std::vector<uint8_t> frame;//-> vektor frame untuk menyimpan satu frame yang berhasil di parse
  while (ParseFrame(serialBuf, frame)) {
    //kirim data frame via ESP-NOW ke MAC Receiver
    esp_err_t res = esp_now_send(RECEIVER_MAC, frame.data(), frame.size());
    if (res == ESP_OK) {
      Serial.printf("Kirim frame idx=%u total=%u len=%u\n",
                    frame[0], frame[1], frame[2]);
    } else {
      Serial.printf("Gagal kirim (kode %d)\n", res);
    }
    delay(5);
  }
} </pre>
**keterangan**  </pre>
1. memastikan ESP Bridge ke mode station </pre>
2. membaca semua byte yang masuk  dari serial </pre>
3. cek apakah buffer (byte yang dikirim) sudah berisi header dan isi data yang memenuhi</pre>
4. parse setiap frame dari serial buffer </pre>
5. menetapkan MAC Address ESP penerima </pre>
6. Menambahkan peer atau perangkat tujuan ke daftar ESP NOW </pre>
7. kirim data frame via ESP NOW ke MAC Receiver </pre> 
## ESP RECEIVER </pre>
### main.cpp
<pre> 
File file;
bool receiving = false;

// menampilkan isi file JSON yang diterima
void tampilkanJSON() {
  File f = SPIFFS.open("/result.json", "r");
  if (!f) {
    Serial.println("Gagal membuka file hasil!");
    return;
  }

  // Alokasikan buffer JSON
  //Deklarasi dokumen JSON statis dengan ukuran buffer 1024 byte
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, f);
  f.close(); //menutup file setelah parsing 

  if (error) {
    Serial.print("Gagal parsing JSON: ");
    //Serial.println(error.c_str()); //Cetak detail error sebagai string
    return;
  }

  // Ambil data dari JSON
  const char* nama = doc["nama"];
  const char* jurusan = doc["jurusan"];
  int umur = doc["umur"];
  const char* deskripsi = doc["deskripsi"];

  Serial.print("NAMA: "); Serial.println(nama);
  Serial.print("JURUSAN: "); Serial.println(jurusan);
  Serial.print("UMUR: "); Serial.println(umur);
  Serial.print("DESKRIPSI DIRI: "); Serial.println(deskripsi);
}

// Fungsi callback ESP-NOW saat data diterima
void onReceive(const uint8_t *mac, const uint8_t *data, int len) {
  String chunk = ""; // untuk menyimpan data yang diterima sebagai string
  for (int i = 0; i < len; i++) chunk += (char)data[i];

  // Jika sudah menerima tanda akhir file
  if (chunk.indexOf("<<END>>") >= 0) {
    file.close();
    Serial.println("\n[ FILE SELESAI DITERIMA ]");
    tampilkanJSON();
    receiving = false;
    return;
  }

  // Jika pertama kali menerima data
  if (!receiving) {
    SPIFFS.remove("/result.json");
    file = SPIFFS.open("/result.json", "w");
    receiving = true;
    Serial.println("Mulai menerima file...");
  }

  // Tambahkan potongan ke file
  file.print(chunk);
  Serial.print(".");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (!SPIFFS.begin(true)) {
    Serial.println("Gagal inisialisasi SPIFFS!");
    return;
  }

  if (esp_now_init() != ESP_OK) {
    Serial.println("Gagal inisialisasi ESP-NOW!");
    return;
  }
  //mendaftarkan callbaxk untuk menerima data 
  esp_now_register_recv_cb(onReceive);

  Serial.println("ESP-Receiver siap menerima data...");
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {}
 </pre>
**keterangan**  </pre>
1. ESP-Bridge menerima file JSON dari laptop (via Serial). </pre>
2. Bridge mengirim file JSON potongan demi potongan ke Receiver lewat ESP-NOW. </pre>
3.  ESP-Receiver: </pre>
Menerima potongan → simpan di SPIFFS </pre>
Setelah tanda <<END>> diterima → tutup file </pre>
Parsing JSON → tampilkan hasil terformat di Serial Monitor </pre>
