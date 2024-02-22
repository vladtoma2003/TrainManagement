	Am inceput prin a adauga niste functii extra care ma vor ajuta la rezolvarea a catorva taskrui (length - lungimea trenului, contains_number - returneaza
nr de vagoane care are o greutate data la intrare, find_max - gaseste greutatea maxima intr-o secventa de vagoane de lungime l).
	Crearea functiilor din tema (in ordinea din fisierul station.c):
	1) open_train_station:
	Am alocat memorie statie si am creat un vector de lungime platforms_no, un vector care va contine adresele peroanelor. Dupa ce am alocat (si verificat
daca alocarea a fost un success) am initializat pointerii catre peroane cu NULL.
	2) close_train_station:
	Am inceput prin a verifica daca exista statia care trebuie inchisa si daca exista vectorul de peroane (daca nu eliberez statia). Dupa verificari, an
parcurs toate peroanele si am folosit functia leave_train (5) pentru a elibera memoria. La sfarsit am eliberat vectorul de peroane si statia.
	3) show_existing_train:
	Am verificat daca exista statia care trebuie afisata si am afisat in fisierul dat la intrare peronul (daca exista), locomotiva(daca exista) si 
vagoanele(daca exista).
	4) arrive_train :
	Am inceput prin a verifica daca datele date la intrare sunt valide (daca nu se opreste functia). Daca sunt valide am alocat memorie pentru locomotiva 
si am pus valoarea puterii in locomotiva.
	5) leave_train :
	Am verificat daca datele date la intrare sunt valida. Daca da am parcurs trenul eliberand fiecare vagon. La sfarist am eliberat si locomotiva si am
pus pointerul catre peron la NULL.
	6) add_train_car:
	Am inceput prin a verifica daca datgele la intrare sunt valide. Daca da am parcurs trenul. Am alocat memorie pentru noul vagon si l-am legat de tren.
	7) remove_train_cars:
	Am verificat daca datele de intrare sunt valide. Aceasta functie sterge toate vagoanele care au o anumita greutate. Am parcurs tot trenul pana am dat
de valoarea greutatii pe care o cautam sau pana am ajuns la final. Daca am ajuns al greutatea cautata, eliberez memoria, leg vagoanele de langa si continui
parcurgerea trenului. In cazul in care vagonul care trebuia sters era primul, se leaga al doilea vagon de locomotiva si se elibereaza memoria primului.
	8) move_train_cars:
	Aceasta functie muta cars_no vagoane de pe pos_a (de pe peronul a) pe pos_b (pe peronul b). Am verificat daca toate datele de intrare sunt corecte.
Am pus un pointer inainte de prima pozitie din secventa care trebuie mutata si un pointer la ultima pozitie din secventa de vagoane. Apoi am pus doi
pointeri la destinatie (secventa va fi pusa intre acesti doi pointeri(*)). Am avut patru cazuri:
 ->Cazul 1: Vagoanele trebuie mutate de pe pozitia unu pe pozitia unu. In acest caz am legat vagonul care urmeaza dupa ultimul din secventa de locomotiva_a
si primul vagon din secventa la locomotiva_b. Am legat si ultimul vagon din secventa de al doilea vagon de pe peronul b.
 ->Cazul 2: Doar pozitia lui a este unu (pozitia de pe care trebuie mutate vagoanele). In acest caz am legat locomotiva de vagonul care urmeaza dupa 
ultimul din secventa si am pus secventa intre cei doi pointeri de la b (*).
 ->Cazul 3: Pozitia lui b este 1 (cea finala, unde trebuie mutate vagoanele). In acest caz am scos secventa din trenul a, am legat vagoanele care raman si
am pus secventa inainte de primul vagon din trenul b.
 ->Cazul 4: Pozitiile lui a si b sunt diferite de 1. Aici am scos vagoanele din trenul a, am legat vagoanele ramase si am pus secventa intre pointerii (*)
din vagonul b.
	9) find_express_train:
	Aceasta functie gaseste cel mai rapid tren. Am verificat statia daca exista. Apoi am parcurs fiecare peron unde am calculat greutatea totala a 
vagoanelor si am scazut aceasta greutate din putere. Diferenta minima este defapt trenul cel mai rapid. Am salvat peronul celui mai rapid tren pentru sfarsit.
	10) find_overload_train:
	Aceasta functie gaseste trenul supraincarcat, care nu poate pleca din statie. Am verificat daca statia exista si apoi am parcurs fiecare peron.
Am caluclat greutatea totala fiecarui tren si am scazut-o din putere. Daca diferenta era negativa, atunci am gasit trenul cautat si am returnat peronul.
	11) find_optimal_train:
	Aceasta functie gaseste trenul cu incarcatura optima (diferenta dintre putere si greutate minima). Am verificat existenta statie. Am parcurs fiecare
peron unde am caluclat greutatea totala si am scazut-o din putere. Am salvat diferenta minima si la sfarsit am returnat pozitia aceste diferente.
	12) find_heaviest_sequence_train:
	Aceasta functie cauta cea mai grea secventa de trenuri. Am verificat datele de intrare. Am parcurs fiecare peron unde am avut doua cazuri:
 ->Cazul 1: cand trebuie verificat doar un vagon (secventa este unu). Aici am luat fiecare greutate si am verificat daca este maxima.
 ->Cazul 2: cand secventa este mai mare de unu. Aici calculez intai greutatea totala din secventa si verific daca este maxima. Dupa aceea scad greutatea
primului vagon din secventa si adun cu greutatea vagonului care urmeaza dupa secventa. Astfel secventa este in continuare de lungimea corecta dar nu
incepe de la acelas vagon. Repet acest proces (scad si adun greutate) pana se termina trenul. Pozitia maxima este returnata la sfarist.
	13) order_train:
	Aceasta functie ordoneaza trenul de pe un peron descrescator. Am verificat intai datele. Apoi am salvat lungimea trenului folosind funcita extra length.
Am aflat greutatea maxima folosind functia extra find_max si numarul de vagoane care au greutatea maxima folosind functia extra contains_number. Apoi am scos 
vagoanele care au greutatea maxima si le-am adaugat folosind add_train_car (aceasta functie le adauga la sfarist). Apoi am cautat noul maxim intr-o secventa
de vagoane mai mica (fara cele adaugate deja la sfarsit). Astfel voi cauta maximul in vagoanele ne sortate. Am repetat acest proces pana am sortat toate
vagoanele.
	14) fix_overload_train:
	Aceasta functie scoate un vagon cu o greutate optima dintr-un tren supraincarcat. Am verificat daca exista statia si daca aceasta statie are un tren
supraincarcat. Daca are calculez greutatea totala si greutatea optima (greutate totala - putere). Astfel pot cauta cea mai mica greutate care este mai mare
decat cea optima, pentru a scoate vagonul care o contine.