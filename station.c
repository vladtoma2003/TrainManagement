#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "station.h"

int length(TrainCar *vagon) { // Calculeaza lungimea trenului
    int contor = 0;
    while(vagon) {
        ++contor;
        vagon = vagon->next;
    }
    return contor;
}

int contains_number (TrainCar *current, int weight) { // Verifica cate vagoane 
    int contor = 0; // de greutate = weight exista
    while(current) {
        if(current->weight == weight) {
            ++contor;
        }
        current = current->next;
    }
    return contor;
}

int find_max (TrainCar *current, int lungime) { // Gaseste greutatea maxima intr-o secventa de 
    int maxim = 0;                          // lungime = lungime
    for(int i = 0; i < lungime; ++i) {
        if(maxim < current->weight) {
            maxim = current->weight;
        }
        current = current->next;
    }
    return maxim;
}

/* Creeaza o gara cu un numar fix de peroane.
 * 
 * platforms_no: numarul de peroane ale garii
 *
 * return: gara creata
 */
TrainStation* open_train_station(int platforms_no) {
   TrainStation *station = malloc(sizeof(TrainStation)); // Aloca memorie si verifica daca e ok
   if(station == NULL) {
       return NULL;
   }
   station->platforms = calloc(platforms_no, sizeof(Train)); // Aloca memorie vectorului si verifica
   if(station == NULL) {
        return NULL;
   }
   station->platforms_no = platforms_no;
   for(int i = 0; i < platforms_no; ++i) { // Initializeaza peroanele la NULL
       station->platforms[i] = NULL;
   }
   return station;
}


/* Elibereaza memoria alocata pentru gara.
 *
 * station: gara existenta
 */
void close_train_station(TrainStation *station) {
    if(station == NULL) { // Verifica daca s-a introdus un station valid
        return;
    }
    if(station->platforms == NULL) { // verifica daca vectorul de peroane este valid
        free(station);
        return;
    }
    for(int i = 0; i < station->platforms_no; ++i) { // Elibereaza fiecare tren de pe peroane
        leave_train(station, i);
    }
    free(station->platforms);
    free(station);
}


/* Afiseaza trenurile stationate in gara.
 *
 * station: gara existenta
 * f: fisierul in care se face afisarea
 */
void show_existing_trains(TrainStation *station, FILE *f) {
    if(station == NULL) { // Verific daca statia est evalida
        return;
    }
    for(int i = 0; i < station->platforms_no; ++i) { // Printez in fisier trenurile
        if(station->platforms[i] == NULL) {
            fprintf(f, "%d: \n", i);
            continue;
        }
        fprintf(f, "%d: (%d)", i,  station->platforms[i]->locomotive_power);
        TrainCar *current = station->platforms[i]->train_cars;
        while(current) {
            fprintf(f, "-|%d|", current->weight);
            current = current->next;
        }
        fprintf(f, "\n");
    }
}


/* Adauga o locomotiva pe un peron.
 * 
 * station: gara existenta
 * platform: peronul pe care se adauga locomotiva
 * locomotive_power: puterea de tractiune a locomotivei
 */
void arrive_train(TrainStation *station, int platform, int locomotive_power) {
    if(station == NULL) { // Verific daca statia este valida
        return;
    }
    if(platform < 0 || platform >= station->platforms_no) { // Verific daca platforma este valida
        return;
    }
    if(locomotive_power < 0) { // Verific daca puterea este valida
        return;
    }
    if(station->platforms[platform] != NULL) { // Verific daca exista deja un tren pe peron
        return;
    }
    station->platforms[platform] = malloc(sizeof(Train)); // Aloc memorie pentru tren si verific alocarea
    if(station->platforms[platform] == NULL) {
        return;
    }
    station->platforms[platform]->locomotive_power = locomotive_power; 
    station->platforms[platform]->train_cars = NULL;
}


/* Elibereaza un peron.
 * 
 * station: gara existenta
 * platform: peronul de pe care pleaca trenul
 */
void leave_train(TrainStation *station, int platform) {
    if(station == NULL) { // Verificare daca exista statia
        return;
    }
    if(platform < 0 || platform >= station->platforms_no) { // Verific daca exista platforma
        return;
    }
    if(station->platforms[platform] == NULL) { // Verific daca este gol peronul
        return;
    }
    if(station->platforms[platform]->train_cars != NULL){ // Eliberez daca exista ceva pe peron
        TrainCar *current = station->platforms[platform]->train_cars;
        TrainCar *aux = NULL;
        while(current != NULL) {
            aux = current->next;
            free(current);
            current=aux;
        }
    }
    free(station->platforms[platform]); // Eliberez memoria alocata pentru tren
    station->platforms[platform] = NULL;
}


