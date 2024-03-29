#version 150

uniform vec4 vLightAmbient;
uniform vec4 vLightDiffuse;
uniform vec4 vLightSpecular;
uniform mat4 vModelView;
uniform vec4 vLightPosition;
uniform sampler2D texMap;
uniform bool isTextureMappingOn;
in vec2 st;
in  vec3 N;
in  vec3 L;
in  vec3 E;
out vec4 fColor;

void main() 
{ 

	float Shininess = 120.0;

	vec3 NN = normalize(N);
	vec3 EE = normalize(E);
	vec3 LL = normalize(L);
	vec4 ambient,diffuse,specular;
	vec3 H = normalize(LL+EE);
	float Kd = max(dot(LL, NN),0.0);
	Kd = dot(LL,NN);
	float Ks = pow(max(dot(NN, H),0.0),Shininess);
	ambient = vLightAmbient;
	diffuse = Kd * vLightDiffuse;
	if(dot(LL,NN) < 0.0) specular = vec4(0.0, 0.0, 0.0, 1.0);
	else specular = Ks*vLightSpecular;
	fColor = vec4((ambient + diffuse + specular).xyz, 1.0);
	if (isTextureMappingOn == true) {
		fColor = fColor *texture2D(texMap, st);
	}

}
