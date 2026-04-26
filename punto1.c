#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Tarea{
int TareaID;//Numerico autoincremental comenzando en 1000
char *Descripcion; //
int Duracion; // entre 10-100
} Tarea;

typedef struct Nodo{
Tarea T;
struct Nodo *Siguiente;
} Nodo;

typedef struct{
    struct Nodo* L;
    int cantidad;
}Lista;

Lista crearLista(){
    Lista nuevaL;             
    nuevaL.L = NULL;           
    nuevaL.cantidad = 0;       
    return nuevaL;            
}

Nodo* crearNodo(int id, char* desc, int dur){
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    nuevo->T.TareaID = id;
    nuevo->T.Duracion = dur;
    nuevo->T.Descripcion = (char*)malloc(strlen(desc)+1);
    strcpy(nuevo->T.Descripcion,desc);
    nuevo->Siguiente = NULL;
    return nuevo;
}

void insertar(Lista *h, Nodo *nuevo){
    nuevo->Siguiente = h->L;
    h->L = nuevo;
    h->cantidad++;
}

void cargarTareas(Lista *pendientes) {
    int id = 1000; 
    int continuar=1;
    char buffer[100];
    int duracion;

     do{
        printf("\nNueva Tarea ID: %d\n", id);
        printf("Descripcion: ");
        fflush(stdin); 
        gets(buffer); 
        
        printf("Duracion (10-100): ");
        scanf("%d", &duracion);

        Nodo* nuevo = crearNodo(id, buffer, duracion);
        insertar(pendientes, nuevo);

        id++;
        printf("Desea cargar otra tarea? (1: Si / 0: No): ");
        scanf("%d", &continuar);
    }while(continuar==1);
}

void mostrar(Lista H){
    Nodo* aux = H.L;                   
    while(aux != NULL){         
        printf("ID:%d\n", aux->T.TareaID);
        printf("Descripcion:%s\n", aux->T.Descripcion);
        printf("Duracion:%d\n", aux->T.Duracion);
        aux = aux->Siguiente;   
    }
}

void liberar(Lista *H){
    Nodo *aux = H->L;
    Nodo *proximo;

    while(aux != NULL){
        proximo = aux->Siguiente;
        free(aux->T.Descripcion);
        free(aux);
        aux = proximo;
    }
}

void transferirTareas(Lista *pendientes, Lista *realizadas) {
    Nodo *actual = pendientes->L;
    Nodo *anterior = NULL;
    int respuesta;

    printf("\n--- Control de Tareas Pendientes ---\n");

    while (actual != NULL) {
        printf("\nID: %d | Descripcion: %s | Duracion: %d\n", 
                actual->T.TareaID, actual->T.Descripcion, actual->T.Duracion);
        printf("\n¿Realizo esta tarea? (1: Si / 0: No): ");
        scanf("%d", &respuesta);

        if (respuesta == 1) {
            Nodo *nodoARealizar = actual;

            if (anterior == NULL) {
                pendientes->L = actual->Siguiente;
            } else {
                anterior->Siguiente = actual->Siguiente;
            }
            actual = actual->Siguiente; 
            pendientes->cantidad--;
            nodoARealizar->Siguiente = realizadas->L;
            realizadas->L = nodoARealizar;
            realizadas->cantidad++;

            printf("Tarea realizada\n");
        } else {
            anterior = actual;
            actual = actual->Siguiente;
        }
    }
}

void pertenece(Lista pendiente,Lista realizada,int id){
    Nodo* aux = pendiente.L;                        
    while(aux != NULL){          
            if(aux->T.TareaID==id){      
                printf("Es una tarea pendiente\n");
                printf("\nID: %d | Desc: %s | Duracion: %d\n", aux->T.TareaID, aux->T.Descripcion, aux->T.Duracion);
                return;
            }
        aux = aux->Siguiente;   
    }
    aux = realizada.L;
    while(aux != NULL){          
        if(aux->T.TareaID==id){      
            printf("Es una tarea realizada\n");
            printf("\nID: %d | Desc: %s | Duracion: %d\n", aux->T.TareaID, aux->T.Descripcion, aux->T.Duracion);
            return;
        }
        aux = aux->Siguiente;   
    }
    printf("\nNo se encontro ninguna tarea con el ID %d.\n", id);
    return;
}

int main(){
Lista pendiente;
Lista realizada;
int id;
pendiente=crearLista();
realizada= crearLista();
cargarTareas(&pendiente);
transferirTareas(&pendiente,&realizada);
printf("Tareas pendientes:\n");
mostrar(pendiente);
printf("Tareas realizadas:\n");
mostrar(realizada);
printf("Id de la tarea a consultar:\n");
scanf("%d",&id);
pertenece(pendiente,realizada,id);
liberar(&pendiente);
liberar(&realizada);
    return 0;
}