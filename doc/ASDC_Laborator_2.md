# ASDC, Laborator nr.2.

A elaborat: **Curmanschii Anton, IA1901**.

Codul sursă întreg puteți privi [pe github-ul meu](https://github.com/AntonC9018/uni_asdc).

Acest document tot este accesibil [pe web](https://github.com/AntonC9018/uni_asdc/blob/master/doc/ASDC_Laborator_2.md).


## Sarcinile

**Să se creeze un fişier textual** care conţine cel puţin 50 de înregistrări, cu cel puţin 5 câmpuri şi are cel puţin 2 tipuri de date, iar câmpul cheie trebuie să fie unic şi neordonat. Fişierul creat este cel de la prima lucrare de laborator.

**Să fie realizat un program în C++**, în care sunt implementate câteva metode de sortare (cel puţin două) în tabele neordonate după câmpul cheie din fişierul textual creat anterior. Pentru fiecare metodă de sortare de analizat complexitatea teoretică şi practică. 

**Să se afişeze următoarele date:** 
- Estimarea teoretică a complexităţii; 
- Număr de comparaţii; 
- Număr de permutări;
- Timpul de execuţie al fiecărui algoritm de sortare. 

**De descris algoritmul metodelor de căutare pe paşi.**


## Introducere

În cadrul laboratorului am realizat 4 metode de sortare:
1. Selection sort;
2. Heap sort;
3. Merge sort;
4. Quick sort.

Însă, voi clarifica un pic termenul "permutare". O permutare (swap) de fapt înseamnă o interschimbare a valorilor între două variabile. Însă, unii algoritmi performează desemnarea (assignment), păstrând valorile pe indicele schimbate într-o memorie aparte prin `memcpy()` înainte de interschimbări. Deci, așa situație (`memcpy()` într-un bufer aparte a întregului vector, pe urmă schimbarea valorii a elementului de pe o anumită poziție) tot o voi considera o permutare. 

## Main

Am realizat două funcții pentru analiza algoritmilor propuși: una pentru verificare, una pentru profilare.

Funcția pentru verificare a algoritmilor descarcă datele din fișierul sortat după câmpul cheie, după ce urmărează acești pași pentru fiecare algoritm:
- amestecă înregistrările într-un mod arbitrar;
- sortează utilizând unul din algoritme;
- verifică, dacă vectorul s-a sortat.

Funcția a ieșit destul de succintă, datorită macrourilor.

```C++
void sort_tests()
{
    auto records = read_records_from_csv("data.csv");
    auto begin = records.begin();
    auto end   = records.end();

#define SHUFFLE()       std::random_shuffle(begin, end)
#define ASSERT_SORTED() assert(Sort::is_sorted(begin, end, compare_func))
#define SORT_TEST(sort) \
    SHUFFLE(); \
    (sort)(begin, end, compare_func); \
    ASSERT_SORTED();

    SORT_TEST(Sort::merge_sort);
    SORT_TEST(Sort::heap_sort);
    SORT_TEST(Sort::quick_sort);
    SORT_TEST(Sort::selection_sort);

    destroy_records(records);
}
```

Funcția pentru profilare citește datele, după ce folosește funcția ce performă experimente, descrisă în laboratorul nr.1, pentru a profila algoritmii. Iarăși, datorită macrourilor, funcția a ieșit foarte succintă.
```C++
void sort_profile()
{
    auto records = read_records_from_csv("data.csv");
    auto begin = records.begin();
    auto end   = records.end();

    const size_t num_experiments = 1000;

#define PERFORM_EXPERIMENTS(text, sort) \
    profiler_perform_experiments( \
        (text), \
        [&](auto p) \
        { \
            std::random_shuffle(begin, end);\
            (sort)(begin, end, compare_func, p); \
        },\
        num_experiments\
    )\

    PERFORM_EXPERIMENTS("Merge Sort.",     Sort::merge_sort);
    PERFORM_EXPERIMENTS("Heap Sort.",      Sort::heap_sort);
    PERFORM_EXPERIMENTS("Quick Sort.",     Sort::quick_sort);
    PERFORM_EXPERIMENTS("Selection Sort.", Sort::selection_sort);

    destroy_records(records);
}
```

Sortarea se face după câmpul textual `first_name`, însă codul permite stabilirea oricărei funcții de comparare. Funcția returnează o valoare > 0 dacă primul argument este mai mare decât al doilea element, 0 dacă sunt egale, < 0 dacă este mai mic (à la `strcmp()` sau `memcmp()`). Însă, în algoritmii este luată în considerare numai condiția < 0 sau > 0, dar nu ambele, ce aș putea simula și cu o funcția care returnează `bool`:
```C++
// Use references here, since it plays out better with templates
s32 compare_func(const Record& a, const Record& b)
{
    return str_compare(str_view(a.first_name), str_view(b.first_name)); 
};
```

Însăși `str_compare()` utilizează `memcmp()` pentru a compara șirurile:
```C++
#define MIN(a, b) ((a) > (b) ? (b) : (a))

int str_compare(str_view_t str1, str_view_t str2)
{
    if (str1.chars == str2.chars)
        return 0;

    int result = memcmp(str1.chars, str2.chars, MIN(str1.length, str2.length));

    return result == 0 ? (int)str1.length - (int)str2.length : result;
}
```


# Prezentarea algoritmilor de sortare

Voi analiza algoritmii de sortare în așa ordine:
- descrierea algoritmului;
- prezentarea codului;
- complexitatea teoretică.

Analiza datelor de profilare urmează după descrierea tuturor algoritmilor.

## Selection sort

### Algoritmul

Selection sort are următoarea logică:
1. din vectorul se selectează elementul minim;
2. acest element se pune pe prima poziție;
3. este sortat restul vectorului prin același algoritm.

### Codul

Iteratorii `start` și `end` conțin pointeri la primul element și elementul după ultimul element, respectiv, ca și în toți algoritmii din `STL`. Această metodă de sortare poate sorta orice structură de date, unde iteratorul poate fi incrementat (forward iterator).
```C++
template<typename Iterator, typename Comparator>
void selection_sort(Iterator start, Iterator end, 
    Comparator compare_func, Profiler* profiler = &_std_profiler)
{
    while (start != end)
    {
        profiler->num_iters++;

        // find the minimum
        Iterator min_iter = start;
        for (Iterator i = start + 1; i != end; i++)
        {
            profiler->num_comparisons++;
            if (compare_func(*i, *min_iter) < 0)
            {
                min_iter = i;
            }
        }

        Sort::swap(&*min_iter, &*start);
        profiler->num_swaps++;

        start++;
    }
}
```

Funcția `swap()` are două implementări: una pentru pointeri și una pentru referințe:
```C++
template<typename T>
inline void swap(T& a, T& b)
{
    T t;
    memcpy(&t, &a, sizeof(T));
    memcpy(&a, &b, sizeof(T));
    memcpy(&b, &t, sizeof(T));
}

template<typename T>
inline void swap(T* a, T* b)
{
    T t;
    memcpy(&t, a, sizeof(T));
    memcpy(a, b,  sizeof(T));
    memcpy(b, &t, sizeof(T));
}
```

### Complexitatea

Selection sort este unul foarte simplu însă nu este practic. Pentru a sorta un vector de $N$ elemente, el trebuie să treacă prin toate elementele câte $N/2$ ori, de unde rezultă că complexitatea-timp este $O(N^{2})$. 

Algoritmele ce urmează toții au compexitatea mai favorabilă și sunt utile în practică.


## Heap sort

### Algoritmul

Nu voi prezenta algoritmul complet, deoarece este destul de mare și complicat.

În scurt, din datele de intrare este construită o structură specială de date, numită `max-heap`. Este arbore, dar poate fi reprezentat în memorie contiguă datorită faptului că este dens. Într-un max heap, elementele sunt aranjate în ordinea descrescătoare: rădăcina este mereu mai mare decât fiii.

Max-heap-ul este construit prin adăugarea de câte un element nou în arbore, după ce arbore este restabilizat (funcția `heapify()`), adică restorăm proprietatea că elementul părinte este mai mare decât fiii săi.

După ce heap-ul a fost constuit, se ia primul element (care este în același timp și elementul maxim) din acest heap și se pune pe ultima poziție în vector, iar elementul din capătul vectorului ia poziția în rădăcină. După aceasta, se restorează proprietatea lui max-heap în acest heap nou, dar fără ultimul element. Repetăm acest procedeu până când heap-ul nu conține nici un element. 

### Codul

L-am definit pe `T` ca `Record`, dar, în principiu, `T`-ul trebuie să fie în template, sau trebuie de utilizat `Iterator` peste tot. Însă, prin așa notația am subliniat faptul că nu putem folosi sortarea această dacă iteratorul nu permite random access.
```C++
template<typename Comparator>
void heapify(T* arr, size_t length, size_t current_index, 
    Comparator compare_func, Profiler* profiler)
{
    profiler->num_iters++;

    size_t index_left  = current_index * 2 + 1;
    size_t index_right = current_index * 2 + 2;

    size_t index_largest = current_index;

    if (index_left < length)
    {
        profiler->num_comparisons++;
        if (compare_func(arr[index_left], arr[index_largest]) > 0)
            index_largest = index_left;
    }

    if (index_right < length)
    {
        profiler->num_comparisons++;
        if (compare_func(arr[index_right], arr[index_largest]) > 0)
            index_largest = index_right;
    }

    if (index_largest != current_index)
    {
        Sort::swap(arr[current_index], arr[index_largest]);
        profiler->num_swaps++;

        heapify(arr, length, index_largest, compare_func, profiler);
    }
}

template<typename Comparator>
void heap_sort_internal(T* arr, size_t length, 
    Comparator compare_func, Profiler* profiler)
{
    for (size_t i = length / 2; i > 0; i--)
    {
        heapify(arr, length, i - 1, compare_func, profiler);
    }

    for (size_t i = length - 1; i > 0; i--)
    {
        Sort::swap(arr[0], arr[i]);
        profiler->num_swaps++;

        heapify(arr, i, 0, compare_func, profiler);
    }
}

template<typename Iterator, typename Comparator>
void heap_sort(Iterator begin, Iterator end, 
    Comparator compare_func, Profiler* profiler = &_std_profiler)
{
    heap_sort_internal(&*begin, &*end - &*begin, compare_func, profiler); 
}
```

### Complexitatea

Adăugarea elementelor într-un max-heap sau restorarea proprietăților de max-heap are complexitatea $log N$. Facem acest lucru de $3N/2$ ori, deci complexitatea-timp este $O(NlogN)$. Însă avantajul este că complexitatea-spațiu este $O(logN)$, deoarece sortarea are loc *in-place*, însă folosim spațiu pe stack pentru apelări recursive.


## Merge sort

### Algoritmul

Algoritmul este destul de ușor de înțeles:
1. Vectorul este înjumătățit, și părțile respective sunt sortate prin acest algoritm.
2. Părțile sunt unite în răspunsul final, prin iterarea în ordine prin elementele vectorilor sortate, și inserarea în următoarea poziție a rezultatului elementului mai mic. Adică:
    - Ținem cont de indicele curent în prima jumătate sortată (inițial 0);
    - Ținem cont de indicele curent în a două jumătate sortată (inițial 0);
    - Într-un ciclu, comparăm elementele de pe pozițiile curente din acele jumătăți sortate. Selectăm valoarea cea mai mică din două și o punem în următoarea poziție din vectorul de răspuns. Incrementăm indicele jumătății a cărei valoare era selectată. Repetăm până când nu ne ajungem la sfârșitul unei jumătăți.
    - Copiem restul celeilalte jumătăți în răspuns.

### Codul

Merge sort iarăși necesită random access.

Funcția-interfață alocă un bufer cu malloc de lungimea a întregului vector. Știu că în gcc se admite alocarea vectorilor temporare pe stack, însă abordarea mea este mai versatilă, cu toate că este și mai puțin eficientă.
```C++
// `end` is one past the last element, like in STL algorithms
template<typename Iterator, typename Comparator>
void merge_sort(Iterator begin, Iterator end, Comparator compare_func, Profiler* profiler = &_std_profiler)
{
    if (begin + 1 < end)
    {
        auto length = end - begin;

        auto buffer = (T*) malloc(length * sizeof(T));

        merge_sort_internal(&*begin, length, compare_func, buffer, profiler);

        free(buffer);
    }
}
```

Funcția principală deja conține pașii explicați mai sus. Am considerat ultimele `memcpy()` tot să fie permutări, astfel este într-un fel mai echitabil.
```C++
template<typename Comparator>
void merge_sort_internal(T* begin, size_t length, Comparator compare_func, T* buffer, Profiler* profiler)
{
    if (length <= 1)  return;

    // Past this point counts as an iteration.
    profiler->num_iters++;

    auto half_length = length / 2;
    auto middle = half_length + begin;

    merge_sort_internal(begin,           half_length, compare_func, buffer, profiler);
    merge_sort_internal(middle, length - half_length, compare_func, buffer, profiler);

    auto next = begin;
    memcpy(buffer, begin, length * sizeof(T));
    
    auto buf_left   = buffer;
    auto buf_middle = buffer + half_length;
    auto buf_right  = buf_middle;
    auto buf_end    = buffer + length;

    while (buf_left < buf_middle && buf_right < buf_end)
    {
        auto comp_result = compare_func(*buf_left, *buf_right);
        profiler->num_comparisons++;

        // Since we've done a memcpy before this, it still kind of counts as a swap
        profiler->num_swaps++;
        
        // buf_left is greater than right, insert the right one
        if (comp_result > 0)
        {
            *next = *buf_right;
            buf_right++;
        }
        else
        {
            *next = *buf_left;
            buf_left++;
        }

        next++;
    }

    // Fill in the rest.
    if (buf_left != buf_middle)
    {
        memcpy(next, buf_left, (buf_middle - buf_left) * sizeof(T));
    }
    else
    {
        memcpy(next, buf_right, (buf_end - buf_right) * sizeof(T));
    }

}
```

### Complexitatea

Complexitatea-timp este $O(NlogN)$ deoarece adâncimea recursivă a algoritmului este $logN$ și la fiecare adâncime se face $N$ comparări. Însă mai avem nevoie de un bufer aparte ce ar ține o copie a întregului vector, deci complexitatea-spațiu este $O(N)$ (datorită faptului că reutilizăm același bufer pentru fiecare iterație, însă dacă alocăm un bufer nou la fiecare iterație, complexitatea-spațiu ar fi tot $O(NlogN)$, deoarece avem $logN$ înjumătățiri).


## Quick sort
### Algoritmul

Quick sort este un algoritm foarte interesant și cel mai practic, după opinia mea. Quick sort-ul lucrează *in-place*, fără un bufer adițional temporar, și în implementarea perfectă permite sortarea oricărei structuri de date cu un *forward iterator*.

Idea cea mai interesantă pe care o exploatează Quick sort-ul este metoda lui de a determina poziția corectă a unui element într-o listă. Observația principală este că poziția unui element într-o listă sortată va fi egală cu numărul de elemente mai mice ca acel element în lista dată (deoarece dacă lista ar fi fost sortată, aceste elemente ar fi fost plasate înainte de element-referință). Dacă înțelegeți observația dată, algoritmul devine trivial de înțeles.

1. Se găsește poziția corectă a ultimului (primului) element din lista curentă, în același timp amplasând elementele mai mice înaintea lui, iar elementele mai mari — după el.
2. Se sortează părțile de la dreapta lui element și de la stânga, prin recursie.

### Codul

Varianta mea lucrează numai cu iteratori bidirecționali, deoarece avem nevoie de poziție a ultimului element din listă. Însă, am putea modifica codul să lucreze cu orice structură de date, folosind primul element ca referință, sau dacă iteratorul `end` ar fi fost însuși ultimul element.
```C++
template<typename Iterator, typename Comparator>
void quick_sort(Iterator begin, Iterator end, 
    Comparator compare_func, Profiler* profiler = &_std_profiler)
{
    if (begin == end || begin + 1 == end) return;

    profiler->num_iters++;

    auto partition_iter = quick_partition(begin, end - 1, compare_func, profiler);
    quick_sort(begin, partition_iter,   compare_func, profiler);
    quick_sort(partition_iter + 1, end, compare_func, profiler);
}
```

Funcția de partiție:
```C++
template<typename Iterator, typename Comparator>
Iterator quick_partition(Iterator begin, Iterator last, 
    Comparator compare_func, Profiler* profiler)
{
    auto pivot_value = *last;
    auto pivot_iter  = begin;

    while (begin != last)
    {
        profiler->num_comparisons++;

        if (compare_func(*begin, pivot_value) < 0)
        {   
            Sort::swap(&*pivot_iter, &*begin);
            profiler->num_swaps++;

            pivot_iter++;
        }
        begin++;
    }

    Sort::swap(&*last, &*pivot_iter);
    profiler->num_swaps++;

    return pivot_iter;
}
```

### Complexitatea

Complexitatea este $O(NlogN)$ în caz mediu și în caz favorabil, dar în unele cazuri nefavorabile algoritmul degenerează la complexitatea de $O(N^{2})$. Complexitatea spațiu este $O(logN)$, însă spațiul se folosește numai pentru recursii.


# Analiza

Pornim codul, și ne uităm la rezultate.

```
Merge Sort.
--------------------------------------
Time elapsed: 650.00000 (ms)
Number of iterations: 999000
Number of comparisons: 8707636
Number of swaps: 9976000
Number of experiments: 1000
Time per experiment: 650.00000 (microsec)
Iterations per experiment: 999.000000
Comparisons per experiment: 8707.635742
Swaps per experiment: 9976.000000
--------------------------------------

Heap Sort.
--------------------------------------
Time elapsed: 1250.00100 (ms)
Number of iterations: 9580784
Number of comparisons: 16851918
Number of swaps: 9080784
Number of experiments: 1000
Time per experiment: 1250.00100 (microsec)
Iterations per experiment: 9580.784180
Comparisons per experiment: 16851.917969
Swaps per experiment: 9080.784180
--------------------------------------

Quick Sort.
--------------------------------------
Time elapsed: 1060.00100 (ms)
Number of iterations: 667140
Number of comparisons: 10982914
Number of swaps: 6119121
Number of experiments: 1000
Time per experiment: 1060.00100 (microsec)
Iterations per experiment: 667.140015
Comparisons per experiment: 10982.914063
Swaps per experiment: 6119.121094
--------------------------------------

Selection Sort.
--------------------------------------
Time elapsed: 27960.11800 (ms)
Number of iterations: 1000000
Number of comparisons: 499500000
Number of swaps: 1000000
Number of experiments: 1000
Time per experiment: 27960.11800 (microsec)
Iterations per experiment: 1000.000000
Comparisons per experiment: 499500.000000
Swaps per experiment: 1000.000000
--------------------------------------
```

După timp, s-a dovedit ca fiind cel mai rapid algoritmul Merge sort, executându-se de 30% mai rapid decât Quick sort. Însă, nu uităm, că Merge sort utilizează mai mult spațiu. Încă, algoritmul acesta este mai agresiv în raport cu interschimbările. Dacă obiectele păstrate în tablou cu date sunt destul de large, sortarea va deveni mai lentă în raport cu ceilalți algoritmi. Însă, numărul de comparări este minim, deoarece unele elemente sunt copiate fără comparări (după ce una din jumătăți a fost parcursă).

Quick sort este mai puțin stabil decât ceilalți algoritmi, dar este și un pic mai eficient din această cauză. Numărul de interschimbări este minim la acest algoritm.

Heap sort este într-un fel un algoritm mediu între Merge și Quick, deoarece este mai stabil, dar și mai lent, și nu necesită extra spațiu, ca sortarea Merge.

Selection sort, evident, nu este competitiv, din cauzele explicate în paragraful desemnat lui. S-a dovedit faptul că este stabil: stabil 1000 de iterații pentru 1000 de elemente.