/* Adauga un vagon la capatul unui tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului adaugat
 */
void add_train_car(TrainStation *station, int platform, int weight) {
    if(station == NULL) { // Verific daca statia este valida
        return;
    }
    if(platform < 0 || platform > station->platforms_no) { // Verific daca peronul este valid
        return;
    }
    if(station->platforms[platform] == NULL) { // Verific daca exista peronul
        return;
    }
    if(station->platforms[platform]->train_cars == NULL) { // Verific daca este gol trenul
        station->platforms[platform]->train_cars = malloc(sizeof(TrainCar));
        if(station->platforms[platform]->train_cars == NULL) {
            return;
        }
        station->platforms[platform]->train_cars->weight = weight;
        station->platforms[platform]->train_cars->next = NULL;
        return;
    }
    TrainCar *current = station->platforms[platform]->train_cars; // pointer la inceput (primul vagon)
    while(current->next != NULL) { // Ma duc pana la ultimul vagon din tren
        current = current->next;
    }
    current->next = malloc(sizeof(TrainCar)); // Aloc memorie pt tren si verific daca s-a alocat
    if(current->next == NULL) {
        return;
    }
    current->next->next = NULL;
    current->next->weight = weight;
}


/* Scoate vagoanele de o anumita greutate dintr-un tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului scos
 */
void remove_train_cars(TrainStation *station, int platform, int weight) {
    if(station == NULL) { // Verific daca statia este valida 
        return;
    }
    if(platform < 0 || platform >= station->platforms_no) { // Verific daca platforma este valida
        return;
    }
    if(station->platforms[platform] == NULL) { // Verific daca platforma este goala
        return;
    }
    if(station->platforms[platform]->train_cars == NULL) { // Verific daca exista vagoane
        return;
    }
    TrainCar *current = station->platforms[platform]->train_cars; // Pointer la inceput (primul vagon)
    TrainCar *prev = NULL; // Pointer la vagonul anterior
    while(current != NULL) { // Parcurgere vagoane
        while(current != NULL && current->weight != weight) { // Se opreste la vagonul cu greutatea cautata
            prev = current;
            current = current->next;
        }
        if(current == NULL) {
            return;
        } 
        if(prev == NULL) { // In caz ca trebuie scos primul vagon
            station->platforms[platform]->train_cars = current->next;
            free(current);
            current = station->platforms[platform]->train_cars;
        } else {
            prev->next = current->next;
            free(current);
            current = prev->next;
        }
    }
}


/* Muta o secventa de vagoane dintr-un tren in altul.
 * 
 * station: gara existenta
 * platform_a: peronul pe care se afla trenul de unde se scot vagoanele
 * pos_a: pozitia primului vagon din secventa
 * cars_no: lungimea secventei
 * platform_b: peronul pe care se afla trenul unde se adauga vagoanele
 * pos_b: pozitia unde se adauga secventa de vagoane
 */
