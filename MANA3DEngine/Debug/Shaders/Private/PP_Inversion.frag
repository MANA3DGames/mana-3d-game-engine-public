#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;



const float offset = 1.0 / 300.0; 

void main()
{
	// Inversion
	FragColor = vec4( vec3( 1.0 - texture( screenTexture, TexCoords ) ), 1.0 );
} 