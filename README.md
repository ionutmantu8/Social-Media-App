# Echipa tema 3 SDA:
Ionut-Gabriel Mantu(aephuoCiedox id catalog / ionut_gabriel.mantu id moodle)
Razvan-Andrei Degeratu (AYuufulohMuT id catalog / razvan.degeratu id moodle)


## Task 1: Retea de prietenie

	Pentru reprezentarea relațiilor de prietenie am ales implementarea
	grafului cu matrice de adiacenta.

	Astfel, adaugarea/eliminarea unei relatii de prietenie presupune 
	switch intre 0 si 1 in graf.

	Gasirea distantei intre doi prieteni este un BFS de la un nod la celalalt.

	Sugerarea prietenilor are doua parti: Gasirea prietenilor prietenilor si 
	gasirea prietenilor. Amandoua se bazeaza pe un vector de booleeni. 
	Pentru a determina sugestiile, elminam din vectorul de fof, pe cei care sunt
	prieteni cu nodul de la care pornim.

	Gasirea prietenilor comuni se face prin cautarea in graf relatie de la
	id1 si id2 la i, unde i trece prin toti ceilalti oameni din graf.

	Numarul de prieteni este gasit prin verificarea de cate ori apare 1 in linia 
	care corespunde nodului id cautat.

	Cel mai popular prieten este gasit prin verificarea numarului de prieteni ai fiecaruia
	si alegerea maximului. Daca cel mai popular este cel cautat, se alege el.
	Daca sunt mai multi, se alege cel cu id-ul cel mai mic.


## Task 2: Postari

	Pentru reprezentarea postarilor am ales un vector de arbori, in care fiecare
	arbore reprezinta o postare impreuna cu reposturile sale. Postarea originala
	este root in arbore.

	Crearea unei postari se face prin crearea unui nou arbore si initializarea root-ului

	Repostarea reprezinta crearea unui copil in arborele postarii originale, sau un copil
	al unui copil al postarii originale(s.a.m.d).
	
	Primul repost comun este gasit prin aplicarea algoritmului 'Lowest common ancestor' pe
	arbore, si cele doua noduri.

	Datul unui like se face prin vectorul de flag-uri like, care e 0 initial sau daca
	se da like de un nr par de ori, 1 daca se da like de un nr impar de ori.


	Ratio verifica si compara numarul de like uri intre o postare si toate repostarile sale.

	Delete post folosete campul de total likes ale postarii si il seteaza pe -1
	pentru a marca postarea stearsa.

	Numarul de like-uri al unei postari este obtinut prin campul de total likes al postarii.

	Repostarile sunt obtinute printr-un DFS in ordinea creeri in arborele corespunzator
	postarii/repostarii cerute.


## Task 3: Social Media

	Functia feed itereaza prin toate postarile si verifica daca au fost create
	de un anume utilizator sau de prietenii sai. Daca da, le afiseaza pe ultimele
	x postari, impreuna cu creatorul fiecareia.

	Pentru a vedea profilul, iteram prin toate rooturile si afisam postarile
	utilizatorului, apoi afisam repostarile sale iterand prin fiecare arbore.

	Pentru a vedea cine a repostat o postare, iteram prin subarborii care pleaca
	din aceasta si verificam cine a repostat, afisand persoanele respective.

	Pentru a gasi o clica, am folosit algoritmul Bron Kerbosch, care gaseste toate
	subgrafurile complete. Am adaptat algoritmul pentru a gasi cea mai mare clica.
