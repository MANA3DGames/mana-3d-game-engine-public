#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec3 fragPosition;
out vec2 fragTexCoords;

uniform mat4 model;
layout (std140) uniform VS_CameraMatrices
{
    mat4 projection;
    mat4 view;
};


void main()
{
        fragTexCoords = vTexCoords;
        fragPosition = vec3( model * vec4( vPosition, 1.0 ) );
        gl_Position = projection * view * vec4( fragPosition, 1.0 );
}

