#pragma once
#ifndef Light
// #define Light

enum LightType {
    Directional,
    Point,
    Spot
};

class Light {
public:
    Light();
    
    float lightStrength;

    LightType lightType;
    
    glm::vec3 direction;
    
    float cutOff;
    float outerCutOff;
    
    glm::vec3 position;
    // float radius; // https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    

    // glm::vec3 lightColor; // replace by ambient, diffuse and specular
    glm::vec3 ambient;  // you may need to set this value at: glm::vec3(0.2f, 0.2f, 0.2f) if there is only one single light
    glm::vec3 diffuse;  // you may need to set this value at: glm::vec3(0.5f, 0.5f, 0.5f) if there is only one single light
    glm::vec3 specular; // you may need to set this value at: glm::vec3(1.0f, 1.0f, 1.0f) if there is only one single light



private:

};


#endif