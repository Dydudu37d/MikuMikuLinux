#include "Fish3D.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>

int main(){
	printf("Start\n");
	SDL_DisplayMode dm;
	int Dw=1000,Dh=1000;
	if (SDL_GetDesktopDisplayMode(0, &dm)){
		Dw=dm.w;
		Dh=dm.h;
	}
	SDL_Window *Window=SDL_CreateWindow("",Dw/4,Dh/4,Dw/2,Dh/2,SDL_WINDOW_SHOWN);
	_Bool running=1;
	SDL_Event Event;
	Square3D Cube=CreateSquare(10, 10, 10);

	while (running){
		while(SDL_PollEvent(&Event)){
			if (Event.type == SDL_QUIT){
				running=0;
			}
			if (Event.type == SDL_KEYDOWN){
				// if(Event.key.keysym.sym == SDLK_XXXX){}
			}
		}
		
		SDL_Delay(16);
	}

	SDL_DestroyWindow(Window);
	SDL_Quit();
	return 0;
}
