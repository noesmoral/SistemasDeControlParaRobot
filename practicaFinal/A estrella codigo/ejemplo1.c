#include <stdio.h>
#include "stdlib.h"
#include <math.h>
#include <sys/time.h>
#include <libplayerc/playerc.h>

//Estructura
struct nodo{
	int x;
	int y;
	int ocupado;
	int peso;
	int g;
	int h;
	int visitado;
	int padre[1][2];
	int principio;
	int fin;
	int analizado;
};

//variables globales
struct nodo almacen[38][41];
int Xsig = 0;
int Ysig = 0;

//los visitados guardalos en una lista para operar sobre ellos
void alrededor(int xRef, int yRef) {
	int x = xRef;
	int y = yRef;
	printf("ALREDEDOR x %d e y %d\n", x, y);
	if (x + 1<=40) {	//DERECHA
		if (almacen[y][x + 1].visitado == 0 && almacen[y][x + 1].ocupado == 0) {
			almacen[y][x + 1].g = 10;
			almacen[y][x + 1].peso = 10 + almacen[y][x + 1].h;
			almacen[y][x + 1].padre[0][0] = yRef;
			almacen[y][x + 1].padre[0][1] = xRef;
			almacen[y][x + 1].visitado = 1;
		}
	}
	if (x + 1 <= 40 && y - 1 >= 0) {	//DERECHA TOP
		if (almacen[y - 1][x + 1].visitado == 0 && almacen[y - 1][x + 1].ocupado == 0) {
			almacen[y - 1][x + 1].g = 14;
			almacen[y - 1][x + 1].peso = 14 + almacen[y - 1][x + 1].h;
			almacen[y - 1][x + 1].padre[0][0] = yRef;
			almacen[y - 1][x + 1].padre[0][1] = xRef;
			almacen[y - 1][x + 1].visitado = 1;
		}
	}
	if (y - 1 >= 0) {	//TOP
		if (almacen[y - 1][x].visitado == 0 && almacen[y - 1][x].ocupado == 0) {
			almacen[y - 1][x].g = 10;
			almacen[y - 1][x].peso = 10 + almacen[y - 1][x].h;
			almacen[y - 1][x].padre[0][0] = yRef;
			almacen[y - 1][x].padre[0][1] = xRef;
			almacen[y - 1][x].visitado = 1;
		}
	}
	if (y - 1 >= 0 && x - 1 >= 0) {	//IZQUIERDA TOP
		if (almacen[y - 1][x - 1].visitado == 0 && almacen[y - 1][x - 1].ocupado == 0) {
			almacen[y - 1][x - 1].g = 14;
			almacen[y - 1][x - 1].peso = 14 + almacen[y - 1][x - 1].h;
			almacen[y - 1][x - 1].padre[0][0] = yRef;
			almacen[y - 1][x - 1].padre[0][1] = xRef;
			almacen[y - 1][x - 1].visitado = 1;
		}
	}
	if (x - 1 >= 0) {	//IZQUIERDA
		if (almacen[y][x - 1].visitado == 0 && almacen[y][x - 1].ocupado == 0) {
			almacen[y][x - 1].g = 10;
			almacen[y][x - 1].peso = 10 + almacen[y][x - 1].h;
			almacen[y][x - 1].padre[0][0] = yRef;
			almacen[y][x - 1].padre[0][1] = xRef;
			almacen[y][x - 1].visitado = 1;
		}
	}
	if (y + 1 <= 37 && x - 1 >= 0) {	//IZQUIERDA BOT
		if (almacen[y + 1][x - 1].visitado == 0 && almacen[y + 1][x - 1].ocupado == 0) {
			almacen[y + 1][x - 1].g = 14;
			almacen[y + 1][x - 1].peso = 14 + almacen[y +1][x - 1].h;
			almacen[y + 1][x - 1].padre[0][0] = yRef;
			almacen[y + 1][x - 1].padre[0][1] = xRef;
			almacen[y + 1][x - 1].visitado = 1;
		}
	}
	if (y + 1 <= 37) {	//BOT
		if (almacen[y + 1][x].visitado == 0 && almacen[y + 1][x].ocupado == 0) {
			almacen[y + 1][x].g = 10;
			almacen[y + 1][x].peso = 10 + almacen[y + 1][x].h;
			almacen[y + 1][x].padre[0][0] = yRef;
			almacen[y + 1][x].padre[0][1] = xRef;
			almacen[y + 1][x].visitado = 1;
		}
	}
	if (y + 1 <= 37 && x + 1 <= 40) {	//DERECHA BOT
		if (almacen[y + 1][x + 1].visitado == 0 && almacen[y + 1][x + 1].ocupado == 0) {
			almacen[y + 1][x + 1].g = 14;
			almacen[y + 1][x + 1].peso = 14 + almacen[y + 1][x + 1].h;
			almacen[y + 1][x + 1].padre[0][0] = y;
			almacen[y + 1][x + 1].padre[0][1] = xRef;
			almacen[y + 1][x + 1].visitado = 1;
		}
	}
}

