#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 vertexPosition_cameraspace;

struct FogParam
{
	vec3 color;		// Fog color
	float start;	// For linear fog
	float end;		// For linear fog
	float density;	// For exponential fog
	int type;		// 0 = linear, 1 = exp, 2 = exp2
	bool enabled;	// Toggle fog
};

float getFogFactor(FogParam fog, float fogDistance)
{
	if(fog.enabled == false)
		return 0.0;

	float factor = 0.0;
	if(fog.type == 0) 
		factor = (fog.end - fogDistance) / (fog.end - fog.start); 
	else if(fog.type == 1) 
		factor = exp(- fog.density * fogDistance); 
	else if(fog.type == 2) 
		factor = exp(- pow(fog.density * fogDistance, 2.0) ); 
       
	factor = 1.0 - clamp(factor, 0.0, 1.0); 
    
	return factor;
}

uniform sampler2D texture_diffuse1;
uniform FogParam fogParam;

void main()
{
     vec4 texel = texture(texture_diffuse1, TexCoords);
    if(texel.a < 0.5)
        discard;
    FragColor = texel;
	
	if(fogParam.enabled)
	{
		float fogDistance = abs(vertexPosition_cameraspace.z);
		float fogFactor = getFogFactor(fogParam, fogDistance);
		FragColor.xyz = mix(FragColor.xyz, fogParam.color, fogFactor);
	}
}