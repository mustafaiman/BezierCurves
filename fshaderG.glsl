#version 150
in vec2 st;
uniform sampler2D texMap;
in  vec4 color;
out vec4 fColor;

void main() 
{ 
	//No fragment processing simply output the interpolated vertex color.
	fColor = color*texture2D(texMap, st);

}

