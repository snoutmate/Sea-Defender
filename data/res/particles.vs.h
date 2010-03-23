
const char data_sha_particles_vs[] = ""
"attribute vec2 pos;\n"
"attribute float size;\n"
"attribute vec2 corner;\n"
"attribute vec4 color;\n"
"\n"
"varying vec2 Texcoord;\n"
"varying vec4 Color;\n"
"\n"
"void main()\n"
"{	\n"
"  Texcoord = corner;\n"
"  Color = color;\n"
"  gl_Position = gl_ProjectionMatrix * vec4(pos + ( (corner-0.5) * size),0.0,1.0);\n"
"}\n"
;

