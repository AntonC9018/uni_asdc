Examen la **ASDC**.

Student: **Curmanschii Acton, IA1901**.

Varianta **20**.


## Nr. 1  

> Daţi noţiunea de listă dublu înlănţuită şi circulară. Explicaţi principiul de funcţionare.

*Lista dublu înlănțuită* este structura de date, construită din noduri și conexiuni dintre ele (pointeri).

Fiecare nod are câte două pointeri: la nodul precedent în listă și la nodul ce urmează. În același timp, fiecare nod poate stoca și datele pe lângă pointerilor.

Capul listei nu ar avea pointer la nodul precedent (ar fi setat la `NULL`), iar coada listei nu ar avea pointer la nodul următor.

*Lista ciclică* tot conține noduri. Nodurile pot fi ca în liste simplu înlănțuite, adică doar cu pointerul la următorul element, ori ca în liste dublu înlănțuite, descris mai sus.

Diferența este că nodul coada este legat cu nodul cap, adică se consideră că coada precedă capul cu o poziție. Astfel se obține structura ciclică.

Elementele se adaugă prin amplasarea nodurilor după sau înainte unui alt nod, după ce se restorează conexiunile. Așa se face în ambele liste.

Pentru iterare pur și simplu urmăm pointeri în una din direcții. În cazul listei ciclice, mai trebuie să verificăm dacă ne-am întors la nodul-start.

## Nr. 2

> Sunt date elemente: line, time, shift, switch, xor, data, page, off, clear, or.

1. *Formulaţi etapele de căutare prin metoda succesivă.*

La fiecare etapă verificăm dacă elementul este acel cerut, unu câte unu, până când nu se ajunge la elementul cerut sau la sfârșitul listei.

2. *Aplicaţi metoda succesivă de căutare pentru un element din şirul dat de elemente.*

Luăm de exemplu primul element, *line*. Primul element din tablou deja este *line*, deci indicele este 0.

Luăm *shift*. *line* nu e *shift*, *time* nu e *shift*, *shift* este *shift*. Indicele este 2.


## Nr. 3

1. *Construiţi un tabel de repartizare cu examinarea liniară (Hash tabel), format din elementele date.*

Vom lua lungimea totală = N = 10.

Funcția de hash va fi F(x) = poziția primei litere în alfabet.

Poziția în tablou P = F(x) % N.

abcdefghijklmnopqrstuvwxyz.

[ _ _ _ _ _ _ _ _ _ _ ] (11 % 10 = 1)

[ _ line _ _ _ _ _ _ _ _ ] (19 % 10 = 9)

[ _ line _ _ _ _ _ _ _ time ] (18 % 10 = 8)

[ _ line _ _ _ _ _ _ shift time ] (18 % 10 = 8)

[ switch line _ _ _ _ _ _ shift time ] (23 % 10 = 3)

[ switch line _ xor _ _ _ _ shift time ] (3 % 10 = 3)

[ switch line _ xor data _ _ _ shift time ] (15 % 10 = 5)

[ switch line _ xor data page _ _ shift time ] (14 % 10 = 4)

[ switch line _ xor data page off _ shift time ] (2 % 10 = 2)

[ switch line clear xor data page off _ shift time ] (14 % 10 = 4)

[ switch line clear xor data page off or shift time ] (14 % 10 = 4)


2. *Explicaţi pe paşi căutarea în acest tabel pentru primul şi pentru ultimul element din şir.*

abcdefghijklmnopqrstuvwxyz

Avem *line*. 

- Calculăm hash-ul h = F(*line*) = poziția lui l în alfabet = 11 (poziția lui a = 0).
- Calculăm indicele inițial P(*line*) = h % 10 = 1.
- Găsim *line* pe poziția 1 din tabel.

Avem *or*.

- h = F(*or*) = 14.
- P(*or*) = h % 10 = 4.
- Pe poziția 4 din tabel găsim *data*. Verificăm următoarea poziție.
- Pe poziția 5 din tabel găsim *page*. Verificăm următoarea poziție.
- Pe poziția 6 din tabel găsim *off*. Verificăm următoarea poziție.
- Pe poziția 7 din tabel găsim *or*.

3. *Explicaţi pe paşi un exemplu de căutare nereuşită în acest tabel (un element inexistent).*

