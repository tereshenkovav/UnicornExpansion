uniform sampler2D texture;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy)*1.25 ;
    gl_FragColor = pixel * gl_Color;
}
