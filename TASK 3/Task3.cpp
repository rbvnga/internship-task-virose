#include <WiFi.h>
#include <esp_now.h>

// Daftar nama dengan nomor indeksnya
typedef enum {
  BISMA = 0,
  JSON,
  FARUG,
  FAUZAN_FIRDAUS,
  AFRICHA_SEKAR_WANGI,
  RAFAINA_ERIN_SADIA,
  ANTONIUS_MICHAEL_YORDANIS_HARTONO,
  DINDA_SOFI_AZZAHRO,
  MUHAMMAD_FAHMI_ILMI,
  DHANISHARA_ZASCHYA_PUTRI_SYAMSUDIN,
  IRSA_FAIRUZA,
  REVALINDA_BUNGA_NAYLA_LAKSONO,
} ADDRESS_ASSIGNMENT;

#define MAC_ADDRESS_TOTAL 12
#define MAC_ADDRESS_LENGTH 6
//menyatakan bahwa 11 adalah indeks dari alamat MAC ku
int mac_index_ku = 11; 

//array mc_addresses yang menyimpan semua alamat MAC
uint8_t mac_addresses[MAC_ADDRESS_TOTAL][MAC_ADDRESS_LENGTH] = {
  {0x24, 0x0A, 0xC4, 0x0A, 0x21, 0x11},  // BISMA
  {0x24, 0x0A, 0xC4, 0x0A, 0x21, 0x10},  // JSON
  {0x24, 0x0A, 0xC4, 0x0A, 0x20, 0x11},  // FARUG
  {0x24, 0x0A, 0xC4, 0x0A, 0x10, 0x10},  // Fauzan Firdaus
  {0x24, 0x0A, 0xC4, 0x0A, 0x10, 0x11},  // Africha Sekar Wangi
  {0x24, 0x0A, 0xC4, 0x0A, 0x11, 0x10},  // Rafaina Erin Sadia
  {0x24, 0x0A, 0xC4, 0x0A, 0x11, 0x11},  // Antonius Michael Yordanis Hartono
  {0x24, 0x0A, 0xC4, 0x0A, 0x12, 0x10},  // Dinda Sofi Azzahro
  {0x24, 0x0A, 0xC4, 0x0A, 0x12, 0x11},  // Muhammad Fahmi Ilmi
  {0x24, 0x0A, 0xC4, 0x0A, 0x13, 0x10},  // Dhanishara Zaschya Putri Syamsudin
  {0x24, 0x0A, 0xC4, 0x0A, 0x13, 0x11},  // Irsa Fairuza
  {0x24, 0x0A, 0xC4, 0x0A, 0x14, 0x10},  // Revalinda Bunga Nayla Laksono
};

const char* names[] = {
  "BISMA",
  "JSON",
  "FARUG",
  "FAUZAN_FIRDAUS",
  "AFRICHA_SEKAR_WANGI",
  "RAFAINA_ERIN_SADIA",
  "ANTONIUS_MICHAEL_YORDANIS_HARTONO",
  "DINDA_SOFI_AZZAHRO",
  "MUHAMMAD_FAHMI_ILMI",
  "DHANISHARA_ZASCHYA_PUTRI_SYAMSUDIN",
  "IRSA_FAIRUZA",
  "REVALINDA_BUNGA_NAYLA_LAKSONO",
};

// Fungsi untuk mendapatkan nama dari indeks
String mac_index_to_names(int index) {
  if (index >= 0 && index < MAC_ADDRESS_TOTAL) {
    return String(names[index]);
  }
  return "tidak diketahui";
}

// Fungsi untuk mendapatkan indeks dari MAC address
int getIndexFromMac(uint8_t* mac) {
  for (int i = 0; i < MAC_ADDRESS_TOTAL; i++) {
    if (memcmp(mac, mac_addresses[i], MAC_ADDRESS_LENGTH) == 0) {
      return i;
    }
  }
  return -1;
}

// fungsi untuk membaca input dari serial monitoe
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
}

//fungsi yang memproses pesan yang diterima
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

void setup() {
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
}
}

void loop() {
  //memanggil fungsi baca serial untuk cek input dan prosesnya untuk menjalankan program
  baca_serial(process_perintah); 
}
