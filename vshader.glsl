#version 150
in  vec4 vPosition;
in	vec4 vColor;
uniform mat4 vProjection;
uniform mat4 vModelView;
out vec4 color;
void main() 
{
	color = vColor;
    gl_Position = vProjection*vModelView*vPosition;
}