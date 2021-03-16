#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Oznaczenia pól
 */
#define ROCKFORD '@'
#define PUSTE ' '
#define ZIEMIA '+'
#define KAMIEN 'O'
#define DIAMENT '$'
#define WYJSCIE 'X'
#define NOWA_LINIA '\n'

/**
 * Polecenia
 */
#define DRUKUJ '\n'
#define GORA 'w'
#define LEWO 'a'
#define DOL 's'
#define PRAWO 'd'

/**
 * Struktura do przechowywania współrzędnych na planszy
 * 'wiersz' to numer wiersza, od `0`
 * 'kolumna' to numer kolumny, od '0'
 */
typedef struct {
    size_t wiersz;
    size_t kolumna;
} wspolrzedne;

/**
 * Struktura do przechowywania ważnych informacji o planszy
 * 'l_wierszy' to liczba wierszy w planszy
 * 'l_kolumn' to liczba kolumn w planszy
 * 'l_diamentow' to aktualna licbza diamentów na plaszy
 * 'Rockford' to współrzędne na których znajduje się Rockford
 * 'czy_jest_Rockford' to informacja o tym czy na planszy znajduje się Rockford
 */
typedef struct {
    char **plansza;
    size_t l_wierszy;
    size_t l_kolumn;
    int l_diamentow;
    wspolrzedne Rockford;
    bool czy_jest_Rockford;
} stan;

/**
 * Funkcja wczytująca 2 liczby określające wymiary planszy
 */
void wczytaj_wymiary(stan *stan_planszy) {
    scanf("%zu%zu", &stan_planszy->l_wierszy, &stan_planszy->l_kolumn);
}

/**
 * Funkcja wczutująca wygląd planszy, pozycję Rockforada i liczbę diamentów
 */
void wczytaj_plansze(stan *stan_planszy) {
    for (size_t i = 0; i < stan_planszy->l_wierszy; i++) {
        getchar();
        for (size_t j = 0; j < stan_planszy->l_kolumn; j++) {
            stan_planszy->plansza[i][j] = (char) getchar();
            if (stan_planszy->plansza[i][j] == DIAMENT) {
                (stan_planszy->l_diamentow)++;
            }
            if (stan_planszy->plansza[i][j] == ROCKFORD) {
                stan_planszy->Rockford.wiersz = i;
                stan_planszy->Rockford.kolumna = j;
                stan_planszy->czy_jest_Rockford = true;
            }
        }
    }
}

/**
 * Funkcja wyświetlający aktualny stan planszy
 */
void wypisz_plansze(stan *stan_planszy) {
    printf("%zu %zu\n", stan_planszy->l_wierszy, stan_planszy->l_kolumn);
    for (size_t i = 0; i < stan_planszy->l_wierszy; i++) {
        for (size_t j = 0; j < stan_planszy->l_kolumn; j++) {
            putchar(stan_planszy->plansza[i][j]);
        }
        putchar(NOWA_LINIA);
    }
}

/**
 * Funkcja stabilizująca planszę po ruchu Rockforda
 */
void stabilizacja_planszy(stan *stan_planszy, size_t poczatek, size_t koniec) {
    for (size_t i = poczatek; i <= koniec; i++) {
        for (size_t j = stan_planszy->l_wierszy - 2; j > 0; j--) {
            size_t k = j;
            char znak = stan_planszy->plansza[k][i];
            while ((znak == KAMIEN || znak == DIAMENT) && (stan_planszy->plansza[k + 1][i] == PUSTE)) {
                stan_planszy->plansza[k + 1][i] = znak;
                stan_planszy->plansza[k][i] = PUSTE;
                k++;
            }
        }
    }
}

/**
 * Funkcja przygotowująca planszę przed rozgrywką
 */
void przygotowanie_planszy(stan *stan_planszy) {
    stan_planszy->l_diamentow = 0;
    stan_planszy->czy_jest_Rockford = false;
    wczytaj_wymiary(stan_planszy);
    stan_planszy->plansza = malloc(stan_planszy->l_wierszy * sizeof(char *));
    for (size_t i = 0; i < stan_planszy->l_wierszy; i++) {
        stan_planszy->plansza[i] = malloc(stan_planszy->l_kolumn * sizeof(char));
    }
    wczytaj_plansze(stan_planszy);
    if (stan_planszy->l_kolumn > 2) {
        stabilizacja_planszy(stan_planszy, 1, stan_planszy->l_kolumn-2);
    }
}

