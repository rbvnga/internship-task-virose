# TASK 4 - CROSS PLATFORM FILE TRANSFER </pre>
## DESKRIPSI </pre>
TASK 4 - CROSS PLATFORM FILE TRANSFER merupakan sistem pengiriman file .json dengan alur sebagai berikut: </pre>
1.laptop: </pre>
• 	Buat file  sesuai ketentuan.</pre>
• 	Pecah file tersebut menjadi beberapa bagian kecil (fragment).</pre>
• 	Kirim tiap bagian ke esp-bridge melalui komunikasi serial.</pre>

2. esp-bridge: </pre>
• 	Terima data dari laptop via serial.</pre>
• 	Teruskan data ke esp-receiver menggunakan ESP-NOW.</pre>

3.esp-receiver: </pre>
• 	Terima semua potongan data dari esp-bridge.</pre>
• 	Susun kembali potongan-potongan tersebut menjadi file utuh.</pre>
• 	Simpan hasilnya ke SPIFFS (penyimpanan internal ESP).</pre>
• 	Setelah selesai, tampilkan hasil akhir di serial monitor sesuai format yang ditentukan.</pre>
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
