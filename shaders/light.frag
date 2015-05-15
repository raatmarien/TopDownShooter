uniform sampler2D normalTex;
uniform vec2 texPos;
uniform vec2 texSize, normalTexSize;

uniform float lightHeight;
uniform vec3 falloff;
uniform vec4 lightColor;

void main() {
    // Look up normal vector
    vec2 normalPos = (gl_TexCoord[0].xy
                     * (texSize / normalTexSize))
                     + texPos;
    normalPos = vec2(0.0, 1.0) + (vec2(1.0, -1.0)
                                  * normalPos);
    vec4 normalColor = texture2D(normalTex, normalPos); 
    vec3 normal = (normalColor.xyz - 0.5) * 2.0;
    normal = normalize(normal);
    
    vec3 lightDir = vec3( 1.0 - (gl_TexCoord[0].x - 0.5) * texSize.x
                        , (gl_TexCoord[0].y - 0.5) * texSize.y 
                        ,  lightHeight);// - vec3(normalPos.x * normalTexSize.x
                                          //  , normalPos.y * normalTexSize.y
                                            //, 0);
    // Get light distance
    float dist = length(lightDir.xy);

    // Get light direction
    lightDir = normalize(lightDir);
    
    // Calculate power given by the dot product of the directions
    float power = max(dot(normal, lightDir), 0.0);
    
    // Calculate the light attenuation based on the falloff 
    float attenuation = 1.0 / (falloff.x + falloff.y * dist
                                + falloff.z * dist * dist);    
    attenuation *= (1.0f - sqrt(dist / (texSize.x / 2)));
    
    float totalStrength = attenuation * power;
    
    vec4 finalColor;
    finalColor.rgb = lightColor.rgb * totalStrength;
    finalColor.a = lightColor.a;
 
    //if (dist < 150) 
    //    finalColor = vec4(1,0,0,1);
    gl_FragColor = finalColor;
}