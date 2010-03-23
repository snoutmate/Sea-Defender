
const char data_sha_staticmesh_fs[] = ""
"uniform sampler2D tex;\n"
"\n"
"void main()\n"
"{\n"
"  vec4 tex_color = texture2D(tex,gl_TexCoord[0].st);\n"
"  gl_FragColor =  gl_Color * gl_Color.a * tex_color.r;\n"
"}\n"
;

