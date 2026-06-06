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
#include <inttypes.h>

void DrawSquare3D(SDL_Renderer *Render, Square3D Obj, int ScreenW, int ScreenH, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int Pov, Pos3D CamPos) {
    SDL_SetRenderDrawColor(Render, r, g, b, a);
    Pos3D Pos = Obj.Pos;

    for (int i = 0; i < 24; i += 4) {
        
        int sx[4], sy[4];
        _Bool face_valid = 1;

        for (int j = 0; j < 4; j++) {
            int idx = i + j;
            int64_t abs_x = Pos.x + Obj.x[idx] - CamPos.x;
            int64_t abs_y = Pos.y + Obj.y[idx] - CamPos.y;
            int64_t abs_z = Pos.z + Obj.z[idx] - CamPos.z;

            if (abs_z <= 0) {
                face_valid = 0;
                break;
            }

            sx[j] = PROJECTION_X(abs_x, abs_z, ScreenW, Pov);
            sy[j] = PROJECTION_Y(abs_y, abs_z, ScreenH, Pov);
        }

        if (face_valid) {
            SDL_RenderDrawLine(Render, sx[0], sy[0], sx[1], sy[1]);
            SDL_RenderDrawLine(Render, sx[1], sy[1], sx[2], sy[2]);
            SDL_RenderDrawLine(Render, sx[2], sy[2], sx[3], sy[3]);
            SDL_RenderDrawLine(Render, sx[3], sy[3], sx[0], sy[0]);
            
            printf("FACE[%d]: (%d,%d) -> (%d,%d) -> (%d,%d) -> (%d,%d) -> ClOSED\n", 
                   i / 4, sx[0], sy[0], sx[1], sy[1], sx[2], sy[2], sx[3], sy[3]);
        }
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
	Square3D Cube=CreateSquare(10, 10, 10, 30);
    Pos3D CamPos={0};
	SDL_Event Event;
	while (running){
		SDL_GetWindowSize(Window, &WindowW, &WindowH);
		SDL_SetRenderDrawColor(Render, 0, 0, 0, 255);
		SDL_RenderClear(Render);
		while(SDL_PollEvent(&Event)){
			if (Event.type == SDL_QUIT) running=0;
		}
		const uint8_t* KeyState = SDL_GetKeyboardState(NULL);
		
		Cube.Pos.x+=(KeyState[SDL_SCANCODE_D]-KeyState[SDL_SCANCODE_A]);
		Cube.Pos.y+=(KeyState[SDL_SCANCODE_Q]-KeyState[SDL_SCANCODE_E]);
		Cube.Pos.z+=(KeyState[SDL_SCANCODE_S]-KeyState[SDL_SCANCODE_W]);

		DrawSquare3D(Render, Cube, WindowW, WindowH, 255, 255, 255, 255,500,CamPos);

		SDL_RenderPresent(Render);
		SDL_Delay(16);
	}

	SDL_DestroyRenderer(Render);
	SDL_DestroyWindow(Window);
	SDL_Quit();
	return 0;
}
