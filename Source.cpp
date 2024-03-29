#include "Source.h"

void debugMatrix(mat4 matrix) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%lf ", matrix[i][j]);

		}
		printf("\n");
	}
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

GLuint loadBMP_custom(const char * imagepath) {
	FILE * file = fopen(imagepath, "rb");
	if (!file) { printf("Image could not be opened\n"); return 0; }
	if (fread(header, 1, 54, file) != 54){ // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}
	if (header[0] != 'B' || header[1] != 'M'){
		printf("Not a correct BMP file\n");
		return 0;
	}
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	int aaa = 0;
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			my_texels[i][j][0] = data[aaa]; aaa++ ;
			my_texels[i][j][1] = data[aaa]; aaa++;
			my_texels[i][j][2] = data[aaa]; aaa++;
			//Sprintf("%c %c %c %c\n", my_texels[i][j][0], my_texels[i][j][1], my_texels[i][j][2], my_texels[i][j][3]);
		}
	}


	//Everything is in memory now, the file can be closed
	fclose(file);
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

vec4 getNormalVector(vec4 v1, vec4 v2, vec4 v3) {
	vec4 nn = normalize(cross(v3 - v2, v2 - v1));
	nn[3] = 0;
	return nn;
}
void assignCoord(int vindex, int i, int j) {
	tex_coord[vindex][0] = (float)j / RESOLUTION;
	tex_coord[vindex][1] = (float)i / RESOLUTION;
}

void writeSurfaceToBuffer(vec4 points[RESOLUTION+1][RESOLUTION+1]) {
	Index = 0;
	for (int i = 0; i < RESOLUTION; i++) {
		for (int j = 0; j < RESOLUTION; j++) {
			vertices[Index] = points[i][j]; assignCoord(Index,i,j); Index++;
			vertices[Index] = points[i + 1][j]; assignCoord(Index, i+1, j); Index++;
			vertices[Index] = points[i][j + 1]; assignCoord(Index, i, j+1); Index++;
			
			
			vec4 normal1 = getNormalVector(vertices[Index-3],vertices[Index-2],vertices[Index-1]);
			normals[i][j][normalIndexes[i][j]++] = normal1;
			normals[i+1][j][normalIndexes[i+1][j]++] = normal1;
			normals[i][j+1][normalIndexes[i][j+1]++] = normal1;


			vertices[Index] = points[i][j + 1]; assignCoord(Index, i, j+1); Index++;
			vertices[Index] = points[i + 1][j]; assignCoord(Index, i+1, j); Index++;
			vertices[Index] = points[i + 1][j + 1];  assignCoord(Index, i+1, j+1); Index++;

			vec4 normal2 = getNormalVector(vertices[Index - 3], vertices[Index - 2], vertices[Index - 1]);
			normals[i][j+1][normalIndexes[i][j+1]++] = normal2;
			normals[i+1][j][normalIndexes[i+1][j]++] = normal2;
			normals[i+1][j+1][normalIndexes[i+1][j+1]++] = normal2;

		}
	}
	Index = 0;
	for (int i = 0; i <= RESOLUTION; i++) {
		for (int j = 0; j <= RESOLUTION; j++) {
			for (int k = 0; k < normalIndexes[i][j]; k++) {
				averageNormals[i][j] += normals[i][j][k];
			}
			averageNormals[i][j] = averageNormals[i][j] / normalIndexes[i][j];
		}
	}
	for (int i = 0; i < RESOLUTION; i++) {
		for (int j = 0; j < RESOLUTION; j++) {

			vNormals[Index] = averageNormals[i][j]; Index++;
			vNormals[Index] = averageNormals[i + 1][j]; Index++;
			vNormals[Index] = averageNormals[i][j + 1]; Index++;

			vNormals[Index] = averageNormals[i][j + 1]; Index++;
			vNormals[Index] = averageNormals[i + 1][j]; Index++;
			vNormals[Index] = averageNormals[i + 1][j + 1]; Index++;

		}
	}

}

