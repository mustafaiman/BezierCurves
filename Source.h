#include "Angel.h"

#define NUM_VERTICES 100000
#define WIN_W glutGet(GLUT_WINDOW_WIDTH)
#define WIN_H glutGet(GLUT_WINDOW_HEIGHT)
#define RESOLUTION 50
#define DIFF 0.05

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
int knobIndex;

mat4 projectionMatrix;
mat4 modelViewMatrix;

vec3 zoomVector(1.0, 1.0, 1.0);

double globalRotateX = 0.0;
double globalRotateY = 0.0;
double globalRotateZ = 0.0;


vec4 light_ambient(0.12, 0.12, 0.12, 1.0);
vec4 light_specular(1.0, 1.0, 1.0, 1.0);
vec4 light_diffuse(0.23, 0.23, 0.23, 1.0);

vec4 light_position(0.0, 5.0, 2.0, 1.0);

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
GLuint activeProgram;
GLuint vModelView;
GLuint vProjection;

GLuint mytex[1];
GLubyte my_texels[512][512][3];
GLfloat tex_coord[NUM_VERTICES][2];

int isTextureMappingOn;
float yDist = -5.0;


int activeKnob;

unsigned char header[54]; // Each BMP file begins by a 54-bytes header
unsigned int dataPos;     // Position in the file where the actual data begins
unsigned int width, height;
unsigned int imageSize;   // = width*height*3
// Actual RGB data
unsigned char * data;