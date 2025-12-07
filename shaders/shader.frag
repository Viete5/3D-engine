#version 330 core
in vec3 norCoord;
in vec2 TexCoord;
in vec3 fragPos;
in vec4 FragPosLightSpace;
out vec4 FragColor;
uniform vec3 lightPos;
uniform vec3 LightColour;
uniform sampler2D shadowMap;
uniform sampler2D ourTexture;
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // 1. Перспективное деление (перевод в NDC)
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    
    // 2. Перевод из [-1, 1] в [0, 1] (так как координаты текстуры от 0 до 1)
    projCoords = projCoords * 0.5 + 0.5;

    // 3. Если за пределами карты теней - не затеняем
    if(projCoords.z > 1.0)
        return 0.0;

    // 4. Текущая глубина фрагмента
    float currentDepth = projCoords.z;

    // 5. Борьба с "Shadow Acne" (полосами) через bias
    // Чем больше угол падения света, тем больше смещение
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);

    // 6. Сэмплинг карты теней + простая PCF (сглаживание краев - опционально, пока простое)
    // Здесь мы просто берем значение из текстуры
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    
    // 7. Сравнение: если мы "глубже", чем ближайшая точка к свету -> мы в тени
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    
    return shadow;
}
void main()
{
    vec3 color = texture(ourTexture, TexCoord).rgb;
    vec3 normal = normalize(norCoord);
    vec3 lightDir = normalize(lightPos - fragPos);
    
    // 1. Ambient (Фоновое) - обязательно нужно, иначе тени будут абсолютно черными
    vec3 ambient = 0.2 * LightColour;

    // 2. Diffuse (Рассеянное) - твой старый Ifactor
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * LightColour;

    // 3. Shadow (Тень)
    float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);

    // Итоговая формула: (Ambient + (1.0 - Shadow) * Diffuse) * TextureColor
    vec3 lighting = (ambient + (1.0 - shadow) * diffuse) * color;
    
    FragColor = vec4(lighting, 1.0);
}