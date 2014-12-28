#version 150
in vec4 vPosition;
in vec4 vNormal;
uniform vec4 vLightPosition;
uniform mat4 vProjection;
uniform mat4 vModelView;
out vec3 N;
out vec3 L;
out vec3 E;
void main() 
{

	gl_Position = vProjection*vModelView*vPosition;
	N = vNormal.xyz;
	L = vLightPosition.xyz - (vModelView*vPosition).xyz;
	if (vLightPosition.w == 0.0) L = vLightPosition.xyz;
	E = (vModelView*vPosition).xyz;


}