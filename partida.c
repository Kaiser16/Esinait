#include "partida.h"

int op_usuario_partida(usuario *u,configuracion c);
int id_objeto(objetos *o,mochila *m,usuario *u,int indice);
int op_admin_partida(usuario *u,configuracion c);
void jugadores_espera(usuario *u, configuracion c);
void terminar_partida_jugadores(usuario **u,Elemento **jm,configuracion c);
void movimiento(Elemento **jm,int indice,configuracion c,int *accion);
void usar_arma(usuario **u,Elemento **jm,mochila **m,int *turno,int alcance,int danio,int *ido,objetos *o,int *acciones);
void mostrar_jugadores_cercanos(Elemento *jm,int idm,int alcance);
void mostrar_objetos_cercanos(Elemento **jm,int *idm,configuracion c,mochila **m,usuario *u,int indice,int *accion);
int turno_jugador(Elemento *jm,char *nick,int indice);
void usar_objeto (usuario **u,objetos *o,mochila **m,int ido,int indice);
int estar_partida(usuario **u,Elemento *jm,int indice);

//cabecera: void lobby (usuario **u,configuracion c,int *indice,Elemento **jm,mochila **m,objetos *o,tormenta **t)
//precondicion: recibe los vectores de estructura por referencia usuario,Elemento,mochila y tormenta y por valor objeto, la estructura configuracion y el indice por referencia del usuario
//postcondicion: muestra el menu del lobby
void lobby (usuario **u,configuracion c,int *indice,Elemento **jm,mochila **m,objetos *o,tormenta **t)
{
	int op,turno=0,idob=-1,acciones=0,cont=0,tormenta=0,aux,i;
	aux=turno_partida;
	do
	{
		if(turno_partida!=-1)cargar_mapa(&(*jm));
		else turno_partida++;
		if(estar_partida(&(*u),*jm,*indice)==0)
		{
			do
			{
				do
				{
					while(aux >= (*t)[tormenta].tiempo && tormenta == ntormentas-1)
					{
						if(aux >= (*t)[tormenta].tiempo)
						{
							aux = aux - (*t)[tormenta].tiempo;
							tormenta++;
						}
					}
					fuera_tormenta(*t,tormenta,&(*jm),&(*u));
					while(strcmp((*jm)[turno].tipo,"Jugador")!=0) 
					{
						turno++;
						if(turno>=nelementos-1) turno=0;
					}
					*indice=indice_usuario((*u),(*jm)[turno].nombre);
					system("cls");
					printf(" |%s|  Vida: %d/100 Escudo: %d/100 \n\n",(*jm)[turno].nombre,(*u)[(*indice)].vida,(*u)[(*indice)].escudo);
					printf(" Acciones: %d/%d\n\n",c.n_acciones-acciones,c.n_acciones);
					printf(" Arma: ");
					if(idob==-1) {printf("Predeterminada\n\n");}
					else printf("%s\n\n",o[idob].item_ID);
					printf(" 1.Ver/Usar Mochila\n");
					printf(" 2.Usar Objeto/Disparar\n");
					printf(" 3.Mover Jugador\n");
					printf(" 4.Ver Objetos Cercanos\n");
					printf(" 5.Ver Oponentes Cercanos\n");
					printf(" 6.Ver Posicion Actual\n");
					printf(" 7.Ver posicion y limites de la tormenta\n");
					printf(" 8.Finalizar Turno\n");
					printf(" 0.Volver al menu principal\n");
					printf("\n\tOpcion: ");
					fflush(stdin);
					scanf("%d",&op);
				}while(op < 0 && op > 7);
				switch (op)
				{
					case 1:
					idob=id_objeto(o,(*m),(*u),*indice);break;
					case 2:
					if(idob!=-1)
					{ 
						printf("Objeto\n");
						if(strcmp(o[idob].tipo,"arma")==0) usar_arma(&(*u),&(*jm),&(*m),&turno,o[idob].alcance,o[idob].porcentaje_d_e,&idob,o,&acciones);
						else
						{
							printf("Accesorio\n");
							usar_objeto(&(*u),o,&(*m),idob,*indice);
						}
					}
					else usar_arma(&(*u),&(*jm),&(*m),&turno,c.dist_fisico,5,&idob,o,&acciones);
					system("pause");
					break;
					case 3: 
					movimiento(&(*jm),turno,c,&acciones);
					break;
					case 4: 
					mostrar_objetos_cercanos(&(*jm),&turno,c,&(*m),(*u),*indice,&acciones);
					break;
					case 5:
					if(idob!=-1){mostrar_jugadores_cercanos(*jm,turno,o[idob].alcance);}
					else mostrar_jugadores_cercanos(*jm,turno,c.dist_fisico);
					system("pause");
					break;
					case 6: 
					printf("\n Posicion: (%d,%d)\n",(*jm)[turno].posx,(*jm)[turno].posy);system("pause");break;
					case 7: 
					printf(" Centro: %i/%i\n",(*t)[tormenta].x,(*t)[tormenta].y);
					printf(" Norte: %i/%i",(*t)[tormenta].x,(*t)[tormenta].y+((*t)[tormenta].diametro/2));	
					printf(" Sur: %i/%i",(*t)[tormenta].x,(*t)[tormenta].y-((*t)[tormenta].diametro/2));
					printf(" Este: %i/%i",(*t)[tormenta].x+((*t)[tormenta].diametro/2),(*t)[tormenta].y);
					printf(" Oeste: %i/%i\n",(*t)[tormenta].x-((*t)[tormenta].diametro/2),(*t)[tormenta].y);
					system("pause");
					break;
					case 8: 
					acciones=c.n_acciones;break;
				}

				if(acciones>=c.n_acciones)
				{
					idob = -1;
					acciones = 0;
					do{
						turno++;
						if(turno>=nelementos-1) turno=0;
					}while(strcmp((*jm)[turno].tipo,"Jugador")!=0);
					if(turno > nelementos)turno = 0;
					cont++;
					if(n_jugadores(*jm)==cont){
						cont=0;
						turno_partida++;
					}
				}

				if(n_jugadores(*jm)==1) 
				{
					terminar_partida_jugadores(&(*u),&(*jm),c);
					op=0;
				}
			} while (op != 0);
			guardar_mapa((*jm),turno);
		}
		else if(strcmp((*u)[*indice].estado,"GO")==0)
		{
			printf("Has muerto\n");
			printf("F\n");
			system("pause");
			strcpy((*u)[*indice].estado,"ON");
			op=0;//Apaño
		}
		else
		{
			if(strcmp((*u)[*indice].perfil,"JGD")==0)
			{
				op = op_usuario_partida(*u,c);
			}
			else if(strcmp((*u)[*indice].perfil,"ADM")==0)
			{
				op = op_admin_partida((*u),c);
			}
			switch(op)
			{
				case 1: strcpy((*u)[*indice].estado,"EE");break;
				case 2: strcpy((*u)[*indice].estado,"ON");break;
				case 3: 
				if(njugadores_EE(*u) >= c.min_jugadores)
				{
					inicializar_vida_escudo(&(*u));
					generar_mapa(&(*jm),&(*u),o,c);
					generar_tormentas(&(*t),c);
					for(i=0;i<nusuarios;i++)
					{
						(*u)[i].pjugadas++;
					}
					turno_partida=-1;
				}
				else
				{ 
					printf(" Minimo de jugadores no superado\n");
					system(" pause");
				}
				break;
			}
		}
	}while (op!=0);
}

