in vec3 Color;
in vec3 TexCoord;

out vec4 outColor;

void main()
{
	outColor = vec4(Color, 1.0);
}
