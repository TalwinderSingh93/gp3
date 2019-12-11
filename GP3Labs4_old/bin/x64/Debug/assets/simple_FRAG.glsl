#version 420

in vec4 colour;
in vec2 TexCoord;

out vec4 fragColour;

uniform sampler2D ourTexture;

//also fake
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 PosView;

void main()
{
	fragColour = texture(ourTexture,TexCoord) * colour;
}