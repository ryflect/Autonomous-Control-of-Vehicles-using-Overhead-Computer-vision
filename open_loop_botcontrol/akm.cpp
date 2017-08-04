#include <SDL2/SDL.h>
#include <iostream>
#include "senddatablue.h"

#define PI 3.14159

int steerangle = 0, speed = 0;
const int steerangleres = 1, speedres = 1;
//const int absmaxangle = int((double(PI)/double(2)) / (double(PI)/double(100))), maxspeed = 128, minspeed = 0;
const int absmaxangle = 30, maxspeed = 127, minspeed = -127;

int infosendcounter = 0;
const int infosendthresh = 1;

void arrowkeyscontrol()
{ 
  //For serial control with Bluetooth/USB (/dev/rfcomm0 or /dev/ttyACM0)
  int fd = getPortHandle();
  usleep(5000000);
  
  SDL_Init(SDL_INIT_VIDEO);   // Initialize SDL2

  SDL_Window *window;        // Declare a pointer to an SDL_Window

  // Create an application window with the following settings:
  window = SDL_CreateWindow( 
    "An SDL2 window",                  //    window title
    SDL_WINDOWPOS_UNDEFINED,           //    initial x position
    SDL_WINDOWPOS_UNDEFINED,           //    initial y position
    640,                               //    width, in pixels
    480,                               //    height, in pixels
    SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL //    flags - see below
  );

  // Check that the window was successfully made
  if(window==NULL)
  {   
    // In the event that the window could not be made...
    std::cout << "Could not create window: " << SDL_GetError() << '\n';
    return;
  }

  // The window is open: enter program loop (see SDL_PollEvent)
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  //while (1) 
  //{
    //SDL_Event event;
    //while (SDL_PollEvent(&event))
    while(1)
    {
      /* handle your event here */
      SDL_PumpEvents();
      if (state[SDL_SCANCODE_UP])
      {
        //printf("Up is pressed.\n");
        speed += speedres*5;
        if (speed > maxspeed)
          speed = maxspeed;
      }

      if (state[SDL_SCANCODE_RIGHT]) 
      {
        //printf("Right is Pressed.\n");
        steerangle += steerangleres*5;
        if (steerangle > absmaxangle)
          steerangle = absmaxangle;
      }

      if (state[SDL_SCANCODE_DOWN])
      {
        //printf("Down is Pressed.\n");
        speed -= speedres*5;
        if (speed < minspeed)
          speed = minspeed;
      }

      if (state[SDL_SCANCODE_LEFT])
      {
        //printf("Left is Pressed.\n");
        steerangle -= steerangleres*5;
        if (steerangle < -absmaxangle)
          steerangle = -absmaxangle;
      }

      if (state[SDL_SCANCODE_Q])
      {
          printf("Exiting.\n");
          //Exit Protocols
          short int encodedData = encodeData(0, 0);
          write(fd, "w", 1);
          write (fd, &encodedData, 2);
          // Close and destroy the window
          SDL_DestroyWindow(window);
          // Clean up
          SDL_Quit();
          exit(1);
      }
      SDL_Delay(200);

      infosendcounter++;
      if (infosendcounter == infosendthresh)
      { 
        infosendcounter = 0;
        //Send with bluetooth
        printf("Sending data -> %d %d.\n",steerangle,speed);
        //Send steerangle and speed after encoding
        short int encodedData = encodeData(steerangle, speed);
        write(fd, "w", 1);
        write (fd, &encodedData, 2);
      }
    }
    /* do some other stuff here -- draw your app, etc. */
  //}

}

int main(int argc, char* argv[])
{
  
  arrowkeyscontrol();


  return 0;   

}