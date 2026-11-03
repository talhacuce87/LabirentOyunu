#include "Konum.hpp"

Yon tersYon(Yon yon) {
    // Enum sirasi ASAGI, SOL, YUKARI, SAG oldugu icin ters yon 2 ileridedir
    return (Yon)((yon + 2) % YON_SAYISI);
}

Konum::Konum(int x, int y, Yon yon) {
    this->x = x;
    this->y = y;
    this->yon = yon;
}

Konum Konum::ilerle(Yon yon) const {
    switch (yon) {
        case ASAGI:  return Konum(x + 1, y, yon);
        case SOL:    return Konum(x, y - 1, yon);
        case YUKARI: return Konum(x - 1, y, yon);
        case SAG:    return Konum(x, y + 1, yon);
    }
    return *this;
}

// Iki konum ayni hucreyi gosteriyorsa esittir (yon karsilastirilmaz)
bool Konum::operator==(const Konum& diger) const {
    return x == diger.x && y == diger.y;
}

bool Konum::operator!=(const Konum& diger) const {
    return !(*this == diger);
}