//cabecera: int op_usuario_partida(usuario *u,configuracion c) 
//precondicion: recibe el vector de estructuras usuario y la estructura configuracion
//postcondicion: muestra las opciones del usuario en partida
int op_usuario_partida(usuario *u,configuracion c)
{
	int r;
	do{
		system("cls");
		printf("\t ||JUGADORES ESPERA||\n\n");
		jugadores_espera(u,c);
		printf(" 1.Unirse\n");
		printf(" 2.Salir lista\n");
		printf(" 0.Salir\n");
		printf("\t Opcion: ");
		scanf("%d",&r);
	}while (r<0&&r>2);
	return r;
}

//cabecera: int op_admin_partida(usuario *u,configuracion c)
//precondicion: recibe el vector de estructuras usuario y la estructura configuracion
//postcondicion: muestra las opciones del administrador en partida
int op_admin_partida(usuario *u,configuracion c)
{
	int r;
	do
	{
		system("cls");
		printf("||JUGADORES ESPERA||\n");
		jugadores_espera(u,c);
		printf(" 1.Unirse\n");
		printf(" 2.Salir lista\n");
		printf(" 3.Iniciar Partida\n");
		printf(" 0.Salir\n");
		printf("\t Opcion: ");
		scanf("%d",&r);
	}while (r<0&&r>4);
	return r;
}

