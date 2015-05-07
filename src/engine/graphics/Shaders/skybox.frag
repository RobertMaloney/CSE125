in vec3 Color;
in vec3 TexCoord;

out vec4 outColor;

uniform samplerCube skybox;

void main()
{
	outColor = texture(skybox, TexCoord);
	//outColor = vec4(Color, 1.0);
}
