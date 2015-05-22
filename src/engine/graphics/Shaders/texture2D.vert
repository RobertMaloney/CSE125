// Basic vertex shader for 2D texturing.
in vec3 position;
in vec3 normal;
in vec3 color;
in vec2 texcoord;

out vec3 Color;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 scale;

void main()
{
	//gl_Position = vec4(position, 1.0);//projection * position?
	gl_Position = vec4( position.x * scale.x, position.y * scale.y, position.z, 1 );
	TexCoord = texcoord;//vec2(texcoord.x,texcoord.y);
	Color = color;
}