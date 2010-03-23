
const char data_sha_particles_fs[] = ""
"uniform sampler2D tex;\n"
"varying vec2 Texcoord;\n"
"varying vec4 Color;\n"
"\n"
"void main()\n"
"{\n"
"  vec4 tex_color = texture2D(tex,Texcoord);\n"
"  gl_FragColor = vec4(Color.rgb, tex_color.a * Color.a);\n"
"}\n"
;

