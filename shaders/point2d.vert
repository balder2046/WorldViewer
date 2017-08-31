#version 330 core
 
layout(location = 0) in vec2 vVertex;	//object space vertex position
layout(location = 1) in float PointSize;
layout(location = 2) in vec4 Color;

//output from the vertex shader
smooth out vec4 vSmoothColor;		//smooth colour to fragment shader
smooth out float pointSize;
void main()
{
	//assign the per-vertex colour to vSmoothColor varying

    gl_Position = vec4(vVertex.x,vVertex.y,0.0,1.0);
    gl_PointSize = PointSize;
   vSmoothColor = Color;

}