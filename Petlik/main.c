#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Stałe
 */
#define LICZBA_ZMIENNYCH 26
#define PODSTAWA 10000

#define WYPISZ '='
#define POCZATEK_PETLI '('
#define KONIEC_PETLI ')'
#define NOWA_LINIA '\n'
#define LITERA_A 'a'

/**
 * Typ do przechowywania informacji o typie instrukcji
 */
typedef enum typ_t {
    INC, ADD, CLR, JMP, DJZ, HLT
} typ_t;

/**
 * Struktura do przechowywania liczb z zakresu zmienncyh języka pętlik
 * 'dlugosc' to liczba cyfr w zapisie o podstawie 10000
 * '*cyfry' to dynamiczna tablica przechowująca cyfry liczby w zapisie o podstawie 10000
 */
typedef struct {
    size_t dlugosc;
    short *cyfry;
} liczby;

/**
 * Struktura do przechowywania instrukcji składających się na kod maszynowy
 * 'funkcja' to numer instrukcji którą należy wykonać
 * 'arg1' to pierwszy argument funkcji
 * 'arg2' to drugi argument funkcji
 */
typedef struct {
    enum typ_t funkcja;
    size_t arg1;
    size_t arg2;
} instrukcje;

/**
 * Struktura do przechowywania kodu maszynowego
 * '*tab' to dynamiczna tablica zawierająca kolejne instrukcje
 * 'max_rozmiar' to rozmiar zaalokowanej aktualnie pamięci w tablicy tab
 * 'akt' to numer aktualnie opracowywanej instrukcji
 */
typedef struct {
    instrukcje *tab;
    size_t max_rozmiar;
    size_t akt;
} kod_maszynowy;

/**
 * Funkcja przygotowywująca zmienne przed wykonywaniem poleceń
 */
void przygotuj_zmienne(liczby *zmienne) {
    for (size_t i = 0; i < LICZBA_ZMIENNYCH; i++) {
        zmienne[i].dlugosc = 1;
        zmienne[i].cyfry = malloc(zmienne[i].dlugosc * sizeof(short));
        zmienne[i].cyfry[0] = 0;
    }
}

/**
 * Funkcja zmienianiająca długość tablicy w której jest przechowywana dana zmienna
 */
void realokuj_zmienna(liczby *zmienna, size_t nowa_dlugosc) {
    zmienna->dlugosc = nowa_dlugosc;
    zmienna->cyfry = realloc(zmienna->cyfry, zmienna->dlugosc * sizeof *zmienna->cyfry);
}

/**
 * Funkcja zwalniająca pamięć na której były przechowywane zmienne
 */
void zwolnij_zmienne(liczby *zmienne) {
    for (size_t i = 0; i < LICZBA_ZMIENNYCH; i++) {
        free(zmienne[i].cyfry);
    }
}

/**
 * Funkcja przygotowywująca tablicę do przechowywania kodu maszynowego
 */
void przygotuj_kod(kod_maszynowy *kod) {
    kod->akt = 0;
    kod->max_rozmiar = 0;
    kod->tab = NULL;
}

/**
 * Funkcja zmienianiająca długość tablicy w której jest przechowywany kod maszynowy
 */
void realokuj_kod(kod_maszynowy *kod, size_t nowa_dlugosc) {
    kod->max_rozmiar = nowa_dlugosc;
    kod->tab = realloc(kod->tab, kod->max_rozmiar * sizeof *(kod->tab));
}

/**
 * Funkcja zwalniająca pamięć na której był zapisany kod maszynowy
 */
void zwolnij_kod(kod_maszynowy *kod) {
    free(kod->tab);
}

/**
 * Funkcja dodająca zmienną 2 do zmiennej 1
 */
