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
uniform int billboard;
uniform float billboardScale;

void main()
{
	Position = vec3(model * vec4(position, 1.0));
	Normal = normalize(mat3(model) * normal);
	Color = color;
	TexCoord = texcoord;

	mat4 bb_mat = view * model;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (i == j) bb_mat[i][j] = billboardScale;
			else bb_mat[i][j] = 0.0;
		}
	}

	if (billboard == 0)
		gl_Position = projection * view * vec4(Position,1.0);
	else
		gl_Position = projection * bb_mat * vec4(position, 1.0);
}

