#version 120

// A LITTLE CHEAT SHEET OF LIGHT PROPERTIES!
//Light properties
// struct gl_LightSourceParameters
// {
//   vec4 position;
//   vec4 halfVector;
//
//   vec4 ambient;
//   vec4 diffuse;
//   vec4 specular;
//
//   vec3 spotDirection;
//   float spotExponent;
//   float spotCutoff;
//   float spotCosCutoff;
//
//   float constantAttenuation;
//   float linearAttenuation;
//   float quadraticAttenuation;
// };
//
// uniform gl_LightSourceParameters gl_LightSource[gl_MaxLights];

uniform sampler2D DiffuseTex;

uniform vec4 SpecularColor;
varying vec2 TexCoord;

varying vec3 VSPosition;
varying vec3 VSNormal;

void main() {
	// light vector
	vec3 L = normalize(gl_LightSource[0].position.xyz - VSPosition);
	// eye vector
	vec3 E = normalize(-VSPosition);
	// half vector using light & eye
	vec3 H = normalize((L+E) * 1.0);

	//ambient color taken directly from the texture
	vec4 ambient  = texture2D(DiffuseTex, TexCoord);
	
	//diffusive component takes the same color as the ambient component and diffuses it	
	vec4 diffusive = ambient * max(dot(VSNormal,L), 0.0);
	//diffusive = clamp(diffusive, 0.0, 10.0);	

	vec4 specular_color = vec4(SpecularColor.rgb, gl_LightSource[0].specular.a);
	vec4 specular = specular_color * pow(max(dot(H,VSNormal),0.0),256.0 * SpecularColor.a);
	//specular = clamp(specular, 0.0, 10.0);
	
	// attenuation
	float attn = 1.0/(gl_LightSource[0].constantAttenuation + 
			  (gl_LightSource[0].linearAttenuation*length(L)) +
			  (gl_LightSource[0].quadraticAttenuation*length(L)));
	
	gl_FragColor = ambient + (diffusive + specular)*attn;

}