void move_train_cars(TrainStation *station, int platform_a, int pos_a, 
                                int cars_no, int platform_b, int pos_b) {
    if((station == NULL) || (station->platforms == NULL)) { // Verific daca exista statia si peroanele
        return;
    }
    if((platform_a < 0) || (platform_a > station->platforms_no)) {
        return; // Verific daca exista peroanele a si b(mai jos)
    }
    if((platform_b < 0) || (platform_b > station->platforms_no)) {
        return;
    }
    if((station->platforms[platform_a] == NULL) || (station->platforms[platform_b] == NULL)) {
        return;
    }
    if((pos_a <= 0) || (pos_b <= 0) || (cars_no <= 0)) {
        return;
    }
    TrainCar *curr_a = station->platforms[platform_a]->train_cars;
    TrainCar *curr_b = station->platforms[platform_b]->train_cars;
    TrainCar *prev_a = NULL;
    TrainCar *prev_b = NULL;
    int contor = 1;
    while(curr_a && contor < pos_a) { // curr_a se duce pe prima pozitie din secventa
        prev_a = curr_a; // prev_a se duce pe cea de dinainte
        curr_a = curr_a->next;
        ++contor;
    }
    if(contor < pos_a) { // verific daca sunt destule vagoane pana la primul din secventa a
        return;
    }
    contor = 1; //resetare contor
    while(curr_b && contor < pos_b) { // mutarea se va face intre prev_b si curr_b
        prev_b = curr_b;
        curr_b = curr_b->next;
        ++contor;
    }
    if(contor < pos_b) { //verific daca sunt destule vagoane pana unde trebuie mutate
        return;
    }
    contor = 1;
    while(curr_a && contor < cars_no) { // il mut pe curr_a pe ultimul vagon din secventa
        curr_a = curr_a->next;
        ++contor;
    }
    if(contor < cars_no || curr_a == NULL) { // verific daca mai sunt destule vagoane
        return;
    }
    if(pos_a == 1 && pos_b == 1) { // Cazul in care trebuie mutat de pe prima pozitie (din a) pe prima pozitie (din b)
        station->platforms[platform_b]->train_cars = station->platforms[platform_a]->train_cars;
        station->platforms[platform_a]->train_cars = curr_a->next;
        curr_a->next = curr_b;
        return;
    }
    if(pos_a == 1) { // Cazul in care trebuie mutat de pe prima pozitie din a
        prev_b->next = station->platforms[platform_a]->train_cars;
        station->platforms[platform_a]->train_cars = curr_a->next;
        curr_a->next = curr_b;
        return;
    }
    if(pos_b == 1) { // Cazul in care trebuie mutat pe prima pozitie din b
        station->platforms[platform_b]->train_cars = prev_a->next;
        prev_a->next = curr_a->next;
        curr_a->next = curr_b;
        return;
    }
    // Restul cazurilor
    prev_b->next = prev_a->next;
    prev_a->next = curr_a->next;
    curr_a->next = curr_b;
}


/* Gaseste trenul cel mai rapid.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_express_train(TrainStation *station) {
    if(station == NULL) { // Verific daca statia este valida
        return -1;
    }
    int greutate = 0;
    int maxim = 0;
    int pos = 0;
    for(int i = 0; i < station->platforms_no; ++i) { // Parcurg peroanele
        greutate = 0;
        if(station->platforms[i] == NULL) { // Verific daca este gol peronul
            continue;
        }
        TrainCar *current = station->platforms[i]->train_cars;
        while(current != NULL) { // Calculez greutatea maxima
            greutate += current->weight;
            current = current->next;
        }
        if(station->platforms[i]->locomotive_power - greutate > maxim) { // Verific daca este diferenta
            maxim = station->platforms[i]->locomotive_power - greutate;// minima si salvez pozitia
            pos = i;                                                    // in caz afirmativ
        }
    }  
    return pos;
}


/* Gaseste trenul supraincarcat.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_overload_train(TrainStation *station) {
    int greutate = 0;
    if(station == NULL) { // Verific daca statia este valida
        return -1;
    }
    for(int i = 0; i < station->platforms_no; ++i) { // Parcurg toate peroanele
        greutate = 0;
        if(station->platforms[i] == NULL) { // Verific daca exista ceva pe peronul curent
            continue;   
        }
        TrainCar *current = station->platforms[i]->train_cars;
        while(current != NULL) { // Calculez greutatea totala
            greutate += current->weight;
            current = current->next;
        }
        if(station->platforms[i]->locomotive_power < greutate) { // verific daca poate pleca trenul
            return i;
        }
    }
    return -1;
}


/* Gaseste trenul cu incarcatura optima.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_optimal_train(TrainStation *station) {
    if(station == NULL) { // Verific daca statia este valida
        return -1;
    }
    int greutate = 0;
    int minim = 1000000;
    int pos = 0;
    for(int i = 0; i < station->platforms_no; ++i) { // Parcurg peroanele
        greutate = 0;
        if(station->platforms[i] == NULL) { // Verific daca exista ceva pe peronul curent
            continue;
        }
        TrainCar *current = station->platforms[i]->train_cars;
        while(current != NULL) { // Calculze greutatea totala
            greutate += current->weight;
            current = current->next;
        }
        if(station->platforms[i]->locomotive_power - greutate < minim) { // Verific daca peronul 
            minim = station->platforms[i]->locomotive_power - greutate;// curent are trenul cu
            pos = i;                                                    // incarcatura optima
        }
    }  
    return pos;
}


/* Gaseste trenul cu incarcatura nedistribuita bine.
 * 
 * station: gara existenta
 * cars_no: lungimea secventei de vagoane
 *
 * return: peronul pe care se afla trenul
 */
