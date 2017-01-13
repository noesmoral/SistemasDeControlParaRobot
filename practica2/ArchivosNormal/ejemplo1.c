#include <stdio.h>
#include <math.h>

#include <libplayerc/playerc.h>

void DibujaCirculo(double xc, double yc, double radio, player_color_t color, playerc_graphics2d_t *graficos){
	double x[16], senos[16], cosenos[16], inc;
	int y,i;
	player_point_2d_t puntos[16];

	inc=2*3.14/16;
	x[0]=0.0;
	for(i=1;i<16;i++){
		x[i]=x[i-1]+inc;	
	}
	for(i=0;i<16;i++){
		senos[i]=sin(x[i]);
		cosenos[i]=cos(x[i]);
	}
	for(i=0;i<16;i++){
		puntos[i].px=xc+radio*cosenos[i];
		puntos[i].py=yc+radio*senos[i];
	}
	playerc_graphics2d_setcolor (graficos,color);
	playerc_graphics2d_draw_polyline (graficos,puntos,16);
}

int
main(int argc, const char **argv)
{
  //variables para calculos
  int i, p, j, zzz, dd; 
  double variacion, circuloX, circuloY;	//radio del circulo y los valores de la hypotesis de ubicacion
  //variables para manejo y utilizacion
  playerc_client_t *client;
  playerc_position2d_t *position2d;
  playerc_laser_t *laser;
  playerc_map_t *mapa;
  playerc_localize_t *localize;
  player_pose2d_t position2d_target;

  //Array con las posiciones a visitar en el recorrido por el mapa //primera posicion de -0.5
  double arrayx[57]={ -0.5,-0.5,4.5,4.5,0.5,0.5,-0.5,-0.5,-4.3,-4.3,-0.5,-0.5,-4.3,-4.3,-6,-6,-9.5,-9.5,-6,-6,-9.5,-9.5,-8.5,-8.5,-8.5,-0.5,-0.5,1,1,2.5,2.5,1,1,9.5,9.5,4.5,4,1,1,4.5,4.5,1,1,-4.5,-4.5,-3.5,-3.5,-6.5,-6.5,-5.5,-5.5,-6.5,-6.5,-3.5,-3.5,-4.5,-4.5};
  double arrayy[57]={ 0,4.5,4.5,8,8,4.5,4.5,5,5,6,6,8,8,5,5,8,8,6,6,5,5,-3.5,-3.5,-4,-3.5,-3.5,-4,-4,-8,-8,-5,-5,-4,-4,-2,-2,0,0,1,1,3,3,0,0,4,4,5,5,4,4,-2,-2,-3,-3,-2,-2,0 };
  double arraya[57]={ 3.14,1.57,0.0,1.57,3.14,-1.57,3.14,1.57,3.14,1.57,0.0,1.57,3.14,-1.57,3.14,1.57,3.14,-1.57,0.0,-1.57,3.14,-1.57,0.0,-1.57,1.57,0.0,-1.57,0.0,-1.57,0.0,1.57,3.14,1.57,0.0,1.57,3.14,1.57,3.14,1.57,0.0,1.57,3.14,-1.57,3.14,1.57,0.0,1.57,3.14,-1.57,0.0,-1.57,3.14,-1.57,0.0,1.57,3.14,1.57}; 

  //drawing
  playerc_graphics2d_t *graficos;
  player_point_2d_t *puntos;
  player_point_2d_t *circulo;
  player_color_t colorR;
  player_color_t colorG;
  player_color_t colorB;
  circulo=(player_point_2d_t *)malloc(sizeof(player_point_2d_t)*(8)); 
  colorR.red=255; colorR.green=0; colorR.blue=0;
  colorG.red=0; colorG.green=255; colorG.blue=0;
  colorB.red=0; colorB.green=0; colorB.blue=255;

  // Create a client and connect it to the server.
  client = playerc_client_create(NULL, "localhost", 6665);
  if (playerc_client_connect(client) != 0)
    {
      fprintf(stderr, "error: %s\n", playerc_error_str());
      return -1;
    }

  // Create and subscrie map
  mapa = playerc_map_create(client, 0);
  if (playerc_map_subscribe(mapa, PLAYER_OPEN_MODE) != 0)
    {
      fprintf(stderr, "error: %s\n", playerc_error_str());
      return -1;
    }

  // Create and subscribe to a position2d device.
  position2d = playerc_position2d_create(client, 1);
  if (playerc_position2d_subscribe(position2d, PLAYER_OPEN_MODE) != 0)
    {
      fprintf(stderr, "error: %s\n", playerc_error_str());
      return -1;
    }

  // Create and subscribe to a laser device
  laser=playerc_laser_create(client,0);
  if (playerc_laser_subscribe(laser,PLAYER_OPEN_MODE))
	return -1;
  
  // Create and subscribe to a localize device
  localize = playerc_localize_create(client, 0);
  if (playerc_localize_subscribe(localize, PLAYER_OPEN_MODE) != 0)
    {
      fprintf(stderr, "error: %s\n", playerc_error_str());
      return -1;
    }

  // Create and subscribe to a graphics device
  graficos = playerc_graphics2d_create(client, 0);
  if (playerc_graphics2d_subscribe(graficos, PLAYER_OPEN_MODE) != 0)
    {
      fprintf(stderr, "error: %s\n", playerc_error_str());
      return -1;
    }

  // Fixing initial position
  playerc_position2d_set_odom(position2d,0.0,0.0,0.0);

  // Clear screen
  playerc_graphics2d_clear(graficos); 

  // Enable motors 
  playerc_position2d_enable(position2d,1);
  playerc_client_read(client);
  
  playerc_map_get_map(mapa);	//obtiene informacion del dispositivo

  for ( i=0 ; i<57 ; i++ )
    {
      position2d_target.px = arrayx[i];
      position2d_target.py = arrayy[i];
      position2d_target.pa = arraya[i];
      
      // Move to pose  
      playerc_position2d_set_cmd_pose(position2d, position2d_target.px , position2d_target.py, position2d_target.pa , 1);

      // Stop when reach the target
      while  (sqrt(pow(position2d->px - position2d_target.px,2.0) + pow(position2d->py - position2d_target.py,2.0)) > 0.5 )
	{
	  // Wait for new data from server
	  playerc_client_read(client);
	  
	  // Print current robot pose
	  //printf("position2d : x %f y %f th %f stall %d\n",position2d->px, position2d->py, position2d->pa, position2d->stall); 

	  

	  //leer particulas
	  playerc_localize_get_particles(localize);

	  //declaracion dinamica para los contenedores en funcion del numero de particulas
	  printf("numero de particulas %d\n",localize->num_particles);
	  puntos=(player_point_2d_t *)malloc(sizeof(player_point_2d_t)*(localize->num_particles));

	  //almacenamos y representamos las particulas
	  playerc_graphics2d_setcolor (graficos, colorR); 
	  for (dd=0 ; dd<localize->num_particles-1 ; dd++){
		puntos[dd].px=localize->particles[dd].pose[0];
		puntos[dd].py=localize->particles[dd].pose[1];
		//posible añadido peso particula
	  }	
		
 	  //limpiaamos los puntos anteriores
  	 playerc_graphics2d_clear(graficos);

	  //dibujamos las particulas
	  playerc_graphics2d_draw_points (graficos, puntos, localize->num_particles);
	
	  //obtenemos informacion para dibujar el circulo (REVISAR FALLA LA OBTENCIUON DE LA HYPOTESIS da 0.0 0.0 SI NO ES ASI REPLANTEAR)
	  variacion=fabs(localize->variance);	
	  printf("valor de la variacion %f\n",localize->variance);
	  circuloX=localize->mean[0];
	  circuloY=localize->mean[1];	  	
	 
	  //dibujamos el Circulo varianza
	  DibujaCirculo(circuloX, circuloY, variacion*10, colorG, graficos);

	  //posicion mas probable
	  DibujaCirculo(circuloX, circuloY, 0.3, colorB, graficos);
	}
    }


  // Unsuscribe and Destroy

  // map
  playerc_map_unsubscribe(mapa); playerc_map_destroy(mapa);	
  // location
  playerc_localize_unsubscribe(localize); playerc_localize_destroy(localize);
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
