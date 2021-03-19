# ASDC, Laborator nr.3: Structuri dinamice de date.

A elaborat: **Curmanschii Anton, IA1901**.

Codul sursă întreg puteți privi [pe github-ul meu](https://github.com/AntonC9018/uni_asdc).

Acest document tot este accesibil [pe web](https://github.com/AntonC9018/uni_asdc/blob/master/doc/ASDC_Laborator_3.md).


## Sarcinile

Să se realizeze următoarele **structuri de date**:
1. Lista simplu înlănțuită;
2. Lista dublu înlănțuită;
3. Lista circulară;
4. Stive;
5. Coze.

În cadrul acestor structuri de date, să se realizeze funcțiile pentru:
- Parcurgere și afișare;
- Inserare;
- Căutare;
- Ștergere.

Să se modifice codul **arborelui binar** ca să include:
- Parcurgerea și afișarea în preordine, postordine și inordine;
- Ștergerea.


## Introducere

În cadrul laboratorului am realizat 4 structuri de date:
1. Lista simplu înlănțuită (*Singly Linked List*);
2. Lista dublu înlănțuită (*Doubly Linked List*);
3. Stive (*Stack*) prin tablou de lungime fixă și prin listă;
4. Cozi (*Queue*) prin bufer ciclic (*Cyclic Buffer*) și prin listă.

Am realizat și funcțiile pentru arbore binar.


## Liste simplu înlănțuite


### Descriere

Listele simplu înlănțiute sunt secvențe de noduri cu date, unde fiecare nod este conectat la cel mult un alt nod.

[Vedeți întregul cod pentru liste.](https://github.com/AntonC9018/uni_asdc/blob/master/src/ds/singly_linked_list.h)

### Realizarea structurilor

În codul ce urmează este definită structura de date ce reprezintă un nod simplu înlănțuit. `t_` în cod este # definit ca `template<typename T>`. Am făcut astfel, deoarece aceasta sintaxa lungă atrage prea multă atenție.
```cpp
t_ struct Singly_Linked_Node
{
    Singly_Linked_Node<T>* next;
    T item;
};
```

Se poate realiza funcții direct pe această structură, însă deseori se mai definește o structură-wrapper, ce reprezintă întreaga listă. În această structură mereu se include un pointer la primul element, deseori un pointer la ultimul element și un contor de câte noduri avem în listă. Faptul că avem așa structură ceva complică realizarea funcțiilor, deoarece trebuie să ținem cont de primul și ultimul element, verificând, dacă un nod nou ar deveni ultimul sau primul element, sau dacă se șterge nodul terminal sau capul, incrementăm și decrementăm contorul, dar în același timp și face structura de date mai flexibilă. Eu am realizat o variantă ce îi ține minte pe *cap* (primul element) și *coada* (ultimul element). Le-am dat și mai multe nume acestor variabile, folosind `union`:
```C++
// In a valid list, these two are either both null, or both not null
t_ struct Singly_Linked_List
{
    union { 
        Singly_Linked_Node<T>* head;
        Singly_Linked_Node<T>* front;
        Singly_Linked_Node<T>* first;
    };
    union {
        Singly_Linked_Node<T>* tail;
        Singly_Linked_Node<T>* back;
        Singly_Linked_Node<T>* last;
    };
}; 
```

### Realizarea funcțiilor

Într-o listă simplu înlănțuită au sens următoarele operații:
- adăugarea unui element înaite de cap;
- adăugarea unui element după coadă;
- adăugarea unui element după un anumit nod;
- adăugarea unui element după nodul cu un anumit item;
- căutarea nodului cu un anumit item;
- ștergerea unui nod după un anumit nod;
- ștergerea nodului cu un anumit item;
- ștergerea capului;
- parcurgerea de la cap până la coada;
- golirea listei.

Aceste operații (dinafara cautărilor) sunt eficiente (nu depind de lungime) pentru liste.

Nu au fost definite operațiile de înserare/ștergere a unui element înainte de un alt element, și operația de indexare, deoarece ele nu au mult sens în listele simplu înlănțuite. Operația ștergerii elementului dinainte unui alt element necesită parcurgerea listei până la acel element și nu e logic s-o definim pe lângă altor operații mai eficiente. Referitor la indexare, în general nu este o idee bună să indexați listele înlănțuite, deoarece indexarea lor nu este eficientă. Dacă le indexați des, probabil aveți nevoie de un array sau un hash map.

Voi arăta aici cod numai la unele funcții.


#### `list_insert_front()`

Adaugă un element dat înainte de capul curent și îl setează pe acest element ca capul curent. 

Când punem un element nou în această poziție, trebuie să-l linkăm doar cu capul trecut din listă, și să resetăm capul stocat în structura wrapper a listei.

Însă, deoarece ținem minte și de coada listei, dacă lista era vidă, capul nou introdus devine simultan și coada listei.

```C++
t_ Singly_Linked_Node<T>* list_insert_front(Singly_Linked_List<T>* list, const T item)
{
    auto prev_head = list->head;

    auto node = singly_linked_node_alloc<T>();
    node->item = item;

    list->head = node;
    node->next = prev_head;

    // This is equivalent to checking 
    // if (!prev_head)
    // Since the list must be always valid
    if (!list->tail)
    {
        list->tail = node;
    }

    return node;
}
```


#### `list_insert_back()`

Situația este asemănătoare, numai că trebuie să-i modificăm pointerul la următorul nod coadei, și argumentul cu lista fiind vidă este inversat din cap în coadă.

```C++
t_ Singly_Linked_Node<T>* list_insert_back(Singly_Linked_List<T>* list, const T item)
{
    auto prev_tail = list->tail;

    auto node = singly_linked_node_alloc<T>();
    node->item = item;
    node->next = NULL;

    list->tail = node;

    if (prev_tail)
    {
        prev_tail->next = node;
    }
    // If there is no tail, there must be no head too
    else
    {
        list->head = node;
    }
    
    return node;
}
```

#### `list_insert_after()`

Comentariul în cod demonstrează situația. În acest caz, deoarece inserăm elementul între două elemente, trebuie să adjustăm 2 pointeri.

```C++
t_ Singly_Linked_Node<T>* list_insert_after(
    Singly_Linked_List<T>* list, 
    Singly_Linked_Node<T>* node, 
    const T item)
{
    auto new_node = singly_linked_node_alloc<T>();
    new_node->item = item;

    // (node) -> (next_node)
    // Changes into
    // (node) -> (new_node) -> (next_node)
    new_node->next = node->next;
    node->next = new_node;

    // Need to adjust the list, in case the new node becomes the last
    if (node == list->tail)
    {
        list->tail = new_node;
    }

    return new_node;
}
```

#### `list_remove_after()`

```C++
t_ Singly_Linked_Node<T>* list_remove_after(
    Singly_Linked_List<T>* list, 
    Singly_Linked_Node<T>* node)
{
    if (node->next)
    {
        // (node) -> (node_next) -> (new_next)
        // Deleting node_next, leaves
        // (node) -> (new_next)
        auto new_next = node->next->next;
        auto removed_node = node->next;
        node->next = new_next;

        // The removed node was the tail
        if (!new_next)
        {
            list->tail = node;
        }

        return removed_node;
    }

    return NULL;
}
```

#### `list_print()`

Voi arăta și o funcție ce ilustrează parcurgerea + afișarea unei liste cu integeri.

```C++
void list_print(const Singly_Linked_List<int>* list)
{
    auto node = list->head;
    while (node)
    {
        printf("%i", node->item);
        node = node->next;
        if (node) printf(" -> ");
    }
    printf("\n");
}
```

### Test

Codul din main demonstrează toata funcționalitate:
```C++
auto list = singly_linked_list_make<int>();

assert(list.head == NULL);
assert(list.tail == NULL);

// 5
auto node_5 = list_insert_back(&list, 5);

assert(node_5->next == NULL);
assert(list.head && list.head == node_5);
assert(list.tail && list.tail == node_5);
assert(node_5->item == 5);

// 3 -> 5
auto node_3 = list_insert_front(&list, 3);

assert(list.head == node_3);
assert(list.tail == node_5);

// 3 -> 1 -> 5
auto node_1 = list_insert_after(&list, node_3, 1);

assert(list.head->next == node_1);
assert(node_1->next    == node_5);

// 3 -> 1 -> 5 -> 2
auto node_2 = list_insert_after(&list, list.tail, 2);

assert(list.tail == node_2);

// 3 -> 1 -> 5
auto removed_2 = list_remove(&list, 2);

assert(list.tail == node_5);
free(removed_2);

// 3 -> 1
auto removed_5 = list_remove_after(&list, node_1);

assert(list.tail == node_1);
free(removed_5);

printf("Printing the list: "); 
list_print(&list); // 3 -> 1

list_free(&list);
```

### Ajantajele și dezavantajele

Principalul ajantaj al listelor este că lungimea lor este flexibilă și că elementele pot fi inserate în orice poziție din listă fără ca alte elemente să fie mutate în memorie: sunt schimbate numai linkurile între ei. 

Dezajantajul este că datele nu stă în memorie consecutiv, ce încetinește lucrul cu ele. Înca listele nu permit random-acces (indexarea) eficientă. În liste, în special în listele care stochează date de dimensiuni mici, fie numere întregi pe 8 biți, jumătatea spațiului se pierde la pointeri către următoarele elemente. 

În plus, este dificil de sortat așa liste, deoarece avem nevoie de ținut cont de mulți pointeri când interschimbăm elementele, și este imposibil să aplicăm oricare alt algoritm de căutare decât căutarea liniară. 

## Liste dublu înlănțuite


### Descriere

Pe când în listele simplu înlănțuite fiecare nod păstrează un poiter numai la un alt nod, într-o listă dublu înlănțuită legătura este bidirecțională. Astfel, fiecare nod are un pointer cât la nodul precenent, atât și la nodul următor.

### Realizarea structurilor

```C++
t_ struct Doubly_Linked_Node
{
    Doubly_Linked_Node<T>* prev;
    Doubly_Linked_Node<T>* next;
    T item;
};
```

Pentru lista dublu înlănțuită tot am realizat un wrapper:
```C++
t_ struct Doubly_Linked_List
{
    union { 
        Doubly_Linked_Node<T>* head;
        Doubly_Linked_Node<T>* front;
        Doubly_Linked_Node<T>* first;
    };
    union {
        Doubly_Linked_Node<T>* tail;
        Doubly_Linked_Node<T>* back;
        Doubly_Linked_Node<T>* last;
    };
};
```


### Realizarea funcțiilor

[Vedeți întregul cod pentru liste.](https://github.com/AntonC9018/uni_asdc/blob/master/src/ds/doubly_linked_list.h)

Într-o listă dublu înlănțuită au sens următoarele operații:
- adăugarea unui element înaite de cap;
- adăugarea unui element după coadă;
- adăugarea unui element după un anumit nod;
- adăugarea unui element înainte de un anumit nod;
- căutarea nodului cu un anumit item;
- căutarea nodului cu un anumit item în ordinea inversă;
- ștergerea nodului cu un anumit item;
- ștergerea nodului cu un anumit item în ordinea inversă;
- ștergerea unui nod din listă;
- parcurgerea de la cap până la coada;
- parcurgerea în ordinea inversă;
- golirea listei.


#### `list_insert_front()`

Ca să inserăm un element înainte de capul curent, trebuie să modificăm cel puțin 3 pointeri:
- pointer către următorul element al nodului nou;
- pointer către elementul precedent al capului veche;
- capul listei, stocat în wrapper-ul.

Dacă lista este vidă (capul a fost nul), trebuie să modificăm și coada listei.

```C++
t_ Doubly_Linked_Node<T>* list_insert_front(Doubly_Linked_List<T>* list, const T item)
{
    auto head_prev = list->head;

    auto node = doubly_linked_node_alloc<T>();
    node->item = item;
    node->prev = NULL;
    node->next = head_prev;

    list->head = node;
    
    if (head_prev)
    {
        head_prev->prev = node;
    }
    else
    {
        list->tail = node;
    }

    return node;
}
```

#### `list_insert_after()`

Aici, vom schimba 3-5 pointeri:
- de la nodul nou la elementul precedent;
- de la nodul după care se inserează nodul nou la acest nod nou;
- de la nodul nou la nodul ce urmează nodul precedent;
- de la nodul (dacă nu e nul) ce urmează nodul după care se inserează nodul nou, la acest nod nou;
- s-ar putea schimba coada listei prin așa inserare.

```C++
t_ Doubly_Linked_Node<T>* list_insert_after(
    Doubly_Linked_List<T>* list, 
    Doubly_Linked_Node<T>* node, 
    const T item)
{
    auto new_node = doubly_linked_node_alloc<T>();
    new_node->item = item;

    // (node) <-> (next_node)
    // Changes into
    // (node) <-> (new_node) <-> (next_node)
    new_node->next = node->next;
    new_node->prev = node;
    if (node->next)  
        node->next->prev = new_node;
    node->next = new_node;

    // Need to adjust the list, in case the new node becomes the last
    if (node == list->tail)
    {
        list->tail = new_node;
    }

    return new_node;
}
```

#### `list_remove_node()`

Datorită faptului că orice nod este conștient de vecinii săi, el poate să se evoce din lista singur. Algoritmul este simplu: pointerul la următorul nod al nodului precedent se schimbă la un pointer către nodul următor, iar pointerul la nodul precedent al nodului ce urmează se schimbă la un pointer către nodul precendent. 

Evident, sunt preconizate și cazurile când elementul ce se șterge este capul sau/și coada. 

```C++
// This function produces unexpected behavior if the given node is not in the given list.
// If the node is in the middle of some other list, it will work fine. 
// Otherwise, a null pointer will be dereferenced so expect a crash.
t_ void list_remove_node(
    Doubly_Linked_List<T>* list, 
    Doubly_Linked_Node<T>* node)
{
    auto next_node = node->next;
    auto prev_node = node->prev;

    if (list->head == node) list->head = next_node;
    // Since the node is not the head node, it must have a node before itself.
    else prev_node->next = next_node;

    if (list->tail == node) list->tail = prev_node;
    // Since the node is not the tail node, it must have a node after itself.
    else next_node->prev = prev_node;
}
```

#### `list_print_backwards()`

```C++
void list_print_backwards(const Doubly_Linked_List<int>* list)
{
    auto node = list->tail;
    while (node)
    {
        printf("%i", node->item);
        node = node->prev;
        if (node) printf(" -> ");
    }
    printf("\n");
}
```

### Test

```C++
auto list = doubly_linked_list_make<int>();

assert(list.head == NULL);
assert(list.tail == NULL);

// 1
auto node_1 = list_insert_front(&list, 1);

assert(node_1->item == 1);
assert(node_1->next == NULL);
assert(node_1->prev == NULL);
assert(list.head == node_1);
assert(list.tail == node_1);

// 1 -> 2
auto node_2 = list_insert_back(&list, 2);

assert(node_1->prev == NULL);
assert(node_1->next == node_2);
assert(node_2->prev == node_1);
assert(node_2->next == NULL);
assert(list.head == node_1);
assert(list.tail == node_2);

// 3 -> 1 -> 2
auto node_3 = list_insert_before(&list, node_1, 3);

assert(node_1->prev == node_3);
assert(node_3->next == node_1);
assert(node_3->prev == NULL);
assert(list.head == node_3);

// 3 -> 1 -> 2 -> 4
auto node_4 = list_insert_after(&list, node_2, 4);

assert(node_2->next == node_4);
assert(node_4->prev == node_2);
assert(node_4->next == NULL);
assert(list.tail == node_4);

// 3 -> 1 -> 2 -> 5 -> 4 
auto node_5 = list_insert_after(&list, node_2, 5); 

assert(node_2->next == node_5);
assert(node_4->prev == node_5);

// 3 -> 1 -> 2 -> 4
auto removed_5 = list_remove(&list, 5);

assert(removed_5 == node_5);
free(removed_5);

// 3 -> 1 -> 2
list_remove_node(&list, node_4);

assert(node_2->next == NULL);
assert(node_2 == list.tail);

// 1 -> 2
auto removed_3 = list_remove(&list, 3);

assert(removed_3 == node_3);
free(removed_3);

// 1 -> 2
auto removed_null = list_remove(&list, 3);
assert(removed_null == NULL);

printf("Printing the list forwards: "); 
list_print(&list); // 1 -> 2

printf("Printing the list backwards: "); 
list_print_backwards(&list); // 2 -> 1

list_free(&list);
```

### Avantajele și dezavantajele

Avantajul principal al anume listei înlănțuite dublu asupra listei simplu înlănțuite este că orice element are posibilitate să se șteargă din lista în orice timp, astfel nu este recesar să aibă acces la predecesorul lui, ca să-l evoace. Al doilea avantaj este că înlănțuirea dublă permite iterarea bidirecțională, ce este util în unele cazuri.

Dezavantejele sunt aceleași ca și la listele simplu înlănțuite, însă, în plus, listele dublu înlănțuite pierd și mai mult spațiu la pointeri, și operațiile de inserare și de ștergere necesită mai multe operații de ajustare a legăturilor.

Acest tip de liste, însă, este mai ușor de sortat, decât listele simplu înlănțuite, dar necesită mai multe operații adăugătoare pentru interschimbarea elementelor între sine, în comparația cu tablouri obișnuite, sau o iterare asupra întregii liste în plus pentru a restabili legătura dublă. Însă, dacă avem nevoie de o colecție sortată și flexibilă, putem utiliza arbori binari de căutare balansați.


## Stive


### Descriere

Stiva este structura de date unde elementele și intră și sunt liate de pe top-ul stivei. Top-ul stivei poate fi fie indicele maxim în tabloul cu date, pentru stivele băzate pe tabouri, fie primul element într-o listă simplu înlănțuită, pentru stivele băzate pe liste.

Am realizat ambele variante de stive, le vom examina separat.

Stivele definesc trei operații principale:
- `push()` pune element pe top-ul stivei;
- `pop()` ia elementul de pe top-ul stivei;
- `peek()` examinează elementul de pe topul stivei, fără a-l elimina.

### Realizarea stivei băzate pe un tablou

[Vedeți întregul cod pentru stiva bazată pe un tablou.](https://github.com/AntonC9018/uni_asdc/blob/master/src/ds/array_stack.h)

Considerăm stiva de o lungime fixă, dată la inițializare. Evident, se poate modifica acest cod ca el să-și rescale tabloul cu elemente, când nu-i de ajuns spațiu, însă atunci codul va deveni mai complicat și lent, dar și mai flexibil. 

```C++
t_ struct Array_Stack
{
    T* items;
    // Shows the next free index in which a new value may be inserted
    size_t count;
};
```

Funcția `stack_push()` pune elementul pe poziția curentă în tablou. Nu fac aici nici un fel de bounds checking pentru a simplifica codul, și deoarece nu păstrez capacitatea tabloului în stivă.

```C++
t_ inline void stack_push(Array_Stack<T>* stack, const T item)
{
    stack->items[stack->count] = item;
    stack->count++;
}
```

Funcția `stack_pop()` face inversul. Aici aș putea face bounds-checking, verificând dacă contorul nu este nul.

```C++
t_ inline T stack_pop(Array_Stack<T>* stack)
{
    stack->count--;
    return stack->items[stack->count];
}
```

Și iată `stack_peek()`. Am definit și o versiune non-const pentru această funcție. Returnez adresa, fiindcă îmi pare logic așa.  În primul rând, evităm copiile elementelor neeliminate, în al doilea rând, permitem utilizatorului să schimbe valoarea itemului de pe top prin acest pointer (dacă este utilizată versiunea non-const).

```C++
t_ inline const T* stack_peek(const Array_Stack<T>* stack)
{
    return &stack->items[stack->count - 1];
}
```

### Realizarea stivei băzate pe o listă

[Vedeți întregul cod pentru stiva bazată pe un listă.](https://github.com/AntonC9018/uni_asdc/blob/master/src/ds/list_stack.h)


În structura pentru stivă, definim numai head-ul listei, deoarece punem și ștergem elementele dintr-un singur loc.

```C++
t_ struct List_Stack
{
    Singly_Linked_Node<T>* top;
};
```

În urma operației `push()`, elementul mereu devine capul listei. Nu avem coada, de aceea nici nu avem logica adăugătoare. Nu avem aici nici probleme cu dimensiunea tabloului, deoarece nodurile nimeresc pe heap.

```C++
t_ inline void stack_push(List_Stack<T>* stack, const T item)
{
    auto node = singly_linked_node_alloc<T>();
    node->item = item;
    node->next = stack->top;
    stack->top = node;
}
```

Operația `pop()` tot este foarte simplă. Aici am putea verifica dacă stiva nu conține nici un element să anulăm operația, însă las aceasta pe utilizatorul interfeței.

```C++
t_ inline T stack_pop(List_Stack<T>* stack)
{
    auto top = stack->top;
    auto item = top->item;
    stack->top = top->next;
    free(top);
    return item;
}
```

Și iată `peek()`:

```C++
t_ inline const T* stack_peek(const List_Stack<T>* stack)
{
    return &stack->top->item;
}
```

### Test

Prin faptul că interfața la cele două realizări este exact aceeași, putem să le verificăm pe ambele printr-o funcție template.

```C++
template<typename Stack>
void stack_test(Stack* stack)
{
    // initally empty
    assert(stack_is_empty(stack));

    // add some elements
    stack_push(stack, 3);
    stack_push(stack, 4);
    stack_push(stack, 5);

    // peek an element
    assert(*stack_peek(stack) == 5);

    // pop some back
    assert(stack_pop(stack) == 5);
    assert(stack_pop(stack) == 4);
    assert(stack_pop(stack) == 3);

    // emptied back
    assert(stack_is_empty(stack));

    stack_free(stack);
}
```

Și le apelăm pentru stack-uri diferite:
```C++
{
    auto stack = array_stack_make<int>(10);
    stack_test(&stack);
}
{
    auto stack = list_stack_make<int>();
    stack_test(&stack);
}
```

### Diferențe dintre cele două realizări

Este evident că prima metodă este mai aplicabilă în cazuri în care lungimea maximală a stivei se știe dinainte, sau când poate fi setată la o lungime maximă sigură, să nu fie posibil să avem o depășire. 

Dacă avem o stivă tot bazăte pe un tablou dar de o lungime dinamică, atunci am putea avea problema de copierea excesivă a datelor, dacă regular punem pe stack elemente peste limită, ceea ce nu se va întâmpla dacă utilizăm stivele băzate pe liste.


## Cozile

### Descriere

O coadă este de fapt inversul unei stive. Aici, elementele sunt plăsate pe top, dar luate de pe fund.

Aici am realizat două metode de păstrare a elementelor: printr-un bufer ciclic și printr-o listă simplu înlănțuită.

Operațiile de bază a unei coade sunt:
- `enqueue()` adaugă un element pe coadă;
- `dequeue()` scoate un element din coada;
- `peek()` returnează următorul element fără a-l scoate din coadă.

### Bufer ciclic

[Vedeți întregul cod pentru bufer ciclic.](https://github.com/AntonC9018/uni_asdc/blob/master/src/ds/cyclic_buffer.h)

Bufer ciclic este un tablou de lungime fixă, în care elementele sunt inserate în poziții consecutive, iar luate din pozițiile trecute, după rând. Indicele inserării este calculat ca (indicele_precedent + 1) mod lungimea_tabloului, deci când se ajunge la capătul tabloului, se întoarce (loops over) la început.

Pentru a realiza aceasta, avem nevoie de 4 informații:
- pointer la începutul tabloului;
- lungimea tabloului (sau pointerul la sfârșitul tabloului);
- pointer (sau indice) la elementul curent care s-ar scoate din coadă;
- pointer la celulă, unde s-ar insereze următorul element (sau un contor).


```C++
t_ struct Cyclic_Buffer
{
    T* items;
    size_t item_index;
    size_t count;
    size_t capacity;
};
```

Realizarea operației `enqueue()` implică calcularea următorului indice, amplasarea elementului pe acea poziție și incrementarea contorului numărului de elemente:

```C++
t_ inline size_t _cycbuf_next_index(const Cyclic_Buffer<T>* cycbuf)
{
    return (cycbuf->count + cycbuf->item_index) % cycbuf->capacity;
}

t_ inline void cycbuf_enqueue(Cyclic_Buffer<T>* cycbuf, const T item)
{
    // assert(!cycbuf_is_full(cycbuf));
    auto index = _cycbuf_next_index(cycbuf);
    cycbuf->items[index] = item;
    cycbuf->count++;
}
```

Operația `dequeue()` implică inversul:

```C++
t_ inline T cycbuf_dequeue(Cyclic_Buffer<T>* cycbuf)
{
    // assert(!cycbuf_is_empty(cycbuf));
    auto index = cycbuf->item_index;
    cycbuf->count--;
    cycbuf->item_index = (cycbuf->item_index + 1) % cycbuf->capacity;
    return cycbuf->items[index];
}
```

Iar `peek()` simplu returnează următorul element:

```C++
t_ inline const T* cycbuf_peek(const Cyclic_Buffer<T>* cycbuf)
{
    // assert(!cycbuf_is_empty(cycbuf));
    return &cycbuf->items[cycbuf->item_index];
}
```

### Coada băzată pe o listă

[Vedeți întregul cod pentru coada băzată pe o listă.](https://github.com/AntonC9018/uni_asdc/blob/master/src/ds/queue.h)

Așa coada este ea însuși esențial simplu o listă:
```C++
t_ using Queue = Singly_Linked_List<T>;
```

Funcțiile de bază în mare parte apelează la aceleași funcții din partea listei:

```C++
t_ inline void q_enqueue(Queue<T>* queue, const T item)
{
    list_insert_back(queue, item);
}

t_ inline T q_dequeue(Queue<T>* queue)
{
    auto head = list_remove_head(queue);
    auto item = head->item;
    free(head);
    return item;
}

t_ inline T* q_peek(Queue<T>* queue)
{
    return &(queue->head->item);
}
```

Astfel, așa coadă este de fapt o listă cu o gamă de capacități special lititată.

### Test

Interfața este practic aceeași, însă numele funcțiilor sunt diferite. Arăt numai suita test la bufer ciclic, deoarece este mai completă.

```C++
auto cycbuf = cycbuf_make<int>(3);

assert(cycbuf_is_empty(&cycbuf));

cycbuf_enqueue(&cycbuf, 1);
cycbuf_enqueue(&cycbuf, 2);
cycbuf_enqueue(&cycbuf, 3);

assert(cycbuf_is_full(&cycbuf));

auto item_1 = cycbuf_dequeue(&cycbuf);

assert(!cycbuf_is_full(&cycbuf));
assert(cycbuf.count == 2);
assert(item_1 == 1);

cycbuf_enqueue(&cycbuf, 4);

auto item_2 = *cycbuf_peek(&cycbuf);

assert(item_2 == 2);

cycbuf_dequeue(&cycbuf);
cycbuf_dequeue(&cycbuf);
cycbuf_dequeue(&cycbuf);

assert(cycbuf_is_empty(&cycbuf));

cycbuf_free(&cycbuf);
```

### Comparație

Aici avem o situație asemănătoare ca la stive: dacă știm dinainte lungimea maximă sigură, utilizăm buferul ciclic, dacă nu — coada băzată pe liste (în general).


## Arbore binar de căutare modificat

În laborator 1 deja am realizat funcțiile pentru inserarea elementelor în arbore binar de căutare și de însăși căutarea, am realizat și funcțiile pentru printarea elementelor în ordine. Rămâne să demonstrez ștergerea și afișarea în diferite moduri.

### Ștergerea

Algoritmul este următorul:
1. Se găsește elementul care trebuie să fie șters în arbore.
2. Dacă el nu are fii, el pur și simplu se elimină din listă.
3. Dacă el are un fiu, arborele se modifică: fiul ia locul părintelui.
4. Dacă el are doi fii, se găsește "succesorul după ordine" al lui, și ia locul lui. Succesorul după ordine este cel mai mic element din subarborele drept al elementului care se șterge.

Am citit la acest algoritm pe [geeksforgeeks](https://www.geeksforgeeks.org/binary-search-tree-set-2-delete/), însă am realizat unele momente ceva diferit. 

Șmecheria mea este ca să utilizez un pointer la memorie în nodul-parinte a succesorului, unde se află un pointer la însăși copil care trebuie să fie schimbat cu locuri cu elementul care se șterge. Realizarea de pe site folosește o apelare recursivă pentru a elimina nodul-succesor, iar eu pur și simplu scriu un `NULL` în memorie unde se află pointerul la el, astfel eliminând legătura dintre părintele și fiul fără a traversa încă o dată arborele (ar fi neeficient). 

```C++
t_
Binary_Tree<T>** bst_min_ptr_in_parent(Binary_Tree<T>** min_node)
{
    while ((*min_node)->left)
    {
        min_node = &(*min_node)->left;
    }

    return min_node;
}

tc_
Binary_Tree<T>* bst_remove(
    Binary_Tree<T>* node, 
    C compare_func)
{
    if (!node) return NULL;

    int comparison_result = compare_func(node->item);
    
    if (comparison_result > 0)
    {
        node->left = bst_remove(node->left, compare_func);
    }
    else if (comparison_result < 0)
    {
        node->right = bst_remove(node->right, compare_func);
    }
    else
    {
        // Right becomes the root
        if (!node->left)
        {
            auto t = node->right;
            free(node);
            return t;
        }
        
        // Left becomes the root
        if (!node->right)
        {
            auto t = node->left;
            free(node);
            return t;
        }

        // Otherwise, the node has 2 children.
        // Find the parent of inorder successor (minimum in the right subtree).
        auto** min_leaf_ptr_in_parent = bst_min_ptr_in_parent(&node->right);
        auto*  min_leaf = *min_leaf_ptr_in_parent;

        // Since the info about the parent of the node that is currently being removed
        // has been lost, copy the value instead.
        node->item = min_leaf->item;

        // We're free to delete this node, since it is terminal.
        free(min_leaf);

        // Remove the dangling pointer from the parent node.
        *min_leaf_ptr_in_parent = NULL;
    }

    return node;
}
```

### Afișarea

Într-un arbore binar, elementul din dreapta este mai mare ca rădăcina, iar elementul din stângă — mereu mai mic. Pentru a afișa arborele în diferite moduri, trebuie pur și simplu să schimbăm ordinea în care parcurgem copiii și ne afișăm datele curente. 

Iată cele trei realizări pentru tipul de date `int`. În ultimul, am pus expresiile în mod aleator:

```C++
template<>
void bst_print_inorder(Binary_Tree<int>* node)
{
    if (!node) return;
    bst_print_inorder(node->left);
    printf("%i ", node->item);
    bst_print_inorder(node->right);
}
template<>
void bst_print_outorder(Binary_Tree<int>* node) 
{
    if (!node) return;
    bst_print_outorder(node->right);
    printf("%i ", node->item);
    bst_print_outorder(node->left);
}

template<>
void bst_print_noorder(Binary_Tree<int>* node)
{
    if (!node) return;
    printf("%i ", node->item);
    bst_print_noorder(node->right);
    bst_print_noorder(node->left);
}
```


### Test

Verificăm funcțiile noi în main:

```C++
void bst_removal()
{
    using namespace DS;
    auto cmp_ints = [](int a, int b) { return a - b; };
    auto find_int = [](int b){ return [=](int a) { return a - b; }; };

    Binary_Tree<int>* bst = NULL;

    //          3              //
    //         / \             //
    //        /   \            //
    //       1     5           //
    //      / \   / \          //
    //     0   2 4   6         //
    bst_insert(&bst, 3, cmp_ints);
    bst_insert(&bst, 1, cmp_ints);
    bst_insert(&bst, 5, cmp_ints);
    bst_insert(&bst, 0, cmp_ints);
    bst_insert(&bst, 2, cmp_ints);
    bst_insert(&bst, 4, cmp_ints);
    bst_insert(&bst, 6, cmp_ints);
    bst_print_inorder(bst); printf("\n");

    // -------------------------------------------------------- //
    // Simplest case, leaf node                                 //
    // -------------------------------------------------------- //
    //         3                  3                   3         //
    //        / \                / \                 / \        //
    //       /   \              /   \               /   \       //
    //      1     5     ->     1     5      ->     1     5      //
    //     / \   / \          / \   / \             \   / \     //
    //    0   2 4   6        x   2 4   6             2 4   6    //
    // -------------------------------------------------------- //
    bst = bst_remove(bst, find_int(0));
    bst_print_inorder(bst); printf("\n");

    // -------------------------------------------------------- //
    // One child                                                //
    // -------------------------------------------------------- //
    //       3                    3                   3         //
    //      / \                  / \                 / \        //
    //     /   \                /   \               /   \       //
    //    1     5      ->      x     5      ->     2     5      //
    //     \   / \              \   / \                 / \     //
    //      2 4   6              2 4   6               4   6    //
    // -------------------------------------------------------- //
    bst = bst_remove(bst, find_int(1));
    bst_print_inorder(bst); printf("\n");

    // -------------------------------------------------------- //
    // Two children                                             //
    // -------------------------------------------------------- //
    //       3                    x                   4         //
    //      / \                  / \                 / \        //
    //     /   \                /   \               /   \       //
    //    2     5      ->      2     5      ->     2     5      //
    //         / \                  / \                   \     //
    //        4   6                4   6                   6    //
    // -------------------------------------------------------- //
    bst = bst_remove(bst, find_int(3)); 
    bst_print_inorder(bst); printf("\n");

    // Make sure they are in this order
    assert(              bst->item == 4);
    assert(        bst->left->item == 2);
    assert(       bst->right->item == 5);
    assert(       bst->right->left == NULL);
    assert(bst->right->right->item == 6);

    bst_free(bst);
}
```

Output:

```C++
0 1 2 3 4 5 6
1 2 3 4 5 6
2 3 4 5 6
2 4 5 6
```

Am făcut și un test pentru afișare, însă el printează prea mult.
```C++
// Should be printed in ascending order
void bst_print()
{
    auto records = read_records_from_csv("data_unordered.csv");
    auto bt      = binary_tree_from_records(records);

    printf("Printing INORDER: \n\n");
    DS::bst_print_inorder(bt);
    printf("\n\n");

    printf("Printing OUTORDER: \n\n");
    DS::bst_print_outorder(bt);
    printf("\n\n");

    printf("Printing NOORDER: \n\n");
    DS::bst_print_noorder(bt);
    printf("\n\n");

    destroy_records(records);
    bst_free(bt);
}
```

### Concluzii

Arborele binar de căutare este destul de practic când avem nevoie de o colecție flexibilă, care vrem și s-o parcurgem în ordine, și să verificăm dacă un element există, rapid ($log N$). Însă, formele primitive de așa structuri de date sunt rar utilizate, deoarece riscul că ele să degradeze la ceva aproximativ egal cu liste înlănțuite este inadmisibil. Se folosesc formele mai avansate. Am auzit numai despre `red-black tree`, dar nu am studiat algoritmul. Știu numai că în `C#` așa arbori se numesc `SortedSet`.