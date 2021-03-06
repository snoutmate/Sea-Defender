uniform sampler2D tex;
varying vec2 Texcoord;
varying vec4 Color;

void main()
{
  vec4 tex_color = texture2D(tex,Texcoord);
  gl_FragColor = vec4(Color.rgb, tex_color.a * Color.a);
}
