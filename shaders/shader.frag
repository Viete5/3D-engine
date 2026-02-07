#version 330 core
in vec3 norCoord;
in vec2 TexCoord;
in vec3 fragPos;
in vec4 FragPosLightSpace;
out vec4 FragColor;
uniform vec3 lightPos;
uniform vec3 LightColour;
uniform vec4 ObjectColor;
uniform sampler2D shadowMap;
uniform sampler2D ourTexture;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    

    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0)
        return 0.0;

    float currentDepth = projCoords.z;

    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);

    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    
    return shadow;
}
void main()
{
    vec3 color = texture(ourTexture, TexCoord).rgb * ObjectColor.rgb;
    vec3 normal = normalize(norCoord);
    vec3 lightDir = normalize(lightPos - fragPos);
    
    //  Ambient
    vec3 ambient = 0.2 * LightColour;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * LightColour;

    //  Shadow
    float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);

    vec3 lighting = (ambient + (1.0 - shadow) * diffuse) * color;
    
    FragColor = vec4(lighting, 1.0);
}