uniform float time;

attribute vec4 pos;
attribute vec2 data;

varying vec2 Texcoord;
varying vec4 Color;

void main()
{	
  float color = data.x;
  float speed = data.y;

  float x_add = (1.0 - (mod(time,speed)/speed)) *2.0 - 0.2;
  vec4 ppos = pos;

  Texcoord = ppos.zw;
  Color = vec4(color,color,color,1.0);

  ppos.x += x_add;

  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(ppos.xy,0.0,1.0);
}
