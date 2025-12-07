#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNor;
out vec2 TexCoord;
out vec3 norCoord;
out vec3 fragPos;
out vec4 FragPosLightSpace;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMatrix;
uniform mat4 lightSpaceMatrix;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTex;
    fragPos = vec3(model*vec4(aPos,1.0));
    norCoord = normalize(mat3(normalMatrix)*aNor);
    FragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
}
