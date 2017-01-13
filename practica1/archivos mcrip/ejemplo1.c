#include <stdio.h>
#include <math.h>

#include <libplayerc/playerc.h>

int
main(int argc, const char **argv)
{
  int i, p, j, zzz;  
  playerc_client_t *client;
  playerc_laser_t *laser;
  playerc_position2d_t *position2d;
  player_pose2d_t position2d_target;
  double arrayx[57]={ -0.5,-0.5,4.5,4.5,0.5,0.5,-0.5,-0.5,-4.3,-4.3,-0.5,-0.5,-4.3,-4.3,-6,-6,-9.5,-9.5,-6,-6,-9.5,-9.5,-8.5,-8.5,-8.5,-0.5,-0.5,1,1,2.5,2.5,1,1,9.5,9.5,4.5,4,1,1,4.5,4.5,1,1,-4.5,-4.5,-3.5,-3.5,-6.5,-6.5,-5.5,-5.5,-6.5,-6.5,-3.5,-3.5,-4.5,-4.5};
  double arrayy[57]={ 0,4.5,4.5,8,8,4.5,4.5,5,5,6,6,8,8,5,5,8,8,6,6,5,5,-3.5,-3.5,-4,-3.5,-3.5,-4,-4,-8,-8,-5,-5,-4,-4,-2,-2,0,0,1,1,3,3,0,0,4,4,5,5,4,4,-2,-2,-3,-3,-2,-2,0 };
  double arraya[57]={ 3.14,1.57,0.0,1.57,3.14,-1.57,3.14,1.57,3.14,1.57,0.0,1.57,3.14,-1.57,3.14,1.57,3.14,-1.57,0.0,-1.57,3.14,-1.57,0.0,-1.57,1.57,0.0,-1.57,0.0,-1.57,0.0,1.57,3.14,1.57,0.0,1.57,3.14,1.57,3.14,1.57,0.0,1.57,3.14,-1.57,3.14,1.57,0.0,1.57,3.14,-1.57,0.0,-1.57,3.14,-1.57,0.0,1.57,3.14,1.57}; 


  // Create a client and connect it to the server.
  client = playerc_client_create(NULL, "localhost", 6665);
  if (playerc_client_connect(client) != 0)
    {
      fprintf(stderr, "error: %s\n", playerc_error_str());
      return -1;
    }

  // Create and subscribe to a position2d device.
  position2d = playerc_position2d_create(client, 0);
  if (playerc_position2d_subscribe(position2d, PLAYER_OPEN_MODE) != 0)
    {
      fprintf(stderr, "error: %s\n", playerc_error_str());
      return -1;
    }

  // Fixing initial position
  playerc_position2d_set_odom(position2d,0.0,0.0,0.0);

  // Create and subscribe to a laser device
  laser=playerc_laser_create(client,0);
  if (playerc_laser_subscribe(laser,PLAYER_OPEN_MODE))
	return -1;
 
  // Enable motors 
  playerc_position2d_enable(position2d,1);
  playerc_client_read(client);
  

  for ( i=0 ; i<57 ; i++ )
    {
      position2d_target.px = arrayx[i];
      position2d_target.py = arrayy[i];
      position2d_target.pa = arraya[i];
      
      // Move to pose  
      playerc_position2d_set_cmd_pose(position2d, position2d_target.px , position2d_target.py, position2d_target.pa , 1);

      // Stop when reach the target
      while  (sqrt(pow(position2d->px - position2d_target.px,2.0) + pow(position2d->py - position2d_target.py,2.0)) > 0.05 )
	{
	  // Wait for new data from server
	  playerc_client_read(client);
	  
	  // Print current robot pose
	  printf("position2d : x %f y %f th %f stall %d\n",position2d->px, position2d->py, position2d->pa, position2d->stall); 
	  // What does mean stall?
	  // x, y, th, world frame or robot frame?
	}
    }


  // Unsuscribe and Destroy
  // position2d
  playerc_position2d_unsubscribe(position2d); playerc_position2d_destroy(position2d);	
  // laser
   playerc_laser_unsubscribe(laser); playerc_laser_destroy(laser); 
  // client
  playerc_client_disconnect(client); playerc_client_destroy(client);

  // End
  return 0;
}
