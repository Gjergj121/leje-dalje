#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct shtetas{
    char idnr[15];
    char emer[30];
    char mbiemer[30];
    int ditelindja; 
    int id_familja;
}*shtetasptr;
typedef struct shtetas Shtetas;

struct data {
    int dita;
    int muaji;
    int viti;
};
typedef struct data Data; 

struct orari{
    int ora;
    int minutat;
};
typedef struct orari Orari;

const int size_persona_leje = 10000;
struct leje{
    Orari time;
    Data data_lejes;
    char id[15];
}*persona_leje[size_persona_leje];
typedef struct leje Leje;
int size_leje[size_persona_leje];// size i cdo pointeri

FILE *fptr; // krijojme nje pointer qe pointon ne file-in ku ruhen te dhenat e shtetasve
FILE *fptr2; // krijoj pointerin per file-in e ri qe do te mbaje leje-daljet

int counter = 0, size = 5000; // variabla global
/* prototipet e funksioneve */
int menu();
void lexo_shtetasit();
void lexo_lejedalje();
void perzgjedhja();
void shto_shtetas();
void shto_shtetas(Shtetas qytetar);
void ndrysho_shtetas();
void ndrysho_shtetas(Shtetas qytetar);
void marr_lejedalje();
int mosha_lejuar(Shtetas q, Data d);
int aprovimi_lejes(Shtetas qytetar, Data d, Orari o);
void kontrollo_lejedalje();
void kontrollo_ne_file(char shtetas_id[], Data shtetas_data, Orari shtetas_ora);
void ruaj_ndryshimet();
void ruaj_shtetasit();
void ruaj_lejedaljet_file();
void printimi_lejedaljeve();
int menu_printimi_lejedaljeve();
void printo_shtetas();
void printo_familje();
void printo_tegjithe();
void printo_shtetas_sipas_lejedaljeve();
void printo_dhjete_me_shpesh(int banues[]);
void printo_dhjete_me_pak(int banues[]);

int main(){
    lexo_shtetasit();
    lexo_lejedalje();
    perzgjedhja();
    return 0;
}

void lexo_shtetasit() {
    if( (fptr=fopen("detyra_data.txt", "r+")) == NULL ){
        printf("Gabim ne hapjen e file-it");
        return;
    }
    shtetasptr = (Shtetas*) malloc(size*sizeof(Shtetas)); // alokoj memorjen

    /*Elementet e file-it i ruaj ne pointerin shtetasptr */
    fseek(fptr, 37, SEEK_SET);// anashkaloj rreshtin e pare te file-it
    while(!feof(fptr)){ 
        fscanf(fptr, "%s", (shtetasptr+counter)->idnr );
        fscanf(fptr, "%s", (shtetasptr+counter)->emer );
        fscanf(fptr, "%s", (shtetasptr+counter)->mbiemer );
        fscanf(fptr, "%d", &(shtetasptr+counter)->ditelindja );
        fscanf(fptr, "%d", &(shtetasptr+counter)->id_familja );
        counter++;
        if(counter == size){// nese memorja e alokuar eshte e zene, e dyfishojme
            size *= 2;
            shtetasptr = (Shtetas*) realloc(shtetasptr, size*sizeof(Shtetas));
        }
    }

    fclose(fptr);
}

void lexo_lejedalje() {
    for(int i = 0; i < size_persona_leje; i++){ // alokoj memorien per secilin pointer te leje-daljes
        size_leje[i] = 1;
        persona_leje[i] = (Leje*) malloc(sizeof(Leje));
    }
    
    Data d;
    Orari o;
    int id_familja;
    char idnr[15];
    
    if( (fptr2 = fopen("LejeDaljet.txt", "r+")) == NULL){
        printf("File nuk ekziston");
        return;
    }

    while(!feof(fptr2)){
        
        int check = fscanf(fptr2, "%s %d %d %d %d %d %d", idnr, &id_familja, &d.dita, &d.muaji, &d.viti, &o.ora, &o.minutat);

        if (check == 1){
            persona_leje[id_familja]->data_lejes.dita = d.dita;
            persona_leje[id_familja]->data_lejes.muaji = d.muaji;
            persona_leje[id_familja]->data_lejes.viti = d.viti;
            persona_leje[id_familja]->time.ora = o.ora;
            persona_leje[id_familja]->time.minutat = o.minutat;
            strcpy(persona_leje[id_familja]->id, idnr);

            size_leje[id_familja]++;
            persona_leje[id_familja] = (Leje*) realloc(persona_leje[id_familja], size_leje[id_familja]*sizeof(Leje));
        }
        
    }
    
    fclose(fptr2);
}

