#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in vec3 Normal;
in vec3 crntPos;

uniform sampler2D tex0;
uniform vec4 lightColor; // 光の色
uniform vec3 lightPos;   // 光の位置
uniform vec3 camPos;     // カメラの位置

void main()
{
    // 1. 環境光 (Ambient)
    float ambient = 0.2f;

    // 2. 拡散光 (Diffuse)
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - crntPos);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // 3. 鏡面光 (Specular)
    float specularLight = 0.5f; // ハイライトの強さ
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16); // 16はハイライトの鋭さ
    float specular = specAmount * specularLight;

    // 全ての光を足し合わせて、テクスチャの色と掛け合わせる
    FragColor = texture(tex0, texCoord) * lightColor * (ambient + diffuse + specular);
}
