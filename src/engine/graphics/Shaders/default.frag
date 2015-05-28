in vec3 Color;
in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D tex;
uniform float hasTex;

void main()
{
    if(hasTex != 0){
		outColor = texture(tex, TexCoord);
		outColor.z = TexCoord.y;
		outColor.w = 1.0;
	    //outColor = vec4(v.x+Color.x, v.y+Color.y, v.z+Color.z, 1.0) ; 
	}else{
	    outColor = vec4(Color, 1.0);
	}
}
