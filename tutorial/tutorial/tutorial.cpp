
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstring>

typedef struct {
    int IQ;
    char* nev;
    unsigned int magassag;
} Human;

int main()
{
    Human* ency = (Human*)malloc(sizeof(Human));
    if (ency == nullptr) {
        return -1;
    }

    ency->IQ = -1;
    ency->magassag = 152;

    char enci_neve[] = "Tess Eniko (skibidi)";
    ency->nev = (char*)malloc(sizeof(enci_neve)+1);
    if (ency->nev == nullptr) {
        return -1;
    }
    strcpy_s(ency->nev, sizeof(enci_neve), enci_neve);



    printf("dis person is called %s", ency->nev);

    free(ency->nev);
    free(ency);
    
}