void dodaj_dwie_zmienne(liczby *zmienna1, liczby *zmienna2) {
    if (zmienna1->dlugosc < zmienna2->dlugosc) {
        size_t dl_pierwszej = zmienna1->dlugosc;
        realokuj_zmienna(zmienna1, zmienna2->dlugosc);
        while (dl_pierwszej < zmienna1->dlugosc) {
            zmienna1->cyfry[dl_pierwszej] = 0;
            dl_pierwszej++;
        }
    }
    bool czy_dodawac = true;
    size_t i = 0;
    while (czy_dodawac) {
        if (i < zmienna2->dlugosc) {
            zmienna1->cyfry[i] = (short) (zmienna1->cyfry[i] + zmienna2->cyfry[i]);
        }
        if (zmienna1->cyfry[i] >= PODSTAWA) {
            zmienna1->cyfry[i] = (short) (zmienna1->cyfry[i] - PODSTAWA);
            if (i + 1 == zmienna1->dlugosc) {
                realokuj_zmienna(zmienna1, zmienna1->dlugosc + 1);
                zmienna1->cyfry[i + 1] = 0;
            }
            (zmienna1->cyfry[i + 1])++;
        } else if (i + 1 >= zmienna2->dlugosc) {
            czy_dodawac = false;
        }
        i++;
    }
}

/**
 * Funkcja dodająca 1 do podanej zmiennej
 */
void dodaj_jeden(liczby *zmienna) {
    bool czy_dodawac = true;
    size_t i = 0;
    (zmienna->cyfry[i])++;
    while (czy_dodawac) {
        if (zmienna->cyfry[i] == PODSTAWA) {
            zmienna->cyfry[i] = 0;
            i++;
            if (i == zmienna->dlugosc) {
                realokuj_zmienna(zmienna, zmienna->dlugosc + 1);
                zmienna->cyfry[i] = 0;
            }
            (zmienna->cyfry[i])++;
        } else {
            czy_dodawac = false;
        }
    }
}

/**
 * Funkcja odejmująca 1 od podanej zmiennej
 */
void odejmij_jeden(liczby *zmienna) {
    bool czy_odejmowac = true;
    size_t i = 0;
    while (czy_odejmowac) {
        if (zmienna->cyfry[i] == 0) {
            zmienna->cyfry[i] = PODSTAWA - 1;
            i++;
        } else {
            (zmienna->cyfry[i])--;
            czy_odejmowac = false;
        }
    }
    if ((zmienna->dlugosc > 1) && (zmienna->cyfry[zmienna->dlugosc - 1] == 0)) {
        realokuj_zmienna(zmienna, zmienna->dlugosc - 1);
    }
}

/**
 * Funkcja nadająca podanej zmiennej wartość 0
 */
void zeruj(liczby *zmienna) {
    realokuj_zmienna(zmienna, 1);
    zmienna->cyfry[0] = 0;
}

/**
 * Funkcja wykonująca instrukcje zapisane w kodzie maszynowym
 */
void zrealizuj_kod(kod_maszynowy *kod, liczby *zmienne) {
    size_t i = 0;
    bool koniec = false;
    while (!koniec) {
        switch (kod->tab[i].funkcja) {
            case INC:
                dodaj_jeden(&zmienne[kod->tab[i].arg1]);
                i++;
                break;
            case ADD:
                dodaj_dwie_zmienne(&zmienne[kod->tab[i].arg1], &zmienne[kod->tab[i].arg2]);
                i++;
                break;
            case CLR:
                zeruj(&zmienne[kod->tab[i].arg1]);
                i++;
                break;
            case JMP:
                i = kod->tab[i].arg1;
                break;
            case DJZ:
                if ((zmienne[kod->tab[i].arg1].dlugosc > 1) || (zmienne[kod->tab[i].arg1].cyfry[0] != 0)) {
                    odejmij_jeden(&zmienne[kod->tab[i].arg1]);
                    i++;
                } else {
                    i = kod->tab[i].arg2;
                }
                break;
            case HLT:
                koniec = true;
        }
    }
}