//busqueda a fuerza bruta en realizad habra que buscar sobre los visitados //V2 guardarlos en una lista y buscar sobre ellos
void  buscar() {
	int balanza = 99998;
	int resultado[1][2];
	int ai=0;
	int aj=0;
	for (ai = 0; ai < 38; ai++)
	{
		for (aj = 0; aj < 41; aj++)
		{
			if (almacen[ai][aj].visitado==1  && almacen[ai][aj].peso<=balanza && almacen[ai][aj].analizado == 0)
			{
				//almacen[Ysig][Xsig].analizado = 1;
				balanza = almacen[ai][aj].peso;
				Ysig = ai;
				Xsig = aj;
			}
		}
	}
	almacen[Ysig][Xsig].analizado = 1;
}

//funcion pasar mapa a robot
int robotMapaX(double valor) {
	int medio = 20;
	int aux;
	aux=(int)valor;
	if (valor < 0) {
		return (int)(medio - (fabs(aux) * 2));
	}
	else {
		return (int)(medio + (fabs(aux) * 2));
	}
}
int robotMapaY(double valor) {
	int medio = 18;
	int aux;
	aux=(int)valor;
	if (valor < 0) {
		return (int)(medio + (fabs(aux) * 2));
	}
	else {
		return (int)(medio - (fabs(aux) * 2));
	}
}

//funcion pasar robot a mapa
double mapaRobotX(int valor) {
	return -10 + (valor*0.5);
}
double mapaRobotY(int valor) {
	return 9 - (valor*0.5);
}

