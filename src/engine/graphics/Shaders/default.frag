in vec3 Position;
in vec3 Normal;
in vec3 Color;
in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D tex;
uniform float hasTex;
uniform int billboard;
uniform vec3 colorOverride;

uniform vec3 camPos;

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

	vec3 inColor = Color;
	float transparency = 1;

	if(hasTex != 0){
		vec4 t = texture(tex, TexCoord);
		inColor = vec3(t);
		transparency = t.a;
	}

	if (billboard != 0)
		inColor = inColor * colorOverride;

	vec3 color = vec3(0);
	for (int i = 0; i < numLights; ++i) {
		Light l = lights[i];
		if (!l.valid) continue;

		vec3 diffuse, ambient, specular;

		// AMBIENT
		ambient = l.colorAmbient * vec3(l.scalarAmbient);

		if (l.mode == 0) { // DIRECT
			vec3 lightvec = normalize(l.position);
			float dotted = clamp(-dot(lightvec, Normal), 0, 1);
			diffuse = l.colorDiffuse * vec3(dotted) * vec3(l.scalarDiffuse);
			
			// SPECULAR
			vec3 eyeVec = normalize(camPos - vec3(Position));
			vec3 reflectVec = reflect(lightvec, Normal);
			dotted = clamp( dot(eyeVec, reflectVec), 0, 1);
			specular = l.colorSpecular * vec3(dotted) * vec3(l.scalarSpecular);
		} else if (l.mode == 1) { // POINT
			vec3 lightvec = normalize(vec3(Position) - l.position);
			float distance = length(lightvec);
			float dotted = clamp(-dot(lightvec, Normal), 0, 1);
			diffuse = l.colorDiffuse * vec3(dotted) * vec3(l.scalarDiffuse) / distance;
			
			// SPECULAR
			vec3 eyeVec = normalize(camPos - vec3(Position));
			vec3 reflectVec = reflect(lightvec, Normal);
			dotted = clamp( dot(eyeVec, reflectVec), 0, 1);
			specular = l.colorSpecular * vec3(dotted) * vec3(l.scalarSpecular);
		}
		color += inColor * (diffuse + ambient + specular);
	}

	outColor = vec4(color,transparency);


}
