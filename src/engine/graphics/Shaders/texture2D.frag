
in vec3 Color;
in vec3 TexCoord;

out vec4 outColor;

uniform sampler2D tex;

void main()
{
	outColor = texture(tex, TexCoord);
	//outColor = vec4(Color, 1.0);
}

