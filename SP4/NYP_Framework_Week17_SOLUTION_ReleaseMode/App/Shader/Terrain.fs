#version 330

smooth in vec2 vTexCoord;
smooth in vec3 vNormal;
smooth in vec3 vWorldPos;
smooth in vec4 vEyeSpacePos;
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


uniform sampler2D gSampler[5];
uniform sampler2D shadowMap;

uniform vec4 vColor;
uniform FogParam fogParam;

//#include "dirLight.frag"
//uniform DirectionalLight sunLight;
uniform float fRenderHeight;
uniform float fMaxTextureU;
uniform float fMaxTextureV;

out vec4 outputColor;

void main()
{
	vec3 vNormalized = normalize(vNormal);
	
	vec4 vTexColor = vec4(0.0);
	
	float fScale = vWorldPos.y/fRenderHeight; // y/15

	const float fRange1 = 0.15f;
	const float fRange2 = 0.3f;
	const float fRange3 = 0.65f;
	const float fRange4 = 0.85f;

	if(fScale >= 0.0 && fScale <= fRange1){
        vTexColor = texture(gSampler[0], vTexCoord); // fungus
    }
	else if(fScale <= fRange2)
	{
		fScale -= fRange1;
		fScale /= (fRange2-fRange1);
		
		float fScale2 = fScale;
		fScale = 1.0-fScale; 
		
		vTexColor += texture(gSampler[0], vTexCoord)*fScale; // fungus
		vTexColor += texture(gSampler[1], vTexCoord)*fScale2; // sandgrass
	}
	else if(fScale <= fRange3){ 
        vTexColor = texture(gSampler[1], vTexCoord); // sandgrass
    }
	else if(fScale <= fRange4)
	{
		fScale -= fRange3;
		fScale /= (fRange4-fRange3);
		
		float fScale2 = fScale;
		fScale = 1.0-fScale; 
		
		vTexColor += texture(gSampler[1], vTexCoord)*fScale; // sandgrass
		vTexColor += texture(gSampler[2], vTexCoord)*fScale2; // rock
	}
	else vTexColor = texture(gSampler[2], vTexCoord); // rock

	vec2 vPathCoord = vec2(vTexCoord.x/fMaxTextureU, vTexCoord.y/fMaxTextureV);
	vec4 vPathIntensity = texture(gSampler[4], vPathCoord);
	fScale = vPathIntensity.x;
  
	vec4 vPathColor = texture(gSampler[3], vTexCoord); // Black color means there is a path, sand
	vec4 vFinalTexColor = fScale*vTexColor+(1-fScale)*vPathColor;

	vec4 vMixedColor = vFinalTexColor*vColor;
	//vec4 vDirLightColor = GetDirectionalLightColor(sunLight, vNormal);

	if(fogParam.enabled)
	{
		float fogDistance = abs(vertexPosition_cameraspace.z);
		float fogFactor = getFogFactor(fogParam, fogDistance);
		vMixedColor.xyz = mix(vMixedColor.xyz, fogParam.color, fogFactor);
	}

	outputColor = vMixedColor;//*(vDirLightColor);
  
}                      