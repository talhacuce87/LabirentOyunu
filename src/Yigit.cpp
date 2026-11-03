#include "Yigit.hpp"
#include <stdexcept>

Dugum::Dugum(const Konum& veri, Dugum* sonraki) {
    this->veri = veri;
    this->sonraki = sonraki;
}

Yigit::Yigit() {
    tepe = 0;
    boyut = 0;
}

Yigit::~Yigit() {
    temizle();
}

void Yigit::ekle(const Konum& konum) {
    // Yeni dugum eski tepenin onune eklenir
    tepe = new Dugum(konum, tepe);
    boyut++;
}

void Yigit::cikar() {
    if (bosMu())
        throw std::out_of_range("Yigit bos: cikar() yapilamaz");

    Dugum* silinecek = tepe;
    tepe = tepe->sonraki;
    delete silinecek;
    boyut--;
}

Konum& Yigit::getir() {
    if (bosMu())
        throw std::out_of_range("Yigit bos: getir() yapilamaz");

    return tepe->veri;
}

bool Yigit::bosMu() const {
    return tepe == 0;
}

int Yigit::elemanSayisi() const {
    return boyut;
}

void Yigit::temizle() {
    while (!bosMu())
        cikar();
}
