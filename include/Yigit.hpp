#ifndef YIGIT_HPP
#define YIGIT_HPP

#include "Konum.hpp"

// Bagli listenin bir elemani
class Dugum {
public:
    Konum veri;
    Dugum* sonraki;

    Dugum(const Konum& veri, Dugum* sonraki = 0);
};

// Bagli liste ile gerceklenmis yigit (LIFO).
// Ekleme ve cikarma listenin basindan yapilir: O(1)
class Yigit {
private:
    Dugum* tepe;
    int boyut;

    // Kopyalama yasak: iki yigit ayni dugumleri silmeye calismasin
    Yigit(const Yigit&);
    Yigit& operator=(const Yigit&);

public:
    Yigit();
    ~Yigit();

    void ekle(const Konum& konum);   // push
    void cikar();                    // pop
    Konum& getir();                  // top (tepedeki eleman)
    bool bosMu() const;
    int elemanSayisi() const;
    void temizle();
};

#endif