int menu(){
    int choice;
    printf("\n-----------------------------------------");
    printf("\n              Detyre  Kursi\n");
    printf("\n1 - Shto nje shtetas te ri");
    printf("\n2 - Ndrysho nje shtetas");
    printf("\n3 - Apliko per Leje Dalje");
    printf("\n4 - Kontrollo nese nje shtetas ka leje");
    printf("\n5 - Printo leje daljet");
    printf("\n6 - Printo listen e 10 shtetasve\n    qe kane levizur me shpesh dhe\n    ato 10 qe kane levizur me pak");
    printf("\n0 - Mbyll programin");
    printf("\n-----------------------------------------\n");
    printf("\nZgjedhja juaj: ");
    scanf("%d", &choice);
    return choice;
}

void perzgjedhja(){
    int zgjedhja = menu();

    while(1){
        switch(zgjedhja){
            case 0:
               ruaj_ndryshimet();
                return;
            case 1:
                shto_shtetas();
                break;
            case 2:
                ndrysho_shtetas();
                break;
            case 3:
                marr_lejedalje();
                break;
            case 4:
                kontrollo_lejedalje();
                break;
            case 5:
                printimi_lejedaljeve();
                break;
            case 6:
                printo_shtetas_sipas_lejedaljeve();
                break;
            default:
                printf("Zgjedhje e gabuar!\n");
                break;
        }
        printf("\nZgjedhja juaj: ");
        scanf("%d", &zgjedhja);
    }
}

void shto_shtetas(){
    Shtetas qytetar;
    printf("Vendosni IDNR: ");
    scanf("%s", qytetar.idnr);
    printf("Vendosni emrin: ");
    scanf("%s", qytetar.emer);
    printf("Vendosni mbiemrin: ");
    scanf("%s", qytetar.mbiemer);
    printf("Vendosni datelindjen (VVVVMMDD): ");
    scanf("%d", &qytetar.ditelindja);
    printf("Vendosni ID e familjes: ");
    scanf("%d", &qytetar.id_familja);
    int i;
    for(i = 0; i < counter; i++){//kontrolloj nese shtetasi ndodhet ne liste
        if( strcmp(qytetar.idnr,(shtetasptr+i)->idnr) == 0){
            printf("\nKy shtetas ndodhet ne liste\n");
            break;
        }
    }
    if(i == counter){ // tregon qe shtetasi eshte unik
        shto_shtetas(qytetar);
    } 
}

void shto_shtetas(Shtetas qytetar){
    int j, k;
    for(j = counter-1; j >= 0; j--){
            if(qytetar.id_familja == (shtetasptr+j)->id_familja ){
                for(k = counter-1; k > j; k--){
                    *(shtetasptr+k+1) = *(shtetasptr+k);
                }
                *(shtetasptr+j+1) = qytetar;
                break;
            }
        }
        if(j == -1){
            *(shtetasptr+counter) = qytetar;
        }
        counter++;
        if(counter == size){// nese memorja e alokuar eshte e zene, e dyfishojme
            size *= 2;
            shtetasptr = (Shtetas*) realloc(shtetasptr, size*sizeof(Shtetas));
        }
        return;
}

void ndrysho_shtetas(){
    Shtetas qytetar;
    printf("Vendosni IDNR e shtetasit : ");
    scanf("%s", qytetar.idnr);
    printf("Ndryshoni te dhenat :\n");
    printf("Vendosni emrin : ");
    scanf("%s", qytetar.emer);
    printf("Vendosni mbiemrin : ");
    scanf("%s", qytetar.mbiemer);
    printf("Vendosni datelindjen (VVVVMMDD) : ");
    scanf("%d", &qytetar.ditelindja);
    printf("Vendosni ID e familjes : ");
    scanf("%d", &qytetar.id_familja);

    ndrysho_shtetas(qytetar);
    return;
}

void ndrysho_shtetas(Shtetas qytetar){
    int i, j, k;
    for(i = 0; i < counter; i++){
        if(strcmp(qytetar.idnr, (shtetasptr+i)->idnr) == 0){
            strcpy((shtetasptr+i)->emer, qytetar.emer);
            strcpy((shtetasptr+i)->mbiemer, qytetar.mbiemer);
            (shtetasptr+i)->ditelindja = qytetar.ditelindja;
            (shtetasptr+i)->id_familja = qytetar.id_familja;
            /* e vendos ne vendin e duhur ne liste duke perdorur Bubble sort */
            Shtetas temp;
            for(j = 0; j < counter-1; j++){
                for(k = 0; k < counter-1-j; k++){
                    if( (shtetasptr+k)->id_familja > (shtetasptr+k+1)->id_familja ){
                            temp = *(shtetasptr+k);
                            *(shtetasptr+k) = *(shtetasptr+k+1);
                            *(shtetasptr+k+1) = temp;
                    }
                }
            }
            return;
        }
    } 
    printf("Ky shtetas nuk ndodhet ne sistem!\n");
    return;
}


