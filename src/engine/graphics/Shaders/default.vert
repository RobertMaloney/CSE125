in vec3 position;
in vec3 normal;
in vec3 color;
in vec2 texcoord;

out vec3 Color;
out vec3 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 pointLight;
uniform vec3 dirLight;

void main()
{
	vec4 worldPos = model * vec4(position, 1.0);
	vec3 worldNorm = normalize(mat3(model) * normal);
	TexCoord = position;

	gl_Position = projection * view * worldPos;

	// Point light
	vec3 lightvec = normalize(vec3(worldPos) - pointLight);
	float dotted = dot(lightvec, worldNorm);

	Color = vec3(0);
	if (dotted < 0) Color += color / length(lightvec) * -dotted;

	// Direction Light
	vec3 dirlight_norm = normalize(dirLight);
	dotted = dot(dirlight_norm, worldNorm);

	if (dotted < 0) Color += color * vec3(-dotted);
}