void setShading(int type) {
	if (type == Wireframe) {
		activeProgram = programWireframe;
		shadingModel = Wireframe;
		glUseProgram(programWireframe);
		gDrawArraysType = GL_LINES;
	}
	else if (type == Gouroud) {
		activeProgram = programGouroud;
		shadingModel = Gouroud;
		glUseProgram(programGouroud);
		gDrawArraysType = GL_TRIANGLES;
	}
	else if (type == Phong) {
		activeProgram = programPhong;
		shadingModel = Phong;
		glUseProgram(programPhong);
		gDrawArraysType = GL_TRIANGLES;
	}
}

void loadProgram(GLuint &program, const char *vertexshader, const char *fragmentshader) {
	program = InitShader(vertexshader, fragmentshader);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	GLuint texloc = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(texloc);
	glVertexAttribPointer(texloc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices) + sizeof(vNormals)));

	GLuint vProjection = glGetUniformLocation(program, "vProjection");
	glUniformMatrix4fv(vProjection, 1, GL_TRUE, projectionMatrix);



	GLuint vLightAmbient = glGetUniformLocation(program, "vLightAmbient");
	glUniform4fv(vLightAmbient, 1, light_ambient);

	GLuint vLightDiffuse = glGetUniformLocation(program, "vLightDiffuse");
	glUniform4fv(vLightDiffuse, 1, light_diffuse);

	GLuint vLightSpecular = glGetUniformLocation(program, "vLightSpecular");
	glUniform4fv(vLightSpecular, 1, light_specular);

	GLuint vLightPosition = glGetUniformLocation(program, "vLightPosition");
	glUniform4fv(vLightPosition, 1, light_position);

	glGenTextures(1, mytex);
	glBindTexture(GL_TEXTURE_2D, mytex[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, my_texels);


	GLuint tex_loc;
	tex_loc = glGetUniformLocation(activeProgram, "texMap");
	glUniform1i(tex_loc, 0);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


}

void writeKnobsToBuffer() {
	knobIndex = Index;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			vertices[knobIndex++] = vec4(knobs[0][i][j], knobs[1][i][j], knobs[2][i][j], 1.0);
		}
	}
}

void refreshKnobs() {

	createPatchInDimension(knobs);

	writeSurfaceToBuffer(points);

	writeKnobsToBuffer();

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(vNormals), vNormals);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(vNormals), sizeof(tex_coord), tex_coord);
}

void init() {

	glEnable(GL_DEPTH_TEST);
	//projectionMatrix = Ortho(-1.0, 1.0, -1.0, 1.0, -2.0, 2.0);
	projectionMatrix = Perspective(45.0, 1.0, 1.0, 20.0);


	loadBMP_custom("texture.bmp");


	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(vNormals) + sizeof(tex_coord), NULL, GL_STATIC_DRAW);



	refreshKnobs();



	loadProgram(programGouroud, "vshaderG.glsl", "fshaderG.glsl");
	loadProgram(programWireframe, "vshaderW.glsl", "fshaderW.glsl");
	loadProgram(programPhong, "vshaderP.glsl", "fshaderP.glsl");


	
	glClearColor(1.0, 0.0, 1.0, 1.0);
	glPolygonMode(GL_FRONT, GL_FILL);

	setShading(Wireframe);



}

void display() {


	vModelView = glGetUniformLocation(activeProgram, "vModelView");
	modelViewMatrix = Translate(0.0, 0.0, yDist) *Scale(zoomVector) * RotateX(globalRotateX)*RotateY(globalRotateY);
	glUniformMatrix4fv(vModelView, 1, GL_TRUE, modelViewMatrix);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawArrays(gDrawArraysType, 0, Index);

	
	if (shadingModel == Wireframe) {
		glEnable(GL_PROGRAM_POINT_SIZE);
		glDrawArrays(GL_POINTS, Index, activeKnob);
		GLuint selectedLoc = glGetUniformLocation(programWireframe, "selected");
		glUniform1i(selectedLoc, 1);
		glDrawArrays(GL_POINTS, Index + activeKnob, 1);
		glUniform1i(selectedLoc, 0);
		glDrawArrays(GL_POINTS, Index + activeKnob + 1, knobIndex - activeKnob - 1);
	}

	glutSwapBuffers();
}

