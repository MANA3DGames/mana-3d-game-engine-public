#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;
layout (location = 5) in ivec4 vJointIds;
layout (location = 6) in vec4 vJointWeights;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoords;


const int MAX_JOINT_COUNT = 100;
const int MAX_JOINT_INFLUENCER_COUNT = 4;



uniform mat4 model;
layout (std140) uniform VS_CameraMatrices
{
    mat4 projection;
    mat4 view;
};


struct MMa 
{
    mat4 matrix;
};
uniform MMa jointTrans[MAX_JOINT_COUNT];


void main()
{
    mat4 jTransform =  jointTrans[vJointIds[0]].matrix * vJointWeights[0];
    jTransform      += jointTrans[vJointIds[1]].matrix * vJointWeights[1];
    jTransform      += jointTrans[vJointIds[2]].matrix * vJointWeights[2];
    jTransform      += jointTrans[vJointIds[3]].matrix * vJointWeights[3];

    fragPosition = vec3( jTransform * model * vec4( vPosition, 1.0 ) );
    fragNormal = mat3( jTransform * transpose( inverse( model ) ) ) * vNormal;
    fragTexCoords = vTexCoords;
    
    gl_Position = projection * view * vec4( fragPosition, 1.0 );
}
