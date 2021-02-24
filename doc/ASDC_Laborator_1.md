# ASDC, Laborator nr.1.

A elaborat: **Curmanschii Anton, IA1901**.

Codul sursă întreg puteți privi [pe github-ul meu](https://github.com/AntonC9018/uni_asdc).

Acest document tot este accesibil [pe web](https://github.com/AntonC9018/uni_asdc/blob/master/doc/ASDC_Laborator_1.md).

## Introducere

În acest laborator trebuia:

1. Să realizez un tip de date specific. Se are în vedere un tip specific de înregistrare, fie informația despre studenți, fie despre jocuri video. În cazul meu, am ales **Date personale cu email și cu ip-adresa**.

2. Să realizez citirea înregistrărilor de tipul dat dintr-un fișier (deserializare) și afișarea lor (serializarea).

3. Să realizez 2+ metode de căutare în tabele ordonate și 2+ metode în tabele neordonate. Pentru metodele de căutare în tabele ordonate am ales **Căutarea binară** și **Căutarea exponențială**, iar pentru cealalt grup **Căutarea secvențială (lineară)** și **Arbore binar**. 

4. Să măsur cât de mult timp se execută diferitele metode. Să le compar pe baza acestor observări. Să suprapun formulele teoretice cu observațiile.


## Înregistrări

Am luat datele de pe [mockaroo](https://www.mockaroo.com) — un site pentru generarea datelor test.

Exemplu fișierului ordonat (afișez numai primele 4 rânduri):
```
id,first_name,last_name,email,gender,ip_address
1,Harry,Munsey,hmunsey0@vimeo.com,M,205.49.157.231
2,Annmaria,McRannell,amcrannell1@nymag.com,F,65.243.229.105
3,Daphene,Ferne,dferne2@nih.gov,F,106.57.147.196
```

Exemplu fișierului neordonat:
```
id,first_name,last_name,email,gender,ip_address
687,Stacy,Dongles,sdonglesj2@e-recht24.de,M,82.14.239.193
310,Eloise,Magenny,emagenny8l@reverbnation.com,F,41.33.179.3
343,Addy,Aubri,aaubri9i@abc.net.au,F,175.43.112.199
```

Deci se observă prin citirea primului rând ce câmpuri trebuie să aibă structura dată. Arăt implementarea structurii date ca un `struct` în C++.

```c++
struct Record
{
    u32 id;
    str_t first_name;
    str_t last_name;
    str_t email;
    Gender gender;
    Ip_Address ip_address;
};
```

Puteți observa niște tipuri noi, poate necunoscute. În primul rând avem *u32*. *u32* semnifică *unsigned int pe 32 de biți*.

`str_t` este tipul pe care îl utilizez pentru reprezentarea șirurilor de caractere (nu utilizez `std::string`). El conține un pointer la șirul de caractere în memorie, nul-terminat, dar mai are câmpul pentru lungime. Acest tip va fi utilizat și mai departe. El este din biblioteca mea personală pentru șiruri de caractere, [strlib](https://github.com/AntonC9018/strlib) care conține funcții utile pentru prelucrarea acestora.

```c++
struct str_t
{
    char* chars;       // A null terminated char array.
    size_t length;     // Size of the data buffer, excluding the null character.
};
```

Pentru sexul folosim un astfel de enum.

```c++
enum Gender
{
    MALE, FEMALE
};
```

O adresă IP (IPv4) este esențial un număr pe 32 de biți. Deci:

```c++
typedef u32 Ip_Address; 
```

Pentru deserializare (citire) a datelor folosim o astfel de funcție. Aici utilizez `strtok()` pentru prelucrarea a tokenilor, dar în curând voi realiza și o funcție a mea pentru `strlib`:

```c++
Record deserialize_record(char* buffer)
{
    Record record;
    char* token = strtok(buffer, ",");

    sscanf(token, "%u", &record.id);
    token = strtok(NULL, ",");
    record.first_name = str_copy(token);
    token = strtok(NULL, ",");
    record.last_name = str_copy(token);
    token = strtok(NULL, ",");
    record.email = str_copy(token);
    token = strtok(NULL, ",");
    record.gender = deserialize_gender(token);
    token = strtok(NULL, ",");
    record.ip_address = deserialize_ip_address(token);

    return record;
}
```

Avem niște funcții necunoscute. 

`str_copy()` copiază un șir nul-terminat cu `malloc()`, și returnează un `str_t`, discutat mai sus.

`deserialize_gender()` citește `M` ca `MALE` si `F` ca `FEMALE`. `report_token_error()` scrie mesajul și dă crash.

```c++
Gender deserialize_gender(const char* gender_token)
{
    if (strcmp(gender_token, "M") == 0)
    {
        return MALE;
    }
    else if (strcmp(gender_token, "F") == 0)
    {
        return FEMALE;
    }
    else
    {
        report_token_error("Gender", gender_token);        
        return MALE;
    }
}
```

`deserialize_ip_address()` citește părțile adresei ca numere, pe urmă utilizează bit shift-ul pentru a le pune în poziția corectă.

```c++
Ip_Address deserialize_ip_address(const char* ip_token)
{
    u32 ip = 0;
    for (int i = 0; i < 4; i++)
    {
        u32 t;
        sscanf(ip_token, "%u", &t);
        ip <<= 8;        
        ip += t;
        ip_token = strchr(ip_token, '.') + 1;
    }
    return ip;
}
```


Serializarea se face asemănător: scriem toate valorile, una după alta, prin cratime.

```c++
void serialize_record(const Record* record, FILE* file)
{
    fprintf(file, "%u", record->id);
    fputc(',', file);
    fputs(record->first_name.chars, file); 
    fputc(',', file);
    fputs(record->last_name.chars, file); 
    fputc(',', file);
    fputs(record->email.chars, file);
    fputc(',', file);
    serialize_gender(record->gender, file);
    fputc(',', file);
    serialize_ip_address(record->ip_address, file);
    fprintf(file, "\n");
}
```

Și iată funcțiile pentru serializare a sexului și a adresei ip. `report_error()` scrie mesajul și dă crash.

```c++
void serialize_gender(Gender gender, FILE* file)
{
    if (gender == MALE)
    {
        fputc('M', file); 
    }
    else if (gender == FEMALE)
    {
        fputc('F', file);
    }
    else
    {
        report_error("The enum Gender does not contain a member \"%i\"", (int)gender);         
    }
}

void serialize_ip_address(Ip_Address ip, FILE* file)
{
    u32 temp = ip;
    for (int i = 0; i < 4; i++)
    {
        u32 t = temp >> (3 - i) * 8;
        temp -= t << (3 - i) * 8;
        fprintf(file, "%u", t);
        if (i < 3)
        {
            fputc('.', file);
        }
    }
}
```

Funcția de citire a datelor din fișier citește câte o line de fiecare dată, apelează `serialize_record()` și culege rezultatele într-un `std::vector`.

```c++
std::vector<Record> read_records_from_csv(const char* fname)
{
    FILE* file = fopen(fname, "r");
    if (!file)
    {
        report_error("Failed to open file %s", fname);
    }
    char buffer[1024];
    fgets(buffer, 1024, file);
    
    std::vector<Record> result;
    
    while (fgets(buffer, 1024, file) != 0)
    {
        result.push_back(deserialize_record(buffer));
    }
    
    fclose(file);

    return result;
}
```

## Căutările

Deci cum deja am menționat, am realizat patru metode. 

Am făcut o funcție pentru a verifica dacă fiecare metoda de căutare dă răspunsuri corecte. Am mai făcut o funcție pentru a măsura performanța metodelor selectate. 

La început voi arăta esența primei funcții. În scurt, aplicăm fiecare metodă de căutare, pe urmă verificând prin `assert` dacă valoarea id-ului a rezultatului este de fapt cea solicitată. Dacă nu, assert-ul ar da crash.

```c++
void search_tests()
{
    auto records =         read_records_from_csv("data_unordered.csv");
    auto records_ordered = read_records_from_csv("data.csv");

    for (u32 i = 1; i <= records.size(); i++)
        assert(linear_search(records, i)->id == i); 

    for (u32 i = 1; i <= records_ordered.size(); i++)
        assert(binary_search(records_ordered, i)->id == i);

    for (u32 i = 1; i <= records_ordered.size(); i++)
        assert(exponential_search(records_ordered, i)->id == i); 

    auto t = binary_tree_from_records(records); 

    for (u32 i = 1; i <= records_ordered.size(); i++)
        assert(DS::find(t, [&](auto rec) { return (s32)rec->id - (s32)(i); })->id == i);
}
```

Și iaca funcția pentru profilare. Arată un pic complicată, în special deoarece sintaxa lambzelor în C++ este prea supraîncărcată, dar eu o voi clarifica. 

În primul rând voi explica terminologia. Un *experiment* este o apelare a metodei de căutare, iar o *iterație* este o iterație sau o recursie în scopul algoritmului (funcției de căutare).

```c++
void profile()
{
    auto records =         read_records_from_csv("data_unordered.csv");
    auto records_ordered = read_records_from_csv("data.csv");
    
    const int num_experiments = 10000;

    // generate random id's for each of the experiments
    std::vector<u64> ids(num_experiments);
    for (int i = 0; i < num_experiments; i++)
    {
        ids[i] = rand() % records.size() + 1;
    }

    // retrieve the random id for the current experiment
    auto id = [&](Profiler* p) { return ids[p->num_experiments % num_experiments]; };


    profiler_perform_experiments(
        "Linear Search: NORMAL.",
        [&](auto p) { linear_search(records, id(p), p); },
        num_experiments
    );

    profiler_perform_experiments(
        "Linear Search: STD::FUNCTION.",
        [&](auto p) { linear_search_stdfunction(records, 
            [=](const Record* r) { return r->id == id(p); }, p); 
        },
        num_experiments
    );

    profiler_perform_experiments(
        "Binary Search: NORMAL.",
        [&](auto p) { binary_search(records_ordered, id(p), p); },
        num_experiments * 100
    );

    profiler_perform_experiments(
        "Exponential Search: NORMAL.",
        [&](auto p) { exponential_search(records_ordered, id(p), p); },
        num_experiments * 100
    );

    {
        using namespace DS;

        profiler_perform_experiments(
            "Binary tree construction.",

            [&](auto p) { 
                auto t = binary_tree_from_records(records);
                destroy(t);
            },

            num_experiments
        );

        auto t = binary_tree_from_records(records);

        profiler_perform_experiments(
            "Binary tree searches (constructed beforehand)",
            [&](auto p) { find(t, 
                [&](auto rec) { return (s32)rec->id - (s32)id(p); }, p); },
            num_experiments * 100
        );
    }
}
```

Urmează clarificarea la cod.


Codul de mai jos permite generarea valorilor pentru id-uri diferite pentru fiecare experiment, dar care vor rămâne stabile pentru fiecare algoritm verificat. `Profiler` conține și contorul experimentului curent. Este folosit modulul, deoarece pentru unele algoritme am dori să luăm un număr mai mare de experimente.

```c++
// generate random id's for each of the experiments
std::vector<u64> ids(num_experiments);
for (int i = 0; i < num_experiments; i++)
{
    ids[i] = rand() % records.size() + 1;
}

// retrieve the random id for the current experiment
auto id = [&](Profiler* p) { return ids[p->num_experiments % num_experiments]; };
```

Acum explicăm cum sunt susținute experimentele. Fragment de cod de mai jos arată un exemplu.

```c++
profiler_perform_experiments(
    "Linear Search: NORMAL.",
    [&](auto p) { linear_search(records, id(p), p); },
    num_experiments
);
```

Deci după executarea este afișat mesajul, și funcția de căutare este apelată de `num_experiments` ori. La fiecare iterație, funcția dată folosește lambda `id()`, explicată mai sus, pentru a primi id-ul pentru iterația curentă. 

Pot arăta și codul sursa acestei funcții. Mai mult despre profilatorul mai pe urmă.

```c++
template<typename Experiment_Function>
static void profiler_perform_experiments(
    const char* description, Experiment_Function func, u32 num_experiments, FILE* file = stdout)
{
    fprintf(file, "%s\n", description);

    Profiler profiler;
    profiler_start(&profiler);

    for (u32 i = 0; i < num_experiments; i++)
    {
        func(&profiler);
        profiler.num_experiments++;
    }

    profiler_report_nicely(file, &profiler);
}
```

Plusul pentru așa implementare este faptul că acest cod poate fi paralelizat ușor, aproape fără modificări. În plus, se elimină repetarea codului și în general, codul devine mai lizibil, dacă cunoașteți sintaxa lambdelor.

Iată un exemplu de output la executare:

```
Linear Search: NORMAL.
--------------------------------------
Time elapsed: 70.00000 (ms)
Number of iterations: 4989788
Number of experiments: 10000
Time per experiment: 7.00000 (microsec)
Iterations per experiment: 498.978790
--------------------------------------

Linear Search: STD::FUNCTION.
--------------------------------------
Time elapsed: 270.00000 (ms)
Number of iterations: 4989788
Number of experiments: 10000
Time per experiment: 27.00000 (microsec)
Iterations per experiment: 498.978790
--------------------------------------

Binary Search: NORMAL.
--------------------------------------
Time elapsed: 190.00000 (ms)
Number of iterations: 8972400
Number of experiments: 1000000
Time per experiment: 0.19000 (microsec)
Iterations per experiment: 8.972400
--------------------------------------

Exponential Search: NORMAL.
--------------------------------------
Time elapsed: 400.00000 (ms)
Number of iterations: 30836200
Number of experiments: 1000000
Time per experiment: 0.40000 (microsec)
Iterations per experiment: 30.836201
--------------------------------------

Binary tree construction.
--------------------------------------
Time elapsed: 2619.09500 (ms)
Number of experiments: 10000
Time per experiment: 261.90950 (microsec)
--------------------------------------

Binary tree searches (constructed beforehand)
--------------------------------------
Time elapsed: 284.00600 (ms)
Number of iterations: 12464000
Number of experiments: 1000000
Time per experiment: 0.28401 (microsec)
Iterations per experiment: 12.464000
--------------------------------------
```

## Profilatorul

Pentru profilatorul am folosit `std::chrono` C++ API care permite să măsurăm timpul cu precizia mare (folosesc microsecundele).

Iată structura profilatorului.

```c++
struct Profiler
{
    u32 num_iters;
    u32 num_compares;
    u32 num_experiments;
    std::chrono::system_clock::time_point started_time;
    std::chrono::microseconds time_elapsed;
};
```

Iată funcțiile pentru începerea și stopare.

```c++
inline void profiler_start(Profiler* profiler = &_std_profiler)
{
    profiler->num_iters = 0;
    profiler->num_compares = 0;
    profiler->num_experiments = 0;
    profiler->started_time = std::chrono::high_resolution_clock::now();
}

inline void profiler_stop(Profiler* profiler = &_std_profiler)
{
    profiler->time_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - profiler->started_time);
}
```

Și funcția principală pentru afișare.

```c++
static void profiler_print(FILE* file = stdout, Profiler* profiler = &_std_profiler)
{
    auto microsecs = (double)profiler->time_elapsed.count();

    fprintf(file, "Time elapsed: %.5f (ms)\n", microsecs / (double)1000);

    if (profiler->num_iters != 0)
    {
        fprintf(file, "Number of iterations: %u\n", profiler->num_iters);
    }

    if (profiler->num_compares != 0)
    {
        fprintf(file, "Number of compares: %u\n", profiler->num_compares);
    }

    if (profiler->num_experiments != 0)
    {
        fprintf(file, "Number of experiments: %u\n", profiler->num_experiments);
        fprintf(file, "Time per experiment: %.5f (microsec)\n", microsecs / profiler->num_experiments);
        
        if (profiler->num_iters != 0)
        {
            fprintf(file, "Iterations per experiment: %f\n", 
                (float)profiler->num_iters / (float)profiler->num_experiments);
        }

        if (profiler->num_compares != 0)
        {
            fprintf(file, "Compares per experiment: %f\n", 
                (float)profiler->num_compares / (float)profiler->num_experiments);
        }
    }
}
```

Pentru simplicitate, am decis să realizez profilatorul într-un singur .h fișier, de aceea funcțiile sunt ba statice ba inline.

Trecem la algoritmele.

# Algoritmele: explicații, cod și comparare.

## Căutarea binară

Căutarea binară este una din cele mai eficiente și populare metode de căutare.

Căutarea binară funcționează numai pe presupunerea că tabelul, asupra căruia algoritmul operează, este sortat.

Plusurile acestui algoritm sunt stabilitatea și generalitatea. El lucrează stabil și eficient cu orice tabel ordonat și nu ia în considerare nici un alt factor, adică nu mai presupune nimic despre datele de intrare. În contrast, de exemplu, *interpolation search* utilizează presupunerea că datele sunt răspândite uniform peste tabel. În același timp generalitatea este și un dezavantaj, deoarece, potențial, sunt ignorate unele fapte despre date care ar putea fi exploatate pentru o căutare mai eficientă.

### Algoritmul

Algoritmul constă în tăierea tabelului în jumătate la fiecare iterație. Se verifică poziția centrală din tabelul curent și bazându-se pe valoarea elementului ce stă în mijlocul, se decide în ce jumătate să continue căutarea. Dacă elementul de pe mijloc este mai mic decât elementul căutat, se ia jumătatea de la dreapta de la acel element, dacă mai mare - cea de la stânga. Evident, dacă elementul este egal cu elementul căutat, se returnează poziția lui.

### Codul

În implementarea de mai jos ar putea fi returnat indicele acestei înregistrări. Algoritmul nu se schimbă din această cauză.

```C++
Record* binary_search(
    std::vector<Record>& records, size_t search_id, Profiler* profiler = &_std_profiler)
{
    size_t start = 0;
    size_t end = records.size() - 1;

    while (start <= end)
    {
        profiler->num_iters++;
        size_t current = (end + start) / 2;

        if (records[current].id == search_id)
        {
            return &records[current];
        }
        if (records[current].id < search_id)
        {
            start = current + 1;
        }
        else
        {
            end = current - 1;
        }
    }
    return NULL;
}
```

### Complexitatea

Deoarece la fiecare iterație tabelul se imparte în jumătate, complexitatea timp este O(log N). Complexitatea de spațiu este, evident O(1).


## Căutarea exponențială

Această metodă de cautare este în special utilă când nu se știu bordurile tabelului la începutul căutării.

### Algoritmul

Algoritmul mereu se începe de la primul element al tabelului. Se folosește un contor, care este dublat la fiecare iterație. Valoarea acestui contor este adăugată la poziția curentă. Deci, la început vor fi încerate pozițiile 0, 1, 2, 4, 8, 16 etc. până când pe poziția verificată nu este găsit un număr mai mare decât cel căutat. Atunci poziția curentă (offsetul) este resetată la poziția verificată trecută, iar contorului este atribuit valoarea 1.

Algoritmul poate fi îmbunătățit prin apelarea căutării binare de la poziția trecută până la poziția următoare (adică, de exemplu, între 8 și 16). Însă nu fac acest lucru în cod.

### Codul

```c++
Record* exponential_search(
    std::vector<Record>& records, size_t search_id, Profiler* profiler = &_std_profiler)
{
    if (records[0].id == search_id)
        return &records[0];

    size_t end = records.size();
    size_t c = 0;
    size_t i = 1;

    while (c < end)
    {
        size_t index = std::min(c + i, end - 1);

        profiler->num_iters++;
        
        if (records[index].id == search_id)
        {
            return &records[index];
        }
        else if (records[index].id < search_id)
        {
            i *= 2;
        }
        else
        {
            c += i / 2;
            i = 1;
        }
    }

    return NULL;
}
```

### Complexitatea

Dacă este utilizată metoda augmentată cu binary search, complexitatea-timp este O(log N).

În cazul în care folosim metoda din cod, este greu de estimat complexitatea.

Complexitatea-spațiu pentru metoda iterativă (cea realizată) este O(1).


## Căutarea lineară

Căutarea lineară este cea mai simplă dar și cea mai primitivă metodă de căutare. Ea nu presupune nimic despre structura datelot, dar este și foarte lentă din acest caz.

### Algoritmul

Se verifică toate elementele, unul după altul, până când nu se ajunge la elementul dorit. Atât.

### Codul

```c++
Record* linear_search(
    std::vector<Record>& records, size_t search_id, Profiler* profiler = &_std_profiler)
{
    for (auto& record : records)
    {
        profiler->num_iters++;
        if (record.id == search_id)
        {
            return &record;
        }
    }
    return NULL;
}
```

### Complexitatea

Evident, complexitatea-timp este O(N). Asemănător evident, complexitatea-spațiu este O(1).


## Arbore binar

Această metodă de căutare implică construirea unei structuri de date speciale înainte de a face căutarea, dar asigură căutarea rapidă.

### Algoritmul

Se construiască un arbore binar — structura de date, unde fiecare nod poate avea 0-2 copii. Copilul de la partea dreapta este mereu mai mare decât parinte (are valoarea mai mare), iar nodul de pe stânga — mereu mai mic.

Pentru a adăuga un nod în acest arbore, se găsește un nod-parinte, care nu are copil în partea dreaptă (sau stângă, în dependența de valoarea elementului inserat în comparația cu acel element părinte). El este găsit, urmărind regulile structurii arborelui — faptul, că elementul din dreapta (stânga) este mai mare (mic) decât părintele. Nodul nou este adăugat ca copil acelui părinte pe partea respectivă.

Utilizând așa arbore se poate ușor găsi orice element, datorită proprietății, menționate mai sus. Fiecare valoare căutată esențial urmărează calea elementului, când el a fost inserat în acest arbore.

În cod este mai clar.

### Codul

```c++
template<typename T>
struct Binary_Tree
{
    Binary_Tree* left;
    Binary_Tree* right;
    T value;
};

template<typename T, typename Comparator>
void insert(Binary_Tree<T>** node, T value, Comparator compare)
{
    while (true)
    {
        if (!*node)
        {
            Binary_Tree<T>* new_node = (Binary_Tree<T>*) malloc(sizeof(Binary_Tree<T>));
            new_node->value = value;
            new_node->left = NULL;
            new_node->right = NULL;
            *node = new_node;
            return;
        }

        s32 comparison_result = compare((*node)->value, value);
        if (comparison_result > 0)
        {
            node = &((*node)->right);
        }
        else
        {
            node = &((*node)->left);
        }
    }
}

template<typename T, typename Comparator>
T find(Binary_Tree<T>* node, Comparator compare, Profiler* profiler = &_std_profiler)
{
    while(true)
    {
        profiler->num_iters++;

        if (!node)
        {
            return {};
        }

        s32 comparison_result = compare(node->value);

        if (comparison_result == 0)
        {
            return node->value;
        }
        if (comparison_result > 0)
        {
            node = node->right;
        }
        else
        {
            node = node->left;
        }
    }
}
```

Această funcție returnează un arbore, generat din tabloul dat. În arborele sunt păstrate pointerii la elemente, dar în principiu aceștia ar fi putut fi și indicii elementelor:

```c++
Binary_Tree<Record*>* binary_tree_from_records(std::vector<Record>& records)
{
    Binary_Tree<Record*>* t = NULL;
    for (auto& record : records)
    {
        insert(&t, &record, [](auto a, auto b) { return (s32)a->id - (s32)b->id; });
        // or insert(&t, record.id, ...);
    }
    return t;
}
```

Și iată un exemplu de căutare după id-ul 69:

```c++
find(t, [](auto rec) { return (s32)rec->id - 69; })
```

### Complexitatea

În cel mai rău caz, construirea arborelui are complexitatea-timp O(N^2), dar în general, este asemănătoare la metode de sortare băzate pe comparare — O(N log N).

Însă căutarea este rapidă. Având în vedere faptul că fiecare urmărire la nod-copil împărțește arborele în jumătate (în cazul așteptat), complexitatea-timp este O(log N), iar implemetarea recursivă a căutării necesită O(1) spațiu.

Căutarea binară are sens s-o utilizăm numai când primim date în forma neordonată, și avem nevoie să facem mai multe căutări, cu toate că sortarea tabloului + căutarea binară ar fi la fel de eficientă.


## Tabele hash

Numai am realizat însăși structura de date de hash map. 

La bază avem un tablou de lungime prestabilită de așa numite `buckets`. De fapt, fiecare `bucket` reprezintă o listă linkată.

Când un nou element se adaugă în tabel, el nimerește în una din aceste `buckets`. Dacă nu este duplicat, el este plasat ca ultimul element în lista linkată, care corespunde `bucket`-ului acesta.

Pozițiile `bucket`-ilor sunt selectate băzându-se pe valoarea hash după modulul lungimii tabloului. Însăși valoarea hash o calculez cu ajutorul algoritmului [FNVa](http://www.isthe.com/chongo/tech/comp/fnv). Acest algoritm este unul foarte simplu de implementat și a fost evidențiat ca unul eficient.

```c++
#define FNV_HASH_64_OFFSET_BASIS 14695981039346656037ULL
#define FNV_HASH_32_OFFSET_BASIS 2166136261U
#define FNV_HASH_64_PRIME        1099511628211ULL
#define FNV_HASH_32_PRIME        16777619U

constexpr size_t FNV_HASH_OFFSET_BASIS = (sizeof(size_t) == 8) ? FNV_HASH_64_OFFSET_BASIS : FNV_HASH_32_OFFSET_BASIS;
constexpr size_t FNV_HASH_PRIME        = (sizeof(size_t) == 8) ? FNV_HASH_64_PRIME : FNV_HASH_32_PRIME;

size_t str_hash(str_view_t str)
{
    size_t h = FNV_HASH_OFFSET_BASIS;
    size_t i = 0;

    // Calculate the hash taking 4 (or 8) bytes at a time
    while (i + sizeof(size_t) <= str.length)
    {
        h ^= *(size_t*)(str.chars + i);
        h *= FNV_HASH_PRIME;
        i += sizeof(size_t);
    }

    // Less than 4 (8) bytes remaining, add them one by one
    for (size_t j = 0; j + i < str.length; j++)
    {
        h ^= (size_t)(str.chars[i + j]) << j * 8;
    }

    if (i < str.length)
    {
        h *= FNV_HASH_PRIME;
    }

    return h;
}
```

Arăt întregul cod pentru hash map.

```c++
struct Hash_Map_Bucket
{
    str_view_t key;
    Record value;
    Hash_Map_Bucket* next_bucket;
};

struct Hash_Map
{
    #define HASH_MAP_BUCKET_COUNT 199
    Hash_Map_Bucket* buckets[HASH_MAP_BUCKET_COUNT];
};

inline Hash_Map hmap_create()
{
    return {0};
}

size_t hmap_hash(Hash_Map* hm, str_view_t key)
{
    return str_hash(key) % HASH_MAP_BUCKET_COUNT;
}

Hash_Map_Bucket* hmap_get_bucket(Hash_Map* hm, str_view_t key)
{
    size_t index = hmap_hash(hm, key);
    auto bucket  = hm->buckets[index];
    while (bucket != NULL)
    {
        if (str_compare(key, bucket->key) == 0)
        {
            return bucket;
        }
        else
        {
            bucket = bucket->next_bucket;
        }
    }
    return NULL;
}

bool hmap_has_key(Hash_Map* hm, str_view_t key)
{
    return hmap_get_bucket(hm, key) != NULL;
}

Record hmap_get(Hash_Map* hm, str_view_t key)
{
    auto bucket = hmap_get_bucket(hm, key);
    return bucket ? bucket->value : Record{0};
}

bool hmap_has_value(Hash_Map* hm, Record value)
{
    for (size_t i = 0; i < HASH_MAP_BUCKET_COUNT; i++)
    {
        auto bucket = hm->buckets[i];

        while (bucket != NULL)
        {
            if (compare_record(&bucket->value, &value))
            {
                return true;
            }
        }
    }
    return false;
}

Hash_Map_Bucket* hmap_make_bucket(str_view_t key, Record value)
{
    auto new_bucket   = (Hash_Map_Bucket*) malloc(sizeof(Hash_Map_Bucket));
    new_bucket->key   = key;
    new_bucket->value = value;
    new_bucket->next_bucket = NULL;
    return new_bucket;
}

// Returns false if an element with that key already existed and was replaced with the new value
// Returns true if the new element has been successfully added
bool hmap_put(Hash_Map* hm, str_view_t key, Record value)
{
    size_t index = hmap_hash(hm, key);
    auto bucket  = hm->buckets[index];

    if (bucket == NULL)
    {
        hm->buckets[index] = hmap_make_bucket(key, value);
        return true;
    }

    auto parent_bucket = bucket;

    while (bucket != NULL)
    {
        // Found a duplicate key.
        // In this case, replace the old value.
        if (str_compare(bucket->key, key) == 0)
        {
            bucket->value = value;
            return false;
        }

        parent_bucket = bucket;
        bucket = bucket->next_bucket;
    }

    // The last bucket in chain, no duplicate keys.
    parent_bucket->next_bucket = hmap_make_bucket(key, value);
    return true;
}

void hmap_free(Hash_Map* hm)
{
    for (size_t i = 0; i < HASH_MAP_BUCKET_COUNT; i++)
    {
        auto bucket = hm->buckets[i];

        while (bucket != NULL)
        {
            auto t = bucket->next_bucket;
            free(bucket);
            bucket = t;
        }
    }
}
```

Am făcut și codul pentru test, însă nu este verificat cum valorile cu același hash nimeresc în același `bucket`. Menționez funcțiile necunoscute: `str_copy_lit()` pune șirul dar constant (string literal) în memorie dinamică cu `malloc()` și `memcpy()`, iar `str_lit()` transformă șirul dat constant (string literal) într-un `str_view_t`, salvând valoarea lungimii.

```c++
void hash_map()
{
    using namespace DS;
    auto hash_map = hmap_create();
    auto hm       = &hash_map;
    
    Record john;
    john.id         = 7;
    john.gender     = FEMALE;
    john.ip_address = deserialize_ip_address("123.123.123.123");
    john.email      = str_copy_lit("@mail.ru");
    john.first_name = str_copy_lit("John");
    john.email      = str_copy_lit("Titor");

    Record okabe;
    okabe.id         = 1;
    okabe.gender     = MALE;
    okabe.ip_address = deserialize_ip_address("231.231.231.231");
    okabe.email      = str_copy_lit("@dmail.com");
    okabe.first_name = str_copy_lit("Okabe");
    okabe.email      = str_copy_lit("Rintarou");


    // First time adding these records
    assert(hmap_put(hm, str_lit("John"),  john));

    // Print the hashed bucket index 
    // Strings are hashed using the FNVa hashing algorithm
    printf("%zu\n", hmap_hash(hm, str_lit("John")));
    printf("%zu\n", hmap_hash(hm, str_lit("Okabe")));
    // Might as well show the hashes themselves
    printf("%zu\n", str_hash(str_lit("John")));
    printf("%zu\n", str_hash(str_lit("Okabe")));

    assert(hmap_put(hm, str_lit("Okabe"), okabe));

    // Okabe has already been added, so he is replaced here
    assert(!hmap_put(hm, str_lit("Okabe"), okabe));

    assert( hmap_has_key(hm, str_lit("Okabe")));
    assert( hmap_has_key(hm, str_lit("John")));
    assert(!hmap_has_key(hm, str_lit("Mayuri")));
    assert(hmap_has_value(hm, okabe));

    auto value = hmap_get(hm, str_lit("Okabe"));

    assert(compare_record( &value, &okabe ));
}
```

### Complexitate

Datorită faptului că utilizând hash-ul drastic reducem numărul de locații posibile, căutarea este FOARTE rapidă, fiind limitată în mare parte de către viteza funcției utilizată la hashing. Numărul de elemente în tabel tot prelungește căutarea, dar acesta poate fi minimizat prin selectarea mărimii tabloului cu bucket-ii proporționale cu numărul de elemente. O funcție hash bună garantează distribuirea uniformă a elementelor între bucketi, deci la sfârșit parcurgerea prelungată a listelor linkate are loc destul de rar.

Complexitatea-timp, așadar, este O(1), dar acest 1 este dependent totuși, în special, de funcția hash.
Comlexitatea-spațiu va fi O(N).


## Comparare

Vom compara metodele după categoriile lor: dacă ele lucrează cu tablouri ordonate sau neordonate. 

```
Linear Search: NORMAL.
--------------------------------------
Time elapsed: 74.00200 (ms)
Number of iterations: 4989788
Number of experiments: 10000
Time per experiment: 7.40020 (microsec)
Iterations per experiment: 498.978790
--------------------------------------

Binary tree construction.
--------------------------------------
Time elapsed: 2460.00300 (ms)
Number of experiments: 10000
Time per experiment: 246.00030 (microsec)
--------------------------------------

Binary tree searches (constructed beforehand)
--------------------------------------
Time elapsed: 280.00000 (ms)
Number of iterations: 12464000
Number of experiments: 1000000
Time per experiment: 0.28000 (microsec)
Iterations per experiment: 12.464000
--------------------------------------
```

Dacă arborele binar este deja construit, căutarea prin arborele acestea (1000 de elemente) este în 7.4/0.28=26.43 ori mai rapidă decât căutarea liniară. Însă, trebuie să nu ne uităm, că complextitățile sunt diferite: timp pierdut la căutarea lineară este proporțional cu N, pe când cel al arborlui binar — cu log N.

Însă construirea arborelui din 1000 de elemente ia mult timp: în 246.0/0.28=878.6 ori mai mult decât însăși căutarea. Aceasta este logic, deoarece construirea arborelui este O(N log N), iar căutarea — O(log N). La împărțire primim O(N).

Evident în acest caz este faptul că arborele binar este mai eficient decât căutarea lineară, numai dacă facem semnificativ mai multe interogări. 

În cazul dat (1000 de elemente) putem calcula când cautarea prin arbore binar devine mai eficientă decât căutarea secvențială.
```
X - numarul de interogari necesar
Solutionam inegalitatea: 
timp_construire_binar + X * lungimea_cautarii_arbore_binar < X * lungimea_cautarii_secventiale
246 + 0.28*X < 7.4*X
246 < 7.12*X
X > 34
```

Urmează să comparăm căutarea binară și cea exponențială.

```
Binary Search: NORMAL.
--------------------------------------
Time elapsed: 190.00000 (ms)
Number of iterations: 8972400
Number of experiments: 1000000
Time per experiment: 0.19000 (microsec)
Iterations per experiment: 8.972400
--------------------------------------

Exponential Search: NORMAL.
--------------------------------------
Time elapsed: 392.00200 (ms)
Number of iterations: 30836200
Number of experiments: 1000000
Time per experiment: 0.39200 (microsec)
Iterations per experiment: 30.836201
--------------------------------------
```

În implementarea mea (metoda nemodificată, descrisă anterior), căutarea exponențială este aproape de 2 ori mai lentă decât cea binară. În general, ei operează cu același set de presupuneri, deci diferența nu va fi drastică.

Însă căutarea binară are avantajul că ea este mai simplă. Ea poate fi implemetată inline, în unele limbaje de programare chiar într-o singură linie, datorită simplității sale. Căutarea exponențială este un pic mai complicată.

Observăm că numărul de iterații pentru căutarea binară este aproximativ log2(N), ceea ce observăm și aici. log2(1000)=9.96, iar noi avem 8.97. Este mai mic, deoarece în unele cazuri ajungem la element mai rapid.