#include "Labirent.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>

static const char YonChr[] = {31, 17, 30, 16}; // ASAGI, SOL, YUKARI, SAG

// Mevcut yone gore deneme sirasi: duz, saga, sola, geri.
// Enum sirasi ASAGI, SOL, YUKARI, SAG oldugu icin yon + 1 her zaman saga donmektir.
static const int DenemeSirasi[YON_SAYISI] = {0, 1, 3, 2};

// Cikti bir konsola gidiyorsa true (dosyaya yonlendirilmisse false)
static bool konsolMu() {
    CONSOLE_SCREEN_BUFFER_INFO bilgi;
    return GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bilgi) != 0;
}

static void imleciBasaAl() {
    COORD bas = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), bas);
}

static void imleciGoster(bool goster) {
    CONSOLE_CURSOR_INFO bilgi;
    HANDLE konsol = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(konsol, &bilgi);
    bilgi.bVisible = goster;
    SetConsoleCursorInfo(konsol, &bilgi);
}

Labirent::Labirent(const char* dosyaAdi) {
    adimSayisi = 0;
    geriDonusSayisi = 0;
    yuklendi = dosyadanOku(dosyaAdi) && baslangicVeCikisBul();
}

bool Labirent::dosyadanOku(const char* dosyaAdi) {
    FILE* fp = fopen(dosyaAdi, "r");
    if (fp == NULL) {
        printf("HATA: %s dosyasi acilamadi\n", dosyaAdi);
        return false;
    }

    // Satir sonu (\r\n) ve uzun satirlar icin tampon genis tutulur
    char satir[GENISLIK + 16];
    int i = 0;
    while (i < YUKSEKLIK && fgets(satir, sizeof(satir), fp) != NULL) {
        int uzunluk = strcspn(satir, "\r\n");

        // Satir tampona sigmadiysa geri kalanini at
        if (satir[uzunluk] == '\0' && uzunluk == (int)sizeof(satir) - 1) {
            int c;
            while ((c = fgetc(fp)) != '\n' && c != EOF);
        }

        // Editorlerin sildigi sondaki bosluklar yol olarak tamamlanir
        for (int j = 0; j < GENISLIK; j++)
            harita[i][j] = (j < uzunluk && satir[j] == DUVAR) ? DUVAR : YOL;
        i++;
    }
    fclose(fp);

    if (i < YUKSEKLIK) {
        printf("HATA: %s dosyasinda %d satir var, %d olmali\n", dosyaAdi, i, YUKSEKLIK);
        return false;
    }
    return true;
}

bool Labirent::baslangicVeCikisBul() {
    // Kenarlar ust satir -> sol sutun -> sag sutun -> alt satir sirasiyla taranir.
    // Bulunan ilk acik kenar hucresi baslangic, son acik kenar hucresi cikistir.
    Konum kenar[2 * GENISLIK + 2 * YUKSEKLIK];
    int sayi = 0;

    // Her kenar hucresinin yonu labirentin icine dogru bakar
    for (int j = 0; j < GENISLIK; j++)
        kenar[sayi++] = Konum(0, j, ASAGI);
    for (int i = 1; i < YUKSEKLIK - 1; i++)
        kenar[sayi++] = Konum(i, 0, SAG);
    for (int i = 1; i < YUKSEKLIK - 1; i++)
        kenar[sayi++] = Konum(i, GENISLIK - 1, SOL);
    for (int j = 0; j < GENISLIK; j++)
        kenar[sayi++] = Konum(YUKSEKLIK - 1, j, YUKARI);

    int ilk = -1, son = -1;
    for (int k = 0; k < sayi; k++) {
        if (yolMu(kenar[k])) {
            if (ilk == -1) ilk = k;
            son = k;
        }
    }

    if (ilk == -1 || ilk == son) {
        printf("HATA: Haritanin kenarinda baslangic ve cikis icin en az 2 bosluk olmali\n");
        return false;
    }

    baslangic = kenar[ilk];
    cikis = kenar[son];
    return true;
}

bool Labirent::yuklendiMi() const {
    return yuklendi;
}

Konum Labirent::getBaslangic() const {
    return baslangic;
}

Konum Labirent::getCikis() const {
    return cikis;
}

bool Labirent::icindeMi(const Konum& konum) const {
    return konum.x >= 0 && konum.x < YUKSEKLIK && konum.y >= 0 && konum.y < GENISLIK;
}

