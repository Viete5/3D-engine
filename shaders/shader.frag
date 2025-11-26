#version 330 core
in vec3 norCoord;
in vec2 TexCoord;
in vec3 fragPos;
out vec4 FragColor;
uniform vec3 lightPos;
uniform vec3 LightColour;
uniform sampler2D ourTexture;
void main()
{
    vec3 Light = normalize(lightPos-fragPos);
    float Ifactor = max(0.0, dot(norCoord,Light));
    vec4 Cmat = texture(ourTexture, TexCoord);
    FragColor = vec4(Cmat.rgb * LightColour * Ifactor,Cmat.a);
}