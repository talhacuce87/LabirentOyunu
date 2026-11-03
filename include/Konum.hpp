#ifndef KONUM_HPP
#define KONUM_HPP

typedef enum {
    ASAGI, SOL, YUKARI, SAG
} Yon;

#define YON_SAYISI 4

// Verilen yonun tersini dondurur (ASAGI <-> YUKARI, SOL <-> SAG)
Yon tersYon(Yon yon);

// Labirentte bir hucre. PDF'teki gosterimle uyumlu olarak
// x = satir, y = sutun; ornek: Konum(0, 19) -> 0. satir, 19. sutun
class Konum {
public:
    int x;
    int y;
    Yon yon;

    Konum(int x = 0, int y = 0, Yon yon = ASAGI);

    // Verilen yonde bir adim ilerlenince varilan konum (yonu de 'yon' olur)
    Konum ilerle(Yon yon) const;

    bool operator==(const Konum& diger) const;
    bool operator!=(const Konum& diger) const;
};

#endif
