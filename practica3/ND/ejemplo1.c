#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#include <libplayerc/playerc.h>

int main(int argc, const char **argv)
{
  int i, p, j, zzz,q;  
  playerc_client_t *client;
  playerc_position2d_t *position2d;
  playerc_position2d_t *position2d2;
  playerc_laser_t *laser;
  player_pose2d_t position2d_target;
  double vel;

  //recorrido 1
  double arrayx[4]={ -4.0,-5.0,-8.0,-8.0};
  double arrayy[4]={ 0.0,-3.25,0.0,7.0};
  double arraya[4]={ 3.14,3.14,1.57,1.57};

  //recorrido 2
  /*double arrayx=3.0;
  double arrayy=2.0;
  double arraya=0.0;*/

  //tiempo y velocidades 
  struct timeval tvalBefore, tvalAfter;
  struct timeval tvalBeforeAcel, tvalAfterAcel;

  //creacion y apaertura fichero para calculos
  FILE* ficheroVelocidades; 
  ficheroVelocidades = fopen("velocidad.txt", "w");
  FILE* ficheroAceleraciones; 
  ficheroAceleraciones = fopen("aceleracion.txt", "w");

  //laser

  //drawing
  playerc_graphics2d_t *graficos;
  player_point_2d_t *puntos;
  player_color_t color;
  puntos=(player_point_2d_t *)malloc(sizeof(player_point_2d_t)*(1)); //(1) punto
  color.red=255; color.green=0; color.blue=0;
  

  // Create a client and connect it to the server.
  client = playerc_client_create(NULL, "localhost", 6665);
  if (playerc_client_connect(client) != 0)
    {
      fprintf(stderr, "error client: %s\n", playerc_error_str());
      return -1;
    }

  // Create and subscribe to a position2d device.
  position2d = playerc_position2d_create(client, 1);
  if (playerc_position2d_subscribe(position2d, PLAYER_OPEN_MODE) != 0)
    {
      fprintf(stderr, "error robot1: %s\n", playerc_error_str());
      return -1;
    }

  // Create and subscribe to a position2d device.
  position2d2 = playerc_position2d_create(client, 2);
  if (playerc_position2d_subscribe(position2d2, PLAYER_OPEN_MODE) != 0)
    {
      fprintf(stderr, "error robot2: %s\n", playerc_error_str());
      return -1;
    }

  // recorrido 1
  playerc_position2d_set_odom(position2d,0.0,0.0,0.0);
  playerc_position2d_set_odom(position2d2,-2.0,0.0,0.0);

  // recorrido 2
  /*playerc_position2d_set_odom(position2d,2.0,-8.0,1.57);
  playerc_position2d_set_odom(position2d2,1.0,-2.0,-1.57);*/


  // Create and subscribe to a laser device
  laser=playerc_laser_create(client,0);
  if (playerc_laser_subscribe(laser,PLAYER_OPEN_MODE))
  {
	fprintf(stderr, "error laser: %s\n", playerc_error_str());
	return -1;
  }
  
  // Create and subscribe to a graphics device
  graficos = playerc_graphics2d_create(client, 0);
  if (playerc_graphics2d_subscribe(graficos, PLAYER_OPEN_MODE) != 0)
  {
	fprintf(stderr, "error graficos: %s\n", playerc_error_str());
	return -1;
  }

  // Fix colour
  playerc_graphics2d_setcolor (graficos, color); 

  // Clear screen
  playerc_graphics2d_clear(graficos); 

  //inicializamos los valores
  q=1;

  //iniciamos contador tiempo general
  gettimeofday (&tvalBefore, NULL);

  //iniciamos contador tiempo y velocidad para aceleracion
  gettimeofday (&tvalBeforeAcel, NULL);
  double vel_ant=0.0;
  double acel=0.0;
  double vel_actl=0.0;
  long double def_tiempo;

  // Enable motors 
  playerc_position2d_enable(position2d,1);
  playerc_client_read(client);
  

  //recorrido 1
  for ( i=0 ; i<4 ; i++ )
    {
      position2d_target.px = arrayx[i];
      position2d_target.py = arrayy[i];
      position2d_target.pa = arraya[i];
      
      // Move to pose  
      playerc_position2d_set_cmd_pose(position2d, position2d_target.px , position2d_target.py, position2d_target.pa , 1);

      // Stop when reach the target
      while  (sqrt(pow(position2d->px - position2d_target.px,2.0) + pow(position2d->py - position2d_target.py,2.0)) > 0.25 )
	{
	  // Wait for new data from server
	  playerc_client_read(client);
	  
	  // Print current robot pose
	  printf("position2d : x %f y %f th %f stall %d\n",position2d->px, position2d->py, position2d->pa, position2d->stall); 
	  // What does mean stall?
	  // x, y, th, world frame or robot frame?

	  //calculamos e imprimimos las velocidades
	  vel= (sqrt(pow(position2d->vx,2.0)+pow(position2d->vy,2.0)));
	  fprintf (ficheroVelocidades, "%d %f\n", q, vel);
	  q=q+1;

	  //calculamos e imprimimos la aceleracion
	  vel_actl = (sqrt(pow(position2d->vx,2.0)+pow(position2d->vy,2.0)));
	  gettimeofday (&tvalAfterAcel, NULL);
	  def_tiempo = ((tvalAfterAcel.tv_sec - tvalBeforeAcel.tv_sec)*1000000L+tvalAfterAcel.tv_usec) - tvalBeforeAcel.tv_usec;
	  def_tiempo = def_tiempo/1000000;
	  acel=(vel_actl-vel_ant)/def_tiempo;
	  fprintf (ficheroAceleraciones, "%f \n", acel);
	  //reinicio para porxima itreracion
	  vel_ant = vel_actl;
 	  gettimeofday (&tvalBeforeAcel, NULL);

	  // Draw current robot pose
	  puntos[0].px=position2d->px;
	  puntos[0].py=position2d->py;
	  playerc_graphics2d_draw_points (graficos, puntos, 1);
	  if (position2d->stall==1){break;}	
	}
	if (position2d->stall==1){break;}
   }


  //recorrido 2
  /*position2d_target.px = arrayx;
  position2d_target.py = arrayy;
  position2d_target.pa = arraya;
      
  // Move to pose  
  playerc_position2d_set_cmd_pose(position2d, position2d_target.px , position2d_target.py, position2d_target.pa , 1);

  // Stop when reach the target
  while  (sqrt(pow(position2d->px - position2d_target.px,2.0) + pow(position2d->py - position2d_target.py,2.0)) > 0.25 ){
	// Wait for new data from server
	playerc_client_read(client);
	  
	// Print current robot pose
	printf("position2d : x %f y %f th %f stall %d\n",position2d->px, position2d->py, position2d->pa, position2d->stall); 
	// What does mean stall?
	// x, y, th, world frame or robot frame?

	//calculamos e imprimimos las velocidades
	vel= (sqrt(pow(position2d->vx,2.0)+pow(position2d->vy,2.0)));
	fprintf (ficheroVelocidades, "%d %f\n", q, vel);
	q=q+1;

	//calculamos e imprimimos la aceleracion
	vel_actl = (sqrt(pow(position2d->vx,2.0)+pow(position2d->vy,2.0)));
	gettimeofday (&tvalAfterAcel, NULL);
	def_tiempo = ((tvalAfterAcel.tv_sec - tvalBeforeAcel.tv_sec)*1000000L+tvalAfterAcel.tv_usec) - tvalBeforeAcel.tv_usec;
	def_tiempo = def_tiempo/1000000;
	acel=(vel_actl-vel_ant)/def_tiempo;
	fprintf (ficheroAceleraciones, "%f \n", acel);
	//reinicio para porxima itreracion
	vel_ant = vel_actl;
 	gettimeofday (&tvalBeforeAcel, NULL);

	// Draw current robot pose
	puntos[0].px=position2d->px;
	puntos[0].py=position2d->py;
	playerc_graphics2d_draw_points (graficos, puntos, 1);
	if (position2d->stall==1){break;}	
  }*/

  //final tiempo ejecucion
  gettimeofday (&tvalAfter, NULL);
  printf("Time in microseconds: %ld microseconds\n",((tvalAfter.tv_sec - tvalBefore.tv_sec)*1000000L+tvalAfter.tv_usec) - tvalBefore.tv_usec);


  //cerramos los ficheros
  fclose(ficheroVelocidades);     printf("Archivo con velocidades completado");
  fclose(ficheroAceleraciones);   printf("Archivo con aceleraciones completado");

  // Unsuscribe and Destroy
  // position2d
  playerc_position2d_unsubscribe(position2d); playerc_position2d_destroy(position2d);
  // position2d
  playerc_position2d_unsubscribe(position2d2); playerc_position2d_destroy(position2d2);
  // laser
   playerc_laser_unsubscribe(laser); playerc_laser_destroy(laser); 
  // graphics2d
  playerc_graphics2d_unsubscribe(graficos); playerc_graphics2d_destroy(graficos);
  // client
  playerc_client_disconnect(client); playerc_client_destroy(client);

  // End
  return 0;
}