Căutăm cuvântul *a*.

- h = F(*a*) = 0.
- P(*a*) = h % 10 = 0.
- Verificăm toate elementele în această ordine, negăsind *a*: *switch line clear xor data page off or shift time*.
- Când ajungem la sfârșitul tabloului, ne întoarcem la primul element. Însă, deja l-am verificat. Prin urmare, tabelul nu conține *a*.


4. *Calculaţi lungimea medie teoretică şi practică de căutare pentru acest tabel.*

Media practică = $M = (1 + 1 + 1 + 3 + 1 + 2 + 1 + 3 + 1 + 4) / 10 = 18 / 10 = 1.8$

Am descris deja media teoretică în prima lucrare. Ea depinde nu numai de numărul de elemente în tablou și de lungimea tabloului, dar încă de funcția hash.

$ M_{teoretica} = N / (M * B_f) $, unde N este numărul de poziții, M este numărul de elemente, $ B_f $ coeficientul care arată cât de bună este funcția hash.


## Nr. 4

> line, time, shift, switch, xor, data, page, off, clear, or

abcdefghijklmnopqrstuvwxyz


1. *Explicaţi pe paşi sortarea vectorului format din primele litere ale elementelor date prin Shellsort.*

[**line**, time, shift, switch, xor, **data**, page, off, clear, or]

line > data

[data, **time**, shift, switch, xor, line, **page**, off, clear, or]

time > page

[data, page, **shift**, switch, xor, line, time, **off**, clear, or]

shift > off

[data, page, off, **switch**, xor, line, time, shift, **clear**, or]

switch > clear

[data, page, off, clear, **xor**, line, time, shift, switch, **or**]

xor > or

[**data**, page, **off**, clear, or, line, time, shift, switch, xor]

[data**, **page**, off, **clear**, or, line, time, shift, switch, xor]

page > clear

[data, clear, **off**, page, **or**, line, time, shift, switch, xor]

[data, clear, off, **page**, or, **line**, time, shift, switch, xor]

page > line

[data, clear, off, line, **or**, page, **time**, shift, switch, xor]

[data, clear, off, line, or, **page**, time, **shift**, switch, xor]

[data, clear, off, line, or, page, **time**, shift, **switch**, xor]

time > switch

[data, clear, off, line, or, page, switch, **shift**, time, **xor**]

[**data**, **clear**, off, line, or, page, switch, shift, time, xor]

data > clear

[clear, **data**, **off**, line, or, page, switch, shift, time, xor]

[clear, data, **off**, **line**, or, page, switch, shift, time, xor]

off > line

[clear, data, line, **off**, **or**, page, switch, shift, time, xor]

[clear, data, line, off, **or**, **page**, switch, shift, time, xor]

[clear, data, line, off, or, **page**, **switch**, shift, time, xor]

[clear, data, line, off, or, page, **switch**, **shift**, time, xor]

switch > shift

[clear, data, line, off, or, page, shift, **switch**, **time**, xor]

[clear, data, line, off, or, page, shift, switch, **time**, **xor**]


2. *Apreciaţi numărul de comparaţii şi numărul de interschimbări.*

Numărul de comparări: 22.

Numărul de interschimbări: 11.

3. *Calculaţi complexitatea teoretică a acestui algoritm de sortare pentru elementele date.*

În general, complexitatea timp este $O(n^2)$



## Nr. 5

1. *Desenaţi arborele binar de căutare, format din elementele date.*

![binar](binar.jpg)

2. *Enumeraţi în inordine nodurile acestui arbore, arătând înscrierea şi extragerea lor în/din stivă.*

*in* reprezintă afișarea, *push* și *pop* sunt operațiile cu stivă.

![stiva](stiva.jpg)

3. *Calculaţi lungimea medie practică de căutare pentru acest arbore.*


$M = (1 + 2 + 2 + 3 + 3 + 3 + 4 + 4 + 5 + 6) / 10 = 33 / 10 = 3.8$

(1 dacă găsim element imediat)


## Nr. 6

1. *Arătaţi alocarea elementelor matricei D[1..3,-1..2, 2..5, -2..0] “pe linii” în MO.*

Toți indicii.

