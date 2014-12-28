#version 150
in vec4 vPosition;
in vec4 vNormal;
uniform vec4 vLightAmbient;
uniform vec4 vLightDiffuse;
uniform vec4 vLightSpecular;
uniform vec4 vLightPosition;
uniform mat4 vProjection;
uniform mat4 vModelView;
out vec3 N;
out vec3 L;
out vec3 E;
out vec4 color;
void main() 
{

	float Shininess = 120.0;

	gl_Position = vProjection*vModelView*vPosition;


	vec3 pos = (vModelView * vPosition).xyz;

	vec3 L = normalize( vLightPosition.xyz - pos );
	vec3 E = normalize( -pos );
	vec3 H = normalize( L + E );

	// Transform vertex normal into eye coordinates
	vec3 N = normalize( vModelView*vec4(vNormal.xyz, 0.0) ).xyz;

	// Compute terms in the illumination equation
	vec4 ambient = vLightAmbient;

	float Kd = max( dot(L, N), 0.0 );
	vec4 diffuse = Kd*vLightDiffuse;

	float Ks = pow( max(dot(N, H), 0.0), Shininess );
	vec4 specular = Ks * vLightSpecular;

	if( dot(L, N) < 0.0 ) {
		specular = vec4(0.0, 0.0, 0.0, 1.0);
	}

	color = ambient + diffuse + specular;
	color.a = 1.0;



}