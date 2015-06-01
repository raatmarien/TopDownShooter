uniform sampler2D texture;

void main()
{
    // lookup the color in the texture
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    
    // determine the rotation based on the vertex alpha
    float alpha = gl_Color.a;
    float angle = 0;
    if (alpha > 0.15 && alpha < 0.35)
        angle = 90;
    if (alpha > 0.4 && alpha < 0.6)
        angle = 180;
    if (alpha > 0.65 && alpha < 0.85)
        angle = 270;
    
    // extract the 2d part of the normal vector
    vec2 oldVector = (color.rgb * 2.0) - 1.0;
    
    // rotate the 2d part of the vector
    float radians = radians(angle);
    
    vec2 vector;    

    vector.x = oldVector.x * cos(radians)
                - (-1.0 * oldVector.y) * sin(radians);
    vector.y = (-1.0 * oldVector.x) * sin(radians)
                + oldVector.y * cos(radians);
    
    // translate the 2d part back to between 0.0 and 1.0
    vector.xy = (vector.xy / 2.0) + 0.5;
    
    // apply the 2d part to the color
    color.xy = vector.xy;
    color.zw = color.zw; 

    // multiply it by the color
    gl_FragColor = color;
}