//cabecera: void jugadores_espera(usuario *u, configuracion c)
//precondicion: recibe el vector de estructuras usuario y la estructura configuracion
//postcondicion: devuelve la lista de jugadores en espera
void jugadores_espera(usuario *u, configuracion c)
{
	int i;
	int n=0;
	for (i=0;i<nusuarios;i++)
	{
		if(strcmp(u[i].estado,"EE")==0) 
		{
			printf(" -%s\n",u[i].nick);
			n++;
		}
	}
	printf("\n (%d/%d) Jugadores (Minimo) unidos a la partida\n\n",njugadores_EE(u),c.min_jugadores);
}

//cabecera: void terminar_partida_jugadores(usuario **u,Elemento **jm)
//precondicion: recibe los vectores de estructuras por referencia usuario y Elemento
//postcondicion: inicializa los datos de los usuarios y del ganador y borra el mapa
void terminar_partida_jugadores(usuario **u,Elemento **jm,configuracion c)
{
	int i;
	for(i=0;i<nusuarios;i++)
	{
		if(estar_partida(&(*u),(*jm),i)==0) 
		{
			printf(" %s, has ganado",(*u)[i].nick);
			system("pause");
			(*u)[i].pganadas++;
			if((*u)[i].pganadas==c.partidas_nivel)(*u)[i].nivel++;
		}
	}
	for (i=0;i<nusuarios;i++)
	{
		if(strcmp((*u)[i].estado,"EJ")==0||strcmp((*u)[i].estado,"GO")==0) strcpy((*u)[i].estado,"ON");
	}
	inicializar_vida_escudo(&(*u));
	borrar_mapa(&(*jm));
}

//cabecera: void movimiento(Elemento **jm,int indice,configuracion c,int *accion)
//precondicion: recibe por referencia el vector de estructuras elemente,la estructura configuracion y los enteros indice y accion, este ultimo por referencia
//postcondicion: mueve al jugador si asi lo requiere
void movimiento(Elemento **jm,int indice,configuracion c,int *accion)
{
	int op;
	do
	{
		printf("|MOVIMIENTOS|\n");
		printf(" 1.Arriba\n");
		printf(" 2.Abajo\n");
		printf(" 3.Izquierda\n");
		printf(" 4.Derecha\n");
		printf(" 0.Salir\n");
		printf("\n Opcion: ");
		scanf("%d",&op);
	} while (op < 0 && op > 4);
	switch (op)
	{
	case 1: 
	(*jm)[indice].posx += c.dist_paso;
	(*accion)++;
	break;
	case 2: 
	(*jm)[indice].posx -= c.dist_paso;
	(*accion)++;
	break;
	case 3: 
	(*jm)[indice].posy -= c.dist_paso;
	(*accion)++;
	break;
	case 4: 
	(*jm)[indice].posy += c.dist_paso;
	(*accion)++;
	break;
	}
}

//cabecera: int id_objeto(objetos *o,mochila *m,usuario *u,int indice)
//precondicion: recibe los vectores de estruturas objetos,mochila y usuario y el indice
//postcondicion: devuelve el id del objeto si existe o -1 si no
int id_objeto(objetos *o,mochila *m,usuario *u,int indice)
{
	int i,id = usarMochila(m,indice,u);
	for(i=0;i<nobjetos;i++)
	{
		if(strcmp(o[i].item_ID,m[id].idobj)==0) return i;
	}
	return -1;
}

