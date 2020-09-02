#version 330 core

struct DirLight 
{
    vec3 direction;
	
    float ambient;
    float diffuse;

	int isActive;
};

struct PointLight 
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    float ambient;
    float diffuse;

	int isActive;
};

struct SpotLight 
{
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    float ambient;
    float diffuse; 
	
	int isActive;    
};

#define DIRECTIONAL_MAX_COUNT 4
#define POINT_LIGHT_MAX_COUNT 4
#define SPOT_LIGHT_MAX_COUNT 4

// ------------------------------
// [Inputs]
// ------------------------------
in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoords;

// ------------------------------
// [Outputs]
// ------------------------------
out vec4 FragColor;

// ------------------------------
// [Uniforms]
// ------------------------------
uniform vec3 viewPos;
uniform bool gammaCorrection;

uniform int directionalLightsCount;
uniform int pointLightsCount;
uniform int spotLightsCount;

uniform DirLight dirLights[DIRECTIONAL_MAX_COUNT];
uniform PointLight pointLights[POINT_LIGHT_MAX_COUNT];
uniform SpotLight spotLights[SPOT_LIGHT_MAX_COUNT];

uniform sampler2D diffuseMap;


// ------------------------------
// [Function Prototypes]
// ------------------------------
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);




void main()
{    
    // properties
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPosition);
    
    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================

	vec3 result;

    // phase 1: directional lighting
	for(int i = 0; i < directionalLightsCount; i++)
		result += CalcDirLight( dirLights[i], norm, viewDir );

    // phase 2: point lights
    for(int i = 0; i < pointLightsCount; i++)
        result += CalcPointLight(pointLights[i], norm, fragPosition, viewDir);    
    
	// phase 3: spot light
	for(int i = 0; i < spotLightsCount; i++)
		result += CalcSpotLight(spotLights[i], norm, fragPosition, viewDir);    
    
    FragColor = vec4(result, 1.0);

	// Apply gamma correction
	if ( gammaCorrection )
	{
		float gamma = 2.2;
		FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
	}
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(diffuseMap, fragTexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseMap, fragTexCoords));

    vec3 result = ambient + diffuse;
    //if ( length( result ) > 0.1f )
    //    result += lightColor;
    return result;
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(diffuseMap, fragTexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseMap, fragTexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    return (ambient + diffuse);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(diffuseMap, fragTexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseMap, fragTexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    return (ambient + diffuse);
}

