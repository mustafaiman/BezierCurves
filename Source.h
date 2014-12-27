#include "Angel.h"

#define NUM_VERTICES 5000
#define WIN_W glutGet(GLUT_WINDOW_WIDTH)
#define WIN_H glutGet(GLUT_WINDOW_HEIGHT)
#define RESOLUTION 25

enum Shading {
	Wireframe = 0,
	Phong = 1,
	Gouroud = 2
};


Shading shadingModel;

vec4 points[RESOLUTION + 1][RESOLUTION + 1];
vec4 vertices[NUM_VERTICES];
vec4 vNormals[NUM_VERTICES];
vec4 normals[RESOLUTION + 1][RESOLUTION + 1][6];
vec4 averageNormals[RESOLUTION + 1][RESOLUTION + 1];
int normalIndexes[RESOLUTION + 1][RESOLUTION + 1];
vec4 colors[NUM_VERTICES];
int Index;

mat4 projectionMatrix;
mat4 modelViewMatrix;

double globalRotateX = 30.0;
double globalRotateY = 20.0;
double globalRotateZ = 0.0;


vec4 light_ambient(1.0, 0.0, 0.0, 1.0);
vec4 light_specular(0.0, 0.0, 1.0, 1.0);
vec4 light_diffuse(0.0, 0.0, 0.0, 1.0);

vec4 light_position(0.0, 2.0, 0.0, 1.0);

mat4 knobs[3] = {
	mat4(
	-0.9, -0.9, -0.9, -0.9,
	-0.3, -0.3, -0.3, -0.3,
	0.3, 0.3, 0.3, 0.3,
	0.9, 0.9, 0.9, 0.9
	),
	mat4(
	-0.5, -0.3, -0.3, -0.5,
	-0.3, 0.2, 0.2, -0.3,
	-0.3, 0.2, 0.2, -0.3,
	-0.5, -0.3, -0.3, -0.5
	),
	mat4(
	0.9, 0.6, -0.6, -0.9,
	0.9, 0.6, -0.6, -0.9,
	0.9, 0.6, -0.6, -0.9,
	0.9, 0.6, -0.6, -0.9
	)
};

GLuint gDrawArraysType;
int programShadingType;
int fShadingType;

GLuint programWireframe;
GLuint programGouroud;
GLuint programPhong;

/*
mat4 knobs[3] = {
	mat4(
	-0.9, -0.9, -0.9, -0.9,
	-0.3, -0.3, -0.3, -0.3,
	0.3, 0.3, 0.3, 0.3,
	0.9, 0.9, 0.9, 0.9
	),
	mat4(
	0.9, 0.6, -0.6, -0.9,
	0.9, 0.6, -0.6, -0.9,
	0.9, 0.6, -0.6, -0.9,
	0.9, 0.6, -0.6, -0.9
	),
	mat4(
	1.0, 0.0, 0.0, 0.0,
	0.5, 0.0, 0.5, 0.0,
	0.2, 0.0, 0.0, 0.0,
	0.4, 0.0, 0.0, 0.0
	)
};
*/