//cabecera: void usar_arma(usuario **u,Elemento **jm,mochila **m,int *turno,int alcance,int danio,int *ido,objetos *o,int *accion)
//precondicion: recive los vectores de estructura por referencia usuario,Elemente,mochila,por valor objetos y enteros por referencia turno, ido y accion y por valor danio y alcance
//postcondicion: devuelve las estructuras inicializadas si el usuario usa un arma contra alguien
void usar_arma(usuario **u,Elemento **jm,mochila **m,int *turno,int alcance,int danio,int *ido,objetos *o,int *accion)
{
	int op,i,cant,dist,indice,indicemapa,r;
	char nick[50];
	do{
		mostrar_jugadores_cercanos((*jm),*turno,alcance);
		printf(" 1.Elegir objetivo\n");
		printf(" 0.Salir\n");
		printf("	Opcion: ");
		fflush(stdin);
		scanf("%d",&op);
	}while(op!=0 && op!=1);
	if(op==1)
	{
		printf(" Introduce su id: ");
		fflush(stdin);
		fgets(nick,50,stdin);
		fixstring(nick);
		for(i=0;i<nelementos;i++)
		{
			if(strcmp((*jm)[i].tipo,"Jugador")==0)
			{
				if(strcmp(nick,(*jm)[i].nombre)==0) r=1;
			}
		}
	
		if(r==0) printf(" Usuario no encontrado\n");
		if(r==1) 
		{
			indice=indice_usuario((*u),nick);
			(*u)[indice].vida-=danio;
			(*accion)++;
			if(*ido!=-1)
			{
				r=0;
				for(i=0;i<nmochilas;i++)
				{
					if(strcmp((*jm)[*turno].nombre,(*m)[i].idusu)==0)
					{
						if(strcmp(o[*ido].item_ID,(*m)[i].idobj)==0)
						{ 
								cant=(*m)[i].unidades;
						}
					}
				}
				if(cant==1) *ido=-1;
			}
			if((*u)[indice].vida<=0)
			{
				strcpy((*u)[indice].estado,"GO");
				borrar_elemento(&(*jm),indicemapa);
				turno_jugador((*jm),(*u)[indice].nick,indicemapa);
			}
		}
	}
}

//cabecera: void mostrar_jugadores_cercanos(Elemento *jm,int idm,int alcance)
//precondicion: recibe el vector de estructuras Elemento y los enteros idm y alcance
//postcondicion: muestra los jugadores al alcance del arma actual
void mostrar_jugadores_cercanos(Elemento *jm,int idm,int alcance)
{
	int i;
	for(i=0;i<nelementos;i++)
	{
		if(strcmp(jm[i].tipo,"Jugador")==0)
		{
			if(i!=idm)
			{
				if(distancia(jm[idm].posx,jm[idm].posy,jm[i].posx,jm[i].posy) <= alcance) printf(" -%s (%i,%i)\n",jm[i].nombre,jm[i].posx,jm[i].posy);
			}
		}
	}
}

//cabecera: void mostrar_objetos_cercanos(Elemento **jm,int *idm,configuracion c,mochila **m,usuario *u,int indice,int *accion)
//precondicion: recibe los vectores de estructuras por referencia Elemento y mochila, el entero idm y accion por referencia, indice por valor y la estructura configuracion
//postcondicion: muestra los objetos al alcance a recoger y los recoge si el usuario lo desea
void mostrar_objetos_cercanos(Elemento **jm,int *idm,configuracion c,mochila **m,usuario *u,int indice,int *accion)
{
	int i,op,id=-1,f=0;
	char nombre[50];
	
		do{
			for(i=0;i<nelementos;i++)
			{	
				printf(" %s %s %i\n",(*jm)[i].tipo,(*jm)[i].nombre,strcmp((*jm)[i].tipo,"Objeto"));
				if(strcmp((*jm)[i].tipo,"Jugador")!=0);
				{
					if(distancia((*jm)[*idm].posx,(*jm)[*idm].posy,(*jm)[i].posx,(*jm)[i].posy)<=c.dist_recoger) printf(" -%s (%i,%i)\n",(*jm)[i].nombre,(*jm)[i].posx,(*jm)[i].posy);
				}
			}
			printf(" 1.Recoger Objeto\n");
			printf(" 0.Salir\n");
			printf("\n Opcion: ");
			scanf("%d",&op);
		}while(op != 0 && op !=1);
		if(op == 1)
		{
			printf(" Introduce el id del objeto: ");
			fflush(stdin);
			fgets(nombre,50,stdin);
			fixstring(nombre);
			i=0;
			do
			{
				if(strcmp((*jm)[i].tipo,"Objeto")==0)
				{
					if(strcmp(nombre,(*jm)[i].nombre)==0)
					{ 
						if(distancia((*jm)[*idm].posx,(*jm)[*idm].posy,(*jm)[i].posx,(*jm)[i].posy)<=c.dist_recoger) 
						{
							id = i;
							f = 1;
						} 
					}
				}
				i++;
			}while(f!=1 && i < nelementos);
			if(f!=1) {printf(" El objeto no existe o esta fuera de rango");}
			else
			{
				printf("Objeto (%s) recogido\n",(*jm)[id].nombre);
				(*accion)++;
				guardar_mochila(&(*m),(*jm)[id].nombre,indice,u,c);
				borrar_elemento(&(*jm),id);
				turno_jugador((*jm),u[indice].nick,*idm);
			}
		}

}

