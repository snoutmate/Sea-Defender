uniform float time;
uniform float vert_pos;
uniform float max_amp;
uniform float amp_mod;

void main()
{
  gl_FrontColor = gl_Color;

  vec4 pos = gl_Vertex;
  float add = (sin((pos.x+(time/16.0))*16.0)/75.0) + (sin((-pos.x+(time/8.0))*8.0)/200.0);

  add *= amp_mod;

  gl_TexCoord[0] = vec4(pos.xy,0.0,1.0);
  
  pos.y *= add;
  pos.y += vert_pos;
  pos.y += pos.z * max_amp;

  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * pos;
}
