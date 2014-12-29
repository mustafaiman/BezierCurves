#version 150
in vec2 st;
uniform sampler2D texMap;
uniform bool isTextureMappingOn;
in  vec4 color;
out vec4 fColor;

void main() 
{ 
	//No fragment processing simply output the interpolated vertex color.
	fColor = color;
	if(isTextureMappingOn == true) {
		fColor = fColor *texture2D(texMap, st);
	}

}

