#version 150 compatibility

out vec2 _texcoord;

void main(){
    _texcoord = gl_MultiTexCoord0.xy;
    gl_Position = gl_ModelViewMatrix * gl_Vertex;
}
