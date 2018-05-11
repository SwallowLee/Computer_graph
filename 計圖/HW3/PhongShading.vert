#version 150 compatibility

out Vertex{
	vec3 vertexEyeSpace;
	vec3 normal;
    vec2 _texcoord;
}Ver;

void main(){
	
    Ver.vertexEyeSpace = vec3(gl_ModelViewMatrix * gl_Vertex);
    Ver.normal = normalize(gl_NormalMatrix * gl_Normal);
    gl_Position =  gl_ModelViewMatrix * gl_Vertex;
	Ver._texcoord = gl_MultiTexCoord0.xy;
}
