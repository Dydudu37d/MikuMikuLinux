#pragma once

#include <stdint.h>

typedef struct {double x,y,z;} __attribute__((aligned(16))) Pos3D;
typedef struct {double x,y,z;} __attribute__((aligned(16))) Scale3D;
typedef struct {double x[3],y[3],z[3];} __attribute__((aligned(16))) FaceTriangle;
typedef struct {double x[4],y[4],z[4];} __attribute__((aligned(16))) FaceSquare;
typedef struct {Pos3D Pos;double x[24],y[24],z[24];uint8_t r[6],g[6],b[6];} __attribute__((aligned(16))) Square3D;
typedef struct {Pos3D Pos;double x[12],y[12],z[12];uint8_t r[4],g[4],b[4];} __attribute__((aligned(16))) Triangle3D;
typedef struct {Pos3D Pos;uint64_t XYZCount;double *x;double *y;double *z;uint8_t *r;uint8_t *g;uint8_t *b;} __attribute__((aligned(16))) Polygon3D;

#define SIGNED_TO_ORDERED_UINT64(i64) ((uint64_t)(i64) ^ 0x8000000000000000ULL)

#define PROJECTION_X(x, z, Screen_W, FOCAL_LENGTH) \
    ((z) > 0 ? (int)( ((double)(x) * (FOCAL_LENGTH)) / (double)(z) + ((double)(Screen_W) / 2.0f) ) : 0)

#define PROJECTION_Y(y, z, Screen_H, FOCAL_LENGTH) \
    ((z) > 0 ? (int)( ((double)(y) * (FOCAL_LENGTH)) / (double)(z) + ((double)(Screen_H) / 2.0f) ) : 0)

#define CreateTriangle(x,y,z,s) ((Triangle3D){(Pos3D){x,y,z},{0,s,0, 0,0,0, 0,0,s, 0,0,s},{0,0,0, 0,s,0, 0,s,0, 0,s,0},{0,0,s, 0,0,s, s,0,0, 0,0,0},{255,255,255,255},{255,255,255,255},{255,255,255,255}})

#define CreateSquare(x,y,z,s) ((Square3D){(Pos3D){x,y,z},{ 0,s,s,0,  0,s,s,0,  0,0,0,0,  s,s,s,s,  0,s,s,0,  0,s,s,0 },{ 0,0,s,s,  0,0,s,s,  0,0,s,s,  0,0,s,s,  0,0,0,0,  s,s,s,s },{ 0,0,0,0,  s,s,s,s,  0,s,s,0,  0,s,s,0,  0,0,s,s,  0,0,s,s },{255,255,255,255,255,255},{255,255,255,255,255,255},{255,255,255,255,255,255}})

#define RotateX(x,y,z,c,s) \
	do {\
		__typeof__(y) old_y=y; \
		y=old_y*c-z*s; \
		z=old_y*s+z*c; \
	} while(0)
#define RotateY(x,y,z,c,s) \
	do {\
		__typeof__(x) old_x=x;\
		x=old_x*c+z*s;\
		z=-old_x*s+z*c;\
	} while(0)
#define RotateZ(x,y,z,c,s) \
	do {\
		__typeof__(x) old_x=x;\
		x=old_x*c-y*s;\
		y=old_x*s+y*c;\
	}while(0)

