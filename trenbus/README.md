Testele sunt in fisierul teste

Testul 0 si 1 sunt mici, pentru cele 20 de puncte cu N <= 1000

Restul testelor sunt mari (N == 100000). Fiecare 3 teste consecutive sunt gandite in mod asemanator (sunt 3 teste random generate cu aceleasi setari). Pentru a mai genera mai multe teste, se face in felul urmator:

    * se editeaza fisierul "a", unde se pun 3 constante N x y. N reprezinta marimea testului, x si y sunt constante ce determina cum se genereaza primul / al doilea arbore: -1000000 <= x, y <= 1000000, valori mici fac ca arborele sa fie mai aproape de o stea, valori mari mai aproape de un lant.
    
    * se ruleaza ./build_test_number _arg_ unde arg este un numar intreg ce reprezinta numarul testului generat (i.e. ./build_test_number 0 creeaza trenbus-0.in)
    
    * se ruleaza ./generate_oks ok_build

Pentru a evalua o sursa, se pune in fisierul surse, si apoi se ruleaza ./evaluate_sursa (exemplu: daca adaug sursa alex_velea.cpp, se ruleaza ./evaluate_sursa alex_velea).
