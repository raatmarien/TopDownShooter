uniform sampler2D texture;
uniform sampler2D normalTex;
uniform vec2 texPos;
uniform vec2 texSize, normalTexSize;

uniform vec3 lightPos;

void main()
{
    // lookup the pixel in the texture
    vec4 lightAttenuation = texture2D(texture, gl_TexCoord[0].xy);
    
    // Look up normal vector
    vec2 normalPos = (gl_TexCoord[0].xy
                     * (texSize / normalTexSize))
                     + texPos;
    normalPos = vec2(0.0, 1.0) + (vec2(1.0, -1.0)
                                  * normalPos);
    vec4 normalColor = texture2D(normalTex, normalPos); 
    vec3 normal = (normalColor.xyz - 0.5) * 2.0;
    normal = normalize(normal);
    
    // Get light direction
    lightPos.xy = vec2(0.0, 1.0) + (vec2(1.0, -1.0)
                                  * lightPos.xy);
    vec3 lightDir = vec3(lightPos.xy - (normalPos.xy), lightPos.z);
    lightDir = normalize(lightDir);
    
    
    // Calculate final light color
    vec4 finalColor;
    finalColor.a = 1.0;
    finalColor.rgb = lightAttenuation * max(dot(normal, lightDir), 0.0);
    
    // multiply it by the color
    gl_FragColor = finalColor; 
}