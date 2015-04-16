in vec3 position;
in vec3 normal;
in vec3 color;

out vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;

void main()
{
	vec4 worldPos = model * vec4(position, 1.0);
	vec3 worldNorm = normalize(mat3(model) * normal);

	gl_Position = projection * view * worldPos;

	vec3 lightvec = normalize(vec3(worldPos) - lightPosition);
	float dotted = dot(lightvec, worldNorm);

	if (dotted > 0) Color = vec3(0);
	else Color = color / length(lightvec) * -dotted;
}

