#version 330 core
struct Materials{
    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Materials material;
uniform Light light;

in vec3 Normal;
in vec3 FragPos;
in vec2 Texcoord;

out vec4 color;

//uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 lightDir;

void main()
{
    vec3 ambient = material.ambient * light.ambient;

    //vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.5f);
//    vec3 diffuse = material.diffuse * diff * light.diffuse;
    vec3 diffuse = texture(material.diffuse, Texcoord).rgb * diff * max(light.diffuse, vec3(0.3f, 0.3f, 0.3f));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 spec_tex = texture(material.specular, Texcoord).rgb;
    vec3 specular = spec_tex * spec * light.specular;

    vec3 emission = texture(material.emission, Texcoord).rgb * diff * max(light.diffuse, vec3(0.7f));
    float temp = step(spec_tex.r, 0.0f);  // if spec_tex.r > 0.0f, then return 0 else 1
    temp = min(step(spec_tex.g, 0.0f), temp);
    temp = min(step(spec_tex.b, 0.0f), temp); // temp == 0: spec_tex is not black; 1: black;

    // we complete the problem four without using if sentence.
    emission = emission * temp;

    vec3 result = ambient + diffuse + specular + emission;
//    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0f);
}
