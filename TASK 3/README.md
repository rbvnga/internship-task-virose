# TASK 3 - SAPA MENYAPA </pre>
## DESKRIPSI
Program ini membangun sistem komunikasi antar 12 perangkat ESP menggunakan protokol ESP_NOW. Program ini memungkinkan setiap ESP bertukar pesan berdasarkan perintah yang dikirim melalui Serial Monitor, dengan format yang telah ditentukan. </pre>
## TODO 1 - Membuat perintah untuk mencetak teks yang menyatakan identitas</pre>
1. Mendefinisikan `mac_index_ku` sesuai dengan indeks alamat MAC yang telah diberikan, yaitu 11. </pre>
<pre> int mac_index_ku = 11; </pre>
2. Namaku dapat dicetak dengan memanggil fungsi `mac_index_to_names` dengan parameter `mac_index_ku`, misalnya </pre>
<pre> message = "HALO, halo " + mac_index_to_names(tujuanIndex) + ", aku " + mac_index_to_names(mac_index_ku); </pre>
## TODO 2 - Membuat perintah pemrosesan data perintah yang akan diterima serial dari laptop </pre>
**Ketentuan penerimaan data dari serial memiliki Struktur sebagai berikut:**

| HEADER	| LENGTH | DATA |
|-----------|------------|---------------|
| 3 byte	| 1 byte | N byte | 

**Perintah yang dapat diproses:**

| Perintah |	Keterangan |	Parameter |
|-----------|------------|---------------|
| 0x00 |	HALO	1 byte | (Tujuan) / String |
| 0x01 |	CEK	1 byte | (Tujuan) / String |
| 0x02 | JAWABAN	| String |

Untuk perintah CEK dan HALO, perintah hanya diikuti 1 parameter yaitu tujuan jika diterima dari Serial. Jika menerima dari ESP-NOW, perintah diikuti dengan String. </pre>

**Contoh input pada serial monitor** </pre>
<pre> FF FF 00 06 00 10 </pre>
- HEADER: 0xFF 0xFF 0x00 </pre>
- LENGTH: Panjang data yang akan dikirim (06 byte) </pre>
- PERINTAH: Mengirimkan perintah 00 atau perintah HALO </pre>
- INDEKS TUJUAN: Mengirimkan pesan ke indeks 10 </pre>

<pre> // fungsi untuk membaca input dari serial monitor
void baca_serial(void (*callback)(const uint8_t *data, int len, int extraParam)) {
  //memeriksa apakah ada input baru
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); //baca input sampai baris baru
    input.trim(); //abaikan spasi 
    
    if (input.length() >= 12) { //pastikan input terdiri dari 12 karakter
      String perintahStr = input.substring(8, 10); // byte ke 9-10 adalah perintah
      String tujuanStr = input.substring(10, 12); // byte ke 11-12 adalah tujuan
      
      int perintah = strtol(perintahStr.c_str(), NULL, 16); //mengubah string perintah menjadi angka (int)
      int tujuanIndex = strtol(tujuanStr.c_str(), NULL, 16); //mengubah string tujuan menjadi angka (int)
      
      if (tujuanIndex >= 0 && tujuanIndex < MAC_ADDRESS_TOTAL && tujuanIndex != mac_index_ku) {
        String message = ""; //membuat pesan (massage) kosong 
        
        if (perintah == 0) { //perintah HALO
          message = "HALO, halo " + mac_index_to_names(tujuanIndex) + ", aku " + mac_index_to_names(mac_index_ku);
        } else if (perintah == 1) { //perintah CEK
          message = "CEK, " + mac_index_to_names(tujuanIndex) + ", ini " + mac_index_to_names(mac_index_ku) + " apa kamu disana?";
        } else if (perintah == 2) { //perintah JAWAB
          Serial.println("Perintah JAWAB hanya untuk diterima dari ESP-NOW, tidak dikirim manual.");
          return;
        }

        //mengubah bentuk teks menjadi byte yang bisa dikirim 
        const char* msgChar = message.c_str();
        uint8_t msgData[message.length() + 1];
        for (int i = 0; i < message.length(); i++) {
          msgData[i] = (uint8_t)msgChar[i];
        }
        msgData[message.length()] = 0;
        // Panggil callback dengan (int) extra sebagai tujuanIndex
        callback(msgData, message.length(), tujuanIndex);  
      } else { //jika indeks tujuan salah
        Serial.println("Indeks tujuan tidak valid"); 
      }
    } else {//jika input salah
      Serial.println("Input tidak valid.");
    }
  }
} </pre>

**Fungsi `baca_serial` bertugas untuk:** </pre>

- Membaca input dari Serial Monitor </pre>
- Memvalidasi dan memproses input </pre>
- Menghasilkan pesan berdasarkan perintah dan tujuan </pre>
- Mengirim data melalui callback dalam bentuk byte </pre>

**Penjelasan alur fungsi `baca_serial`:** </pre>

1. Mengecek apakah ada data yang masuk dari Serial Monitor </pre>
2. Memastikan input memiliki 12 karakter agar bisa diproses </pre>
3. Mengambil karakter ke-9 dan ke-10 sebagai kode perintah serta karakter ke-11 dan ke-12 sebagai indeks tujuan </pre>
4. Mengubah input string menjadi integer atau bilangan bulat </pre>
5. Memastikan indeks tujuan valid dan bukan milik perangkat sendiri </pre>
6. Mengidentifikasi perintah dan membuat masing-masing pesannya </pre>
7. Mengubah pesan string menjadi array byte untuk dikirim melalui fungsi `callback` </pre>