void rightClickMenu(int id) {
	if (id == 10) {
		isTextureMappingOn = 1 - isTextureMappingOn;

	}
	else {
		setShading(id);	
	}
	GLuint tex_loc;
	tex_loc = glGetUniformLocation(activeProgram, "isTextureMappingOn");
	glUniform1i(tex_loc, isTextureMappingOn);
	glutPostRedisplay();
}

float initialX;
float initialY;

float getRelativeX(int x)  {
	return (float)x / (WIN_W / 2) - 1.0;
}
float getRelativeY(int y) {
	return (float)(WIN_H - y) / (WIN_H / 2) - 1.0;
}
void mouseCallback(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		initialX = getRelativeX(x);
		initialY = getRelativeY(y);	

	}
	else if (button == 3) {
		yDist += 0.1;
	}
	else if (button == 4) {
		yDist -= 0.1;
	}

	glutPostRedisplay();
}

void onMotion(int x, int y) {
	globalRotateX = globalRotateX + (getRelativeY(y) - initialY) / 2 * 180;
	globalRotateY = globalRotateY + (getRelativeX(x) - initialX) / 2 * 180;
	if (globalRotateX >= 360.0) globalRotateX -= 360.0;
	else if (globalRotateX < -360.0) globalRotateX += 360.0;
	if (globalRotateY >= 360.0) globalRotateY -= 360.0;
	else if (globalRotateY < -360.0) globalRotateY += 360.0;

	initialX = getRelativeX(x);
	initialY = getRelativeY(y);
	glutPostRedisplay();
}

void onKeyboard(unsigned char key, int x, int y) {
	int i = activeKnob / 4;
	int j = activeKnob % 4;
	switch (key) {
	case 'q':
		knobs[2][i][j] += DIFF;
		break;
	case 'Q':
		knobs[2][i][j] += DIFF;
		break;
	case 'z':
		knobs[2][i][j] -= DIFF;
		break;
	case 'Z':
		knobs[2][i][j] -= DIFF;
		break;
	case 'w':
		knobs[1][i][j] += DIFF;
		break;
	case 'W':
		knobs[1][i][j] += DIFF;
		break;
	case 's':
		knobs[1][i][j] -= DIFF;
		break;
	case 'S':
		knobs[1][i][j] -= DIFF;
		break;
	case 'a':
		knobs[0][i][j] -= DIFF;
		break;
	case 'A':
		knobs[0][i][j] -= DIFF;
		break;
	case 'd':
		knobs[0][i][j] += DIFF;
		break;
	case 'D':
		knobs[0][i][j] += DIFF;
		break;
	case 9:
		activeKnob++;
		activeKnob %= 16;
		break;
	};
	refreshKnobs();
	glutPostRedisplay();

}

int main(int argc, char **argv) {
	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(1000, 1000);

	glutInitContextVersion(3, 2);

	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("Bezier Curve");

	glutCreateMenu(rightClickMenu);
	glutAddMenuEntry("Wireframe", Wireframe);
	glutAddMenuEntry("Gouroud", Gouroud);
	glutAddMenuEntry("Phong", Phong);
	glutAddMenuEntry("Toggle Texture Mapping", 10);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	glutMouseFunc(mouseCallback);

	glutMotionFunc(onMotion);

	glutKeyboardFunc(onKeyboard);

	glewExperimental = GL_TRUE;

	glewInit();

	init();

	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}