bool Labirent::yolMu(const Konum& konum) const {
    return icindeMi(konum) && harita[konum.x][konum.y] == YOL;
}

bool Labirent::cikisMi(const Konum& konum) const {
    return konum == cikis;
}

void Labirent::yazdir(const Konum& imlec) const {
    // Tum ekran tek seferde basilir; karakter karakter yazmaktan cok daha hizlidir
    char ekran[YUKSEKLIK][GENISLIK];
    for (int i = 0; i < YUKSEKLIK; i++)
        for (int j = 0; j < GENISLIK; j++)
            ekran[i][j] = harita[i][j];
    ekran[imlec.x][imlec.y] = YonChr[imlec.yon];

    if (!konsolMu()) {
        // Dosyaya yonlendirilmis cikti: karakterler oldugu gibi yazilir
        for (int i = 0; i < YUKSEKLIK; i++) {
            fwrite(ekran[i], 1, GENISLIK, stdout);
            putchar('\n');
        }
        return;
    }

    // Windows 11 konsolu 16, 17, 30, 31 kodlarini kontrol karakteri sayip gostermiyor.
    // Bu yuzden MB_USEGLYPHCHARS ile kod sayfasi 437'deki ok sekillerine
    // cevrilip Unicode olarak yazilir; eski ve yeni konsolda ayni gorunur.
    wchar_t genis[YUKSEKLIK * (GENISLIK + 1)];
    int k = 0;
    for (int i = 0; i < YUKSEKLIK; i++) {
        MultiByteToWideChar(437, MB_USEGLYPHCHARS, ekran[i], GENISLIK, genis + k, GENISLIK);
        k += GENISLIK;
        genis[k++] = L'\n';
    }
    DWORD yazilan;
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), genis, k, &yazilan, NULL);
}

bool Labirent::coz(bool animasyon) {
    bool ziyaret[YUKSEKLIK][GENISLIK];
    memset(ziyaret, 0, sizeof(ziyaret));

    yigit.temizle();
    adimSayisi = 0;
    geriDonusSayisi = 0;

    yigit.ekle(baslangic);
    ziyaret[baslangic.x][baslangic.y] = true;

    // Konsoldaysak her karede ekran temizlemek yerine imleci basa aliriz (titreme olmaz)
    bool konsol = animasyon && konsolMu();
    if (konsol) {
        system("cls");
        imleciGoster(false);
    }

    // Ekrandaki okun yonu: ileri giderken gelis yonu, geri donerken onun tersi.
    // Yigittaki gelis yonleri degistirilmez; ard arda geri donuslerde gerekiyor.
    Yon okYonu = baslangic.yon;

    bool bulundu = false;
    while (!yigit.bosMu()) {
        // Yigitin tepesi her zaman su an bulunulan konumdur
        const Konum& simdiki = yigit.getir();

        if (animasyon) {
            if (konsol) imleciBasaAl();
            yazdir(Konum(simdiki.x, simdiki.y, okYonu));
            fflush(stdout);
            Sleep(BEKLEME_MS);
        }

        if (cikisMi(simdiki)) {
            bulundu = true;
            break;
        }

        // Gidilebilecek ve daha once ugranmamis ilk komsuya ilerle
        bool ilerledi = false;
        for (int i = 0; i < YON_SAYISI && !ilerledi; i++) {
            Yon yon = (Yon)((simdiki.yon + DenemeSirasi[i]) % YON_SAYISI);
            Konum sonraki = simdiki.ilerle(yon);
            if (yolMu(sonraki) && !ziyaret[sonraki.x][sonraki.y]) {
                ziyaret[sonraki.x][sonraki.y] = true;
                yigit.ekle(sonraki);
                adimSayisi++;
                okYonu = yon;
                ilerledi = true;
            }
        }

        // Cikmaz sokak: tepedeki konumu atip bir onceki konuma geri don (backtracking)
        if (!ilerledi) {
            okYonu = tersYon(simdiki.yon);
            yigit.cikar();
            geriDonusSayisi++;
        }
    }

    if (konsol)
        imleciGoster(true);

    puts(bulundu ? "CIKISA GELINDI" : "CIKIS BULUNAMADI");
    return bulundu;
}

Yigit& Labirent::getYol() {
    return yigit;
}

int Labirent::getAdimSayisi() const {
    return adimSayisi;
}

int Labirent::getGeriDonusSayisi() const {
    return geriDonusSayisi;
}
