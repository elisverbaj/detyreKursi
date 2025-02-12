#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_KURSET 100
#define MAX_STUDENTET 100
#define MAX_INSTRUKTORET 50
#define MAX_REGJISTRIMET 600

typedef struct {
    int id;
    char emri[50];
    char mbiemri[50];
    char ditelindja[11];
} Student;

typedef struct {
    int id;
    char emri[50];
    char mbiemri[50];
    char ditelindja[11];
    char kategorite[3][20];  // Robotike, Programim, Dizajn Grafik
} Instruktor;

typedef struct {
    int id;
    char data_fillimit[11];
    char ditet[20];  // Ditet kur zhvillohet kursi
    char ora_fillimit[6];
    char kategoria[20];
    int instruktor_id;
    int studentet_regjistruar[6];
    int numri_studentet;
} Kurs;

Student studentet[MAX_STUDENTET];
Instruktor instruktoret[MAX_INSTRUKTORET];
Kurs kurset[MAX_KURSET];

int numri_studentet = 0;
int numri_instruktoret = 0;
int numri_kurset = 0;

void ruaj_studentet() {
    FILE *file = fopen("student.dat", "wb");
    if (file == NULL) {
        perror("Nuk mund të hapet skedari për shkrim");
        return;
    }
    fwrite(studentet, sizeof(Student), numri_studentet, file);
    fclose(file);
}

void lexo_studentet() {
    FILE *file = fopen("student.dat", "rb");
    if (file) {
        numri_studentet = fread(studentet, sizeof(Student), MAX_STUDENTET, file);
        fclose(file);
    }
}

void ruaj_instruktoret() {
    FILE *file = fopen("instruktor.dat", "wb");
    if (file == NULL) {
        perror("Nuk mund të hapet skedari për shkrim");
        return;
    }
    fwrite(instruktoret, sizeof(Instruktor), numri_instruktoret, file);
    fclose(file);
}

void lexo_instruktoret() {
    FILE *file = fopen("instruktor.dat", "rb");
    if (file) {
        numri_instruktoret = fread(instruktoret, sizeof(Instruktor), MAX_INSTRUKTORET, file);
        fclose(file);
    }
}

void ruaj_kurset() {
    FILE *file = fopen("kurs.dat", "wb");
    if (file == NULL) {
        perror("Nuk mund të hapet skedari për shkrim");
        return;
    }
    fwrite(kurset, sizeof(Kurs), numri_kurset, file);
    fclose(file);
}

void lexo_kurset() {
    FILE *file = fopen("kurs.dat", "rb");
    if (file) {
        numri_kurset = fread(kurset, sizeof(Kurs), MAX_KURSET, file);
        fclose(file);
    }
}

void custom_strptime(const char *date_str, struct tm *tm) {
    sscanf(date_str, "%d/%d/%d", &tm->tm_mday, &tm->tm_mon, &tm->tm_year);
    tm->tm_mon -= 1;  // Muajt që nga janari
    tm->tm_year -= 1900;  // Vitet që nga 1900
}

int llogarit_moshen(const char *ditelindja) {
    struct tm tm = {0};
    time_t t = time(NULL);
    struct tm *now = localtime(&t);
    custom_strptime(ditelindja, &tm);
    int mosha = now->tm_year - tm.tm_year;
    if (now->tm_mon < tm.tm_mon || (now->tm_mon == tm.tm_mon && now->tm_mday < tm.tm_mday)) {
        mosha--;
    }
    return mosha;
}

void regjistro_student() {
    Student s;
    printf("Jepni ID: ");
    scanf("%d", &s.id);
    printf("Jepni Emrin: ");
    scanf("%s", s.emri);
    printf("Jepni Mbiemrin: ");
    scanf("%s", s.mbiemri);
    printf("Jepni Ditëlindjen (dd/mm/vvvv): ");
    scanf("%s", s.ditelindja);

    int mosha = llogarit_moshen(s.ditelindja);
    if (mosha < 5 || mosha > 14) {
        printf("Ky student nuk mund të regjistrohet pasi mosha e tij nuk është brenda intervalit të lejuar!\n");
        return;
    }

    studentet[numri_studentet++] = s;
    ruaj_studentet();
    printf("Studenti u regjistrua me sukses!\n");
}

