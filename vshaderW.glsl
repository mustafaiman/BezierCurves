#version 150
in vec4 vPosition;
in vec4 vNormal;
uniform vec4 vLightAmbient;
uniform vec4 vLightDiffuse;
uniform vec4 vLightSpecular;
uniform vec4 vLightPosition;
uniform mat4 vProjection;
uniform mat4 vModelView;
uniform int shadingType;
out vec4 color;
void main() 
{

	gl_Position = vProjection*vModelView*vPosition;
	color = vec4(0.0, 0.0, 0.0, 1.0);


}