/**
 * Funkcja zwaniająca pamięć na koniec rozgrywki
 */
void zwolnienie_pamieci(stan *stan_planszy) {
    for (size_t i = 0; i < stan_planszy->l_wierszy; i++) {
        free(stan_planszy->plansza[i]);
    }
    free(stan_planszy->plansza);
}

/**
 * Funkcja ustawiająca wsþółrzędne na które ma się przemeścić Rockford
 * Jeśli znak nie odpowiada żadnemu ruchowi to wynikiem jest false
 */
bool wczytaj_ruch(char znak, wspolrzedne *docelowe, stan *stan_planszy) {
    docelowe->wiersz = stan_planszy->Rockford.wiersz;
    docelowe->kolumna = stan_planszy->Rockford.kolumna;
    switch (znak) {
        case LEWO:
            docelowe->kolumna -= 1;
            break;
        case PRAWO:
            docelowe->kolumna += 1;
            break;
        case GORA:
            docelowe->wiersz -= 1;
            break;
        case DOL:
            docelowe->wiersz += 1;
            break;
        default:
            return false;
    }
    return true;
}

/**
 * Funkcja przemieszczająca obiekt z jednego pola na inne
 * Może przemieścić Rockforda albo kamień
 */
void przemieszczenie(stan *stan_planszy, wspolrzedne skad, wspolrzedne dokad) {
    if (stan_planszy->plansza[dokad.wiersz][dokad.kolumna] != WYJSCIE) {
        stan_planszy->plansza[dokad.wiersz][dokad.kolumna] = stan_planszy->plansza[skad.wiersz][skad.kolumna];
    }
    stan_planszy->plansza[skad.wiersz][skad.kolumna] = PUSTE;
}

/**
 * Funkcja przemieszczająca Rockforda, zmieniająca jego zapisane współrzędne położenia
 * i stabilizująca planszę po ruchu
 */
void ruch_Rockforda(stan *stan_planszy, wspolrzedne docelowe) {
    przemieszczenie(stan_planszy, stan_planszy->Rockford, docelowe);
    stan_planszy->Rockford.wiersz = docelowe.wiersz;
    stan_planszy->Rockford.kolumna = docelowe.kolumna;
    stabilizacja_planszy(stan_planszy, docelowe.kolumna - 1, docelowe.kolumna + 1);
}

/**
 * Funkcja wykonująca ruch Rockforda jeżeli jest możliwy i aktualizująca stan planszy
 */
void ruch(stan *stan_planszy, wspolrzedne docelowe) {
    char znak = stan_planszy->plansza[docelowe.wiersz][docelowe.kolumna];
    if (znak == PUSTE || znak == ZIEMIA || znak == DIAMENT) {
        ruch_Rockforda(stan_planszy, docelowe);
        if (znak == DIAMENT) {
            (stan_planszy->l_diamentow)--;
        }
    } else if (znak == KAMIEN && docelowe.wiersz == stan_planszy->Rockford.wiersz) {
        wspolrzedne za_kamieniem;
        za_kamieniem.wiersz = docelowe.wiersz;
        za_kamieniem.kolumna = 2 * docelowe.kolumna - stan_planszy->Rockford.kolumna;
        if (stan_planszy->plansza[za_kamieniem.wiersz][za_kamieniem.kolumna] == PUSTE) {
            przemieszczenie(stan_planszy, docelowe, za_kamieniem);
            ruch_Rockforda(stan_planszy, docelowe);
        }
    } else if (znak == WYJSCIE && stan_planszy->l_diamentow == 0) {
        stan_planszy->czy_jest_Rockford = false;
        ruch_Rockforda(stan_planszy, docelowe);
    }
}


/**
 * Funkcja przeprowadzająca całą rozgrywkę
 */
void gra() {

    stan stan_planszy;
    przygotowanie_planszy(&stan_planszy);

    char znak;
    wspolrzedne docelowe;
    while (scanf("%c", &znak) != EOF) {
        if (znak == DRUKUJ) {
            wypisz_plansze(&stan_planszy);
        } else if (stan_planszy.czy_jest_Rockford && wczytaj_ruch(znak, &docelowe, &stan_planszy)) {
            ruch(&stan_planszy, docelowe);
        }
    }

    zwolnienie_pamieci(&stan_planszy);
}


int main(void) {
    gra();
    return 0;
}
