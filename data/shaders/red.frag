uniform sampler2D texture;
uniform float stage;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy) ;
    pixel.r = (1.0 - stage) * pixel.r + stage ;
    pixel.g = (1.0 - stage) * pixel.g ;
    pixel.b = (1.0 - stage) * pixel.b ;
    gl_FragColor = pixel * gl_Color;
}
