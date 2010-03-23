uniform sampler2D tex;

void main()
{
  vec4 tex_color = texture2D(tex,gl_TexCoord[0].st);
  gl_FragColor =  gl_Color * gl_Color.a * tex_color.r;
}
