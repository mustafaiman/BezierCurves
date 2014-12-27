#version 150

in  vec4 color;
out vec4 fColor;

void main() 
{ 
	//No fragment processing simply output the interpolated vertex color.
    fColor = color;
}

