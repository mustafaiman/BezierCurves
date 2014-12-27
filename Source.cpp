#include "Angel.h"
#define NUM_VERTICES 5000
#define WIN_W glutGet(GLUT_WINDOW_WIDTH)
#define WIN_H glutGet(GLUT_WINDOW_HEIGHT)

#define NUM_CURVES 5000
#define RESOLUTION 20
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

vec4 points[RESOLUTION + 1][RESOLUTION + 1];
point4 vertices[NUM_VERTICES];
int Index;
color4 colors[NUM_VERTICES];
mat4 coefficients;

mat4 projectionMatrix;

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
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 
		0.0, 0.0, 0.0, 0.0, 
		0.0, 0.0, 0.0, 0.0
	)
};

void debugMatrix(mat4 matrix) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%lf ", matrix[i][j]);

		}
		printf("\n");
	}
}

void createPoint(point4 point) {
	vertices[Index] = point;
	Index++;

}

void writeToBuffer(mat4 &mm) {
	for (int i = 0; i < 4; i++) {
		vertices[Index++] = mm[0];
	}
}

void debugVector(vec4 &pp) {
	for (int i = 0; i < 4; i++) {
		printf("%lf ", pp[i]);
	}
	printf("\n");
}

void writeToBuffer(vec4 &pp) {
	vertices[Index++] = pp;
}

double bernstein(int k, double u) {
	if (k == 0) {
		return (1 - u) * (1 - u) * (1 - u);
	}
	else if (k == 1) {
		return 3 * u * (1 - u) * (1 - u);
	}
	else if (k == 2) {
		return 3 * u * u * (1 - u);
	}
	else if (k == 3) {
		return u * u * u;
	}
}

void createPatchInDimension(mat4 knobs[]) {
	for (int i = 0; i <= RESOLUTION; i++) {
		for (int j = 0; j <= RESOLUTION; j++) {
			double u = 1.0 / RESOLUTION*i;
			double v = 1.0 / RESOLUTION*j;
			vec4 point = { 0.0, 0.0, 0.0, 1.0 };
			for (int d = 0; d < 3; d++) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						//printf("%lf %lf %lf\n", knobs[d][i][j], bernstein(i, u), bernstein(j, v));
						point[d] += knobs[d][i][j] * bernstein(i, u) * bernstein(j, v);
					}
				}
			}
			points[i][j] = point;
		}
	}
	
	
}

void writeSurfaceToBuffer(vec4 points[RESOLUTION+1][RESOLUTION+1]) {
	for (int i = 0; i < RESOLUTION; i++) {
		for (int j = 0; j < RESOLUTION; j++) {
			vertices[Index] = points[i][j]; Index++;
			vertices[Index] = points[i][j+1]; Index++;
			vertices[Index] = points[i+1][j]; Index++;
			vertices[Index] = points[i][j+1]; Index++;
			vertices[Index] = points[i+1][j]; Index++;
			vertices[Index] = points[i+1][j+1]; Index++;
		}
	}
}

void init() {
	projectionMatrix = Ortho(-0.5, 0.5, -2.0, 2.0, -1.0, 1.0);

	createPatchInDimension(knobs);

	writeSurfaceToBuffer(points);


	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	GLuint vProjection = glGetUniformLocation(program, "vProjection");
	glUniformMatrix4fv(vProjection, 1, false, projectionMatrix);
	
	glClearColor(1.0, 0.0, 1.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);




}

void display() {

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, Index);

	glutSwapBuffers();
}

int main(int argc, char **argv) {
	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(512, 512);

	glutInitContextVersion(3, 2);

	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("Test Window");

	glewExperimental = GL_TRUE;

	glewInit();

	init();

	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}