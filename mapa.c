#include "mapa.h"

//FUNCIONES

void guardar_mapa (Elemento *vector,int indice);//Guardar en ficheros
void aleatorio(int* x, int* y,configuracion c);

//Cebecera: void aleatorio(int* x, int* y)
//Precondicion: x e y deben estar inicializados
//Postcondicion: Devuelve por referencia dos numeros aleatorios
void aleatorio(int* x, int* y,configuracion c){
	int raiz;
	srand (time(NULL));
	do{
		*x = (rand() % ((c.radio_mapa+1)*2))-c.radio_mapa;//Aleatorio para el eje x
		*y = (rand() % ((c.radio_mapa+1)*2))-c.radio_mapa;//Aleatorio para el eje y
		raiz = sqrt(pow(*x,2) + pow(*y,2));
	}while(raiz > c.radio_mapa);//Mientras el modulo de las posiciones x e y sea mayor que el radio del mapa se repita ya que no se puede generar fuera
}

//Cabecera: void generar(Elementos vector[], configuracion c, usuario u[])
//Precondicion: Ninguna
//Postcondicion: Guardar en vector[] los datos de cada elemento que haya en el mapa generado
void generar_mapa (Elemento **vector, usuario **u,objetos *o,configuracion c){

	int i,njugadores,obj_id,vindice,n=0;
	int *v;
	
    njugadores= njugadores_EE((*u));
    srand (time(NULL));
	v = (int*) malloc (n*sizeof(int));
	for(i=0;i<nusuarios;i++)
	{
		if(strcmp((*u)[i].estado,"EE")==0)
		{
			strcpy((*u)[i].estado,"EJ");
			n++;
			v = (int*) realloc (v,n*sizeof(int));
			v[n-1] = i;
		}
	}
	nelementos=0;
	do{//Bucle para guardar jugadores
		nelementos++;
		(*vector)= (Elemento*) realloc((*vector), nelementos*sizeof(Elemento));
		strcpy((*vector)[nelementos-1].tipo, "Jugador");
        vindice = rand() % n;
		strcpy((*vector)[nelementos-1].nombre,(*u)[v[vindice]].nick);
		aleatorio(&(*vector)[nelementos-1].posx, &(*vector)[nelementos-1].posy,c);
        v[vindice] = v[n-1];
		n--;
		v = (int*) realloc (v,n*sizeof(int));
	}while(n>0);
	for(i=0 ; i<(njugadores*3) ; i++){//Bucle para guardar objetos

        nelementos++;
        (*vector)= (Elemento*) realloc((*vector), nelementos*sizeof(Elemento));

        strcpy((*vector)[nelementos-1].tipo, "Objeto");

        obj_id=(rand() % (nobjetos));//Numero de objetos

        strcpy((*vector)[nelementos-1].nombre,o[obj_id].item_ID);

        aleatorio(&(*vector)[nelementos-1].posx, &(*vector)[nelementos-1].posy,c);
	}
}

//Cabecera: void guardar_mapa (Elemento vector[])
//Precondicion: Que el vector vector[] de Elementos este inicializado
//Postcondicion: Imprimir en un fichero todos los datos contenidos en el vector vector[]
void guardar_mapa (Elemento *vector,int indice)
{
	FILE *fp;
	int i;

	if((fp= fopen("mapa.txt","w+"))==NULL){
		printf("No se ha podido crear mapa.txt\n");
	}
    else{
        for(i=0; i<nelementos; i++){
            fprintf(fp, "%s/%s/%i/%i\n",vector[indice].tipo, vector[indice].nombre, vector[indice].posx, vector[indice].posy);
			indice++;
			if(indice == nelementos) indice=0;  
        }
        fclose(fp);
    }
}

void borrar_elemento(Elemento **vector,int id)
{
	if(id!=nelementos-1)
	{
		strcpy((*vector)[id].nombre,(*vector)[nelementos-1].nombre);
		strcpy((*vector)[id].tipo,(*vector)[nelementos-1].tipo);
		(*vector)[id].posx = (*vector)[nelementos-1].posx;
		(*vector)[id].posy = (*vector)[nelementos-1].posy;
	}
	nelementos--; 
	(*vector) = (Elemento*) realloc ((*vector),nelementos*(sizeof(Elemento)));
}

void elementos_mapa(Elemento *e)
{
	printf("-----------Elementos Mapa---------\n");
	for(int i=0;i<nelementos;i++)
	{
		printf("%s/%s/%i/%i\n",e[i].tipo,e[i].nombre,e[i].posx,e[i].posy);
	}
	system("pause");
	printf("\n\n");
}

void cargar_mapa(Elemento **e)
{
	char cadena[100];
	char *ptr;
	char *delim="/";
	FILE *f;
	nelementos=0;
	f=fopen("mapa.txt","r");
	while(fgets(cadena,100,f)!=NULL)
	{
		fixstring(cadena);
		fflush(stdin);
		*e=(Elemento* )realloc((*e),(nelementos+1)*sizeof(Elemento));
		nelementos++;
		ptr=strtok(cadena,delim);
		strcpy((*e)[nelementos-1].tipo,ptr);
		ptr=strtok(NULL,delim);
		strcpy((*e)[nelementos-1].nombre,ptr);
		ptr=strtok(NULL,delim);
		(*e)[nelementos-1].posx=atoi(ptr);
		ptr=strtok(NULL,delim);
		(*e)[nelementos-1].posy=atoi(ptr);
	}
	fclose(f);
}

int n_jugadores(Elemento *jm)
{
	int cont,i;
	for(i=0;i<nelementos;i++)
	{
		if(strcmp(jm[i].tipo,"Jugador")==0) cont++;
	}
	return cont;
}

void borrar_mapa(Elemento **jm)
{
	nelementos=0;
	(*jm)=(Elemento*)realloc((*jm),nelementos*sizeof(Elemento));
}