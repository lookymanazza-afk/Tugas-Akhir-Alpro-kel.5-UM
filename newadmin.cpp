#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// 1. ENUM: Status & Kategori
enum Status { TERSEDIA, DISEWA, MAINTENANCE };
enum Kategori { STUDIO, ALAT };

// 2. UNION: Data spesifik kategori
union DetailInfo {
    int kapasitasOrang; // Untuk STUDIO
    int stokUnit;       // Untuk ALAT
};

// 3. STRUCT: Struktur data aset
struct AsetStudio {
    int id;
    string nama;
    Kategori kategori;
    Status status;
    string jamTersedia;
    double hargaPerJam;
    DetailInfo info;
};

// 4. ARRAY STATIS: Pilihan slot jam operasional studio
const int JUMLAH_SLOT_JAM = 5;
const string JAM_OPERASIONAL[JUMLAH_SLOT_JAM] = {
    "08:00 - 11:00", 
    "11:00 - 14:00", 
    "14:00 - 17:00", 
    "17:00 - 20:00", 
    "20:00 - 23:00"
};

// FUNCTION KEAMANAN: Verifikasi Password Admin (maksimal 3x percobaan)
bool verifikasiAdmin() {
    const string PASSWORD_ADMIN = "admin123"; // Ubah password di sini jika perlu
    string inputPass;
    int batasPercobaan = 3;

    cout << "\n--- AUTENTIKASI ADMIN ---\n";
    for (int i = 1; i <= batasPercobaan; i++) {
        cout << "Masukkan Password Admin (Percobaan " << i << "/" << batasPercobaan << "): ";
        cin >> inputPass;

        if (inputPass == PASSWORD_ADMIN) {
            cout << "[AKSES DITERIMA] Selamat datang, Admin!\n";
            return true;
        } else {
            cout << "[SALAH] Password tidak sesuai!\n";
        }
    }

    cout << "[AKSES DITOLAK] Anda telah gagal 3 kali. Kembali ke Menu Utama.\n";
    return false;
}

string getStatusText(Status s) {
    switch (s) {
        case TERSEDIA: return "TERSEDIA";
        case DISEWA: return "DISEWA";
        case MAINTENANCE: return "MAINTENANCE";
        default: return "UNKNOWN";
    }
}

// 5. FUNCTION ADMIN: Menambah data baru ke Array Dinamis
void tambahDataAset(AsetStudio*& daftar, int& jumlah) {
    AsetStudio* daftarBaru = new AsetStudio[jumlah + 1];

    for (int i = 0; i < jumlah; i++) {
        daftarBaru[i] = daftar[i];
    }

    AsetStudio itemBaru;
    cout << "\n--- INPUT DATA STUDIO / ALAT BARU ---\n";
    cout << "Masukkan ID (angka unik, misal 101): ";
    cin >> itemBaru.id;
    cin.ignore();

    cout << "Nama Studio / Alat: ";
    getline(cin, itemBaru.nama);

    int pilKat;
    cout << "Kategori (1. Studio | 2. Alat): ";
    cin >> pilKat;
    itemBaru.kategori = (pilKat == 1) ? STUDIO : ALAT;

    cout << "Pilih Slot Jam Operasional:\n";
    for (int i = 0; i < JUMLAH_SLOT_JAM; i++) {
        cout << "  " << i + 1 << ". " << JAM_OPERASIONAL[i] << endl;
    }
    int pilJam;
    cout << "Pilihan jam (1-" << JUMLAH_SLOT_JAM << "): ";
    cin >> pilJam;
    if (pilJam >= 1 && pilJam <= JUMLAH_SLOT_JAM) {
        itemBaru.jamTersedia = JAM_OPERASIONAL[pilJam - 1];
    } else {
        itemBaru.jamTersedia = JAM_OPERASIONAL[0];
    }

    cout << "Harga Sewa per Jam (Rp): ";
    cin >> itemBaru.hargaPerJam;

    int pilStatus;
    cout << "Status Awal (1. Tersedia | 2. Disewa | 3. Maintenance): ";
    cin >> pilStatus;
    if (pilStatus == 1) itemBaru.status = TERSEDIA;
    else if (pilStatus == 2) itemBaru.status = DISEWA;
    else itemBaru.status = MAINTENANCE;

    if (itemBaru.kategori == STUDIO) {
        cout << "Kapasitas Maksimal Orang: ";
        cin >> itemBaru.info.kapasitasOrang;
    } else {
        cout << "Jumlah Stok Unit: ";
        cin >> itemBaru.info.stokUnit;
    }

    daftarBaru[jumlah] = itemBaru;

    delete[] daftar;
    daftar = daftarBaru;
    jumlah++;

    cout << "\n[SUKSES] Data '" << itemBaru.nama << "' berhasil ditambahkan ke sistem!\n";
}

