#shader vertex
#version 330
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 meshColor;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
out vec3 ocolor;
out vec3 facenormal;
out vec3 FragPos;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	ocolor = meshColor;
    FragPos = vec3(model * vec4(aPos, 1.0));
    facenormal = mat3(transpose(inverse(model))) * normal;
}

#shader fragment
#version 330
uniform vec3 viewPos;
in vec3 ocolor;
in vec3 facenormal;
in vec3 FragPos;
out vec4 Fragment;
void main()
{
    vec3 lightColor = vec3(1.0,1.0,1.0);
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    float diffuseStrength = 0.6;
    vec3 lightPos = vec3(1.0,1.0,10.0);
    vec3 norm = normalize(facenormal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * diff * lightColor;

    float specularStrength = 0.2;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * ocolor;
    Fragment = vec4(result, 1.0);
}