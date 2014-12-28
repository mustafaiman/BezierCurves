#version 150
in vec4 vPosition;
uniform mat4 vProjection;
uniform mat4 vModelView;
uniform int selected;
out vec4 color;
void main() 
{

	gl_PointSize = 4.0;
	gl_Position = vProjection*vModelView*vPosition;

	if(selected == 1) color = vec4(1.0, 0.0, 0.0, 1.0);
	else color = vec4(0.0, 0.0, 0.0, 1.0);


}