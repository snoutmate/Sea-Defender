uniform sampler2D tex;
uniform float stipple;

void main()
{
  vec4 tex_color = texture2D(tex,gl_TexCoord[0].st);
  tex_color.a *= mix(1.0,ceil(sin(gl_TexCoord[0].s*8.0)),stipple);
  gl_FragColor =  gl_Color * tex_color;
}
