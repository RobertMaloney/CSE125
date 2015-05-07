in vec3 position;
in vec3 normal;
in vec3 color;
in vec2 texcoord;

out vec3 Color;
out vec3 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * vec4(position, 1.0);
	TexCoord = position;
	Color = color;
}

