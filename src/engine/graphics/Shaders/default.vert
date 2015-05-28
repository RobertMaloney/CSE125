in vec3 position;
in vec3 normal;
in vec3 color;
in vec2 texcoord;


out vec3 Position;
out vec3 Normal;
out vec3 Color;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float hasTex;

void main()
{
	Position = vec3(model * vec4(position, 1.0));
	Normal = normalize(mat3(model) * normal);
	Color = color;
	TexCoord = texcoord;

	gl_Position = projection * view * vec4(Position,1.0);

	

	
}