//programa principal
int main(int argc, const char **argv)
{
	int i, p, j, zzz,q;  
  	playerc_client_t *client;
  	playerc_position2d_t *position2d;
  	playerc_laser_t *laser;
  	player_pose2d_t position2d_target;
  	double vel;

  	//pintar
	playerc_graphics2d_t *graficos;
	player_point_2d_t *puntos;  
	player_color_t colorR;
	colorR.red=255; colorR.green=0; colorR.blue=0;

  	// Create a client and connect it to the server.
  	client = playerc_client_create(NULL, "localhost", 6665);
  	if (playerc_client_connect(client) != 0){
     	fprintf(stderr, "error client: %s\n", playerc_error_str());
		return -1;
    	}

	// Create and subscribe to a position2d device.
	position2d = playerc_position2d_create(client,1);
	if (playerc_position2d_subscribe(position2d, PLAYER_OPEN_MODE) != 0){
	      fprintf(stderr, "error robot1: %s\n", playerc_error_str());
	      return -1;
	}

	// Create and subscribe to a graphics device
  	graficos = playerc_graphics2d_create(client, 0);
  	if (playerc_graphics2d_subscribe(graficos, PLAYER_OPEN_MODE) != 0){
      		fprintf(stderr, "error: %s\n", playerc_error_str());
      		return -1;
    	}

  	playerc_position2d_set_odom(position2d,0.0,0.0,0.0);

  	// Create and subscribe to a laser device
  	laser=playerc_laser_create(client,0);
  	if (playerc_laser_subscribe(laser,PLAYER_OPEN_MODE)){
		fprintf(stderr, "error laser: %s\n", playerc_error_str());
		return -1;
 	}

  	// Enable motors 
  	playerc_position2d_enable(position2d,1);
  	playerc_client_read(client);

  	//inicializamos los valores
  	q=1;

	//variables
	int     xini = 20;
	int	yini = 18;
	int	xdes = 6;
	int	ydes = 18;
	int auxY, auxX;
	//mapa
	int mapa[37][41] = {{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }};

	

	//se crea e inicializa la estructura
	printf("Empezamos\n");
	int bi=0;
	int bj=0;
	for (bi = 0; bi < 38; bi++)
	{
		for (bj = 0; bj < 41; bj++)
		{
			almacen[bi][bj].x = bj;
			almacen[bi][bj].y = bi;
			almacen[bi][bj].ocupado = mapa[bi][bj];
			almacen[bi][bj].peso = 99999;
			almacen[bi][bj].g = 0;
			almacen[bi][bj].h = 0;
			almacen[bi][bj].padre[0][0] = bi;
			almacen[bi][bj].padre[0][1] = bj;
			almacen[bi][bj].visitado = 0;
			almacen[bi][bj].analizado = 0;
			almacen[bi][bj].principio = 0;
			almacen[bi][bj].fin = 0;
		}
	}
	printf("Fin inicializacion\n");
	double prueba;
	double prueba1;
	double prueba2;
	double prueba3;
	//se pide inicio y fin y se meten en el almacen
	printf("Inserte la posicion X de comienzo del robot:");
	scanf("%lf", &prueba);
	while(getchar()!='\n');
	xini=robotMapaX(prueba);
	printf("Inserte la posicion Y de comienzo del robot:");
	scanf("%lf", &prueba1);
	while(getchar()!='\n');
	yini=robotMapaY(prueba1);
	//ponemos lo valores de la casilla de inicio
	almacen[yini][xini].principio = 1;
	almacen[yini][xini].visitado = 1;
	almacen[yini][xini].analizado = 1;
	printf("\nInserte la posicion de destino del robot:");
	scanf("%lf", &prueba2);
	while(getchar()!='\n');
	xdes=robotMapaX(prueba2);
	printf("\nInserte la posicion de destino del robot:");
	scanf("%lf", &prueba3);
	while(getchar()!='\n');
	ydes=robotMapaY(prueba3);
	//ponemmos los valores del final
	almacen[ydes][xdes].fin = 1;

	printf("Inicio calculo h\n");
	//bucle calcular la distancia h de cada uno a fin
	int ci=0;
	int cj=0;
	for (ci = 0; ci < 38; ci++)
	{
		for (cj = 0; cj < 41; cj++)
		{
			auxX = cj - xdes;
			auxY = ci - ydes;
			almacen[ci][cj].h = (abs(auxX)+abs(auxY))*6;
		}
	}
	printf("Fin calculo h\n");

	printf("Inicio calculo alrededor de inicial\n");
	//empezar por el primero llamando a la funcion alrededor //falla ya que crea uno nuevo que no vale a la derecha del todo uno mas
	alrededor(xini,yini);
	printf("Fin calculo alrededor de inicial\n");

	printf("Inicio busqueda del camino\n");
	//funcion buscar camino
	while (almacen[ydes][xdes].visitado == 0){
		//buscar el menor
		buscar();
		//propagar desde el menor
		//mostrar recorrido por pasos
		/*int pu,pa;
		for (pu = 0; pu < 38; pu++) {
			for (pa = 0; pa < 41; pa++)
			{
				printf(" %d ", almacen[pu][pa].visitado);
			}
			printf("\n");
		}*/
		alrededor(Xsig, Ysig);
	}
	printf("Fin busqueda del camino\n");

	//motrar mapas resultado
	int di=0;
	int dj=0;
	for (di = 0; di < 38; di++){
		for (dj = 0; dj < 41; dj++)
		{
			printf(" %d ", almacen[di][dj].peso);
		}
		printf("\n");
	}
	int ei=0;
	int ej=0;
	printf("\n");
	for (ei = 0; ei < 38; ei++) {
		for (ej = 0; ej < 41; ej++)
		{
			printf(" %d ", almacen[ei][ej].visitado);
		}
		printf("\n");
	}
	int fi=0;
	int fj=0;
	printf("\n");
	for (fi = 0; fi < 38; fi++) {
		for (fj = 0; fj < 41; fj++)
		{
			printf(" %d ", almacen[fi][fj].ocupado);
		}
		printf("\n");
	}

	//recolocar el camino desde el final al inicio y sacar en orden
	printf("Inicio recuento\n");
	int contTotal = 2;
	printf("Posicion de destion y es  %d\n", almacen[ydes][xdes].y);
	printf("Posicion de destion x es  %d\n", almacen[ydes][xdes].x);
	int YRetroceso = almacen[ydes][xdes].padre[0][0];
	int XRetroceso = almacen[ydes][xdes].padre[0][1];
	int XRetrocesoProx, YRetrocesoProx;
	printf("Mi padre y es  %d\n", YRetroceso);
	printf("Mi padre x es  %d\n", XRetroceso);
	printf("Peso de mi padre es  %d\n", almacen[YRetroceso][XRetroceso].peso);
	do{
		YRetrocesoProx = almacen[YRetroceso][XRetroceso].padre[0][0];
		XRetrocesoProx = almacen[YRetroceso][XRetroceso].padre[0][1];
		printf("Mi padre y es  %d\n", YRetrocesoProx);
		printf("Mi padre x es  %d\n", XRetrocesoProx);
		printf("Peso de mi padre es  %d\n", almacen[YRetroceso][XRetroceso].peso);
		XRetroceso = XRetrocesoProx;
		YRetroceso = YRetrocesoProx;
		contTotal = contTotal + 1;
	}while (YRetroceso != almacen[YRetroceso][XRetroceso].padre[0][0] || XRetroceso != almacen[YRetroceso][XRetroceso].padre[0][1]);
	printf("puntos totales: %d\n", contTotal);
	printf("Fin recuento\n");

	//structuras auxiliares
	int *arrayXinvertido, *arrayYinvertido;
	double *arrayX, *arrayY, *arrayA;
	arrayXinvertido=(int *)malloc(contTotal*sizeof(int));
	arrayYinvertido=(int *)malloc(contTotal*sizeof(int));
	arrayX=(double *)malloc(contTotal*sizeof(double));
	arrayY=(double *)malloc(contTotal*sizeof(double));
	arrayA=(double *)malloc(contTotal*sizeof(double));

	//meterlo en un array en orden inverso
	int contTotal1 = 0;
	arrayXinvertido[contTotal1]=almacen[ydes][xdes].x;
	arrayYinvertido[contTotal1]=almacen[ydes][xdes].y;
	contTotal1 = contTotal1 + 1;
	YRetroceso = almacen[ydes][xdes].padre[0][0];
	XRetroceso = almacen[ydes][xdes].padre[0][1];
	arrayXinvertido[contTotal1]=XRetroceso;
	arrayYinvertido[contTotal1]=YRetroceso;
	do{
		YRetrocesoProx = almacen[YRetroceso][XRetroceso].padre[0][0];
		XRetrocesoProx = almacen[YRetroceso][XRetroceso].padre[0][1];
		XRetroceso = XRetrocesoProx;
		YRetroceso = YRetrocesoProx;
		contTotal1 = contTotal1 + 1;
		arrayXinvertido[contTotal1]=XRetroceso;
		arrayYinvertido[contTotal1]=YRetroceso;
	}while (YRetroceso != almacen[YRetroceso][XRetroceso].padre[0][0] || XRetroceso != almacen[YRetroceso][XRetroceso].padre[0][1]);

	//creamos el vector para los puntos
	puntos=(player_point_2d_t *)malloc(sizeof(player_point_2d_t)*(contTotal));

	//convertir del mapa a absolutas   
	int as=0;
	int des=contTotal-1;
	int hi;
	int aux;
	printf("contador: %d\n", contTotal);
	//ordenamos los puntos del recorrido
	for( i=0 ; i<contTotal ; i++ ){
		aux=arrayXinvertido[des];
		arrayX[as]=mapaRobotX((int)aux);
		printf("puntos x: %lf\n", arrayX[as]);
		puntos[as].px=arrayX[as];
		aux=arrayYinvertido[des];
		arrayY[as]=mapaRobotY(aux);
		printf("puntos y: %lf\n", arrayY[as]);
		puntos[as].py=arrayY[as];
		arrayA[as]=3.14;	
		as=as+1;
		des=des-1;	
	}

 	  //limpiaamos los puntos anteriores
  	 playerc_graphics2d_clear(graficos);

	  //dibujamos las particulas
	  playerc_graphics2d_draw_points (graficos, puntos, contTotal);

  	//bucle del recorrido
  	for ( i=0 ; i<contTotal ; i++ ){
     		position2d_target.px = arrayX[i];
		printf("proximo x: %lf\n", arrayX[i]);
      		position2d_target.py = arrayY[i];
		printf("proximo y: %lf\n", arrayY[i]);
      		position2d_target.pa = arrayA[i];
      		
      		// Move to pose  
      		playerc_position2d_set_cmd_pose(position2d, position2d_target.px , position2d_target.py, position2d_target.pa , 1);

      		// Stop when reach the target
     		while  (sqrt(pow(position2d->px - position2d_target.px,2.0) + pow(position2d->py - position2d_target.py,2.0)) > 0.35 ){
	  		// Wait for new data from server
	  		playerc_client_read(client);
	  
	  		// Print current robot pose
	  		printf("position2d : x %f y %f th %f stall %d\n",position2d->px, position2d->py, position2d->pa, position2d->stall); 
		}
   	}

  	// Unsuscribe and Destroy
  	// position2d
  	playerc_position2d_unsubscribe(position2d); playerc_position2d_destroy(position2d);
  	// laser
   	playerc_laser_unsubscribe(laser); playerc_laser_destroy(laser); 
	// graphics2d
  	playerc_graphics2d_unsubscribe(graficos); playerc_graphics2d_destroy(graficos);
  	// client
  	playerc_client_disconnect(client); playerc_client_destroy(client);

  	// End
  	return 0;
}
