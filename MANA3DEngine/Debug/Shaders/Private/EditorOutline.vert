#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

// ------------------------------
// [Uniforms]
// ------------------------------
uniform mat4 model;
layout (std140) uniform VS_CameraMatrices
{
    mat4 projection;
    mat4 view;
};


void main()
{
	//gl_Position = projection * view * model * vec4( vPosition, 1.0 );



	//float scaleFactor = 0.1;
	//vec3 scaleVertex = vPosition + vNormal * scaleFactor;
	//gl_Position = projection * view * model * vec4( scaleVertex.xyz, 1.0 );	


    

	float scaleFactor = 0.1;

	mat4 modelviewMatrix = view * model;

	mat3 normalMatrix = mat3( 
	modelviewMatrix[0].xyz,
	modelviewMatrix[1].xyz,
	modelviewMatrix[2].xyz );

	vec3 viewSpaceNormal = normalMatrix * vNormal;

	vec4 replacementPosition = normalize( projection * vec4( viewSpaceNormal, 0.0 ) ) * scaleFactor;

	gl_Position = projection * view * model * vec4( vPosition.xyz, 1.0 ) + replacementPosition;	
}