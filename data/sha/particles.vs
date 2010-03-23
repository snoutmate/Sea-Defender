attribute vec2 pos;
attribute float size;
attribute vec2 corner;
attribute vec4 color;

varying vec2 Texcoord;
varying vec4 Color;

void main()
{	
  Texcoord = corner;
  Color = color;
  gl_Position = gl_ProjectionMatrix * vec4(pos + ( (corner-0.5) * size),0.0,1.0);
}
