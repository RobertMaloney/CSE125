in vec3 position;
in vec3 normal;
in vec3 color;
in vec2 texcoord;

out vec3 Color;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 camPos;
uniform float hasTex;

struct Light {
	bool valid;
	int mode;

	vec3 position;

	float scalarDiffuse;
	vec3 colorDiffuse;

	float scalarSpecular;
	vec3 colorSpecular;

	float scalarAmbient;
	vec3 colorAmbient;
};

const int MAXLIGHTS = 20;
uniform int numLights;
uniform Light lights[MAXLIGHTS];

void main()
{
	vec4 worldPos = model * vec4(position, 1.0);
	vec3 worldNorm = normalize(mat3(model) * normal);
	TexCoord = vec2(position.x, position.y);

	gl_Position = projection * view * worldPos;
	/*
	// Point light
	vec3 lightvec = normalize(vec3(worldPos) - pointLight);
	float dotted = dot(lightvec, worldNorm);

	Color = vec3(0);
	if (dotted < 0) Color += color / length(lightvec) * -dotted;

	// Direction Light
	vec3 dirlight_norm = normalize(dirLight);
	dotted = dot(dirlight_norm, worldNorm);

	if (dotted < 0) Color += color * vec3(-dotted);
	*/

	Color = vec3(0);
	for (int i = 0; i < numLights; ++i) {
		Light l = lights[i];
		if (!l.valid) continue;

		vec3 diffuse, ambient, specular;

		// AMBIENT
		ambient = l.colorAmbient * vec3(l.scalarAmbient);

		if (l.mode == 0) { // DIRECT
			vec3 lightvec = normalize(l.position);
			float dotted = clamp(-dot(lightvec, worldNorm), 0, 1);
			diffuse = l.colorDiffuse * vec3(dotted) * vec3(l.scalarDiffuse);
			
			// SPECULAR
			vec3 eyeVec = normalize(camPos - vec3(worldPos));
			vec3 reflectVec = reflect(lightvec, worldNorm);
			dotted = clamp( dot(eyeVec, reflectVec), 0, 1);
			specular = l.colorSpecular * vec3(dotted) * vec3(l.scalarSpecular);
		} else if (l.mode == 1) { // POINT
			vec3 lightvec = normalize(vec3(worldPos) - l.position);
			float distance = length(lightvec);
			float dotted = clamp(-dot(lightvec, worldNorm), 0, 1);
			diffuse = l.colorDiffuse * vec3(dotted) * vec3(l.scalarDiffuse) / distance;
			
			// SPECULAR
			vec3 eyeVec = normalize(camPos - vec3(worldPos));
			vec3 reflectVec = reflect(lightvec, worldNorm);
			dotted = clamp( dot(eyeVec, reflectVec), 0, 1);
			specular = l.colorSpecular * vec3(dotted) * vec3(l.scalarSpecular);
		}
		Color += color * (diffuse + ambient + specular);
	}
}

