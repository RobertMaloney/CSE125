// Basic vertex shader for 2D texturing.
in vec3 position;
in vec3 normal;
//in vec3 color;
in vec2 texcoord;

//out vec3 Color;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 scale;

void main()
{
    
	gl_Position = vec4( (position.x - 0.5) * scale.x, (position.y - 0.5) * scale.y, position.z, 1 );

	TexCoord =vec2(position.x, position.y);

}