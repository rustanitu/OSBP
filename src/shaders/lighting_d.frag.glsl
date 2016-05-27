#version 450

in vec2 st;

uniform sampler2D vertex_tex;
uniform sampler2D normal_tex;
uniform sampler2D amb_tex;
uniform sampler2D diff_tex;
uniform sampler2D spec_tex;

uniform vec3 wrl_eye;
uniform vec3 gamb;

uniform int nlight;
uniform vec3 lpos[20];
uniform vec3 lamb[20];
uniform vec3 ldiff[20];
uniform vec3 lspec[20];
uniform vec3 lspot[20];

out vec3 color;
void main()
{
  // Read attributes from texture
  vec3 vertex = texture(vertex_tex, st).xyz;
  vec3 normal = normalize(texture(normal_tex, st).xyz);
  vec3 mamb = texture(amb_tex, st).rgb;
  vec3 mdiff = texture(diff_tex, st).rgb;
  vec3 mspec = texture(spec_tex, st).rgb;
  float mshi = texture(spec_tex, st).a;

  // Global Ambient Contribuition
  color = mamb * gamb;

  vec3 eye = normalize(wrl_eye - vertex);
  
  // Light Contibuition
  for (int i = 0; i < nlight && i < 20; i++)
  {
    vec3 light = normalize(lpos[i] - vertex);
  
    // Diffuse
    float ndotl = dot(normal, light);
    vec3 cdiff = mdiff * max(ndotl, 0.0f);
  
    // Specular
    vec3 cspec = vec3(0.0f);
    if (ndotl > 0.0f)
    {
      vec3 refl = normalize(reflect(-light, normal));
      cspec = mspec * pow(max(dot(refl, eye), 0.0f), mshi);
    }
  
    // Final Color
    vec3 c = mamb * lamb[i];
    c += cdiff * ldiff[i];
    c += cspec * lspec[i];
    float dist = length(vertex - light);
    float katt = nlight + dist;
    katt += dist * dist;
    float kspot = pow(max(dot(-light, normalize(lspot[i] - lpos[i])), 0.0f), 32.0f) * nlight;
    c *= kspot;
    c /= katt;
    color += c;
  }
}