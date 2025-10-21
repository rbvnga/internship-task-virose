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

#### CMakeLists.txt
<pre># Menentukan versi minimum CMake yang diperlukan  cmake_minimum_required(VERSION 3.10)
project(FileTransferLaptop)
 
set(CMAKE_CXX_STANDARD 17)

# Tambahkan include dan library path untuk serial
include_directories("C:/Users/asusv/serial/include")
link_directories("C:/Users/asusv/serial/build/Release")

add_executable(FileTransferLaptop src/main.cpp)

# Menautkan link library "serial" ke executable "FileTransferLaptop"
target_link_libraries(FileTransferLaptop serial)</pre>

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
