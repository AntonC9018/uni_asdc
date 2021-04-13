# ASDC, Laborator nr.4: Tablouri multidimensionale.

A elaborat: **Curmanschii Anton, IA1901**.

Codul sursă întreg puteți privi [pe github-ul meu](https://github.com/AntonC9018/uni_asdc).

Acest document tot este accesibil [pe web](https://github.com/AntonC9018/uni_asdc/blob/master/doc/ASDC_Laborator_4.md).


## Sarcinile

Varianta 20.

1. Să se realizeze câte o structură de date pentru:
    - Tabloul multidimensional fără vectorul definitor;
    - Tabloul multidimensional accelerat cu ajutorul vectorului definitor;
    - Vectorul Iliffe.
2. Să se realizeze păstrarea datelor în diferite moduri: pe coloane și pe rânduri. 
3. Să se măsure timpul de execuție unei programe cu diferite moduri de adresare.
4. Corectitudinea codului trebuie să fie verificată pe baza unui tablou multidimensional cu următoarele diapazoane de indici: $\[l_1, h_1\] = \[3, 6\]; \[l_2, h_2\] = \[1, 3\]; \[l_3, h_3\] = \[-3, -1\]; \[l_4, h_4\] = \[-5, -3\]$.

Am realizat modurile de adresare diferite numai pentru primele două tablouri, deoarece nu înțeleg sensul acestora pentru vectorii Iliffe.


## Introducere

Proprietățile tablourilor:
1. Ele păstrează itemi după indice.
2. Fiecare element poate fi adresat într-un mod unic printr-un set de indici. Numărul de indici coincide cu dimensiunea tabloului. Un exemplu al sintaxei: `arr[1][2]` înseamnă adresarea elementului cu indicii `1, 2` din tabloul, numit `arr` de dimensiunea 2.
3. Diapazonul indicilor individuali poate fi oricare. De exemplu, dacă diapazonul primului indice este $l_1..h_1 = -1..1$ atunci primul indice poate avea valorile $-1, 0$ și $1$.
4. Tablourile de obicei păstrează datele în memorie începând cu zero, și terminând cu lungimea diapazonului (exclusiv), dar ascund acest lucru, luând la intrare indicii din orice set. De exemplu, dacă diapazonul este $l_1..h_1 = -1..1$, în memorie datele vor fi stocate sub indicii $0, 1, 2$, deoarece lungimea intervalului este 3.

Dacă presupunem că datele tabloului multidimensional sunt păstrate secvențial în memorie, atunci, pentru a putea adresa orice element, trebuie să calculăm "costul" asociat cu orice indice. 
Pentru tablourile unidimensionale, costul acesta ar fi 1, deoarece dacă schimbăm unicul indice cu unu, poziția secvențială tot se schimbă cu unu.
În cazul unui tablou bidimensional, schimbarea primului indice sare peste întregul rând de valori (presupunem adresarea pe rânduri, discutată mai pe urmă), de aceea costul asociat primului indice este numărul de elemente în fiecare rând. Costul asociat ultimului indice rămâne 1.
Dacă ne abstractizăm la mai multe dimensiuni, putem deduce că costul asociat ultimului indice va fi mereu 1, costul penultimului va fi egal cu lungimea diapazonului ultimului indice, iar lungimea penpenultimului va fi egală cu lungimea penultimului interval înmulțită cu costul, asociat indicelui de pe penultimă poziție.

Putem nota lucrul acesta matematic. Fie $n_i$ lungimea diapazonului $i$, $l_i, h_i$ — prima și ultima valoare din diapazonul, asociat indicelui de pe poziția $i$, iar $D_i$ să fie costul asociat indicelui de pe poziția $i$. Mai fie $N_{dim}$ numărul de dimensiuni.

Atunci $D_{N_{dim}} = 1, D_{N_{dim} - 1} = n_{N_{dim}}, D_{N_{dim} - 2} = n_{N_{dim} - 1} * n_{N_{dim}}$. 
În general, $D_i = D_{i + 1} * n_{i + 1}$.

Putem reprezenta dimensiunile prin diapazoanele în felul următor: $n_i = h_i - l_i + 1$.

Pentru a adresa orice element, vom normaliza indicele, scăzând $l_i$ și înmulțind cu costul corespunzător, $D_i$; indicele de pe poziția $i$ îl vom nota $j_i$.

$$j_{secvențial} = \sum_{i = 1}^{N_{dim}} (j_i - l_i) * D_i$$

Însă am putea stoca datele "pe coloane". De exemplu, în cazul unui tablou bidimensional, primul indice ar reprezinta coloana iar al doilea — rândul. În acest caz, dacă iterăm standart (parcurgem fiecare rând în ordine), se va schimba mai rapid primul indice, care corespunde coloanei, iar al doilea indice, care corespunde rândului, se va schimba mai lent.
Aceasta se explică prin faptul că costul primului indice este 1 (reprezintă coloana), iar al indicelui 2 — lungimea coloanei (reprezintă întregul rând).
În cazul mai general, formula costurilor de mai sus se schimbă la următoarea:
$$D_1 = 1, D_i = D_{i - 1} * n_{i - 1}$$
Formula adresării rămâne neschimbată.  


## Realizare

Vom examina la început tabloul multidimensional simplu, pe urmă îl vom remodela într-un tablou accelerat cu ajutorul vectorului definitor.

### Învelitor naiv

Deci am realizat o clasă care reprezintă un învelitor la un tablou liniar, dar care permite adresarea cu mai mulți indice.

Am hotărât să încerc să realizez această structură de date într-un stil ceva diferit de cum programez în C++ de obicei. Eu de obicei mă străduiesc să programez mai mult în stilul procedural, mai apropiat de C, dar această dată am hotărât să încerc să folosesc metodele.

De menționat încă faptul că structura de date nu invocă constructorii (destuctorii) elementelor la inițializare (ștergere), deci dacă vreți s-o folosiți cu obiecte C++, trebuie s-o modificați. 

Deci iată structură și interfața, ce reprezintă structura de date. Adresarea se face prin operatorul supraîncărcat [], care ia un tablou cu indicii la intrare. Prin metoda `set_range()` se stabilesc intervalele de pe indicii. Proprietatea `size()` returnează numărul total de elemente în întregul tablou, iar metodele `size()`, `start()` și `end()` care iau la intrare indicele poziției, returnează datele despre diapazoane. `init()` joacă rolul constructorului, iar `free()` — al destructorului.

```C++
template<typename T, size_t N_DIM>
struct Raw_Array_Wrapper
{
    T* items;
    Index_Range ranges[N_DIM];
    Major_Order order;

    T& operator[](const ssize_t[N_DIM]);
    void set_range(size_t index, Index_Range range);
    void init(Major_Order major_order);
    size_t size() const;
    size_t size(size_t index_depth) const;
    ssize_t start(size_t index_depth) const;
    ssize_t end(size_t index_depth) const;

    void free();
};
```

Metoda de adresare de fiecare dată calculează costurile din nou, pe urmă folosindu-le la calcularea indicelui fizic (secvențial). Ea realizează formula pentru $j_{secvențial}$ descrisă mai sus.

```C++
template<typename T, size_t N_DIM>
T& Raw_Array_Wrapper<T, N_DIM>::operator[](const ssize_t indices[N_DIM])
{
    size_t index_costs[N_DIM];
    size_t physical_index = 0;

    init_costs(index_costs, ranges, N_DIM, order);
    
    for (size_t i = 0; i < N_DIM; i++)
    {
        physical_index += (indices[i] - ranges[i].start) * index_costs[i];
    }

    return items[physical_index];
}
```

Costurile sunt calculate după formulele lor în următoarele funcții:
```C++
enum Major_Order { ROW_MAJOR, COL_MAJOR };

inline void init_costs_row_major(size_t* index_costs, const Index_Range* ranges, const size_t n_dim)
{
    index_costs[n_dim - 1] = 1;
    for (size_t i = n_dim - 1; i > 0; i--)
    {
        index_costs[i - 1] = index_costs[i] * ranges[i].size();
    }
}

inline void init_costs_col_major(size_t* index_costs, const Index_Range* ranges, const size_t n_dim)
{
    index_costs[0] = 1;
    for (size_t i = 1; i < n_dim; i++)
    {
        index_costs[i] = index_costs[i - 1] * ranges[i - 1].size();
    }
}

inline void init_costs(size_t* index_costs, const Index_Range* ranges, const size_t n_dim, const Major_Order order)
{
    if (order == ROW_MAJOR) init_costs_row_major(index_costs, ranges, n_dim);
    else                    init_costs_col_major(index_costs, ranges, n_dim);
}
```

`Index_Range` resprezintă valorile $l_i$ (`start`), $h_i$ (`end`) și prin urmare proprietatea $n_i$ (`size`).

```C++
struct Index_Range
{
    ssize_t start; 
    ssize_t end;

    size_t size() const;
};

size_t Index_Range::size() const { return (size_t)(end - start) + 1; }
```

### Observații

Putem observa că nu este eficient să calculăm `D_i` la fiecare deducere a indicelui secvențial. Putem face aceasta, dar problema nu va fi optimizată la maxim.

Observația ce ne aduce la conceperea *vectorului definitor* și optimizarea maximală a problemei este că formula indicelui secvențial poate fi decompusă în două părți:

$$j_{secvențial} = \sum_{i = 1}^{N_{dim}} (j_i - l_i) * D_i = \sum_{i = 1}^{N_{dim}} j_i * D_i - \sum_{i = 1}^{N_{dim}} l_i * D_i$$

Aici, $\sum_{i = 1}^{N_{dim}} l_i * D_i$ este o constantă, deci o putem stoca pentru a face mai puține calculări.

### Vectorul definitor

Vectorul definitor (în engleză, *dope vector*) combină stocarea lor $D_i$ și lor $\sum_{i = 1}^{N_{dim}} l_i * D_i$. În plus, la inițializare se calculează numărul de elemente total.

Iată realizarea mea a vectorului definitor în C++ care combină toate conceptele și formulele introduse mai sus. De menționat faptul că acest vector nu depinde nici de tipul de date stocate în tablou, nici de însuși tabloul. 

```C++
template<size_t N_DIM>
struct Dope_Vector
{
    Index_Range ranges[N_DIM];
    size_t total_length;
    size_t index_costs[N_DIM];
    size_t index_cost_constant_term;

    void init_cache_row_major();
    void init_cache_col_major();
    void init_constants();
    size_t map_indices_to_physical_index(const ssize_t indices[N_DIM]) const;
};

template<size_t N_DIM>
void Dope_Vector<N_DIM>::init_constants()
{
    index_cost_constant_term = 0;
    total_length = 1;
    for (size_t i = 0; i < N_DIM; i++)
    {
        index_cost_constant_term += index_costs[i] * ranges[i].start; 
        total_length *= ranges[i].size();
    }
}

template<size_t N_DIM>
void Dope_Vector<N_DIM>::init_cache_row_major()
{
    init_costs_row_major(index_costs, ranges, N_DIM);
    init_constants();
}

template<size_t N_DIM>
void Dope_Vector<N_DIM>::init_cache_col_major()
{
    init_costs_col_major(index_costs, ranges, N_DIM);
    init_constants();
}

template<size_t N_DIM>
size_t Dope_Vector<N_DIM>::map_indices_to_physical_index(const ssize_t indices[N_DIM]) const
{
    size_t index = 0;
    for (size_t i = 0; i < N_DIM; i++)
    {
        index += indices[i] * index_costs[i];
    }
    return index - index_cost_constant_term;
}
```

### Învelitor avansat

A doua versiune a învelitorului deja folosește vectorul definitor. 

Are exact aceași interfață.

```C++
template<typename T, size_t N_DIM>
struct Dope_Wrapper
{
    T* items;
    Dope_Vector<N_DIM> dope;

    T& operator[](const ssize_t[N_DIM]);
    void set_range(size_t index, Index_Range range);
    void init(Major_Order major_order);
    size_t size() const;
    size_t size(size_t index_depth) const;
    ssize_t start(size_t index_depth) const;
    ssize_t end(size_t index_depth) const;

    void free();
};
```

Însă inițializarea și adresarea sunt mai simple.

```C++
template<typename T, size_t N_DIM>
void Dope_Wrapper<T, N_DIM>::init(Major_Order major_order)
{
    if (major_order == ROW_MAJOR) dope.init_cache_row_major();
    else                          dope.init_cache_col_major();
    items = (T*) malloc(size() * sizeof(T));
}

template<typename T, size_t N_DIM>
T& Dope_Wrapper<T, N_DIM>::operator[](const ssize_t indices[N_DIM])
{
    auto physical_index = dope.map_indices_to_physical_index(indices);
    return items[physical_index];
}
```

### Test

Datorită faptului că ei au aceeași interfață, le putem verifica într-o singură funcție. Am definit unele macro-uri care vor fi folosite pe urmă și în funcțiile pentru profilare.

```C++
const static DS::Index_Range array_ranges[] { {3, 6}, {1, 3}, {-3, -1}, {-5, -3} };

#define CREATE_ARRAY(mode)             \
    Wrapper_Type arr;                  \
    arr.set_range(0, array_ranges[0]); \
    arr.set_range(1, array_ranges[1]); \
    arr.set_range(2, array_ranges[2]); \
    arr.set_range(3, array_ranges[3]); \
    arr.init((mode));                  \
    ssize_t indices[4];

#define i indices[0]
#define j indices[1]
#define k indices[2]
#define w indices[3]

#define ITERATE_ROWS \
    for (i = arr.start(0); i <= arr.end(0); i++) \
    for (j = arr.start(1); j <= arr.end(1); j++) \
    for (k = arr.start(2); k <= arr.end(2); k++) \
    for (w = arr.start(3); w <= arr.end(3); w++) 

#define ITERATE_COLS \
    for (w = arr.start(3); w <= arr.end(3); w++) \
    for (k = arr.start(2); k <= arr.end(2); k++) \
    for (j = arr.start(1); j <= arr.end(1); j++) \
    for (i = arr.start(0); i <= arr.end(0); i++) 


template<typename Wrapper_Type>
void array_test_mode(DS::Major_Order mode)
{
    CREATE_ARRAY(mode);

    size_t el_count = 0;

    if (mode == DS::ROW_MAJOR)
        ITERATE_ROWS { arr[indices] = el_count++; }
    else
        ITERATE_COLS { arr[indices] = el_count++; }

    assert(el_count == arr.size());

    ITERATE_ROWS { printf("arr[%2zi][%2zi][%2zi][%2zi] = %i\n", i, j, k, w, arr[indices]); }
    printf("\n");

    ITERATE_COLS { printf("arr[%2zi][%2zi][%2zi][%2zi] = %i\n", i, j, k, w, arr[indices]); }
    printf("\n");

    // should always print sequentially
    for (size_t _i = 0; _i < arr.size(); _i++)
    {
        printf("arr[%zu] = %i\n", _i, arr.items[_i]);
    }   
    printf("\n");
 
    arr.free();
}

#undef i
#undef j
#undef k
#undef w
```

Funcțiile afișează mult, deoarece tabloul are 108 de elemente, însă esența este că, dacă tabloul păstrează datele corect, atunci pentru orice mod de adresare selectat, dacă elementele au fost inițializate după acel mod, atribuind elementelor de pe indici respectivi numerele consecutive de la 0 pâna la lungimea tabloului, afișarea secvențială a tuturor elementelor va afișa elementele în ordinea crescătoare, adică orice element va avea valoarea indicelui său. 
La executarea codului observăm că afișarea secvențială a întregului tablou mereu afișează datele crescător ceea ce înseamnă că tabloul funcționează corect.


### Iliffe

Tabloul lui Iliffe folosește o altă modalitate pentru a păstra datele. Nu este utilizat nici un spațiu secvențial. În loc de aceasta, fiecare vector conține în mod secvențial pointeri la vectori asemănători de dimensiunea cu 1 mai mică, iar vectorii de dimensiunea 1 păstrează respectiv datele.

Dacă presupunem adresarea de la 0 la N (exclusiv), atunci următoarea structura de date ar folosi ideea descrisă.

```C++
template<typename T, size_t N_DIM>
struct Iliffe_Vector
{
    size_t size;
    Iliffe_Vector<T, N_DIM - 1>* vectors;

    Iliffe_Vector<T, N_DIM - 1>& operator[](size_t index);
    const Iliffe_Vector<T, N_DIM - 1>& operator[](size_t index) const;

    static Iliffe_Vector<T, N_DIM> make(size_t size);

    void free();
};

template<typename T, size_t N_DIM>
Iliffe_Vector<T, N_DIM> Iliffe_Vector<T, N_DIM>::make(size_t size)
{
    Iliffe_Vector<T, N_DIM> result;
    result.vectors = (Iliffe_Vector<T, N_DIM - 1>*) calloc(
        sizeof(Iliffe_Vector<T, N_DIM - 1>), size);
    result.size = size;
    return result;
}

template<typename T, size_t N_DIM>
Iliffe_Vector<T, N_DIM - 1>& Iliffe_Vector<T, N_DIM>::operator[](size_t index)
{
    assert(index < size);
    return vectors[index];
}

template<typename T, size_t N_DIM>
const Iliffe_Vector<T, N_DIM - 1>& Iliffe_Vector<T, N_DIM>::operator[](size_t index) const
{
    assert(index < size);
    return vectors[index];
}

template<typename T, size_t N_DIM>
void Iliffe_Vector<T, N_DIM>::free()
{
    for (size_t i = 0; i < size; i++)
    {
        vectors[i].free();
    }
    free(vectors);
}

template<typename T>
struct Iliffe_Vector<T, 1>
{
    size_t size;
    T* elements;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    static Iliffe_Vector<T, 1> make(size_t size);

    void free();
};

template<typename T>
Iliffe_Vector<T, 1> Iliffe_Vector<T, 1>::make(size_t size)
{
    Iliffe_Vector<T, 1> result;
    result.elements = (T*) malloc(sizeof(T) * size);
    result.size = size;
    return result;
}

template<typename T>
T& Iliffe_Vector<T, 1>::operator[](size_t index)
{
    assert(index < size);
    return elements[index];
}

template<typename T>
const T& Iliffe_Vector<T, 1>::operator[](size_t index) const
{
    assert(index < size);
    return elements[index];
}

template<typename T>
void Iliffe_Vector<T, 1>::free()
{
    free(elements);
}
```

Cu diapazoane nu este mai complicat. Vom păstra diapazonul în fiecare structură.

```C++
template<typename T, size_t N_DIM>
struct Ranged_Iliffe_Vector
{
    Index_Range range;
    Ranged_Iliffe_Vector<T, N_DIM - 1>* vectors;
    
    size_t size();

    Ranged_Iliffe_Vector<T, N_DIM - 1>& operator[](ssize_t index);
    const Ranged_Iliffe_Vector<T, N_DIM - 1>& operator[](ssize_t index) const;

    static Ranged_Iliffe_Vector<T, N_DIM> make(Index_Range range);

    void free();
};

template<typename T, size_t N_DIM>
Ranged_Iliffe_Vector<T, N_DIM> Ranged_Iliffe_Vector<T, N_DIM>::make(Index_Range range)
{
    assert(range.end >= range.start);

    Ranged_Iliffe_Vector<T, N_DIM> result;
    result.range = range;
    result.vectors = (Ranged_Iliffe_Vector<T, N_DIM - 1>*) calloc(
        sizeof(Ranged_Iliffe_Vector<T, N_DIM - 1>), result.size());

    return result;
}

template<typename T, size_t N_DIM>
size_t Ranged_Iliffe_Vector<T, N_DIM>::size()
{
    return range.size();
}

template<typename T, size_t N_DIM>
Ranged_Iliffe_Vector<T, N_DIM - 1>& Ranged_Iliffe_Vector<T, N_DIM>::operator[](ssize_t index)
{
    assert(index >= range.start && index <= range.end);
    return vectors[index - range.start];
}

template<typename T, size_t N_DIM>
const Ranged_Iliffe_Vector<T, N_DIM - 1>& Ranged_Iliffe_Vector<T, N_DIM>::operator[](ssize_t index) const
{
    assert(index >= range.start && index <= range.end);
    return vectors[index - range.start];
}

template<typename T, size_t N_DIM>
void Ranged_Iliffe_Vector<T, N_DIM>::free()
{
    for (size_t i = 0; i < size(); i++)
    {
        vectors[i].free();
    }
    std::free(vectors);
}

template<typename T>
struct Ranged_Iliffe_Vector<T, 1>
{
    Index_Range range;
    T* elements;

    size_t size();

    T& operator[](ssize_t index);
    const T& operator[](ssize_t index) const;

    static Ranged_Iliffe_Vector<T, 1> make(Index_Range range);

    void free();
};

template<typename T>
Ranged_Iliffe_Vector<T, 1> Ranged_Iliffe_Vector<T, 1>::make(Index_Range range)
{
    assert(range.end >= range.start);

    Ranged_Iliffe_Vector<T, 1> result;
    result.range = range;
    result.elements = (T*) malloc(sizeof(T) * result.size());

    return result;
}

template<typename T>
size_t Ranged_Iliffe_Vector<T, 1>::size()
{
    return range.size();
}

template<typename T>
T& Ranged_Iliffe_Vector<T, 1>::operator[](ssize_t index)
{
    assert(index >= range.start && index <= range.end);
    return elements[index - range.start];
}

template<typename T>
const T& Ranged_Iliffe_Vector<T, 1>::operator[](ssize_t index) const
{
    assert(index >= range.start && index <= range.end);
    return elements[index - range.start];
}

template<typename T>
void Ranged_Iliffe_Vector<T, 1>::free()
{
    std::free(elements);
}
```

Crearea tabloului cu diapazoanele date mie se va realiza în modul următor:

```C++
DS::Ranged_Iliffe_Vector<int, 4> iliffe_create_vector()
{
    using namespace DS;

    auto arr = Ranged_Iliffe_Vector<int, 4>::make(array_ranges[0]);
    for (ssize_t i = array_ranges[0].start; i <= array_ranges[0].end; i++)
    {
        arr[i] = Ranged_Iliffe_Vector<int, 3>::make(array_ranges[1]);
        for (ssize_t j = array_ranges[1].start; j <= array_ranges[1].end; j++)
        {
            arr[i][j] = Ranged_Iliffe_Vector<int, 2>::make(array_ranges[2]);
            for (ssize_t k = array_ranges[2].start; k <= array_ranges[2].end; k++)
            {
                arr[i][j][k] = Ranged_Iliffe_Vector<int, 1>::make(array_ranges[3]);
                // în arr[i][j][k] se poate de pus int-uri prin indexare 
            }   
        }   
    }
    return arr;
}
```

Dezavantajele sunt aparente:
1. Pentru a ajunge la elemente trebuie să trecem printr-o secvență de dereferiri ale pointerilor, ce este lent.
2. Datele nu sunt stocate într-un loc central în memorie, deci iterarea direct pe memorie este imposibilă.
3. Parcurgerea în ordine va fi mai lentă, deoarece datele nu stă consecutiv în memorie.
4. Fiecare vector Iliffe este independent de ceilalți (aceasta ar fi considerat și ca un avantaj în unele cazuri).

Avantajele:
1. Este mai flexibil. Este mai ușor să schimbăm dimensiunile. Putem realiza și ideea de *jagged arrays*, adică ca vectorii la același nivel să aibă lungimi diferite. Aceasta este imposibil de făcut cu tablouri multidimensionale ordinare.
2. Este ușor să schimbăm doar o parte mică de date, de aceea vectorii iliffe sunt utile la crearea structurilor de date *immutable*.


## Profilare

Arăt toate funcțiile folosite la profilare. 
Funcția principală folosește profilatorul descris în prima lucrare pentru a executa unele teste la aceste structuri de date:

```C++
void arrays_profile()
{
    using namespace DS;
    const size_t num_experiments = 100000;

    profiler_perform_experiments(
        "Raw_Array_Wrapper --- ROW/COLUMN MAJOR",
        array_profile_rowcol<Raw_Array_Wrapper<int, 4>>,
        num_experiments
    );

    profiler_perform_experiments(
        "Raw_Array_Wrapper --- SEQUENTIAL",
        array_profile_sequential<Raw_Array_Wrapper<int, 4>>,
        num_experiments
    );

    profiler_perform_experiments(
        "Dope_Wrapper --- ROW/COLUMN MAJOR",
        array_profile_rowcol<Dope_Wrapper<int, 4>>,
        num_experiments
    );

    profiler_perform_experiments(
        "Dope_Wrapper --- SEQUENTIAL",
        array_profile_sequential<Dope_Wrapper<int, 4>>,
        num_experiments
    );

    profiler_perform_experiments(
        "Iliffe --- ROW ITERATION",
        iliffe_profile_rows,
        num_experiments
    );

    profiler_perform_experiments(
        "Iliffe --- COLUMN ITERATION",
        iliffe_profile_cols,
        num_experiments
    );
}
```

Funcțiile de profilare la tablouri obișnuite folosesc macrourile menționate în secțiunea dedicată lor de mai sus. 
Preîntâmpin că rezultatele la iterare după coloane la vectori Iliffe nu vor fi echitabile, deoarece nu putem lua valorile diapazoanelor din înșiși vectorii Iliffe corespunzători când iterăm după coloane. 
Încă, adresările pe rând și pe coloană sunt combinate într-o singură categorie la tablouri obișnuite, deoarece ele urmăresc de fapt aceeași logică și ar avea unele rezultate asemănătoare, de aceea nu necesită să le distingem la profilare.
Știu că sumez gunoiul din vectorii dar nici nu utilizez valoarea primită. 
Prin sumarea aceasta am prevenit optimizarea posibilă de către compilator prin iluzia că totuși fac ceva cu valorile.

```C++
template<typename Wrapper_Type>
void array_profile_rowcol(Profiler* profiler)
{
    CREATE_ARRAY(DS::ROW_MAJOR);
    int sum = 0;
    ITERATE_ROWS { 
        sum += arr[indices]; 
        profiler->num_iters++; 
    }
    arr.free();
}

template<typename Wrapper_Type>
void array_profile_sequential(Profiler* profiler)
{
    CREATE_ARRAY(DS::ROW_MAJOR);
    int sum = 0;
    for (size_t _i = 0; _i < arr.size(); _i++)
    {
        sum += arr.items[_i];
        profiler->num_iters++; 
    }
    arr.free(); 
}

#undef i
#undef j
#undef k
#undef w


void iliffe_profile_rows(Profiler* profiler)
{
    auto arr = iliffe_create_vector();
    
    int sum = 0;
    for (ssize_t i = arr.range.start;          i <= arr.range.end;          i++)
    for (ssize_t j = arr[i].range.start;       j <= arr[i].range.end;       j++)
    for (ssize_t k = arr[i][j].range.start;    k <= arr[i][j].range.end;    k++)
    for (ssize_t w = arr[i][j][k].range.start; w <= arr[i][j][k].range.end; w++)
    {
        sum += arr[i][j][k][w];
        profiler->num_iters++;
    }

    arr.free();
}

void iliffe_profile_cols(Profiler* profiler)
{
    auto arr = iliffe_create_vector();
    int sum = 0;
    
    // this is the best approximation we can get
    // since the vectors containing the ranges are not accessible in column-major order
    // there is no way to iterate over them, without knowing the ranges in advance.
    for (ssize_t w = array_ranges[3].start; w <= array_ranges[3].end; w++)
    for (ssize_t k = array_ranges[2].start; k <= array_ranges[2].end; k++)
    for (ssize_t j = array_ranges[1].start; j <= array_ranges[1].end; j++)
    for (ssize_t i = array_ranges[0].start; i <= array_ranges[0].end; i++)
    {
        sum += arr[i][j][k][w];
        profiler->num_iters++;
    }

    arr.free();
}
```

Urmează rezultatele. 
Se observă că iterarea pe un vector accelerat este de aproape 2 ori mai rapidă decât pe învelitorul naiv. 
Iterarea secvențială este atât de lentă pentru învelitorul naiv, deoarece lungimea este calculată din nou la fiecare iterație (însă optimizarea elimină această problemă). 
Clar din ce cauză este lentă iterarea pe vectori Iliffe: pointerii. 
Iterarea pe coloane la vectori Iliffe s-a măsurat mai rapidă din cauzele deja explicate mai sus.
În principiu, iterarea pe rânduri ar fi mai rapidă, deoarece iterăm liniar prin vectori, iar datele ce stă liniar în memorie permite parcurgerea mai rapidă de către procesor.

```
Raw_Array_Wrapper --- ROW/COLUMN MAJOR          
--------------------------------------          
Time elapsed: 720.00100 (ms)                    
Number of iterations: 10800000                  
Number of experiments: 100000                   
Time per experiment: 7.20001 (microsec)         
Iterations per experiment: 108.000000           
--------------------------------------          
                                                
Raw_Array_Wrapper --- SEQUENTIAL                
--------------------------------------          
Time elapsed: 430.00000 (ms)                    
Number of iterations: 10800000                  
Number of experiments: 100000                   
Time per experiment: 4.30000 (microsec)         
Iterations per experiment: 108.000000           
--------------------------------------          
                                                
Dope_Wrapper --- ROW/COLUMN MAJOR               
--------------------------------------          
Time elapsed: 350.00000 (ms)                    
Number of iterations: 10800000                  
Number of experiments: 100000                   
Time per experiment: 3.50000 (microsec)         
Iterations per experiment: 108.000000           
--------------------------------------          
                                                
Dope_Wrapper --- SEQUENTIAL                     
--------------------------------------          
Time elapsed: 70.00000 (ms)                     
Number of iterations: 10800000                  
Number of experiments: 100000                   
Time per experiment: 0.70000 (microsec)         
Iterations per experiment: 108.000000           
--------------------------------------          
                                                
Iliffe --- ROW ITERATION                        
--------------------------------------          
Time elapsed: 1210.00600 (ms)                   
Number of iterations: 10800000                  
Number of experiments: 100000                   
Time per experiment: 12.10006 (microsec)        
Iterations per experiment: 108.000000           
--------------------------------------          
                                                
Iliffe --- COLUMN ITERATION                     
--------------------------------------          
Time elapsed: 852.00400 (ms)                    
Number of iterations: 10800000                  
Number of experiments: 100000                   
Time per experiment: 8.52004 (microsec)         
Iterations per experiment: 108.000000           
--------------------------------------          
```