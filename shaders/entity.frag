
// Calculates Phong colour at each fragment.
// Ambient, diffuse and specular terms.
// Uses interpolated position and normal values passed from vertex shader.

#version 330

in vec4 vertex;
in vec3 normal;
in vec2 st;
in vec4 shadowCoord;

layout(location = 0) out vec4 fragColour;

uniform sampler2D texMap;
uniform samplerCube cubeMap;
uniform sampler2DShadow shadowMap;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat4 inv_view;

// Light parameters
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

uniform vec3 mtl_ambient;   // Ambient surface colour
uniform vec3 mtl_diffuse;   // Diffuse surface colour
uniform vec3 mtl_specular;  // Specular surface colour
uniform vec3 emission;      // Specular surface colour

uniform float shininess;
uniform int render_shadows;

uniform bool use_mtl = false;
uniform float fog_density = 0.02;

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);


// Returns a random number based on a vec3 and an int.
float random(vec3 seed, int i){
    vec4 seed4 = vec4(seed,i);
    float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}


// Multiple lights code from
// http://www.tomdalling.com/blog/modern-opengl/08-even-more-lighting-directional-lights-spotlights-multiple-lights
vec3 ApplyLight(Light light, vec3 surfaceColor, vec3 normal, vec4 vertex_world, vec4 vertex_view) {
    vec3 light_surface_dir;
    float attenuation = 1.0;

    if(light.position.w == 0.0) {
        //directional light
        light_surface_dir = normalize(light.position.xyz);
        attenuation = 1.0; //no attenuation for directional lights
    } else {
        //point light
        light_surface_dir = normalize(vec3(light.position - vertex_world));
        float distanceToLight = length(vec3(light.position - vertex_world));
        attenuation = clamp(1.0 - distanceToLight*distanceToLight/(light.radius*light.radius), 0.0, 1.0);

        //cone restrictions (affects attenuation)
        float lightToSurfaceAngle = acos(dot(-light_surface_dir, normalize(light.coneDirection)));
        if(lightToSurfaceAngle > light.coneAngle){
            attenuation = 0.0;
        }
    }

    //ambient
    vec3 ambient = surfaceColor.rgb * light.ambient;

    //diffuse
    float sDotN = max(0.0, dot(normal, light_surface_dir));
    vec3 diffuse = sDotN * surfaceColor.rgb * light.diffuse;

    // Convert world space variables to view space
    vec3 normal_view = normalize(mat3(view) *normal);
    vec3 view_dir = normalize(-vertex_view.xyz);
    vec3 light_surface_view_dir = normalize(vec3(view * light.position - vertex_view));

    // Specular
    vec3 halfDir = normalize(light_surface_view_dir + view_dir);
    float specAngle = max(dot(halfDir, normal_view), 0.0);

    vec3 reflection = reflect(view_dir, normal_view);
    vec3 r_world = vec3(inv_view * vec4(reflection, 0.0));
    vec3 specular = mtl_diffuse * texture(cubeMap, -r_world).rgb * light.specular * pow(specAngle, shininess);

    return emission + ambient + attenuation*(diffuse + specular);
}

vec3 applyFog( in vec3  rgb,       // original color of the pixel
               in float distance ) // camera to point distance
{
    float fogAmount = 1.0 - exp( -distance*fog_density);
    vec3  fogColor  = vec3(0.5,0.6,0.7);
    return mix(rgb, fogColor, fogAmount);
}

void main(void) {
    vec4 vertex_view = view * vertex;

    vec3 lit_colour = vec3(0);
    float texture_alpha = texture(texMap, st).a;
    // Do not render or blend clear pixels
    if(texture_alpha < 0.1){
        discard;
    }

    // 8 pass poisson sampled shadows 
    float visibility = 1.0;
    if(render_shadows == 1){
        int num_passes = 8;
        for (int i=0;i<8;i++){
            vec3 current = vec3(shadowCoord.xy + poissonDisk[i]/750.0, shadowCoord.z);
            if ( texture(shadowMap, current) <  0.5 ){
                visibility -= 0.5/num_passes;
            }
        }
    }

    for(int i = 0; i < num_lights; ++i){
        lit_colour += ApplyLight(lights[i], vec3(texture(texMap, st)), normal, vertex, vertex_view);
    }

    lit_colour = lit_colour * visibility;
    lit_colour = applyFog(lit_colour,-vertex_view.z);
    fragColour = vec4(lit_colour, texture_alpha);
}