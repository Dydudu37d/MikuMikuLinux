#include "Fish3D.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_rect.h>
#include <stdint.h>
#include <SDL2/SDL.h>

void DrawSquare3D(SDL_Renderer *Render,Square3D Obj,int ScreenW,int ScreenH,uint8_t r,uint8_t g,uint8_t b,uint8_t a,int Pov){
	if (Obj.Pos.z == 0) return;
	SDL_SetRenderDrawColor(Render, r, g, b, a);
	Pos3D Pos=Obj.Pos;
	for (int i = 0; i < 24; i++) {
		if (Pos.z + Obj.z[i] <= 0) continue;
		int next_idx = (i & ~3) + ((i + 1) & 3); 
		if (Pos.z + Obj.z[next_idx] <= 0) continue;
		int x1 = PROJECTION_X(Obj.x[i],        Pos.x, Pos.z + Obj.z[i],        ScreenW, Pov);
		int y1 = PROJECTION_Y(Obj.y[i],        Pos.y, Pos.z + Obj.z[i],        ScreenH, Pov);
		int x2 = PROJECTION_X(Obj.x[next_idx], Pos.x, Pos.z + Obj.z[next_idx], ScreenW, Pov);
		int y2 = PROJECTION_Y(Obj.y[next_idx], Pos.y, Pos.z + Obj.z[next_idx], ScreenH, Pov);
		SDL_RenderDrawLine(Render, x1, y1, x2, y2);
	}
}

int main(){
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL Init Failed: %s\n", SDL_GetError());
		return -1;
	}
	SDL_DisplayMode dm;
    
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
		printf("Get mode failed: %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	int ScreenW = dm.w;
	int ScreenH = dm.h;
	
	SDL_Window* Window = SDL_CreateWindow("MikuMikuLinux", ScreenW/4, ScreenH/4, ScreenW/2, ScreenH/2, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!Window){
		printf("Window Error: %s\n",SDL_GetError());
		SDL_Quit();
		return -1;
	}
	SDL_Renderer* Render = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!Render){
		printf("Render Error: %s\n",SDL_GetError());
		SDL_DestroyWindow(Window);
		SDL_Quit();
		return -1;
	}
	int WindowW,WindowH;
	_Bool running=1;
	Square3D Cube=CreateSquare(10, 10, 10);
	SDL_Event Event;
	while (running){
		SDL_GetWindowSize(Window, &WindowW, &WindowH);
		SDL_SetRenderDrawColor(Render, 0, 0, 0, 255);
		SDL_RenderClear(Render);
		while(SDL_PollEvent(&Event)){
			if (Event.type == SDL_QUIT) running=0;
		}
		const uint8_t* KeyState = SDL_GetKeyboardState(NULL);
		
		Cube.Pos.x+=(KeyState[SDL_SCANCODE_D]-KeyState[SDL_SCANCODE_A])*10;
		Cube.Pos.y+=(KeyState[SDL_SCANCODE_E]-KeyState[SDL_SCANCODE_Q])*10;
		Cube.Pos.z+=(KeyState[SDL_SCANCODE_W]-KeyState[SDL_SCANCODE_S])* 1;

		DrawSquare3D(Render, Cube, WindowW, WindowH, 255, 255, 255, 255,1000);

		SDL_RenderPresent(Render);
		SDL_Delay(16);
	}

	SDL_DestroyRenderer(Render);
	SDL_DestroyWindow(Window);
	SDL_Quit();
	return 0;
}
