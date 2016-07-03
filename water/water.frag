#version 330

in vec4 clipSpace;
in vec2 texCoords;
in vec3 vertCamDir;
in vec4 vertex_view;

out vec4 fragColour;

uniform sampler2D refractionTexture;
uniform sampler2D reflectionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;

uniform vec3 lightColour;

uniform float movement;

uniform float fog_density = 0.02;
const float strength = 0.02;

const float shininess = 30.0;
const float reflectance = 0.3;

const vec3 normal = vec3(0.0, 1.0, 0.0);

vec3 applyFog(vec3 rgb, float distance ){
    float fogAmount = 1.0 - exp( -distance*fog_density);
    vec3  fogColor  = vec3(0.5,0.6,0.7);
    return mix(rgb, fogColor, fogAmount);
}

void main(void) {

    vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;

    vec2 distortedTexCoords = texture(dudvMap, vec2(texCoords.x + movement, texCoords.y)).rg*0.1;
    distortedTexCoords = texCoords + vec2(distortedTexCoords.x, distortedTexCoords.y+movement);
    vec2 totalDistort = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * strength;

    vec2 reflectCoord =  vec2(ndc.x, -ndc.y) + totalDistort;
    reflectCoord.x =  clamp(reflectCoord.x, 0.001, 0.999);
    reflectCoord.y =  clamp(reflectCoord.y, -0.999, -0.001);
    vec2 refractCoord =  clamp(vec2(ndc.x, ndc.y) + totalDistort, 0.001, 0.999);

    float fresnelFactor = dot(normal, normalize(vertCamDir));
    fresnelFactor = clamp(fresnelFactor, 0.2, 0.8);

    vec4 refractionColour = texture(refractionTexture, refractCoord);
    vec4 reflectionColour = texture(reflectionTexture, reflectCoord);

    fragColour = mix(reflectionColour, refractionColour, fresnelFactor);
    fragColour = mix(fragColour, vec4(0.0, 0.3, 0.4, 1.0), 0.2);
    fragColour = vec4(applyFog(fragColour.rgb, -vertex_view.z), 1.0);
}