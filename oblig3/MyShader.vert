#version 120

varying vec3 VSPosition;
varying vec3 VSNormal;
varying vec2 TexCoord;

void main() {
  // Insert the vertex shader here
  // Transform the vertex position to view space for fragment sahder
  // Transform the normal to view space for fragment sahder (remeber the matrix!)
  // Pass on the texture coordinates
   VSPosition = vec3(gl_ModelViewMatrix * gl_Vertex);       
   VSNormal = normalize(gl_NormalMatrix * gl_Normal);
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   TexCoord = gl_MultiTexCoord0.xy;
}
