#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char ime[30];
    char prezime[30];
    int bodovi;
} Student;

int main(void) {
    FILE* datoteka = fopen("data.txt", "r");

    if (datoteka == NULL) {
        printf("Greska pri otvaranju datoteke.\n");
        return 1;
    }

    char redak[100];
    int brojStudenata = 0;

    while (fgets(redak, sizeof(redak), datoteka) != NULL) {
        brojStudenata++;
    }

    printf("Broj studenata: %d\n", brojStudenata);

    if (brojStudenata == 0) {
        fclose(datoteka);
        return 0;
    }

    Student* studenti = malloc(brojStudenata * sizeof(Student));

    if (studenti == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        fclose(datoteka);
        return 1;
    }

    rewind(datoteka);

    for (int i = 0; i < brojStudenata; i++) {
        if (fscanf(datoteka, "%29s %29s %d",
            studenti[i].ime,
            studenti[i].prezime,
            &studenti[i].bodovi) != 3) {
            free(studenti);
            fclose(datoteka);
            printf("Greska pri citanju podataka.\n");
            return 1;
        }
    }

    int najveciBrojBodova = studenti[0].bodovi;

    for (int i = 1; i < brojStudenata; i++) {
        if (studenti[i].bodovi > najveciBrojBodova) {
            najveciBrojBodova = studenti[i].bodovi;
        }
    }

    for (int i = 0; i < brojStudenata; i++) {
        float postotak = 0.0f;

        if (najveciBrojBodova > 0) {
            postotak = (float)studenti[i].bodovi
                / najveciBrojBodova * 100.0f;
        }

        printf("%s %s - %d, %.2f%%\n",
            studenti[i].ime,
            studenti[i].prezime,
            studenti[i].bodovi,
            postotak);
    }

    free(studenti);
    fclose(datoteka);

    return 0;
}