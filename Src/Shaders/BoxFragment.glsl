#version 330 core
in vec3 Normal;
in vec3 worldPos;
in vec2 Texcoord;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
//    vec3 diffuse;
//    vec3 specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform DirectionalLight directionalLight;
//int a = 4;
uniform PointLight pointLight[4];
uniform SpotLight spotLight;

uniform vec3 lightPos;
uniform vec3 camPos;


void main() {
    vec3 result = vec3(0.0f);
    
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - worldPos);
    vec3 L = vec3(1.0f);
    vec3 H = vec3(1.0f);
    
    vec4 baseTex = texture(material.diffuse, Texcoord);
    vec4 specularTex = texture(material.diffuse, Texcoord);
    
    // Directional Light Contribution
    L = normalize(-directionalLight.direction);
    H = normalize(L + V);
    
    vec3 ambient = baseTex.rgb * directionalLight.ambient;
    vec3 diffuse = baseTex.rgb * max(dot(N, L), 0.0) * directionalLight.diffuse;
    vec3 specular = specularTex.rgb * pow(max(dot(N, H), 0.0), material.shininess) * directionalLight.specular;
    
    // Point Light Contribution
    for (int i = 0; i < 4; i++) {
        L = normalize(pointLight[i].position - worldPos);
        H = normalize(L + V);

        float distance = length(pointLight[i].position - worldPos);
        float attenuation = 1.0 / (pointLight[i].constant + pointLight[i].linear * distance + pointLight[i].quadratic * distance * distance);

        ambient += baseTex.rgb * pointLight[i].ambient * attenuation;
        diffuse += baseTex.rgb * pointLight[i].diffuse * max(dot(N, L), 0.0) * attenuation;
        specular += specularTex.rgb * pointLight[i].specular * pow(max(dot(H, L), 0.0), material.shininess) * attenuation;
    }

    // Spot Light Contribution
    // 反余弦是一个开销很大的计算。我们应该使用余弦值来节约性能
    L = normalize(spotLight.position - worldPos);
    vec3 spotDir = normalize(-spotLight.direction);
    float theta = dot(L, spotDir); // Remember, we have turned this value into cosine type
    if (theta > spotLight.outerCutOff) {
        H = normalize(L + V);
        float distance = length(spotLight.position - worldPos);
        float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));

        float epsilon = spotLight.cutOff - spotLight.outerCutOff;
        float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

        ambient += baseTex.rgb * spotLight.ambient * attenuation;
        diffuse += baseTex.rgb * spotLight.diffuse * max(dot(N, L), 0.0f) * intensity * attenuation;
        specular += specularTex.rgb * spotLight.specular * pow(max(dot(H, L), 0.0), material.shininess) * intensity * attenuation;
    } 
//    else {
//        // 否则，使用环境光，让场景在聚光之外时不至于完全黑暗
//        ambient += baseTex.rgb * spotLight.ambient;
//    }
    
//    result = ambient + diffuse;
    result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
