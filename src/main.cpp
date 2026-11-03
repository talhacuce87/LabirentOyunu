#include "Labirent.hpp"

int main() {
    Labirent labirent("Harita.txt");
    if (!labirent.yuklendiMi())
        return 1;

    return labirent.coz() ? 0 : 1;
}
