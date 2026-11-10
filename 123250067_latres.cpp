#include <iostream>
#include <string>
using namespace std;

struct nodeQueue {
    int nota;
    string nama;
    nodeQueue* next;
};

struct nodeStack {
    int nota;
    string nama;
    nodeStack* next;
};

struct nodeTree {
    int nota;
    string nama;
    nodeTree* kiri;
    nodeTree* kanan;
};

nodeQueue* front = NULL;
nodeQueue* last = NULL;
nodeStack* top = NULL;
nodeTree* root = NULL;

// menambahkan node baru ke tree berdasarkan nomor nota.
// nota lebih kecil masuk ke kiri, nota lebih besar masuk ke kanan.
// mengembalikan akar tree setelah penambahan.
nodeTree* tambah_tree(nodeTree* akar, int nota, string nama) {
    if (akar == NULL) {
        return new nodeTree {nota, nama, NULL, NULL};
    }
    if (nota < akar->nota) {
        akar->kiri = tambah_tree(akar->kiri, nota, nama);
    } else {
        akar->kanan = tambah_tree(akar->kanan, nota, nama);
    }
    return akar;
}

// mencari node di tree berdasarkan nomor nota.
// mengembalikan node yang ditemukan, atau null jika tidak ada.
nodeTree* cari_tree(nodeTree* akar, int nota) {
    if (akar == NULL || akar->nota == nota) {
        return akar;
    }
    if (nota < akar->nota) {
        return cari_tree(akar->kiri, nota);
    }
    return cari_tree(akar->kanan, nota);
}

// menghapus node di tree berdasarkan nomor nota.
// harus menangani 3 kasus: tanpa anak, satu anak, dan dua anak.
// mengembalikan akar tree setelah penghapusan.
nodeTree* hapus_tree(nodeTree* akar, int nota) {
    if (akar == NULL) {
        return NULL;
    }
    if (nota < akar->nota) {
        akar->kiri = hapus_tree(akar->kiri, nota);
    } else if (nota > akar->nota) {
        akar->kanan = hapus_tree(akar->kanan, nota);
    } else {
        if (akar->kiri == NULL) {
            nodeTree* temp = akar->kanan;
            delete akar;
            return temp;
        } else if (akar->kanan == NULL) {
            nodeTree* temp = akar->kiri;
            delete akar;
            return temp;
        }
        nodeTree* temp = akar->kanan;
        while (temp && temp->kiri != NULL) {
            temp = temp->kiri;
        }
        akar->nota = temp->nota;
        akar->nama = temp->nama;
        akar->kanan = hapus_tree(akar->kanan, temp->nota);
    }
    return akar;
}

// menampilkan isi tree dengan urutan: node, kiri, kanan.
// parameter no dipakai sebagai nomor urut tampilan.
void preorder(nodeTree* akar, int& no) {
    if (akar == NULL) {
        return;
    }
    cout << no << ". [" << akar->nota << "] " << akar->nama << endl;
    no++;
    preorder(akar->kiri, no);
    preorder(akar->kanan, no);
}

// menampilkan isi tree dengan urutan: kiri, node, kanan.
// hasilnya akan terurut berdasarkan nomor nota.
void inorder(nodeTree* akar, int& no) {
    if (akar == NULL) {
        return;
    }
    inorder(akar->kiri, no);
    cout << no << ". [" << akar->nota << "] " << akar->nama << endl;
    no++;
    inorder(akar->kanan, no);
}

// menampilkan isi tree dengan urutan: kiri, kanan, node.
void postorder(nodeTree* akar, int& no) {
    if (akar == NULL) {
        return;
    }
    postorder(akar->kiri, no);
    postorder(akar->kanan, no);
    cout << no << ". [" << akar->nota << "] " << akar->nama << endl;
    no++;
}

// meminta input nomor nota dan nama cucian, lalu memasukkannya ke queue (posisi paling belakang).
// error handling: nota bukan angka atau kurang dari 1, nota sudah dipakai
// (cek di queue dan stack), dan nama kosong.
void tambah_cucian() {
    int nota;
    string nama;
    cout << "Masukkan nomor nota: ";
    cin >> nota;
    if (cin.fail() || nota <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Nomor nota harus berupa angka lebih dari 0!" << endl << endl;
        return;
    }
    nodeQueue* q = front;
    while (q != NULL) {
        if (q->nota == nota) {
            cout << "Nomor nota sudah digunakan!" << endl << endl;
            return;
        }
        q = q->next;
    }
    nodeStack* s = top;
    while (s != NULL) {
        if (s->nota == nota) {
            cout << "Nomor nota sudah digunakan!" << endl << endl;
            return;
        }
        s = s->next;
    }
    cout << "Masukkan nama cucian: ";
    cin.ignore();
    getline(cin, nama);
    if (nama.empty()) {
        cout << "Nama cucian tidak boleh kosong!" << endl << endl;
        return;
    }
    nodeQueue* baru = new nodeQueue {nota, nama, NULL};
    if (last == NULL) {
        front = last = baru;
    } else {
        last->next = baru;
        last = baru;
    }
}

// menampilkan seluruh cucian di queue dari depan sampai belakang.
// error handling jika queue kosong.
void lihat_antrean() {
    if (front == NULL) {
        cout << "Antrean cucian masih kosong!" << endl << endl;
        return;
    }
    cout << "=== Antrean Cucian ===" << endl << endl;
    nodeQueue* q = front;
    int no = 1;
    while (q != NULL) {
        cout << no << ". [" << q->nota << "] " << q->nama << endl;
        no++;
        q = q->next;
    }
    cout << endl;
}

