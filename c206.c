/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	 // Set the first, last, and active elements to NULL, indicating an empty list
    list->firstElement = NULL;
    list->lastElement = NULL;
    list->activeElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	// Pointer to traverse the list
    DLLElementPtr current = list->firstElement;
    DLLElementPtr next;

    // Traverse the list
    while (current != NULL) {
        // Store the pointer to the next element before freeing the current
        next = current->nextElement;
        // Free the current element
        free(current);
        // Move to the next element
        current = next;
    }

    // Reset the list to an empty state
    list->firstElement = NULL;
    list->lastElement = NULL;
    list->activeElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
	// Allocate memory for the new node
    DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement));
    // Check if memory allocation was successful
    if (newElement == NULL) {
        DLL_Error();
        return;
    }
    // Set the data field of the new node
    newElement->data = data;
    // Set the next pointer of the new node to the current first node
    newElement->nextElement = list->firstElement;
    // Set the previous pointer of the new node to NULL
    newElement->previousElement = NULL;
    // Update the first node to point back to the new node
    if (list->firstElement != NULL) {
        list->firstElement->previousElement = newElement;
    }
    // Update the list's first node pointer to the new node
    list->firstElement = newElement;
    // If the list was empty, update the last node pointer too
    if (list->lastElement == NULL) {
        list->lastElement = newElement;
    }
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
	 // Allocate memory for the new node
    DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement));
    // Check if memory allocation was successful
    if (newElement == NULL) {
        DLL_Error();
        return;
    }
    // Set the data field of the new node
    newElement->data = data;
    // Set the previous pointer of the new node to the current last node
    newElement->previousElement = list->lastElement;
    // Set the next pointer of the new node to NULL
    newElement->nextElement = NULL;
    // Update the last node to point forward to the new node
    if (list->lastElement != NULL) {
        list->lastElement->nextElement = newElement;
    }
    // Update the list's last node pointer to the new node
    list->lastElement = newElement;
    // If the list was empty, update the first node pointer too
    if (list->firstElement == NULL) {
        list->firstElement = newElement;
    }
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	// Set the active element to the first element of the list
	list->activeElement = list->firstElement;  
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	// Set the active element to the last element of the list
	list->activeElement = list->lastElement;  
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
	// Check if the list is empty
	if (list->firstElement == NULL) {  
        DLL_Error();
    } else {
		// Get the data of the first element
        *dataPtr = list->firstElement->data;  
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
	// Check if the list is empty
	if (list->lastElement == NULL) {  
        DLL_Error();
    } else {
		// Get the data of the last element
        *dataPtr = list->lastElement->data;  
    }
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	 if (list->firstElement != NULL) {
        DLLElementPtr temp = list->firstElement;
        list->firstElement = list->firstElement->nextElement;
        if (list->activeElement == temp) {
			// If first element was active, activity is NULL
            list->activeElement = NULL;  
        }
        if (list->firstElement != NULL) {
			// Update the previous pointer of the new first element
            list->firstElement->previousElement = NULL;  
        } else {
			// If list becomes empty, update the last element pointer
            list->lastElement = NULL;  
        }
		// Free the deleted first element
        free(temp);  
    }
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	if (list->lastElement != NULL) {
        DLLElementPtr temp = list->lastElement;
        list->lastElement = list->lastElement->previousElement;
        if (list->activeElement == temp) {
			// If last element was active, activity is lost
            list->activeElement = NULL;  
        }
        if (list->lastElement != NULL) {
			// Update the next pointer of the new last element
            list->lastElement->nextElement = NULL;  
        } else {
			// If list becomes empty, update the first element pointer
            list->firstElement = NULL;  
        }
		// Free the deleted last element
        free(temp);  
    }
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	if (list->activeElement != NULL && list->activeElement->nextElement != NULL) {
        DLLElementPtr temp = list->activeElement->nextElement;
		// Update the next pointer of the active element
        list->activeElement->nextElement = temp->nextElement;  
        if (temp->nextElement != NULL) {
			// Update the previous pointer of the next element
            temp->nextElement->previousElement = list->activeElement;  
        } else {
			// If deleted element was the last, update the last element pointer
            list->lastElement = list->activeElement;  
        }
        free(temp);  // Free the deleted element
    }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	if (list->activeElement != NULL && list->activeElement->previousElement != NULL) {
        DLLElementPtr temp = list->activeElement->previousElement;
		// Update the previous pointer of the active element
        list->activeElement->previousElement = temp->previousElement;  
        if (temp->previousElement != NULL) {
			// Update the next pointer of the previous element
            temp->previousElement->nextElement = list->activeElement;  
        } else {
			// If deleted element was the first, update the first element pointer
            list->firstElement = list->activeElement;  
        }
		// Free the deleted element
        free(temp);  
    }
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
	// Check if the list is active
	if (list->activeElement != NULL) {  
        DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement));  // Allocate memory for new element
        if (newElement == NULL) {
			// Call error function if memory allocation fails
            DLL_Error();  
            return;
        }
		// Set the data of new element
        newElement->data = data; 
		// Set the next pointer of new element
        newElement->nextElement = list->activeElement->nextElement;  
		// Set the previous pointer of new element
        newElement->previousElement = list->activeElement;  
        if (list->activeElement->nextElement != NULL) {
			// Update the previous pointer of the next element
            list->activeElement->nextElement->previousElement = newElement;  
        } else {
			// Update the last element pointer if new element is inserted at the end
            list->lastElement = newElement;  
        }
		// Update the next pointer of the active element
        list->activeElement->nextElement = newElement;  
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
	// Check if the list is active
	if (list->activeElement != NULL) {  
		// Allocate memory for new element
        DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement));  
        if (newElement == NULL) {
			// Call error function if memory allocation fails
            DLL_Error();  
            return;
        }
		// Set the data of new element
        newElement->data = data;  
		// Set the next pointer of new element
        newElement->nextElement = list->activeElement;  
		// Set the previous pointer of new element
        newElement->previousElement = list->activeElement->previousElement;  
        if (list->activeElement->previousElement != NULL) {
			 // Update the next pointer of the previous element
            list->activeElement->previousElement->nextElement = newElement; 
        } else {
			// Update the first element pointer if new element is inserted at the beginning
            list->firstElement = newElement;  
        }
		// Update the previous pointer of the active element
        list->activeElement->previousElement = newElement;  
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
	// Check if the list is active
	if (list->activeElement != NULL) {  
		// Get the value of the active element
        *dataPtr = list->activeElement->data;  
    } else {
		// Call error function if the list is not active
        DLL_Error();  
    }
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	// Check if the list is active
	if (list->activeElement != NULL) { 
		// Set the new value of the active element 
        list->activeElement->data = data;  
    }
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	// Check if the list is active
	if (list->activeElement != NULL) { 
		// Move the activity to the next element 
        list->activeElement = list->activeElement->nextElement;  
    }
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	 // Check if the list is active
	if (list->activeElement != NULL) { 
		// Move the activity to the previous element
        list->activeElement = list->activeElement->previousElement;  
    }
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	// Return non-zero if the list is active, otherwise return 0
	return (list->activeElement != NULL);  
}

/* Konec c206.c */
