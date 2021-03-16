#include <stdio.h>
#include <stdbool.h>

#define TALIA 81
#define WIELKOSC_SETU 3
#define MIN_NA_STOLE 12
#define DZIELNIK 10

// wczytuje karty i zapisuje je w tablicy, a jako wynik podaje liczbę wczytanych kart
int wczytanie(int karty[], int indeks_poczatkowy, int ile_wczytac) {
    int x;
    int liczba_wczytanych = 0;
    while ((liczba_wczytanych < ile_wczytac) && (scanf("%d", &x) > 0)) {
        karty[indeks_poczatkowy + liczba_wczytanych] = x;
        liczba_wczytanych++;
    }
    return liczba_wczytanych;
}

// wyświetla karty odkryte obecnie na stole
void wyswietlanie_stolu(int karty[], int liczba_kart) {
    printf("=");
    for (int i = 0; i < liczba_kart; i++) {
        printf(" %d", karty[i]);
    }
    printf("\n");
}

// wypisuje karty tworzące set i w tablicy wstawia 0 na ich miejsce
void wypisanie_setu(int karty[], int a, int b, int c) {
    printf("- %d %d %d\n", karty[a], karty[b], karty[c]);
    karty[a] = 0;
    karty[b] = 0;
    karty[c] = 0;
}

// sprawdza czy podane 3 karty tworzą set
// zauważam że 1+1+1 = 3, 2+2+2 = 6, 3+3+3=9, 1+2+3=6, a inne kombinacje nie dają wyniku podzielnego przez 3
bool czy_spelnia_warunek(int karty[], int a, int b, int c) {
    int suma;
    for (int x = 1; x <= 1000; x *= DZIELNIK) {
        suma = (karty[a] / x) % DZIELNIK + (karty[b] / x) % DZIELNIK + (karty[c] / x) % DZIELNIK;
        if (suma % 3 != 0) {
            return false;
        }
    }
    return true;
}

// sprawdza każdą kombinacje kart po kolei, jeżeli trafimy na set to go wypisuje , a wynikiem jest true
bool szukanie_z_wypisaniem_setu(int karty[], int liczba_kart) {
    for (int i = 0; i < liczba_kart - 2; i++) {
        for (int j = i + 1; j < liczba_kart - 1; j++) {
            for (int k = j + 1; k < liczba_kart; k++) {
                if (czy_spelnia_warunek(karty, i, j, k)) {
                    wypisanie_setu(karty, i, j, k);
                    return true;
                }
            }
        }
    }
    return false;
}

// przesuwa odkryte karty tak aby wypełnić komórki które miały wartość 0
void przesuwanie_kart(int karty[], int liczba_kart) {
    int licznik_0 = 0;
    for (int i = 0; i < liczba_kart; i++) {
        while (karty[i + licznik_0] == 0) {
            licznik_0++;
        }
        karty[i] = karty[i + licznik_0];
    }
}

// przeprowadza grę
void gra_w_set() {
    int karty[TALIA];
    int liczba_kart = wczytanie(karty, 0, 12);

    bool czy_koniec = false;

    while (czy_koniec == false) {
        wyswietlanie_stolu(karty, liczba_kart);
        if (szukanie_z_wypisaniem_setu(karty, liczba_kart)) { //mamy set
            liczba_kart -= WIELKOSC_SETU;
            przesuwanie_kart(karty, liczba_kart);
            if (liczba_kart < MIN_NA_STOLE) {
                liczba_kart += wczytanie(karty, liczba_kart, 3);
            }
        } else if (wczytanie(karty, liczba_kart, WIELKOSC_SETU) > 0) { //nie mamy setu ale można dołożyć karty
            liczba_kart += WIELKOSC_SETU;
            printf("+\n");
        } else { //nie mamy setu nie możemy dołożyć kart
            czy_koniec = true;
        }

    }
}

int main(void) {

    gra_w_set();

    return 0;
}