void marr_lejedalje(){
    Shtetas qytetar;
    Data d;
    printf("Vendosni ID e personit qe deshiron leje-dalje: ");
    scanf("%s", qytetar.idnr);
    int i;
    Orari o;
    /* kontrolloj nese ekziston ky shtetas */
    for(i = 0; i < counter; i++){
        if(strcmp( (shtetasptr+i)->idnr, qytetar.idnr ) == 0){
            strcpy(qytetar.emer, (shtetasptr+i)->emer);
            strcpy(qytetar.mbiemer, (shtetasptr+i)->mbiemer);
            qytetar.ditelindja = (shtetasptr+i)->ditelindja;
            qytetar.id_familja = (shtetasptr+i)->id_familja;
            break;
        }
    }
    if(i == counter){
        printf("Ky shtetas nuk ndodhet ne sistem!");
        return;
    }
    printf("Vendosni daten (diten muajin vitin) e daljes: ");
    scanf("%d %d %d",&d.dita, &d.muaji, &d.viti);
    if( (d.dita<1 || d.dita>31) || (d.muaji<1 || d.muaji>12) || (d.viti!=2020) ){
        printf("Date e papranueshme!");
        return;
    }
    printf("Vendosni oren (8 00-15 00): "); 
    scanf("%d %d", &o.ora, &o.minutat);
    if( (o.ora < 8 || o.ora > 15) || (o.minutat < 0 || o.minutat >= 60) ){
        printf("Orar i papranueshem!");
        return;
    }
    int mosha = mosha_lejuar(qytetar, d); 
    if(mosha == 0){
        printf("Shtetasi eshte nen 18 vjec!");
        return;
    }
    int aprovim = aprovimi_lejes(qytetar, d, o);
    if(aprovim == 0){
        printf("Eshte dhene nje leje per familjen tuaj ne kete dite!");
        return;
    }
    return;
}

int mosha_lejuar(Shtetas q, Data d){
    Data dtl;
    dtl.dita = q.ditelindja % 10;
    q.ditelindja /= 10;
    dtl.dita += (q.ditelindja % 10)*10;
    q.ditelindja /= 10;
    dtl.muaji = q.ditelindja % 10;
    q.ditelindja /= 10;
    dtl.muaji += (q.ditelindja % 10)*10;
    q.ditelindja /= 10;
    dtl.viti = q.ditelindja;

    if(d.viti-dtl.viti > 18){
        return 1;
    }
    else if (d.viti-dtl.viti < 18){
        return 0;
    }
    else {
        if(d.muaji > dtl.muaji){
            return 1;
        }
        else if (d.muaji < dtl.muaji){
            return 0;
        }
        else {
            if(d.dita >= dtl.dita){
                return 1;
            }
            else{
                return 0;
            }
        }
    }
}
int aprovimi_lejes(Shtetas qytetar, Data d, Orari o){
    int i;
    for(i = 0; i < size_leje[qytetar.id_familja]; i++){
        if(d.viti == (persona_leje[qytetar.id_familja]+i)->data_lejes.viti && d.muaji  == (persona_leje[qytetar.id_familja]+i)->data_lejes.muaji && d.dita == (persona_leje[qytetar.id_familja]+i)->data_lejes.dita ){
            return 0;
        }
    }
    // Nuk u gjet asnje pjestar i familjes me leje ne ate date
    (persona_leje[qytetar.id_familja]+i-1)->data_lejes.dita = d.dita;
    (persona_leje[qytetar.id_familja]+i-1)->data_lejes.muaji = d.muaji;
    (persona_leje[qytetar.id_familja]+i-1)->data_lejes.viti = d.viti;
    (persona_leje[qytetar.id_familja]+i-1)->time.ora = o.ora;
    (persona_leje[qytetar.id_familja]+i-1)->time.minutat = o.minutat;
    strcpy( (persona_leje[qytetar.id_familja]+i-1)->id, qytetar.idnr);
    size_leje[qytetar.id_familja]++;
    persona_leje[qytetar.id_familja] = (Leje*) realloc(persona_leje[qytetar.id_familja], size_leje[qytetar.id_familja]*sizeof(Leje));
    printf("Leja u pranua!");
    return 1;
}