// mengambil cucian paling depan dari queue, lalu memindahkannya ke stack (posisi paling atas)
// dan memasukkannya ke tree (rak). node queue dihapus setelah dipindahkan.
// error handling jika queue kosong.
void proses_cucian() {
    if (front == NULL) {
        cout << "Antrean cucian masih kosong!" << endl << endl;
        return;
    }
    nodeQueue* hapus = front;
    nodeStack* baru = new nodeStack {hapus->nota, hapus->nama, top};
    top = baru;
    root = tambah_tree(root, hapus->nota, hapus->nama);
    cout << "Cucian [" << hapus->nota << "] " << hapus->nama << " telah diproses!" << endl << endl;
    front = hapus->next;
    if (front == NULL) {
        last = NULL;
    }
    delete hapus;

}

// menampilkan seluruh cucian di stack dari atas sampai bawah.
// error handling jika stack kosong.
void lihat_riwayat() {
    if (top == NULL) {
        cout << "Riwayat cucian masih kosong!" << endl << endl;
        return;
    }
    cout << "=== Riwayat Cucian ===" << endl << endl;
    nodeStack* s = top;
    int no = 1;
    while (s != NULL) {
        cout << no << ". [" << s->nota << "] " << s->nama << endl;
        no++;
        s = s->next;
    }
    cout << endl;
}

// meminta input nomor nota, lalu mencarinya di tree (rak) dan menampilkan datanya.
// error handling jika rak kosong, input nota tidak valid, atau nota tidak ditemukan.
void cari_cucian() {
    if (root == NULL) {
        cout << "Rak masih kosong!" << endl << endl;
        return;
    }
    int nota;
    cout << "Masukkan nomor nota: ";
    cin >> nota;
    if (cin.fail() || nota <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Nomor nota harus berupa angka lebih dari 0!" << endl << endl;
        return;
    }
    nodeTree* hasil = cari_tree(root, nota);
    if (hasil == NULL) {
        cout << "Cucian dengan nota " << nota << " tidak ditemukan di rak!" << endl << endl;
        return;
    }
    cout << "Cucian dengan nota " << nota << " ditemukan di rak!" << endl;
    cout << "Nama: " << hasil->nama << endl << endl;
}

// meminta user memilih pre-order, in-order, atau post-order,
// lalu menampilkan isi tree sesuai pilihan.
// error handling jika rak kosong atau pilihan tidak valid.
void lihat_rak() {
    if (root == NULL) {
        cout << "Rak masih kosong!" << endl << endl;
        return;
    }
    char pil;
    int no = 1;
    cout << "=== Lihat Isi Rak ===" << endl << endl;
    cout << "Pilih jenis tampilan:" << endl;
    cout << "1. Pre-order" << endl;
    cout << "2. In-order" << endl;
    cout << "3. Post-order" << endl;
    cout << "Pilihan: ";
    cin >> pil;
    switch (pil) {
        case '1':
            preorder(root, no);
            break;
        case '2':
            inorder(root, no);
            break;
        case '3':
            postorder(root, no);
            break;
        default:
            cout << "Pilihan tidak valid!" << endl << endl;
    }
    cout << endl;
}

// meminta input nomor nota cucian yang diambil pelanggan, lalu menghapusnya dari tree (rak).
// data di stack tidak ikut terhapus.
// error handling jika rak kosong, input nota tidak valid, atau nota tidak ditemukan.
void ambil_cucian() {
    if (root == NULL) {
        cout << "Rak masih kosong!" << endl << endl;
        return;
    }
    int nota;
    cout << "Masukkan nomor nota: ";
    cin >> nota;
    if (cin.fail() || nota <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Nomor nota harus berupa angka lebih dari 0!" << endl << endl;
        return;
    }
    nodeTree* hasil = cari_tree(root, nota);
    if (hasil == NULL) {
        cout << "Cucian dengan nota " << nota << " tidak ada di rak!" << endl << endl;
        return;
    }
    cout << "Cucian " << hasil->nama << " telah diambil pelanggan!" << endl << endl;
    root = hapus_tree(root, nota);
}

// menampilkan menu utama dan memanggil fungsi sesuai pilihan user sampai user memilih keluar.
// menu: tambah cucian (bisa diulang), lihat antrean, proses cucian, lihat riwayat,
// cari cucian, lihat isi rak, ambil cucian, dan keluar.
// error handling jika pilihan di luar menu.
int main(){
    char pilihan;
    do {
        cout << "=== Menu Utama ===" << endl;
        cout << "1. Tambah Cucian" << endl;
        cout << "2. Lihat Antrean" << endl;
        cout << "3. Proses Cucian" << endl;
        cout << "4. Lihat Riwayat" << endl;
        cout << "5. Cari Cucian" << endl;
        cout << "6. Lihat Isi Rak" << endl;
        cout << "7. Ambil Cucian" << endl;
        cout << "8. Keluar" << endl;
        cout << "Pilihan: ";
        cin >> pilihan;
        switch (pilihan) {
            case '1': {
                char pil_tambah = 'y';
                while (pil_tambah == 'y' || pil_tambah == 'Y') {
                    tambah_cucian();
                    cout << "Apakah anda ingin menambahkan cucian lagi? (y/n): ";
                    cin >> pil_tambah;
                }
                break;
                
            }
            case '2':
                lihat_antrean();
                break;
            case '3':
                proses_cucian();
                break;
            case '4':
                lihat_riwayat();
                break;
            case '5':
                cari_cucian();
                break;
            case '6':
                lihat_rak();
                break;
            case '7':
                ambil_cucian();
                break;
            case '8':
                cout << "Terima kasih telah menggunakan program ini!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid!" << endl << endl;
        }
    } while (pilihan != '8');
}