```
  0: [ 1, -1,  2, -2]
  1: [ 1, -1,  2, -1]
  2: [ 1, -1,  2,  0]
  3: [ 1, -1,  3, -2]
  4: [ 1, -1,  3, -1]
  5: [ 1, -1,  3,  0]
  6: [ 1, -1,  4, -2]
  7: [ 1, -1,  4, -1]
  8: [ 1, -1,  4,  0]
  9: [ 1, -1,  5, -2]
 10: [ 1, -1,  5, -1]
 11: [ 1, -1,  5,  0]
 12: [ 1,  0,  2, -2]
 13: [ 1,  0,  2, -1]
 14: [ 1,  0,  2,  0]
 15: [ 1,  0,  3, -2]
 16: [ 1,  0,  3, -1]
 17: [ 1,  0,  3,  0]
 18: [ 1,  0,  4, -2]
 19: [ 1,  0,  4, -1]
 20: [ 1,  0,  4,  0]
 21: [ 1,  0,  5, -2]
 22: [ 1,  0,  5, -1]
 23: [ 1,  0,  5,  0]
 24: [ 1,  1,  2, -2]
 25: [ 1,  1,  2, -1]
 26: [ 1,  1,  2,  0]
 27: [ 1,  1,  3, -2]
 28: [ 1,  1,  3, -1]
 29: [ 1,  1,  3,  0]
 30: [ 1,  1,  4, -2]
 31: [ 1,  1,  4, -1]
 32: [ 1,  1,  4,  0]
 33: [ 1,  1,  5, -2]
 34: [ 1,  1,  5, -1]
 35: [ 1,  1,  5,  0]
 36: [ 1,  2,  2, -2]
 37: [ 1,  2,  2, -1]
 38: [ 1,  2,  2,  0]
 39: [ 1,  2,  3, -2]
 40: [ 1,  2,  3, -1]
 41: [ 1,  2,  3,  0]
 42: [ 1,  2,  4, -2]
 43: [ 1,  2,  4, -1]
 44: [ 1,  2,  4,  0]
 45: [ 1,  2,  5, -2]
 46: [ 1,  2,  5, -1]
 47: [ 1,  2,  5,  0]
 48: [ 2, -1,  2, -2]
 49: [ 2, -1,  2, -1]
 50: [ 2, -1,  2,  0]
 51: [ 2, -1,  3, -2]
 52: [ 2, -1,  3, -1]
 53: [ 2, -1,  3,  0]
 54: [ 2, -1,  4, -2]
 55: [ 2, -1,  4, -1]
 56: [ 2, -1,  4,  0]
 57: [ 2, -1,  5, -2]
 58: [ 2, -1,  5, -1]
 59: [ 2, -1,  5,  0]
 60: [ 2,  0,  2, -2]
 61: [ 2,  0,  2, -1]
 62: [ 2,  0,  2,  0]
 63: [ 2,  0,  3, -2]
 64: [ 2,  0,  3, -1]
 65: [ 2,  0,  3,  0]
 66: [ 2,  0,  4, -2]
 67: [ 2,  0,  4, -1]
 68: [ 2,  0,  4,  0]
 69: [ 2,  0,  5, -2]
 70: [ 2,  0,  5, -1]
 71: [ 2,  0,  5,  0]
 72: [ 2,  1,  2, -2]
 73: [ 2,  1,  2, -1]
 74: [ 2,  1,  2,  0]
 75: [ 2,  1,  3, -2]
 76: [ 2,  1,  3, -1]
 77: [ 2,  1,  3,  0]
 78: [ 2,  1,  4, -2]
 79: [ 2,  1,  4, -1]
 80: [ 2,  1,  4,  0]
 81: [ 2,  1,  5, -2]
 82: [ 2,  1,  5, -1]
 83: [ 2,  1,  5,  0]
 84: [ 2,  2,  2, -2]
 85: [ 2,  2,  2, -1]
 86: [ 2,  2,  2,  0]
 87: [ 2,  2,  3, -2]
 88: [ 2,  2,  3, -1]
 89: [ 2,  2,  3,  0]
 90: [ 2,  2,  4, -2]
 91: [ 2,  2,  4, -1]
 92: [ 2,  2,  4,  0]
 93: [ 2,  2,  5, -2]
 94: [ 2,  2,  5, -1]
 95: [ 2,  2,  5,  0]
 96: [ 3, -1,  2, -2]
 97: [ 3, -1,  2, -1]
 98: [ 3, -1,  2,  0]
 99: [ 3, -1,  3, -2]
100: [ 3, -1,  3, -1]
101: [ 3, -1,  3,  0]
102: [ 3, -1,  4, -2]
103: [ 3, -1,  4, -1]
104: [ 3, -1,  4,  0]
105: [ 3, -1,  5, -2]
106: [ 3, -1,  5, -1]
107: [ 3, -1,  5,  0]
108: [ 3,  0,  2, -2]
109: [ 3,  0,  2, -1]
110: [ 3,  0,  2,  0]
111: [ 3,  0,  3, -2]
112: [ 3,  0,  3, -1]
113: [ 3,  0,  3,  0]
114: [ 3,  0,  4, -2]
115: [ 3,  0,  4, -1]
116: [ 3,  0,  4,  0]
117: [ 3,  0,  5, -2]
118: [ 3,  0,  5, -1]
119: [ 3,  0,  5,  0]
120: [ 3,  1,  2, -2]
121: [ 3,  1,  2, -1]
122: [ 3,  1,  2,  0]
123: [ 3,  1,  3, -2]
124: [ 3,  1,  3, -1]
125: [ 3,  1,  3,  0]
126: [ 3,  1,  4, -2]
127: [ 3,  1,  4, -1]
128: [ 3,  1,  4,  0]
129: [ 3,  1,  5, -2]
130: [ 3,  1,  5, -1]
131: [ 3,  1,  5,  0]
132: [ 3,  2,  2, -2]
133: [ 3,  2,  2, -1]
134: [ 3,  2,  2,  0]
135: [ 3,  2,  3, -2]
136: [ 3,  2,  3, -1]
137: [ 3,  2,  3,  0]
138: [ 3,  2,  4, -2]
139: [ 3,  2,  4, -1]
140: [ 3,  2,  4,  0]
141: [ 3,  2,  5, -2]
142: [ 3,  2,  5, -1]
143: [ 3,  2,  5,  0]
```

