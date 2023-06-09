#version 330 core

//Attributs des sommets

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTextCoords;

//Matrices uniformes (on les passent depuis le main)

uniform mat4 uMVPMatrix; 
uniform mat4 uMVMatrix; 
uniform mat4 uNormalMatrix;

//variables de sortie
out vec3 vPosition_vs; 
out vec3 vNormal_vs; 
out vec2 vTextCoords; 

void main(){

    vec4 vertexPosition = vec4(aVertexPosition, 1);
    vec4 vertexNormal = vec4(aVertexNormal, 0);

    vPosition_vs = vec3(uMVMatrix * vertexPosition);
    vNormal_vs = vec3(uNormalMatrix*vertexNormal);
    vTextCoords = aVertexTextCoords;

    gl_Position = uMVPMatrix * vertexPosition;
}
