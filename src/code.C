#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct {
    long waga;      //największa wartość o wadze mniejszej równej
    long* tab;      //to jakimi pakunkami możemy to zrealizować, 1-bierzym dany produkt, 0 nie
} node;

void gen(long ** ne,int n)      //alokowanie tablicy długości n
{
    *ne=(long*)malloc(sizeof(long)*n);
    for (int i = 0; i < n; i++)
    {
        (*ne)[i]=0;
    }
}

void copy(long *ne,long *por, int n)    //do tablicy ne przypisuje wartość tablicy por
{
    for (int i = 0; i < n; i++)
    {
        ne[i]=por[i];
    }
}
static PyObject *
paker_pakowanko(PyObject *self, PyObject *args)
{
    long num;
    PyObject *lista;
    if (! PyArg_ParseTuple( args, "lO", &num, &lista)) return NULL; //wczytujemy maksymalną wagę i listę
    long *tab;  // lista przedmiotów
    int n=PyList_Size(lista);   //długość listy
    tab=(long*)malloc(sizeof(long)* (n+1)); //alokujemy pamięć na listę

    tab[0]=0;   //w sumie to zerowy obiekt nie jest ważny
    for (int i=0;i<n;i++)   //rozpakowujemy listę
    {
        PyObject *temp=PyList_GetItem(lista, i);    
        tab[i+1]=PyLong_AsLong(temp);
        if (tab[i+1]<0)
        {
            PyErr_BadArgument();
            return NULL;
        }
    }
    node **maxa;
    maxa=(node**)malloc(sizeof(node*)* (n+1)); //alokujemy pamięć  
    for (int i=0;i<n+1;i++)
    {
        maxa[i]=(node*)malloc(sizeof(node)* (num+1));
    }
    for (int i = 0; i <=n; i++)
    {
        for (int j = 0; j <= num; j++)
        {
            maxa[i][j].waga=0;
            gen(&(maxa[i][j].tab),n);
            
        }
        
    }
    for (int a = 1; a<=n; ++a)  //główna część algorytmu
    {
        for (int j = 0; j <= num; j++)
        {
            if (tab[a]<=j)
            {
                if (maxa[a-1][j].waga>tab[a]+maxa[a-1][j-tab[a]].waga)
                {
                    maxa[a][j].waga=maxa[a-1][j].waga;
                    copy(maxa[a][j].tab,maxa[a-1][j].tab,n);
                }
                else
                {
                    maxa[a][j].waga=maxa[a-1][j-tab[a]].waga+tab[a];
                    copy(maxa[a][j].tab,maxa[a-1][j-tab[a]].tab,n);
                    maxa[a][j].tab[a-1]=1;
                }
                

            }
            else
            {
                maxa[a][j].waga=maxa[a-1][j].waga;
                copy(maxa[a][j].tab,maxa[a-1][j].tab,n);
            }
        }
    }
    PyObject *wyn=PyList_New(0); //wynik
    for (int i =0;i<n;i++)
    {
        if (maxa[n][num].tab[i]==1)
        {
            PyObject *temp=PyLong_FromLong(tab[i+1]); 
            PyList_Append(wyn,temp);    //dodajemy elementy do wyniku
        }
    }
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //dealokujemy
    for (int i = 0;i <= n; i++)
    {
        for (int  j = 0; j <= num; j++)
        {
            free(maxa[i][j].tab);
        }
        
        free(maxa[i]);
    }
    free(tab);
    free(maxa);
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    return wyn;
}

static PyMethodDef paker_methodes[]={
    {"pakuj_plecak", (PyCFunction)(void(*)(void))paker_pakowanko, METH_VARARGS | METH_KEYWORDS,
     "rozwiązuje problem plecakowy"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef pakermodule = {
    PyModuleDef_HEAD_INIT,
    "paker",
    NULL,
    -1,
    paker_methodes
};


PyMODINIT_FUNC
PyInit_paker(void)
{
    return PyModule_Create(&pakermodule);
}