2. *Reprezentaţi structura logică a matricei*

Nu înțeleg ce anume aveți în vedere.

Voi da definiție.

Matricea este o structură de date unde fiecare element este indexat de 1 sau mai multe indici. 
În cazul dat, avem 4 indici.

Fiecare indice este luat dintr-un anumit interval. În acest caz, avem intervalele `[1..3, -1..2, 2..5, -2..0]`.

În memorie datele sunt stocate linear, cu toate că indexarea are loc prin o mulțime de indici. 


3. *Construiţi vectorul definitor pentru matricea dată.*

$D_4 = 1$

$D_3 = D_4 * (0 - (-2) + 1) = 3$

$D_2 = D_3 * (5 - 2 + 1) = 3 * 4 = 12$

$D_1 = D_2 * (2 - (-1) + 1) = 12 * 4 = 48$

$l_i$ sunt luate din { $1, -1, 2, -2$ }.

$h_i$ sunt luate din { $3, 2, 5, 0$ }.

$\sum_i l_i * D_i = 1 * 48 + (-1) * 12 + 2 * 3 + (-2) * 1 = 48 - 12 + 6 - 2 = 40$

$L_{totala} = 144$

4. *Desenaţi schema vectorilor Iliffe pentru matricea dată.*

Prea mult de desenat. Am desenat numai primii vectori. 144 de elemente este de fapt prea mult.

![Primele](iliffe.jpg)


5. *Arătaţi accesarea elementelor prin exemplu pentru 2 elemente diferite (în baza vectorilor Iliffe)*

De exemplu element M[1, 1, 2, 0].
1. Urmărim primul pointer (are indicii [1, 2, 3]) din vectorul de bază de dimensiunea 4. El ne duce la un vector de dimensiunea 3.
2. Urmărim al 3-lea pointer din acest vector (are indicii [-1, 0, 1, 2]). El ne aduce la un vector de dim. 2.
3. Urmărim primul pointer din acest vector (are indicii [2, 3, 4, 5]). El ne aduce la un vector de dim. 1.
4. Elementul cerut este pe ultima poziție din acest vector (are indicii [-2, -1, 0]). 

Dacă grafic:

![Indexex1](index_ex1.jpg)

M[2, 2, 5, -1]

![Indexex2](index_ex2.jpg)