#ifndef LABIRENT_HPP
#define LABIRENT_HPP

#include "Konum.hpp"
#include "Yigit.hpp"

#define YUKSEKLIK 20
#define GENISLIK 50

#define DUVAR '#'
#define YOL ' '

#define BEKLEME_MS 50   // animasyonda iki adim arasi bekleme

class Labirent {
private:
    char harita[YUKSEKLIK][GENISLIK];
    Konum baslangic;
    Konum cikis;
    bool yuklendi;

    Yigit yigit;            // baslangictan bulunulan konuma kadar olan yol
    int adimSayisi;         // ileri adim (push) sayisi
    int geriDonusSayisi;    // geri donus (pop) sayisi

    bool dosyadanOku(const char* dosyaAdi);
    bool baslangicVeCikisBul();

public:
    Labirent(const char* dosyaAdi);

    bool yuklendiMi() const;
    Konum getBaslangic() const;
    Konum getCikis() const;

    bool icindeMi(const Konum& konum) const;   // harita sinirlari icinde mi
    bool yolMu(const Konum& konum) const;      // icinde ve uzerinden gecilebilir mi
    bool cikisMi(const Konum& konum) const;

    // Haritayi ekrana basar; 'imlec' konumuna yonunu gosteren ok cizilir
    void yazdir(const Konum& imlec) const;

    // Yigit ile backtracking yaparak cikisi arar. Cikis bulunursa true doner
    // ve yigitta baslangictan cikisa kadar olan yol kalir.
    // animasyon = false ise ekrana cizim yapilmaz (test icin).
    bool coz(bool animasyon = true);

    Yigit& getYol();
    int getAdimSayisi() const;
    int getGeriDonusSayisi() const;
};

#endif
