uniform sampler2D texture;
uniform float angle;

void main()
{
    // lookup the color in the texture
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    
    // extract the 2d part of the normal vector
    vec2 vector = (color.rgb * 2.0) - 1.0;
    
    // rotate the 2d part of the vector
    float radians = radians(angle);
    vector.x = vector.x * cos(radians)
                - (-1.0 * vector.y) * sin(radians);
    vector.y = (-1.0 * vector.x) * sin(radians)
                + vector.y * cos(radians);
    
    // translate the 2d part back to between 0.0 and 1.0
    vector.xy = (vector.xy / 2.0) + 0.5;
    
    // apply the 2d part to the color
    vec4 newColor;
    newColor.xy = vector.xy;
    newColor.zw = color.zw; 

    // multiply it by the color
    gl_FragColor = gl_Color * newColor;
}