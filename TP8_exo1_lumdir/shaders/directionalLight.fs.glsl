#version 330 core

in vec3 vPosition_vs;
in vec3 vNormal_vs;
// in vec2 vTextCoords;

uniform vec3 uLightDir;
uniform vec3 uLightIntensity;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

//uniform sampler2D uTexture;

out vec4 fFragColor;


vec3 blinnPhong(){
    vec3 Li = uLightIntensity;
    vec3 N = vNormal_vs;
    vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightDir);
    vec3 halfVector = (w0+wi)/2.f;

    //la formule imbuvable du tp
    return Li * ( uKd * dot(wi,N) + uKs * pow(dot(halfVector, N), uShininess));
    // return Li*(uKd*max(dot(wi, N), 0.) + uKs*max(pow(dot(halfVector, N), 0.), uShininess));
}

void main(){
    //vec2 fCorrectCoords = vec2(vTextCoords.x, 1-vTextCoords.y);
    fFragColor = vec4(blinnPhong(),1.);

    //fFragColor = texture(uTexture, fCorrectCoords);
}

