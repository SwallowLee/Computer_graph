#version 330 compatibility
#extension GL_EXT_gpu_shader4: enable
#extension GL_EXT_geometry_shader4: enable

layout(triangles) in;
layout(triangle_strip, max_vertices = 200) out;

in vec2 _texcoord[];

out vec3 vertexEyeSpace;
out vec3 normal;
out vec2 tex;

uniform int uLevel = 2;
uniform float uRadius = 1.0;

vec4 center = vec4(0.0, 1.0, 0.0, 1.0);
void ProduceVertex( float s, float t, vec3 V0, vec3 V01, vec3 V02)
{
	vec3 v = V0 + s*V01 + t*V02;
	v = v - (gl_ModelViewMatrix*center).xyz;
	v = normalize(v);
	normal = v;
	v = v * uRadius;
	v = v + (gl_ModelViewMatrix*center).xyz;
	vertexEyeSpace = v;
	gl_Position = gl_ProjectionMatrix * vec4(v, 1.0);
	
	tex = _texcoord[0] + (_texcoord[1]-_texcoord[0])*s + (_texcoord[2]-_texcoord[0])*t;
	EmitVertex( );
}

void main(){
	vec3 V01 = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
	vec3 V02 = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
	vec3 V0 = (gl_in[0].gl_Position).xyz;
	int numLayers;
	if(uLevel != 0)
		numLayers = 1 << uLevel;	//uLevel*2
	else
		numLayers = 1;	//uLevel*2
	
	float size = 1.0 / float(numLayers);

	int j, k;
	for(int i = 1; i <= numLayers; i++)
	{
		k = i;
		for(j = 0; j < i; j++)
		{
			ProduceVertex(size*k, size*j, V0, V01, V02); //丟01 02的比例
			k--;
			ProduceVertex(size*k, size*j, V0, V01, V02); //丟01 02的比例
		}
		ProduceVertex(size*k, size*j, V0, V01, V02); //丟01 02的比例
		EndPrimitive();
	}
}