#version 400

in vec2 textureCoord; 
in vec3 norm;
in vec3 fragPos;

out vec4 fragColor;

uniform sampler2D texture1;
uniform float alpha = 0.3; 

void main()
{
    vec3 greenColor = vec3(1.0, 1.0, 1.0);
    fragColor = vec4(greenColor, alpha) * texture(texture1, textureCoord);
}
