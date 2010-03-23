
const char data_sha_line_fs[] = ""
"uniform sampler2D tex;\n"
"uniform float stipple;\n"
"\n"
"void main()\n"
"{\n"
"  vec4 tex_color = texture2D(tex,gl_TexCoord[0].st);\n"
"  tex_color.a *= mix(1.0,ceil(sin(gl_TexCoord[0].s*8.0)),stipple);\n"
"  gl_FragColor =  gl_Color * tex_color;\n"
"}\n"
;

