#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ODDELOVACE " (),.;:"
#define INPUTFILE "jizerska50.txt"
#define OUTPUTFILE "vysledkova_listina.txt"
#define DELKA 100

typedef struct {
    int minuty;
    int sekundy;
    int setiny;
} ZTRATY;

typedef struct {
    int cislo;
    char prijmeni[20];
    char jmeno[20];
    int rocnik;
    char stat[5];
    int minuty;
    int sekundy;
    int setiny;
    ZTRATY ztraty;
} SOUTEZICI;

SOUTEZICI *readfile(SOUTEZICI *retezec, int *pocet) {
    char pole[DELKA];
    SOUTEZICI *new_ptr;
    int i = 0;
    int j = 0;
    char *pch;
    FILE *fr = fopen(INPUTFILE, "r");
    if (fr == NULL) {
        printf("Nepodarilo se otevrit soubor %s.\n", INPUTFILE);
        return NULL;
    } else {
        while (fgets(pole, DELKA, fr) != NULL) {
            if (i != 0) {
                new_ptr = (SOUTEZICI *) realloc(retezec, sizeof(SOUTEZICI) * i);
                if (new_ptr == NULL) {
                    printf("Chyba realokace pametoveho bloku.\n");
                    free(retezec);
                } else {
                    retezec = new_ptr;
                    j = 0;
                    pch = strtok(pole, ODDELOVACE);
                    while (pch != NULL) {
                        switch (j) {
                            case 0:
                                retezec[i - 1].cislo = atoi(pch);
                                break;
                            case 1:
                                strcpy(retezec[i - 1].prijmeni, pch);
                                break;
                            case 2:
                                strcpy(retezec[i - 1].jmeno, pch);
                                break;
                            case 3:
                                retezec[i - 1].rocnik = atoi(pch);
                                break;
                            case 4:
                                strcpy(retezec[i - 1].stat, pch);
                                break;
                            case 5:
                                retezec[i - 1].minuty = atoi(pch);
                                break;
                            case 6:
                                retezec[i - 1].sekundy = atoi(pch);
                                break;
                            case 7:
                                retezec[i - 1].setiny = atoi(pch);
                                break;

                        }
                        pch = strtok(NULL, ODDELOVACE);
                        j++;
                    }
                }
            }
            i++;
        }
    }
    if (fclose(fr) == EOF) {
        printf("Nepodarilo se zavrit soubor %s.\n", INPUTFILE);
    }
    *pocet = i - 1;
    return retezec;
}

int prevodCasu(SOUTEZICI *soutezici, int i) {
    return soutezici[i].minuty * 6000 + soutezici[i].sekundy * 100 + soutezici[i].setiny;
}

void swap(SOUTEZICI *vyraz1, SOUTEZICI *vyraz2) {
    SOUTEZICI temp = *vyraz1;
    *vyraz1 = *vyraz2;
    *vyraz2 = temp;
}

void bubblesort(SOUTEZICI *soutezici, int n) {
    int i;
    int j;
    int swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        for (j = 0; j < n - 1 - i; j++) {
            if (prevodCasu(soutezici, j) > prevodCasu(soutezici, j + 1)) {
                swap(&soutezici[j], &soutezici[j + 1]);
                swapped = 1;
            }
        }
        if (swapped == 0) {
            break;
        }
    }
}

ZTRATY prevodZpet(int ms) {
    ZTRATY cas;
    cas.minuty = ms / 6000;
    cas.sekundy = (ms % 6000) / 100;
    cas.setiny = ms % 100;
    return cas;
}

void doSouboru(SOUTEZICI *retezec, int pocet) {
    FILE *fw = fopen(OUTPUTFILE, "w");
    if (fw == NULL) {
        printf("Nepodarilo se otevrit soubor %s.\n", OUTPUTFILE);
        return;
    }

    fprintf(fw, "V Y S L E D K O V A   L I S T I N A  -  J I Z E R S K A  5 0\n");
    fprintf(fw, "-----------------------------------------------------------------------\n");
    fprintf(fw, "poradi  cislo   prijmeni     jmeno      rocnik  stat    cas      ztraty\n");
    fprintf(fw, "-----------------------------------------------------------------------\n");

    int casViteze = prevodCasu(retezec, 0);

    for (int i = 0; i < pocet; i++) {
        int ms = prevodCasu(retezec, i) - casViteze;

        if (i != 0) {
            retezec[i].ztraty = prevodZpet(ms);
        } else {
            retezec[i].ztraty.minuty = 0;
            retezec[i].ztraty.sekundy = 0;
            retezec[i].ztraty.setiny = 0;
        }

        fprintf(fw, " %2d %8d %15s %10s %5d %5s %3d:%02d:%02d +%2d:%02d:%02d\n",
                i + 1,
                retezec[i].cislo,
                retezec[i].prijmeni,
                retezec[i].jmeno,
                retezec[i].rocnik,
                retezec[i].stat,
                retezec[i].minuty,
                retezec[i].sekundy,
                retezec[i].setiny,
                retezec[i].ztraty.minuty,
                retezec[i].ztraty.sekundy,
                retezec[i].ztraty.setiny);
    }

    fclose(fw);
    if (fw == EOF){
        printf("Soubor %s se nepodarilo zavrit", OUTPUTFILE);
    }
}

void vypis(SOUTEZICI *retezec, int pocet) {
    int nejmladsi = retezec[0].rocnik;
    int indexZavodnika = 0;
    int pocetCechu = 0;

    printf("S T A R T O V N I   L I S T I N A  -  J I Z E R S K A 5 0\n");
    printf("----------------------------------------------------------\n");
    printf(" startovni cislo    prijmeni        jmeno    rocnik   stat\n");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < pocet; i++) {
        if (retezec[i].rocnik > nejmladsi) {
            nejmladsi = retezec[i].rocnik;
            indexZavodnika = i;
        }
        if (strcmp(retezec[i].stat, "CZE") == 0) {
            pocetCechu++;
        }
        printf("%15d %15s %10s %5d %8s\n",
               retezec[i].cislo,
               retezec[i].prijmeni,
               retezec[i].jmeno,
               retezec[i].rocnik,
               retezec[i].stat);
    }

    printf("----------------------------------------------------------\n");
    printf("Pocet zavodniku : %d\n", pocet);
    printf("Nejmladsi zavodnik je %s %s ze zeme %s, je mu %d let.\n",
           retezec[indexZavodnika].jmeno,
           retezec[indexZavodnika].prijmeni,
           retezec[indexZavodnika].stat,
           2025 - retezec[indexZavodnika].rocnik);
    printf("Pocet Cechu : %d\n", pocetCechu);
}

int main() {
    SOUTEZICI *soutezici = NULL;
    int pocet;
    soutezici = readfile(soutezici, &pocet);
    vypis(soutezici, pocet);
    bubblesort(soutezici, pocet);
    doSouboru(soutezici, pocet);
    return 0;
}