void krijo_instruktor() {
    Instruktor i;
    printf("Jepni ID: ");
    scanf("%d", &i.id);
    printf("Jepni Emrin: ");
    scanf("%s", i.emri);
    printf("Jepni Mbiemrin: ");
    scanf("%s", i.mbiemri);
    printf("Jepni Ditëlindjen (dd/mm/vvvv): ");
    scanf("%s", i.ditelindja);

    printf("Jepni Kategoritë (Robotike, Programim, Dizajn Grafik), përfundoni me 'done':\n");
    for (int j = 0; j < 3; j++) {
        scanf("%s", i.kategorite[j]);
        if (strcmp(i.kategorite[j], "done") == 0) {
            strcpy(i.kategorite[j], "");
            break;
        }
    }

    instruktoret[numri_instruktoret++] = i;
    ruaj_instruktoret();
    printf("Instruktori u krijua me sukses!\n");
}

void krijo_kurs() {
    Kurs c;
    printf("Jepni ID: ");
    scanf("%d", &c.id);
    printf("Jepni Datën e Fillimit (dd/mm/vvvv): ");
    scanf("%s", c.data_fillimit);
    printf("Jepni Ditet (p.sh., Hën Mër): ");
    scanf("%s", c.ditet);
    printf("Jepni Orën e Fillimit (hh:mm): ");
    scanf("%s", c.ora_fillimit);
    printf("Jepni Kategorinë (Robotike, Programim, Dizajn Grafik): ");
    scanf("%s", c.kategoria);
    printf("Jepni ID-në e Instruktorit: ");
    scanf("%d", &c.instruktor_id);

    int instruktori_gjetur = 0;
    for (int i = 0; i < numri_instruktoret; i++) {
        if (instruktoret[i].id == c.instruktor_id) {
            instruktori_gjetur = 1;
            int kategoria_pershtatur = 0;
            for (int j = 0; j < 3; j++) {
                if (strcmp(instruktoret[i].kategorite[j], c.kategoria) == 0) {
                    kategoria_pershtatur = 1;
                    break;
                }
            }
            if (!kategoria_pershtatur) {
                printf("Ky instruktor nuk mund të zhvillojë këtë kurs, ju lutemi zgjidhni një instruktor tjetër!\n");
                return;
            }
        }
    }

    if (!instruktori_gjetur) {
        printf("Instruktor nuk u gjet!\n");
        return;
    }

    c.numri_studentet = 0;
    kurset[numri_kurset++] = c;
    ruaj_kurset();
    printf("Kursi u krijua me sukses!\n");
}

void regjistro_student_ne_kurs() {
    int student_id, kurs_id;
    printf("Jepni ID-në e Studentit: ");
    scanf("%d", &student_id);
    printf("Jepni ID-në e Kursit: ");
    scanf("%d", &kurs_id);

    int student_index = -1, kurs_index = -1;
    for (int i = 0; i < numri_studentet; i++) {
        if (studentet[i].id == student_id) {
            student_index = i;
            break;
        }
    }

    if (student_index == -1) {
        printf("Studenti nuk u gjet!\n");
        return;
    }

    for (int i = 0; i < numri_kurset; i++) {
        if (kurset[i].id == kurs_id) {
            kurs_index = i;
            break;
        }
    }

    if (kurs_index == -1) {
        printf("Kursi nuk u gjet!\n");
        return;
    }

    if (kurset[kurs_index].numri_studentet >= 6) {
        printf("Ky kurs është plotësisht i mbushur, ju lutemi zgjidhni një kurs tjetër!\n");
        return;
    }

    kurset[kurs_index].studentet_regjistruar[kurset[kurs_index].numri_studentet++] = student_id;
    ruaj_kurset();
    printf("Studenti u regjistrua në kurs me sukses!\n");
}

