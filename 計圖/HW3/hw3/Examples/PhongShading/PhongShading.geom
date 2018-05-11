#version 150 compatibility

layout(triangles) in;
layout(triangle_strip, max_vertices=255) out;

in vec2 _texcoord[];

out vec3 vertexEyeSpace;
out vec3 normal; 
out vec2 texcoord;

int level = 2;
float radius = 1.0;

vec4 center = vec4(0.0, 1.0, 0.0, 1.0);

vec4 origin , dt , ds;
vec2 tex_origin , tex_dt , tex_ds;

void ProduceVertex( float s, float t ){

	vec3 tmp = origin.xyz + s * ds.xyz + t * dt.xyz;
	tmp = tmp - (gl_ModelViewMatrix * center).xyz;
	tmp = normalize( tmp );
	normal = tmp;

	tmp = tmp * radius;
	tmp = tmp + (gl_ModelViewMatrix * center).xyz;
	gl_Position = gl_ProjectionMatrix * vec4( tmp , 1.0 );
	vertexEyeSpace = tmp;

	texcoord= tex_origin + s * tex_ds + t * tex_dt;
	
	EmitVertex( );

}

void main( ){

	int numLayers = 1 << level;

	origin = gl_in[0].gl_Position;
	ds = gl_in[1].gl_Position - gl_in[0].gl_Position;
	dt = gl_in[2].gl_Position - gl_in[0].gl_Position;
	ds = ds / float ( numLayers );
	dt = dt / float ( numLayers );
	
	tex_origin = _texcoord[0];
	tex_ds = _texcoord[1] - _texcoord[0];
	tex_dt = _texcoord[2] - _texcoord[0];
	tex_ds = tex_ds / float ( numLayers );
	tex_dt = tex_dt / float ( numLayers );
	
	for(int layer = 0 ;layer < numLayers ; ++layer){	
		int i = layer + 1;
		int j = 0;
		ProduceVertex( i , j );
		for(int k = 0 ; k < layer + 1 ; ++k ){
			--i;
			ProduceVertex( i , j );
			++j;
			ProduceVertex( i , j );
		}	
		EndPrimitive();
	}
}