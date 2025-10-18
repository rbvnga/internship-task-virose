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

HEADER: 0xFF 0xFF 0x00 </pre>
LENGTH: Panjang data yang akan dikirim (N byte) </pre>
DATA: Data yang akan dikirim berukuran N byte </pre>

**Perintah yang dapat diproses:**

| Perintah |	Keterangan |	Parameter |
|-----------|------------|---------------|
| 0x00 |	HALO	1 byte | (Tujuan) / String |
| 0x01 |	CEK	1 byte | (Tujuan) / String |
| 0x02 | JAWABAN	| String |

Untuk perintah CEK dan HALO, perintah hanya diikuti 1 parameter yaitu tujuan jika diterima dari Serial. Jika menerima dari ESP-NOW, perintah diikuti dengan String. </pre>

**Contoh input pada serial monitor** </pre>
<pre> // fungsi untuk membaca input dari serial monitoe
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

**Fungsi <pre> baca_serial <pre> bertugas untuk:** </pre>

• 	Membaca input dari Serial Monitor </pre>
• 	Memvalidasi dan memproses input</pre>
• 	Menghasilkan pesan berdasarkan perintah dan tujuan </pre>
• 	Mengirim data melalui callback dalam bentuk byte </pre>

**Penjelasan alur fungsi <pre> baca_serial <pre>:** </pre>

1. Mengecek apakah ada data yang masuk dari Serial Monitor </pre>
2. Memastikan input memiliki 12 karakter agar bisa diproses </pre>
3. Mengambil karakter ke-9 dan ke-10 sebagai kode perintah serta karakter ke-11 dan ke-12 sebagai indeks tujuan </pre>
4. Mengubah input string menjadi integer atau bilangan bulat </pre>
5. Memastikan indeks tujuan valid dan bukan milik perangkat sendiri </pre>
6. Mengidentifikasi perintah dan membuat masing-masing pesannya </pre>
7. Mengubah pesan string menjadi array byte untuk dikirim melalui fungsi <pre> callback <pre>


## TODO 2 - Membuat perintah untuk memproses perintah yang diterima baik melalui Serial dan ESP-NOW </pre>
**Ketentunan pemrosesan perintah:** </pre>
Perintah HALO: </pre>

Jika menerima perintah HALO dari Serial, maka kirimkan packet perintah ke pengirim dengan format: "HALO(enum), 'Halo [Nama Tujuan] Aku [Namamu]'"
Jika menerima perintah HALO dari ESP-NOW, maka kirimkan perintah JAWABAN ke tujuan pengirim dengan format "JAWAB(enum), 'Halo Juga [Nama Pengirim] Aku [Namamu]'".
Perintah CEK:</pre>

Jika menerima perintah CEK dari Serial, maka kirimkan packet perintah ke pengirim dengan format: "CEK(enum), '[Nama tujuan] ini [Namamu] apa kamu disana?'"
Jika menerima perintah CEK dari ESP-NOW, maka kirimkan perintah JAWABAN ke tujuan pengirim dengan format "JAWAB(enum), 'Iya Aku [Nama pengirim] Disini - [Namamu]'".
Perintah JAWAB:</pre>

Perintah JAWAB cuma menerima dari ESP-NOW, cetak ke Serial Monitor String yang diterima".