int find_heaviest_sequence_train(TrainStation *station, int cars_no, TrainCar **start_car) {
    *start_car = NULL;
    if(station == NULL) { // Verific daca statia este valida
        return -1;
    }
    if(cars_no <= 0) { // Verific daca numarul de vagoane este valid
        return -1;
    }
    int greutate = 0;
    int maxim = 0;
    int pos = -1;
    for(int i = 0; i < station->platforms_no; ++i) { // Parcurg toate peroanele
        if(station->platforms[i] == NULL) { // Verific daca exista peronul curent
            continue;
        }
        TrainCar *current = station->platforms[i]->train_cars;
        TrainCar *prev = NULL;
        if(cars_no == 1) { // Cazul in care trebuie mutat un singur vagon
            while(current) { // Cauta vagonul cu greutate maxima si ii salvez pozitia
                greutate = current->weight;
                if(maxim < greutate) {
                    maxim = greutate;
                    pos = i;
                    *start_car = current;
                }
                current = current->next;
            }
        } else { // Cazul in care trebuie mutate mai multe vagoane
            while(current) {
                int contor = 0;
                greutate = 0;
                if(prev == NULL) { // Pentru primul vagon din tren
                    prev = current;
                    while(current && contor < cars_no) { // Calculez greutatea totala din secventa
                        greutate += current->weight;
                        ++contor;
                        current = current->next;
                    }
                    if((contor == cars_no) && (greutate > maxim)) { // Verific daca au fost destule
                        maxim = greutate;                       // vagoane in secventa si daca este
                        pos = i;                                // maxima greutatea
                        *start_car = prev;
                    }
                    while(current) { // Parcurg restul vagoanelor unul cate unul
                        greutate += current->weight; // adaug si scad greutatile
                        greutate -= prev->weight; // si verific daca este maxima
                        current = current->next; // cea curenta
                        prev = prev->next;
                        if(greutate > maxim) {
                            maxim = greutate;
                            pos = i;
                            *start_car = prev;
                        }
                    }
                }

            }
        }
    }  
    return pos;
    
}


/* Ordoneaza vagoanele dintr-un tren in ordinea descrescatoare a greutatilor.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 */
void order_train(TrainStation *station, int platform) {
    if(station == NULL || station->platforms[platform] == NULL) {
        return; // verific daca exista statia si peronul
    }
    if(station->platforms[platform]->train_cars == NULL) {
        return; // verific daca sunt vagoane in tren
    }
    TrainCar *prim_vagon = station->platforms[platform]->train_cars; //tine minte primul vagon
    int current_length = length(prim_vagon); // tine minte lungimea trenului
    int maxim = find_max(prim_vagon, current_length); // tine minte maximul
    int nr_maxime = contains_number(prim_vagon, maxim); // tine minte cate maxime sunt
    while(current_length) {
        remove_train_cars(station, platform, maxim); // sterge vagoanele care au greutate maxima
        for(int i = 0; i < nr_maxime; ++i) { // adauga la coada toate "maximele" pe care le-a sters
            add_train_car(station, platform, maxim);
        }
        current_length -= nr_maxime; // nu mai cauta maxim in vagoanele sortate deja
        prim_vagon = station->platforms[platform]->train_cars;
        maxim = find_max(prim_vagon, current_length);
        nr_maxime = contains_number(prim_vagon, maxim);
    }

}


/* Scoate un vagon din trenul supraincarcat.
 * 
 * station: gara existenta
 */
void fix_overload_train(TrainStation *station) {
    if(station == NULL || station->platforms == NULL) { // Verific daca statia este valida
        return;
    }
    int peron = find_overload_train(station);
    if(peron == -1) { // Verific daca exista tren care nu poate pleca din statie
        return;
    }
    TrainCar *current = station->platforms[peron]->train_cars;
    int putere = station->platforms[peron]->locomotive_power;
    int greutate_totala = 0;
    while(current) { // calculare greutate maxima
        greutate_totala += current->weight;
        current = current->next;
    }
    current = station->platforms[peron]->train_cars;
    int greutate_optima = greutate_totala - putere;
    int minim = putere;
    while(current) { // Caut greutatea optima care trebuie scoasa
        if((minim > current->weight) && (current->weight >= greutate_optima)) {
            minim = current->weight;
        }
        current = current->next;
    }
    current = station->platforms[peron]->train_cars;
    TrainCar *prev = NULL;
    while(current) { // Scot vagonul cu greutate optima
        if(current->weight == minim && prev) {
            prev->next = current->next;
            free(current);
            return;
        } else if(current->weight == minim) {
            station->platforms[peron]->train_cars = current->next;
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}
