#version 310 es
precision highp float;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
  
  
  struct Light {
    // Point light
    vec3 position;
    float constant;
    float linear;
    float quadratic;

    // Directional light
    vec3 direction;


    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    int lightType;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;


void main()
{
  	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    vec3 norm = normalize(Normal);

    vec3 lightDir;
    if(light.lightType == 0)
    {
        float distance    = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
        lightDir = normalize(light.position - FragPos);
    }
    if(light.lightType == 1)
    {
        lightDir = normalize(-light.direction);
    }
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}