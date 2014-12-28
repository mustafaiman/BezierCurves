#version 150
in vec4 vPosition;
uniform mat4 vProjection;
uniform mat4 vModelView;
out vec4 color;
void main() 
{

	gl_PointSize = 2.0;
	gl_Position = vProjection*vModelView*vPosition;
	color = vec4(0.0, 0.0, 0.0, 1.0);


}