// 6. FUNCTION TAMPILKAN DATA
void tampilkanJadwal(AsetStudio* daftar, int jumlah) {
    if (jumlah == 0) {
        cout << "\n[INFO] Belum ada data studio atau alat yang diinput oleh Admin.\n";
        return;
    }

    cout << "\n=========================================================================================\n";
    cout << left << setw(5) << "ID" 
         << setw(22) << "Nama Studio/Alat" 
         << setw(12) << "Kategori" 
         << setw(16) << "Jadwal Ready" 
         << setw(14) << "Harga/Jam" 
         << setw(14) << "Status" 
         << "Keterangan" << endl;
    cout << "=========================================================================================\n";

    for (int i = 0; i < jumlah; i++) {
        cout << left << setw(5) << daftar[i].id
             << setw(22) << daftar[i].nama
             << setw(12) << (daftar[i].kategori == STUDIO ? "Studio" : "Alat")
             << setw(16) << daftar[i].jamTersedia
             << "Rp " << setw(11) << daftar[i].hargaPerJam
             << setw(14) << getStatusText(daftar[i].status);

        if (daftar[i].kategori == STUDIO) {
            cout << "Kapasitas: " << daftar[i].info.kapasitasOrang << " org";
        } else {
            cout << "Tersedia: " << daftar[i].info.stokUnit << " unit";
        }
        cout << endl;
    }
    cout << "=========================================================================================\n";
}

// 7. SORTING
void urutkanKetersediaan(AsetStudio* daftar, int jumlah) {
    if (jumlah < 2) return;
    for (int i = 0; i < jumlah - 1; i++) {
        for (int j = 0; j < jumlah - i - 1; j++) {
            if (daftar[j].status > daftar[j + 1].status || 
               (daftar[j].status == daftar[j + 1].status && daftar[j].hargaPerJam > daftar[j + 1].hargaPerJam)) {
                AsetStudio temp = daftar[j];
                daftar[j] = daftar[j + 1];
                daftar[j + 1] = temp;
            }
        }
    }
    cout << "\n[INFO] Daftar berhasil diurutkan berdasarkan ketersediaan & harga termurah.\n";
}

// 8. SEARCHING & POINTER
AsetStudio* cariAsetByID(AsetStudio* daftar, int jumlah, int targetID) {
    for (int i = 0; i < jumlah; i++) {
        if (daftar[i].id == targetID) {
            return &daftar[i];
        }
    }
    return nullptr;
}

// 9. BOOKING (KLIEN)
void bookingLayanan(AsetStudio* daftar, int jumlah) {
    if (jumlah == 0) {
        cout << "\n[INFO] Belum ada layanan yang tersedia untuk dibooking.\n";
        return;
    }

    int idCari;
    cout << "\nMasukkan ID Studio/Alat yang ingin disewa: ";
    cin >> idCari;

    AsetStudio* item = cariAsetByID(daftar, jumlah, idCari);

    if (item == nullptr) {
        cout << "[GAGAL] ID tidak ditemukan.\n";
        return;
    }

    if (item->status == TERSEDIA) {
        item->status = DISEWA;
        cout << "[BERHASIL] Anda membooking '" << item->nama 
             << "' pada jadwal " << item->jamTersedia << ".\n";
    } else {
        cout << "[MAAF] '" << item->nama << "' sedang berstatus " 
             << getStatusText(item->status) << ".\n";
    }
}

int main() {
    int jumlahAset = 0;
    AsetStudio* daftarAset = nullptr;

    int role;
    do {
        cout << "\n=========================================\n";
        cout << "   SISTEM MANAJEMEN PERSEWAAN STUDIO\n";
        cout << "=========================================\n";
        cout << "1. Masuk sebagai ADMIN (Input Data)\n";
        cout << "2. Masuk sebagai CALON KLIEN (Cek & Sewa)\n";
        cout << "3. Keluar Program\n";
        cout << "Pilih akses (1-3): ";
        cin >> role;

        if (role == 1) {
            // Panggil fungsi verifikasi password sebelum masuk ke Menu Admin
            if (verifikasiAdmin()) {
                int menuAdmin;
                do {
                    cout << "\n--- MENU ADMIN ---\n";
                    cout << "1. Tambah Data Studio / Alat Baru\n";
                    cout << "2. Lihat Semua Data Terinput\n";
                    cout << "3. Kembali ke Menu Utama\n";
                    cout << "Pilih menu (1-3): ";
                    cin >> menuAdmin;

                    if (menuAdmin == 1) {
                        tambahDataAset(daftarAset, jumlahAset);
                    } else if (menuAdmin == 2) {
                        tampilkanJadwal(daftarAset, jumlahAset);
                    }
                } while (menuAdmin != 3);
            }

        } else if (role == 2) {
            int menuKlien;
            do {
                cout << "\n--- MENU CALON KLIEN ---\n";
                cout << "1. Lihat Jadwal & Ketersediaan Terbaru\n";
                cout << "2. Urutkan yang Ready & Termurah\n";
                cout << "3. Booking Studio / Sewa Alat\n";
                cout << "4. Kembali ke Menu Utama\n";
                cout << "Pilih menu (1-4): ";
                cin >> menuKlien;

                switch (menuKlien) {
                    case 1:
                        tampilkanJadwal(daftarAset, jumlahAset);
                        break;
                    case 2:
                        urutkanKetersediaan(daftarAset, jumlahAset);
                        tampilkanJadwal(daftarAset, jumlahAset);
                        break;
                    case 3:
                        bookingLayanan(daftarAset, jumlahAset);
                        break;
                }
            } while (menuKlien != 4);
        }
    } while (role != 3);

    delete[] daftarAset;
    return 0;
}