//
// Calculates Phong colour at each fragment.
// Ambient, diffuse and specular terms.
// Uses interpolated position and normal values passed from vertex shader.

#version 330

in vec4 vertex;
in vec3 normal;
in vec2 st;
in vec4 shadowCoord;

layout(location = 0) out vec4 fragColour;

uniform sampler2D blendMap;
uniform sampler2D backMap;
uniform sampler2D rMap;
uniform sampler2D gMap;
uniform sampler2D bMap;
uniform sampler2DShadow shadowMap;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;




#define MAX_LIGHTS 10
uniform int num_lights;
uniform struct Light {
    vec4 position;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float radius;
    float coneAngle;
    vec3 coneDirection;
} lights[MAX_LIGHTS];

uniform float shininess = 32;
uniform float fog_density = 0.02;

vec2 poissonDisk[16] = vec2[](
   vec2(-0.94201624, -0.39906216),
   vec2(0.94558609, -0.76890725),
   vec2(-0.094184101, -0.92938870),
   vec2(0.34495938, 0.29387760),
   vec2(-0.91588581, 0.45771432),
   vec2(-0.81544232, -0.87912464),
   vec2(-0.38277543, 0.27676845),
   vec2(0.97484398, 0.75648379),
   vec2(0.44323325, -0.97511554),
   vec2(0.53742981, -0.47373420),
   vec2(-0.26496911, -0.41893023),
   vec2(0.79197514, 0.19090188),
   vec2(-0.24188840, 0.99706507),
   vec2(-0.81409955, 0.91437590),
   vec2(0.19984126, 0.78641367),
   vec2(0.14383161, -0.1410079 )
);

// Returns a random number based on a vec3 and an int.
float random(vec3 seed, int i){
    vec4 seed4 = vec4(seed,i);
    float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}

// Multiple lights code from
// http://www.tomdalling.com/blog/modern-opengl/08-even-more-lighting-directional-lights-spotlights-multiple-lights
vec3 applyLight(Light light, vec3 surfaceColor, vec3 normal, vec3 surface_pos) {
    vec3 light_dir;
    float attenuation = 1.0;

    if(light.position.w == 0.0) {
        //directional light
        light_dir = normalize(light.position.xyz);
        attenuation = 1.0; //no attenuation for directional lights
    } else {
        //point light
        light_dir = normalize(light.position.xyz - surface_pos);
        float distanceToLight = length(light.position.xyz - surface_pos);
        attenuation = clamp(1.0 - distanceToLight*distanceToLight/(light.radius*light.radius), 0.0, 1.0);

        //cone restrictions (affects attenuation)
        float lightToSurfaceAngle = acos(dot(-light_dir, normalize(light.coneDirection)));
        if(lightToSurfaceAngle > light.coneAngle){
            attenuation = 0.0;
        }
    }

    //ambient
    vec3 ambient = surfaceColor.rgb * light.ambient;

    //diffuse
    float sDotN = max(0.0, dot(normal, light_dir));
    vec3 diffuse = sDotN * surfaceColor.rgb * light.diffuse;

    // Specular is ignored for terrain.
    // Extension: Use a specular map for this

    return ambient + attenuation*(diffuse);
}

vec3 applyFog(vec3 rgb, float cam_point_dist) {
    float fogAmount = 1.0 - exp( -cam_point_dist*fog_density);
    vec3 fogColor = vec3(0.5,0.6,0.7);
    return mix(rgb, fogColor, fogAmount);
}

void main(void) {
    vec4 blendColour = texture(blendMap, st);
    float backgroundAmount = 1.0 - (blendColour.r + blendColour.g + blendColour.b);
    vec2 st_tiled = st * 64.0f;
    vec4 backComponent = texture(backMap, st_tiled) * backgroundAmount;
    vec4 rComponent = texture(rMap, st_tiled) * blendColour.r;
    vec4 gComponent = texture(gMap, st_tiled) * blendColour.g;
    vec4 bComponent = texture(bMap, st_tiled) * blendColour.b;

    vec4 mixedColour = backComponent + rComponent + gComponent + bComponent;
    vec4 cameraSpaceVert = view * vertex;

    // 8 pass poisson sampled shadows
    float visibility = 1.0;
    int num_passes = 8;
    for (int i=0;i<8;i++){
        // int index = int(16.0*random(floor(vertex.xyz*1000.0), i))%16;
        vec3 current = vec3(shadowCoord.xy + poissonDisk[i]/2000.0, shadowCoord.z);
        if ( texture(shadowMap, current) < 0.5 ){
            visibility -= 0.5/num_passes;
        }
    }

    vec3 lit_colour = vec3(0);
    for(int i = 0; i < num_lights; ++i){
        lit_colour += applyLight(lights[i], mixedColour.rgb, normal, vertex.xyz);
    }
    lit_colour = lit_colour * visibility;
    lit_colour = applyFog(lit_colour,-cameraSpaceVert.z);

    fragColour = vec4(lit_colour, 1.0);
}