/**
 * Funkcja zapisująca typ i argument instrukcji na wskazanym miejscu w kodzie maszynowym
 */
void zapisz_instrukcje(kod_maszynowy *kod, typ_t funkcja, size_t arg1) {
    kod->tab[kod->akt].funkcja = funkcja;
    kod->tab[kod->akt].arg1 = arg1;
    (kod->akt)++;
}

/**
 * Funkcja wykonująca optymalizację kodu maszynowego
 */
void zrob_optymalizacje(kod_maszynowy *kod, size_t zapisane) {
    size_t koniec = kod->akt - 1;
    kod->akt = zapisane;
    size_t warunek = kod->tab[zapisane].arg1;
    while (kod->akt < koniec) {
        kod->tab[kod->akt].arg2 = warunek;
        zapisz_instrukcje(kod, ADD, kod->tab[kod->akt + 1].arg1);
    }
    zapisz_instrukcje(kod, CLR, warunek);
}

/**
 * Funkcja wczytująca kod programu w języku pętlik i zamieniająca go na kod maszynowy
 */
void kompiluj(kod_maszynowy *kod, size_t zapisane) {
    bool optymalizacja = true;
    size_t znak;
    while ((znak = (size_t) getchar()) != NOWA_LINIA) {
        if (kod->akt == kod->max_rozmiar) {
            realokuj_kod(kod, 2 * kod->max_rozmiar + 1);
        }
        if (znak == POCZATEK_PETLI) {
            optymalizacja = false;
            znak = (size_t) getchar() - LITERA_A;
            zapisz_instrukcje(kod, DJZ, znak);
            kompiluj(kod, (kod->akt) - 1);
        } else if (znak == KONIEC_PETLI) {
            if (optymalizacja == true) {
                zrob_optymalizacje(kod, zapisane);
            } else {
                zapisz_instrukcje(kod, JMP, zapisane);
                kod->tab[zapisane].arg2 = kod->akt;
            }
            return;
        } else {
            if ((zapisane < kod->akt) && (znak == kod->tab[zapisane].arg1 + LITERA_A)) {
                optymalizacja = false;
            }
            zapisz_instrukcje(kod, INC, (size_t) (znak - LITERA_A));
        }
    }
    if (kod->akt == kod->max_rozmiar) {
        realokuj_kod(kod, kod->max_rozmiar + 1);
    }
    kod->tab[kod->akt].funkcja = HLT;
}

/**
 * Funkcja kompilująca i wykonująca cały program z języku pętlik
 */
void wykonaj_program(liczby zmienne[]) {
    kod_maszynowy kod;
    przygotuj_kod(&kod);
    kompiluj(&kod, 0);
    zrealizuj_kod(&kod, zmienne);
    zwolnij_kod(&kod);
}

/**
 * Funkcja wypisująca podaną zmienną
 */
void wypisz_zmienna(liczby *zmienna) {
    printf("%hd", zmienna->cyfry[zmienna->dlugosc - 1]);
    for (int i = (int) zmienna->dlugosc - 2; i >= 0; i--) {
        printf("%04hd", zmienna->cyfry[i]);
    }
    putchar(NOWA_LINIA);
}

/**
 * Funkcja obsługująca zadanie
 * Umożliwia wykonywanie programów w języku pętlik oraz wyświetlanie wartości zmiennych a-z
 */
void zrealizuj_zadanie() {
    liczby zmienne[LICZBA_ZMIENNYCH];
    przygotuj_zmienne(zmienne);
    int znak;
    while ((znak = getchar()) != EOF) {
        if (znak == WYPISZ) {
            int numer = getchar() - LITERA_A;
            wypisz_zmienna(&zmienne[numer]);
            getchar();
        } else {
            ungetc(znak, stdin);
            wykonaj_program(zmienne);
        }
    }
    zwolnij_zmienne(zmienne);
}

int main(void) {
    zrealizuj_zadanie();
    return 0;
}