//cabecera: int turno_jugador(Elemento *jm,char *nick,int indice)
//precondicion: recibe el vector de estructuras Elemento,la cadena nick del usuario y el indice
//postcondicion: devuelve el turno del jugador si lo encuentra e indice si no
int turno_jugador(Elemento *jm,char *nick,int indice)
{
	int i;
	do
	{
		if(strcmp(nick,jm[i].nombre)==0) 
		{
			return i;
		}
		i++;
	}while(i<nelementos);
	return indice;
}

//cabecera: void usar_objeto (usuario **u,objetos *o,mochila **m,int ido,int indice)
//precondicion: recibe el vector de estructuras usuario y mochila por referencia, objetos por valor, y los enteros ido e indice
//postcondicion: usa el objeto siendo estos accesorios para curar o aumentar escudo
void usar_objeto (usuario **u,objetos *o,mochila **m,int ido,int indice)
{
	int r=0,i=0,cant;
	do{
		if(strcmp(o[ido].item_ID,(*m)[i].idobj)==0) 
		{
			if(strcmp((*u)[indice].nombre,(*m)[i].idusu)==0) r=1;
			else i++;
		}
		else i++;
	}while(r!=1 && i < nmochilas);
	if(r==1)
	{
		if(strcmp(o[ido].descripcion,"escudo")==0)
		{
			printf(" Escudo %i ->",(*u)[indice].escudo);
			(*u)[indice].escudo+=o[ido].porcentaje_d_e;
			if((*u)[indice].escudo > 100) (*u)[indice].escudo=100;
			printf(" %i\n",(*u)[indice].escudo);
			
		}
		else
		{
			printf(" Vida %i ->",(*u)[indice].vida);
			(*u)[indice].vida+=o[ido].porcentaje_d_e;
			if((*u)[indice].vida > 100) (*u)[indice].vida=100;
			printf(" %i\n",(*u)[indice].vida);
		}
		for(i=0;i<nmochilas;i++)
		{
			if(strcmp((*u)[indice].nombre,(*m)[i].idusu)==0)
			{
				if(strcmp(o[ido].item_ID,(*m)[i].idobj)==0)
				{
					cant=(*m)[i].unidades;
					eliminarobjeto(&(*m),i);
				}
			}
		}
		if(cant==1) ido=-1;
	}
}

//cabecera: int estar_partida(usuario **u,Elemento *jm,int indice)
//precondicion: recibe el vector de estructuras por referencia usuario y por valor Elemento y el entero indice
//postcondicion: devuelve 0 si el jugador esta en la partida y -1 si no
int estar_partida(usuario **u,Elemento *jm,int indice)
{
	int i;
	for(i=0;i<nelementos;i++)
	{
		if(strcmp((*u)[indice].nick,jm[i].nombre)==0)
		{
			strcpy((*u)[indice].estado,"EJ");
			return 0;
		}
	}
	return -1;
}