void kontrollo_lejedalje(){
    Data shtetas_data;
    char shtetas_id[15];
    Orari shtetas_ora;
    printf("Vendosni ID e shtetasit: ");
    scanf("%s", shtetas_id);
    printf("Vendosni daten (dita muaji viti): ");
    scanf("%d %d %d", &shtetas_data.dita, &shtetas_data.muaji, &shtetas_data.viti);
    printf("Vendosni oren (ora minutat): ");
    scanf("%d %d", &shtetas_ora.ora, &shtetas_ora.minutat);
    kontrollo_ne_file(shtetas_id, shtetas_data, shtetas_ora);
}

void kontrollo_ne_file(char shtetas_id[], Data shtetas_data, Orari shtetas_ora){
    int id_familja = -1;

    for (int i = 0; i < counter; i++) {
        if( strcmp((shtetasptr + i)->idnr, shtetas_id) == 0){
            id_familja = (shtetasptr + i)->id_familja;
            break;
        }
    }

    if (id_familja == -1) {
        printf("Ky shtetas nuk ekziston!");
        return;
    }

    for (int i = 0; i < size_leje[id_familja]; i++) {
        Leje leje = *(persona_leje[id_familja] + i);

        if(strcmp(shtetas_id, leje.id) == 0 && leje.data_lejes.dita == shtetas_data.dita && shtetas_data.muaji == leje.data_lejes.muaji && shtetas_data.viti == leje.data_lejes.viti && shtetas_ora.ora == leje.time.ora && shtetas_ora.minutat == leje.time.minutat) {
            printf("Shtetasi i dhene ka leje ne daten dhe oren e caktuar!");
            return;
        }
    }

    printf("Shtetasi i dhene nuk ka leje ne daten dhe oren e caktuar!");
}

void ruaj_ndryshimet(){
    ruaj_shtetasit();
    ruaj_lejedaljet_file();
}

void ruaj_shtetasit(){
    if( (fptr=fopen("detyra_data.txt", "r+")) == NULL ){
        printf("Gabim ne hapjen e file-it");
        return;
    }
    fseek(fptr, 37, SEEK_SET);
    for(int i = 0; i < counter; i++){
        fprintf(fptr, "\n%s %s %s %d %d", (shtetasptr+i)->idnr, (shtetasptr+i)->emer, (shtetasptr+i)->mbiemer, (shtetasptr+i)->ditelindja, (shtetasptr+i)->id_familja);
    }
    fclose(fptr);
}

void ruaj_lejedaljet_file(){
    if( (fptr2 = fopen("LejeDaljet.txt", "w+")) == NULL){
        printf("Gabim ne hapjen e file-it");
        return;
    }
    
    for (int j = 0; j < size_persona_leje; j++) {
        for (int i = 0; i < size_leje[j] - 1; i++) {
            fprintf(fptr2, "%s %d %d %d %d %d %d\n", (persona_leje[j] + i)->id, j, (persona_leje[j] + i)->data_lejes.dita, (persona_leje[j] + i)->data_lejes.muaji, (persona_leje[j] + i)->data_lejes.viti, (persona_leje[j] + i)->time.ora, (persona_leje[j] + i)->time.minutat);
        }
    }

    fclose(fptr2);
    return;
}

int menu_printimi_lejedaljeve(){
    int zgjedhja;
    printf("Zgjidhni nje nga opsionet e meposhtme:\n");
    printf("1 - Printo leje daljet per nje shtetas\n");
    printf("2 - Printo leje daljet per nje familje\n");
    printf("3 - Printo te gjitha leje daljet\n");
    printf("Zgjedhja juaj: ");
    scanf("%d", &zgjedhja);
    return zgjedhja;
}

void printimi_lejedaljeve(){
    switch( menu_printimi_lejedaljeve() ){
        case 1:
            printo_shtetas();
            break;
        case 2:
            printo_familje();
            break;
        case 3:
            printo_tegjithe();
            break;
        default:
            printf("Keni shtypur nje numer te gabuar!");
            break;
    }
}

