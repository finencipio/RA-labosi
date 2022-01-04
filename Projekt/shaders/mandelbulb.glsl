#version 450 core

#define MIN_EPSILON 1e-4
#define minRange 1e-5
#define Bailout 2.0

out vec4 outColor;

uniform vec2 resolution;
uniform float power;
uniform vec3  cameraPosition;
uniform mat3 cameraRotation;

const float cameraFocalLength = 1;

float aspectRatio = resolution.x / resolution.y;

vec3  w = vec3(0, 0, 1);
vec3  v = vec3(0, 1, 0);
vec3  u = vec3(1, 0, 0);

const vec3 lightColor1 = vec3(0.968, 0.972, 0.686);
const vec3 lightColor2 = vec3(0.533, 0.478, 0.145);
const vec3 darkColor = vec3(0, 0, 0);
const vec3 glowColor = vec3(0.980, 0.870, 0.639);

const int stepLimit = 300;
const int colorIterations = 12;

vec3 rayDirection(vec2 pixel)
{
    vec2 p = (0.5 * resolution - pixel) / vec2(resolution.x, -resolution.y);
    p.x *= aspectRatio;
    vec3 d = (p.x * u + p.y * v - cameraFocalLength * w);
    
    return normalize(cameraRotation * d);
}

vec2 DE(vec3 w)
{
    vec3 z = w;
    float dr = 1.0;
    float r  = length(z);

    int steps;
    
    for (int i = 0; i < colorIterations; i++) {
        steps = i;
        r = length(z);
		if (r > Bailout) break;

		float theta = acos(z.z/r);
		float phi = atan(z.y,z.x);
		dr =  pow( r, power-1.0)*power*dr + 1.0;

		float zr = pow(r, power);
		theta = theta * power;
		phi = phi * power;

		z = zr*vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z+=w;
    }

    return vec2(0.5 * log(r) * r / dr, steps);
}

bool intersectBoundingSphere(vec3 origin,
                             vec3 direction,
                             out float tmin,
                             out float tmax)
{
    bool hit = false;
    float b = dot(origin, direction);
    float c = dot(origin, origin) - 9.0;
    float disc = b*b - c;
    tmin = tmax = 0.0;

    if (disc > 0.0) {
        float sdisc = sqrt(disc);
        float t0 = -b - sdisc;
        float t1 = -b + sdisc;

        if (t0 >= 0.0) {
            tmin = t0;
            tmax = t0 + t1;
        } else if (t0 < 0.0) {
            tmax = t1;
        }
        hit = true;
    }

    return hit;
}

vec4 render(vec2 pixel)
{
    vec3  ray_direction = rayDirection(pixel);
    float ray_length;
    vec3  ray = cameraPosition + ray_length * ray_direction;
    
    float eps = MIN_EPSILON;
    vec2  dist;
    int   steps = 0;
    bool  hit = false;
    float tmin = 0.0;
    float tmax = 50.0;
    
    vec4 color;
    
    if (intersectBoundingSphere(ray, ray_direction, tmin, tmax)) {
        ray_length = tmin;
        ray = cameraPosition + ray_length * ray_direction;
        
        for (int i = 0; i < stepLimit; i++) {
            steps = i;
            dist = DE(ray);
            
            if (ray_length > tmax) {
                break;
            }

            ray_length += dist.x;
            ray = cameraPosition + ray_length * ray_direction;
            eps = ray_length * MIN_EPSILON;

            if (dist.x < eps) {
                hit = true;
                break;
            }
        }
    }

    float hiddenFactor = min(steps * 2.0 / stepLimit, 1.0);

    if (hit) {
        vec3 lcolor = mix(lightColor1, lightColor2, dist.y * 1.0 / colorIterations);
        color.rgb = mix(lcolor, darkColor, hiddenFactor);
        color.a = 1.0;
    } else {
        color.rgb = mix(darkColor, glowColor, hiddenFactor);
        color.a = 1.0;
    }
    
    return color;
}

void main()
{

    // Output to screen
    outColor = render(gl_FragCoord.xy);

}