## TODO 3 - Membuat perintah untuk memproses perintah yang diterima baik melalui Serial dan ESP-NOW </pre>
**Ketentunan pemrosesan perintah:** </pre>

**Perintah HALO:** </pre>
- Jika menerima perintah HALO dari Serial, maka kirimkan packet perintah ke pengirim dengan format: "HALO(enum), 'Halo [Nama Tujuan] Aku [Namamu]'" </pre>
- Jika menerima perintah HALO dari ESP-NOW, maka kirimkan perintah JAWABAN ke tujuan pengirim dengan format "JAWAB(enum), 'Halo Juga [Nama Pengirim] Aku [Namamu]'". </pre>

**Perintah CEK:</pre>**
- Jika menerima perintah CEK dari Serial, maka kirimkan packet perintah ke pengirim dengan format: "CEK(enum), '[Nama tujuan] ini [Namamu] apa kamu disana?'" </pre>
- Jika menerima perintah CEK dari ESP-NOW, maka kirimkan perintah JAWABAN ke tujuan pengirim dengan format "JAWAB(enum), 'Iya Aku [Nama pengirim] Disini - [Namamu]'". </pre>
Perintah JAWAB:</pre>

**Perintah JAWAB** </pre> 
- hanya menerima dari ESP-NOW, cetak ke Serial Monitor String yang diterima </pre>

Setiap perintah yang diterima dari serial diproses melalui fungsi `baca_serial`, sedangkan setiap perintah yang diterima dari ESP_NOW diproses melalui fungsi `process_perintah`

<pre> //fungsi yang memproses pesan yang diterima
void process_perintah(const uint8_t *data, int len, int index_mac_address_asal) {
  //ubah data byte yang diterima menjadi teks
  String message = String((char*)data); 
  
  if (message.startsWith("HALO")) { //jika mendapatkan pesan HALO
    //membuat balasan dari pesan HALO lalu mengirimnya 
    String reply = "JAWAB, Halo Juga halo " + mac_index_to_names(index_mac_address_asal) + ", Aku " + mac_index_to_names(mac_index_ku);
    uint8_t* senderMac = mac_addresses[index_mac_address_asal];
    esp_now_send(senderMac, (uint8_t*)reply.c_str(), reply.length());
  } else if (message.startsWith("CEK")) { //jika mendapatkan pesan CEK
    //membuat balasan dari pesan CEK lalu mengirimnya 
    String reply = "JAWAB, Iya aku " + mac_index_to_names(index_mac_address_asal) + " Disini - " + mac_index_to_names(mac_index_ku);
    uint8_t* senderMac = mac_addresses[index_mac_address_asal];
    esp_now_send(senderMac, (uint8_t*)reply.c_str(), reply.length());
  } else if (message.startsWith("JAWAB")) { //jika mendapatkan pesan JAWAB
    //mencetak balasan dari pesan JAWAB
    Serial.println("Pesan JAWAB diterima: " + message);
  }
}

// Callback untuk menerima data dari ESP-NOW
void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  //untuk mengetahui indeks pengirim 
  int senderIndex = getIndexFromMac((uint8_t*)mac); 
  //setiap pesan yang diterima diproses dengan fungsi prosess_perintah
  process_perintah(incomingData, len, senderIndex);
}
</pre> 

**penjelasan alur** </pre> 
1. 	ESP menerima data →  `onDataRecv` aktif </pre> 
2. 	MAC pengirim diidentifikasi → `senderIndex` </pre> 
3. 	Data diproses → `process_perintah`</pre> 
4. 	Mengirim pesan balasan dari setiap perintah yang diterima dari ESP-NOW </pre>

## FUNGSI SETUP() </pre> 
Fungsi setup() bertujuan untuk menginisialisasi komunikasi ESP-NOW antar perangkat ESP </pre> 

<pre> void setup() {
  Serial.begin(115200); //memulai serial untuk komunikasi dengan komputer

  WiFi.disconnect(true); // Putus koneksi WiFi sebelumnya
  WiFi.mode(WIFI_STA); //mengatur perangkat untuk mode WIFI STATION
  delay(100); 

  //inisialisasi ESP NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inisialisasi ESP-NOW");
    return;
  }
  //mendaftarkan fungsi onDataRecv untuk menerima pesan
  esp_now_register_recv_cb(onDataRecv);
  //menyatakan bahwa program sudah siap
  Serial.println("ESP-NOW siap.");

  //mendeklarasikan struktur untuk menyimpan informasi peer
  esp_now_peer_info_t peerInfo;
  for (int i = 0; i < MAC_ADDRESS_TOTAL; i++) {
  if (i != mac_index_ku) { //memastikan agar tidak menambahkan diri sendiri senbagai peer 
    memcpy(peerInfo.peer_addr, mac_addresses[i], 6);
    peerInfo.channel = 1;
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.print("Gagal menambahkan peer ke indeks ");
      Serial.println(i);
    }
  }
} </pre> 
## FUNGSI LOOP() </pre> 
Fungsi `loop()` berperan sebagai inti dari siklus eksekusi utama dalam program ini. Pemanggilan fungsi `baca_serial(process_perintah)` menyatakan bahwa setiap kali loop berjalan, program akan memeriksa apakah ada input baru dari Serial Monitor. Jika ada, input tersebut akan dibaca dan diproses oleh fungsi `baca_serial` , yang kemudian akan meneruskan data ke fungsi `process_perintah` sebagai callback. </pre> 
<pre> void loop() {
  //memanggil fungsi baca serial untuk cek input dan prosesnya untuk menjalankan program
  baca_serial(process_perintah); 
} </pre> 