void printo_shtetas(){
    char id[15];
    printf("Vendosni ID e shtetasit: ");
    scanf("%s", id);
    int id_familja = -1;

    for (int i = 0; i < counter; i++) {
        if( strcmp((shtetasptr + i)->idnr, id) == 0){
            id_familja = (shtetasptr + i)->id_familja;
            break;
        }
    }

    if (id_familja == -1) {
        printf("Ky shtetas nuk ekziston!");
        return;
    }

    for (int i = 0; i < size_leje[id_familja]; i++) {
        if(strcmp(id, (persona_leje[id_familja] + i)->id ) == 0) {
            printf("Data : %d %d %d\n", (persona_leje[id_familja] + i)->data_lejes.dita, (persona_leje[id_familja] + i)->data_lejes.muaji, (persona_leje[id_familja] + i)->data_lejes.viti);
            printf("Ora - %d:%d\n", (persona_leje[id_familja] + i)->time.ora, (persona_leje[id_familja] + i)->time.minutat);
        }
    }
}

void printo_familje(){
    int id_familja;
    printf("Vendosni numrin e familjes : ");
    scanf("%d", &id_familja);

    for(int i = 0; i < size_leje[id_familja] - 1; i++) {
        printf("Data : %d %d %d\n", (persona_leje[id_familja] + i)->data_lejes.dita, (persona_leje[id_familja] + i)->data_lejes.muaji, (persona_leje[id_familja] + i)->data_lejes.viti);
        printf("Ora - %d:%d\n", (persona_leje[id_familja] + i)->time.ora, (persona_leje[id_familja] + i)->time.minutat);
    }
}

void printo_tegjithe(){

    for(int j = 0; j < size_persona_leje; j++) {
        for(int i = 0; i < size_leje[j] - 1; i++) {
            printf("ID : %s, ID e familjes : %d, Data : %d %d %d, Ora - %d:%d\n", (persona_leje[j] + i)->id, j, (persona_leje[j] + i)->data_lejes.dita,  (persona_leje[j] + i)->data_lejes.muaji,  (persona_leje[j] + i)->data_lejes.viti,  (persona_leje[j] + i)->time.ora,  (persona_leje[j] + i)->time.minutat);
            
        }
    } 
}

void printo_shtetas_sipas_lejedaljeve(){

    int banues[counter] = {0}; //mban numrin e hereve qe ka dale secili banor

    for(int j= 0; j< counter; j++){
         for(int i = 0; i < size_leje[(shtetasptr+j)->id_familja] - 1; i++) {
            if (strcmp((persona_leje[(shtetasptr+j)->id_familja] + i)->id, (shtetasptr+j)->idnr) == 0){
                banues[j]++;
            }
        }
         
    }
    
    printo_dhjete_me_shpesh(banues);
    printo_dhjete_me_pak(banues);
}

void printo_dhjete_me_shpesh(int banues[]){
    int b[counter];
    int index_banor[counter];
    int i,j;
    for(i = 0; i < counter; i++){
        b[i]= banues[i];
        index_banor[i]= i;
    }
    /* Bubble sort ne rend zbrites */
    int temp;
    for(i = 0; i < counter-1; i++){
        for(j = 0; j < counter-1; j++){
            if(b[j] < b[j+1]){
                temp = b[j];
                b[j] = b[j+1];
                b[j+1] = temp;

                temp = index_banor[j];
                index_banor[j] = index_banor[j+1];
                index_banor[j+1] = temp;
            }
        }
    }
    printf("\n10 shtetasit qe kane levizur me shpesh :\n");
    for(i = 0; i < 10; i++){
        if(banues[index_banor[i]] == 0){
            break;
        }
        printf("%d. %s %s (%d here)\n", i+1, (shtetasptr+index_banor[i])->emer, (shtetasptr+index_banor[i])->mbiemer, banues[index_banor[i]]);
    }
}

void printo_dhjete_me_pak(int banues[]){
    int b[counter];
    int index_banor[counter];
    int i,j;
    for(i = 0; i < counter; i++){
        b[i]= banues[i];
        index_banor[i]= i;
    }
    /* Bubble sort ne rend rrites */
    int temp;
    for(i = 0; i < counter-1; i++){
        for(j = 0; j < counter-1; j++){
            if(b[j] > b[j+1]){
                temp = b[j];
                b[j] = b[j+1];
                b[j+1] = temp;

                temp = index_banor[j];
                index_banor[j] = index_banor[j+1];
                index_banor[j+1] = temp;
            }
        }
    }
    for(i = 0; i < counter; i++){
        if(banues[index_banor[i]] != 0){ //anashkaloj te gjithe banoret qe nuk kane dale asnjehere
            printf("\n10 shtetasit qe kane levizur me pak :\n");
            for(j = 0; j < 10; j++){
                printf("%d. %s %s (%d here)\n", j+1, (shtetasptr+index_banor[i+j])->emer, (shtetasptr+index_banor[i+j])->mbiemer, banues[index_banor[i+j]]);
                if(i+j+1 == counter){
                    break;
                }
            }
            break;
        }
    }
}