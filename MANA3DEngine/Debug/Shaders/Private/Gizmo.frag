#version 330 core
out vec4 FragColor;

in vec2 fragTexCoords;

uniform sampler2D diffuseMap;

void main()
{             
	vec4 texColor = texture( diffuseMap, fragTexCoords );
    if( texColor.a < 0.1 )
        discard;
    FragColor = texColor;
}