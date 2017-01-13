#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <libplayerc/playerc.h>

//main principal
int main(int argc, const char **argv){
  //variables para calculos
  int i, p, j, zzz, dd; 
  double variacion, circuloX, circuloY;	//radio del circulo y los valores de la hypotesis de ubicacion

  //variables para manejo y utilizacion
  playerc_client_t *client;
  playerc_planner_t *planned;
  playerc_position2d_t *position2d;
  playerc_laser_t *laser;
  playerc_map_t *mapa;
  playerc_localize_t *localize;
  player_pose2d_t position2d_target;

  //valores posicion final
  double PFX, PFY, PFG;

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
  if (playerc_client_connect(client) != 0){
      fprintf(stderr, "error: %s\n", playerc_error_str());
      return -1;
  }

  // Create and subscrie map
  mapa = playerc_map_create(client, 0);
  if (playerc_map_subscribe(mapa, PLAYER_OPEN_MODE) != 0){
      fprintf(stderr, "error: %s\n", playerc_error_str());
      return -1;
  }

  // Create and subscribe to a position2d device.
  position2d = playerc_position2d_create(client, 1);
  if (playerc_position2d_subscribe(position2d, PLAYER_OPEN_MODE) != 0){
      fprintf(stderr, "error: %s\n", playerc_error_str());
      return -1;
  }

  // Create and subscribe to a laser device
  laser=playerc_laser_create(client,0);
  if (playerc_laser_subscribe(laser,PLAYER_OPEN_MODE)){
	return -1;
  }

  // Create and subscribe to a localize device
  localize = playerc_localize_create(client, 0);
  if (playerc_localize_subscribe(localize, PLAYER_OPEN_MODE) != 0){
      fprintf(stderr, "error: %s\n", playerc_error_str());
      return -1;
  }

  // Create and subscribe to a graphics device
  graficos = playerc_graphics2d_create(client, 0);
  if (playerc_graphics2d_subscribe(graficos, PLAYER_OPEN_MODE) != 0){
      fprintf(stderr, "error: %s\n", playerc_error_str());
      return -1;
  }

  // Create and subscribe to a graphics device
  planned = playerc_planner_create(client, 0);
  if (playerc_planner_subscribe(planned, PLAYER_OPEN_MODE) != 0){
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

  //planner
  playerc_planner_enable(planned,1);
  printf("\nInserte posicion X:"); 
  scanf("%lf",&PFX);
  printf("  a insertado %lf",PFX); 
  printf("\nInserte posicion Y:"); 
  scanf("%lf",&PFY);
  printf("  a insertado %lf",PFY); 
  printf("\nInserte posicion º:"); 
  scanf("%lf",&PFG);
  printf("  a insertado %lf",PFG); 
  playerc_planner_set_cmd_pose(planned,PFX,PFY,PFG);
  
  //hasta que no se localice no empieza a planificar creo ya que las primeras iteraciones no localiza nada
  playerc_planner_get_waypoints(planned);
  
  //Informacion planner
  printf("\nNumero de puntos en la ruta: %d\n",planned->waypoint_count); 
  printf("\nValor Inicio X: %f\n",planned->px); 
  printf("\nValor Inicio Y: %f\n",planned->py);
  printf("\nValor Inicio º: %f\n",planned->pa);
  printf("\nValor Destino X: %f\n",planned->gx); 
  printf("\nValor Destino Y: %f\n",planned->gy);
  printf("\nValor Destino º: %f\n",planned->ga);
  printf("\nValor actual dentro del waypoint X: %f\n",planned->wx); 
  printf("\nValor actual dentro del waypoint Y: %f\n",planned->wy);
  printf("\nValor actual dentro del waypoint º: %f\n",planned->wa); 
  

  while(sqrt(pow(position2d->px - PFX,2.0) + pow(position2d->py - PFY,2.0)) > 0.5){
	  // Wait for new data from server
	  playerc_client_read(client);
	  
	  // Print current robot pose
	  printf("position2d : x %f y %f th %f stall %d\n",position2d->px, position2d->py, position2d->pa, position2d->stall); 

	  //leer particulas y dibujar 
	  playerc_localize_get_particles(localize);

	  //PLANNER
          //playerc_planner_get_waypoints(planned);
          printf("\nNumero de puntos en la ruta: %d\n",planned->waypoint_count); 
 	  printf("\nValor Inicio X: %f\n",planned->px); 
  	  printf("\nValor Inicio Y: %f\n",planned->py);
 	  printf("\nValor Inicio º: %f\n",planned->pa);
  	  printf("\nValor Destino X: %f\n",planned->gx); 
  	  printf("\nValor Destino Y: %f\n",planned->gy);
  	  printf("\nValor Destino º: %f\n",planned->ga);
  	  printf("\nValor actual dentro del waypoint X: %f\n",planned->wx); 
  	  printf("\nValor actual dentro del waypoint Y: %f\n",planned->wy);
  	  printf("\nValor actual dentro del waypoint º: %f\n",planned->wa); 
  }


  // Unsuscribe and Destroy
  //planner
  playerc_planner_unsubscribe(planned); playerc_planner_destroy(planned);	
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
