#include "Fish3D.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_rect.h>
#include <math.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <inttypes.h>

typedef struct {
    int id;
    uint64_t z_key;
} FaceKey;

static inline uint64_t DoubleToBits(double d) {
    union { double d; uint64_t u; } cast;
    cast.d = d;
    uint64_t mask = -((int64_t)(cast.u >> 63)) | 0x8000000000000000ULL;
    return cast.u ^ mask;
}

void RadixSortFaces(FaceKey* input, int n) {
    __attribute__((aligned(64))) FaceKey output[n];
    
    for (int byte_idx = 0; byte_idx < 8; byte_idx++) {
        int count[256] = {0};
        int shift = byte_idx * 8;

        for (int i = 0; i < n; i++) {
            int bucket = (input[i].z_key >> shift) & 0xFF;
            count[bucket]++;
        }

        int total = 0;
        for (int i = 255; i >= 0; i--) {
            int amt = count[i];
            count[i] = total;
            total += amt;
        }

        for (int i = 0; i < n; i++) {
            int bucket = (input[i].z_key >> shift) & 0xFF;
            output[count[bucket]++] = input[i];
        }

        for (int i = 0; i < n; i++) {
            input[i] = output[i];
        }
    }
}

static inline void Square3DRotateX(Square3D* Obj, double angle){
    double ARGangle = angle * M_PI / 180.0;
    double s = sin(ARGangle); 
    double c = cos(ARGangle);
    double old_y=0;
    
    #pragma clang loop vectorize(enable) interleave(enable)
    #pragma clang loop unroll(enable)
    for (int i = 0; i < 24; i++) {
        old_y = Obj->y[i];
        Obj->y[i] = old_y * c - Obj->z[i] * s;
        Obj->z[i] = old_y * s + Obj->z[i] * c;
    }
}

static inline void Square3DRotateY(Square3D* Obj, double angle){
    double ARGangle = angle * M_PI / 180.0;
    double s = sin(ARGangle); 
    double c = cos(ARGangle);
    double old_x=0;

    #pragma clang loop vectorize(enable) interleave(enable)
    #pragma clang loop unroll(enable)
    for (int i = 0; i < 24; i++) {
        old_x = Obj->x[i];
        Obj->x[i] = old_x * c + Obj->z[i] * s;
        Obj->z[i] = -old_x * s + Obj->z[i] * c;
    }
}

static inline void Square3DRotateZ(Square3D* Obj, double angle){
    double ARGangle = angle * M_PI / 180.0;
    double s = sin(ARGangle);
    double c = cos(ARGangle);
    double old_x=0;
    
    #pragma clang loop vectorize(enable) interleave(enable)
    #pragma clang loop unroll(enable)
    for (int i = 0; i < 24; i++) {
        old_x = Obj->x[i];
        Obj->x[i] = old_x * c - Obj->y[i] * s;
        Obj->y[i] = old_x * s + Obj->y[i] * c; 
    }
}

void DrawSquare3D(SDL_Renderer *Render, Square3D Obj, int ScreenW, int ScreenH, uint8_t a, int Pov, Pos3D CamPos, _Bool Fill) {
    Pos3D Pos = Obj.Pos;
    FaceKey faces[6];

    for (int i = 0; i < 6; i++) {
        faces[i].id = i * 4; 
        double avg_z = 0;
        for (int j = 0; j < 4; j++) {
            avg_z += (Pos.z + Obj.z[i * 4 + j] - CamPos.z);
        }
        avg_z /= 4.0;
        
        faces[i].z_key = DoubleToBits(avg_z); 
    }

    RadixSortFaces(faces, 6);

    for (int f = 0; f < 6; f++) {
        int i = faces[f].id;
        
        SDL_SetRenderDrawColor(Render, Obj.r[i>>2], Obj.g[i>>2], Obj.b[i>>2], a);
        int sx[4], sy[4];
        _Bool Draw = 1;
        
        for (int j = 0; j < 4; j++) {
            int idx = i + j;
            double abs_x = Pos.x + Obj.x[idx] - CamPos.x;
            double abs_y = Pos.y + Obj.y[idx] - CamPos.y;
            double abs_z = Pos.z + Obj.z[idx] - CamPos.z;
            if (abs_z <= 0) {
                Draw = 0;
                break;
            }
            sx[j] = PROJECTION_X(abs_x, abs_z, ScreenW, Pov);
            sy[j] = PROJECTION_Y(abs_y, abs_z, ScreenH, Pov);
        }

        if (!Fill && Draw) {
            SDL_RenderDrawLine(Render, sx[0], sy[0], sx[1], sy[1]);
            SDL_RenderDrawLine(Render, sx[1], sy[1], sx[2], sy[2]);
            SDL_RenderDrawLine(Render, sx[2], sy[2], sx[3], sy[3]);
            SDL_RenderDrawLine(Render, sx[3], sy[3], sx[0], sy[0]);
        } else if (Fill && Draw) {
            SDL_Vertex vertices[] = {
                { {sx[0], sy[0]}, {Obj.r[i>>2], Obj.g[i>>2], Obj.b[i>>2], a}, {0.0f, 0.0f} },
                { {sx[1], sy[1]}, {Obj.r[i>>2], Obj.g[i>>2], Obj.b[i>>2], a}, {1.0f, 0.0f} },
                { {sx[2], sy[2]}, {Obj.r[i>>2], Obj.g[i>>2], Obj.b[i>>2], a}, {1.0f, 1.0f} },
                { {sx[3], sy[3]}, {Obj.r[i>>2], Obj.g[i>>2], Obj.b[i>>2], a}, {0.0f, 1.0f} }
            };
            int indices[] = { 0, 1, 2, 2, 3, 0 };
            SDL_RenderGeometry(Render, NULL, vertices, 4, indices, 6);
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
    Cube.r[0]=0;
    Cube.g[0]=255;
    Cube.b[0]=0;
    Cube.r[1]=255;
    Cube.g[1]=255;
    Cube.b[1]=0;
    Cube.r[2]=255;
    Cube.g[2]=0;
    Cube.b[2]=0;
    Cube.r[3]=0;
    Cube.g[3]=0;
    Cube.b[3]=255;
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
		
		CamPos.x+=(KeyState[SDL_SCANCODE_D]-KeyState[SDL_SCANCODE_A]);
		CamPos.y+=(KeyState[SDL_SCANCODE_Q]-KeyState[SDL_SCANCODE_E]);
		CamPos.z+=(KeyState[SDL_SCANCODE_W]-KeyState[SDL_SCANCODE_S]);

		DrawSquare3D(Render, Cube, WindowW, WindowH, 255, 500, CamPos, 1);
        
        Square3DRotateY(&Cube, 2);


		SDL_RenderPresent(Render);
		SDL_Delay(16);
	}

	SDL_DestroyRenderer(Render);
	SDL_DestroyWindow(Window);
	SDL_Quit();
	return 0;
}
