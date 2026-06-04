#pragma once

#include <stdint.h>
#include <math.h>

typedef struct {int64_t x,y,z;} __attribute__((aligned(16))) Pos3D;
typedef struct {int64_t x,y,z;} __attribute__((aligned(16))) Scale3D;
typedef struct {int64_t x[3],y[3],z[3];} __attribute__((aligned(16))) FaceTriangle;
typedef struct {int64_t x[4],y[4],z[4];} __attribute__((aligned(16))) FaceSquare;
typedef struct {Pos3D Pos;int64_t x[24],y[24],z[24];} __attribute__((aligned(16))) Square3D;
typedef struct {Pos3D Pos;int64_t x[12],y[12],z[12];} __attribute__((aligned(16))) Triangle3D;
typedef struct {Pos3D Pos;uint64_t FaceCount;FaceTriangle Faces[];} __attribute__((aligned(16))) Polygon3D;

#define SIGNED_TO_ORDERED_UINT64(i64) ((uint64_t)(i64) ^ 0x8000000000000000ULL)

#define PROJECTION_X(local_x, world_pos_x, z, Screen_W, FOCAL_LENGTH) \
    (z ? (int)( (((double)(local_x) - (local_x ? local_x/2 : 0)) * FOCAL_LENGTH) / (double)(z) + (double)(world_pos_x) + ((double)(Screen_W) / 2.0f) ) : 0)

#define PROJECTION_Y(local_y, world_pos_y, z, Screen_H, FOCAL_LENGTH) \
    (z ? (int)( (((double)(local_y) - (local_y ? local_y/2 : 0)) * FOCAL_LENGTH) / (double)(z) + (double)(world_pos_y) + ((double)(Screen_H) / 2.0f) ) : 0)


#define CreateTriangle(x,y,z) ((Triangle3D){(Pos3D){x,y,z},{0,1,0, 0,0,0, 0,0,1, 0,0,1},{0,0,0, 0,1,0, 0,1,0, 0,1,0},{0,0,1, 0,0,1, 1,0,0, 0,0,0}})

#define CreateSquare(x,y,z) ((Square3D){(Pos3D){x,y,z},{0,1,1,0, 0,0,0,0, 0,1,1,0, 0,1,1,0, 0,1,1,0, 1,1,1,1},{0,0,0,0, 0,1,1,0, 0,0,1,1, 0,0,1,1, 1,1,1,1, 0,1,1,0},{0,0,1,1, 0,1,1,0, 1,1,1,1, 0,0,0,0, 0,0,1,1, 0,0,1,1}})

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

