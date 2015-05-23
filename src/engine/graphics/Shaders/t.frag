
in vec2 UV;
out vec4 color;

uniform sampler2D texSampler; // Uchwyt tekstury

void main()
{
  color = texture(texSampler, UV);
}