void fshi_instruktor() {
    int instruktor_id;
    printf("Jepni ID-në e Instruktorit për të fshirë: ");
    scanf("%d", &instruktor_id);

    for (int i = 0; i < numri_kurset; i++) {
        if (kurset[i].instruktor_id == instruktor_id) {
            printf("Ky instruktor nuk mund të fshihet pasi ai/ajo po zhvillon një kurs!\n");
            return;
        }
    }

    int index = -1;
    for (int i = 0; i < numri_instruktoret; i++) {
        if (instruktoret[i].id == instruktor_id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Instruktor nuk u gjet!\n");
        return;
    }

    for (int i = index; i < numri_instruktoret - 1; i++) {
        instruktoret[i] = instruktoret[i + 1];
    }

    numri_instruktoret--;
    ruaj_instruktoret();
    printf("Instruktori u fshi me sukses!\n");
}

void fshi_kurs() {
    int kurs_id;
    printf("Jepni ID-në e Kursit për të fshirë: ");
    scanf("%d", &kurs_id);

    int index = -1;
    for (int i = 0; i < numri_kurset; i++) {
        if (kurset[i].id == kurs_id) {
            if (kurset[i].numri_studentet > 0) {
                printf("Ky kurs nuk mund të fshihet pasi studentët janë të regjistruar në të!\n");
                return;
            }
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Kurs nuk u gjet!\n");
        return;
    }

    for (int i = index; i < numri_kurset - 1; i++) {
        kurset[i] = kurset[i + 1];
    }

    numri_kurset--;
    ruaj_kurset();
    printf("Kursi u fshi me sukses!\n");
}

void c_regjistro_student_nga_kurs() {
    int student_id, kurs_id;
    printf("Jepni ID-në e Studentit për të çregjistruar: ");
    scanf("%d", &student_id);
    printf("Jepni ID-në e Kursit: ");
    scanf("%d", &kurs_id);

    int kurs_index = -1;
    for (int i = 0; i < numri_kurset; i++) {
        if (kurset[i].id == kurs_id) {
            kurs_index = i;
            break;
        }
    }

    if (kurs_index == -1) {
        printf("Kurs nuk u gjet!\n");
        return;
    }

    int student_index = -1;
    for (int i = 0; i < kurset[kurs_index].numri_studentet; i++) {
        if (kurset[kurs_index].studentet_regjistruar[i] == student_id) {
            student_index = i;
            break;
        }
    }

    if (student_index == -1) {
        printf("Studenti nuk është regjistruar në këtë kurs!\n");
        return;
    }

    for (int i = student_index; i < kurset[kurs_index].numri_studentet - 1; i++) {
        kurset[kurs_index].studentet_regjistruar[i] = kurset[kurs_index].studentet_regjistruar[i + 1];
    }

    kurset[kurs_index].numri_studentet--;
    ruaj_kurset();
    printf("Studenti u çregjistrua nga kursi me sukses!\n");
}

void listoi_te_gjitha_kurset() {
    printf("Kurset:\n");
    for (int i = 0; i < numri_kurset; i++) {
        printf("ID: %d, Data e Fillimit: %s, Ditet: %s, Ora e Fillimit: %s, Kategoria: %s, Instruktor ID: %d, Studentë: %d\n",
               kurset[i].id, kurset[i].data_fillimit, kurset[i].ditet, kurset[i].ora_fillimit, kurset[i].kategoria,
               kurset[i].instruktor_id, kurset[i].numri_studentet);
    }
}

void listoi_te_gjithe_studentet() {
    printf("Studentët:\n");
    for (int i = 0; i < numri_studentet; i++) {
        printf("ID: %d, Emri: %s, Mbiemri: %s, Ditëlindja: %s\n",
               studentet[i].id, studentet[i].emri, studentet[i].mbiemri, studentet[i].ditelindja);
    }
}

void listoi_te_gjithe_instruktoret() {
    printf("Instruktorët:\n");
    for (int i = 0; i < numri_instruktoret; i++) {
        printf("ID: %d, Emri: %s, Mbiemri: %s, Ditëlindja: %s, Kategoritë: %s %s %s\n",
               instruktoret[i].id, instruktoret[i].emri, instruktoret[i].mbiemri, instruktoret[i].ditelindja,
               instruktoret[i].kategorite[0], instruktoret[i].kategorite[1], instruktoret[i].kategorite[2]);
    }
}

void listoi_studentet_ne_kurs() {
    int kurs_id;
    printf("Jepni ID-në e Kursit: ");
    scanf("%d", &kurs_id);

    int kurs_index = -1;
    for (int i = 0; i < numri_kurset; i++) {
        if (kurset[i].id == kurs_id) {
            kurs_index = i;
            break;
        }
    }

    if (kurs_index == -1) {
        printf("Kurs nuk u gjet!\n");
        return;
    }

    printf("Studentët në Kursin %d:\n", kurs_id);
    for (int i = 0; i < kurset[kurs_index].numri_studentet; i++) {
        int student_id = kurset[kurs_index].studentet_regjistruar[i];
        for (int j = 0; j < numri_studentet; j++) {
            if (studentet[j].id == student_id) {
                printf("ID: %d, Emri: %s, Mbiemri: %s, Ditëlindja: %s\n",
                       studentet[j].id, studentet[j].emri, studentet[j].mbiemri, studentet[j].ditelindja);
            }
        }
    }
}

void listoi_kurset_nga_instruktor() {
    int instruktor_id;
    printf("Jepni ID-në e Instruktorit: ");
    scanf("%d", &instruktor_id);

    printf("Kurset nga Instruktori %d:\n", instruktor_id);
    for (int i = 0; i < numri_kurset; i++) {
        if (kurset[i].instruktor_id == instruktor_id) {
            printf("ID: %d, Data e Fillimit: %s, Ditet: %s, Ora e Fillimit: %s, Kategoria: %s, Studentë: %d\n",
                   kurset[i].id, kurset[i].data_fillimit, kurset[i].ditet, kurset[i].ora_fillimit,
                   kurset[i].kategoria, kurset[i].numri_studentet);
        }
    }
}

void listoi_studentet_nga_instruktor() {
    int instruktor_id;
    printf("Jepni ID-në e Instruktorit: ");
    scanf("%d", &instruktor_id);

    printf("Studentët nga Instruktori %d:\n", instruktor_id);
    for (int i = 0; i < numri_kurset; i++) {
        if (kurset[i].instruktor_id == instruktor_id) {
            for (int j = 0; j < kurset[i].numri_studentet; j++) {
                int student_id = kurset[i].studentet_regjistruar[j];
                for (int k = 0; k < numri_studentet; k++) {
                    if (studentet[k].id == student_id) {
                        printf("ID: %d, Emri: %s, Mbiemri: %s, Ditëlindja: %s\n",
                               studentet[k].id, studentet[k].emri, studentet[k].mbiemri, studentet[k].ditelindja);
                    }
                }
            }
        }
    }
}

void menu() {
    int zgjedhje;
    do {
        printf("\nSistemi i Menaxhimit të Qendrës së Kurseve\n");
        printf("1. Regjistro Studente\n");
        printf("2. Krijo Instruktor\n");
        printf("3. Krijo Kurs\n");
        printf("4. Regjistro Student në Kurs\n");
        printf("5. Fshi Instruktor\n");
        printf("6. Fshi Kurs\n");
        printf("7. Çregjistro Student nga Kursi\n");
        printf("8. Listo të Gjitha Kurset\n");
        printf("9. Listo të Gjithë Studentët\n");
        printf("10. Listo të Gjithë Instruktorët\n");
        printf("11. Listo Studentët në një Kurs\n");
        printf("12. Listo Kurset nga Instruktor\n");
        printf("13. Listo Studentët nga Instruktor\n");
        printf("0. Dil\n");
        printf("Jepni zgjedhjen tuaj: ");
        scanf("%d", &zgjedhje);

        switch(zgjedhje) {
            case 1: regjistro_student(); break;
            case 2: krijo_instruktor(); break;
            case 3: krijo_kurs(); break;
            case 4: regjistro_student_ne_kurs(); break;
            case 5: fshi_instruktor(); break;
            case 6: fshi_kurs(); break;
            case 7: c_regjistro_student_nga_kurs(); break;
            case 8: listoi_te_gjitha_kurset(); break;
            case 9: listoi_te_gjithe_studentet(); break;
            case 10: listoi_te_gjithe_instruktoret(); break;
            case 11: listoi_studentet_ne_kurs(); break;
            case 12: listoi_kurset_nga_instruktor(); break;
            case 13: listoi_studentet_nga_instruktor(); break;
            case 0: exit(0);
            default: printf("Zgjedhje e pavlefshme! Ju lutemi provoni përsëri.\n");
        }
    } while(zgjedhje != 0);
}

int main() {
    lexo_studentet();
    lexo_instruktoret();
    lexo_kurset();
    menu();
    return 0;
}
