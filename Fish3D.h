#pragma once

#include <stdint.h>
#include <math.h>

typedef struct {int64_t x,y,z;} Pos3D;
typedef struct {int64_t x,y,z;} Scale3D;
typedef struct {int64_t x[3],y[3],z[3];} FaceTriangle;
typedef struct {int64_t x[4],y[4],z[4];} FaceSquare;
typedef struct {Pos3D Pos;FaceSquare Faces[6];} Square3D;
typedef struct {Pos3D Pos;FaceTriangle Faces[4];} Triangle3D;
typedef struct {Pos3D Pos;uint64_t FaceCount;FaceTriangle Faces[];} Polygon3D;

#define SIGNED_TO_ORDERED_UINT64(i64) ((uint64_t)(i64) ^ 0x8000000000000000ULL)
#define PROJECTION_X(x, z, Screen_W,FOCAL_LENGTH) (z ? ((double)(x) * FOCAL_LENGTH) / (double)(z) + ((double)(Screen_W) / 2.0f):0 )
#define PROJECTION_Y(y, z, Screen_H,FOCAL_LENGTH) (z ? ((double)(y) * FOCAL_LENGTH) / (double)(z) + ((double)(Screen_H) / 2.0f):0 )

#define CreateTriangle(x,y,z) ((Triangle3D){(Pos3D){x,y,z},\
	{(FaceTriangle){{0,1,0},{0,0,0},{0,0,1}},(FaceTriangle){{0,0,0},{0,1,0},{0,0,1}},\
	(FaceTriangle){{0,0,1},{0,1,0},{1,0,0}},(FaceTriangle){{0,0,1},{0,1,0},{0,0,0}}}})

#define CreateSquare(x,y,z) ((Square3D){(Pos3D){x,y,z},{\
	(FaceSquare){{0,1,1,0},{0,0,0,0},{0,0,1,1}},(FaceSquare){{0,0,0,0},{0,1,1,0},{0,0,1,1}},\
	(FaceSquare){{0,1,1,0},{0,0,1,1},{1,1,1,1}},(FaceSquare){{0,1,1,0},{0,0,1,1},{0,0,0,0}},\
	(FaceSquare){{0,1,1,0},{1,1,1,1},{0,0,1,1}},(FaceSquare){{1,1,1,1},{0,1,1,0},{0,0,1,1}}}})

#define CreatePolygon(x,y,z,Faces) ((Polygon3D){(Pos3D){x,y,z},Faces})

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

