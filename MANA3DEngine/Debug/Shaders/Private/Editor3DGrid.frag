#version 330 core

// ------------------------------
// [Outputs]
// ------------------------------
out vec4 FragColor;

// ------------------------------
// [Uniforms]
// ------------------------------
uniform vec4 fColor;


void main()
{
	float val = 0.35;
    //FragColor = vec4( val, val, val, 1.0 );
	//FragColor = vec4( fColor, 1.0 );
	FragColor = fColor;
}