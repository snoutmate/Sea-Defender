#CXX = g++
CXX = colorgcc
#CXXFLAGS = -ffunction-sections -fdata-sections -Wall -Os -fno-exceptions -fno-unwind-tables -fno-rtti -minline-all-stringops -ggdb
CXXFLAGS = -ffunction-sections -fdata-sections -Wall -Os -fno-exceptions -fno-unwind-tables -fno-rtti -minline-all-stringops -ggdb

# -Weffc++ -U_FORTIFY_SOURCE
# NDEBUG pro release ?
LDFLAGS = -Wl,--gc-sections -L/usr/X11R6/lib
LIBS = /usr/local/lib/libglfw.a -pthread -lGL -lm -lGLU -lX11 -lXrandr
INCLUDES = 
FILE2C = utils/file2c.rb 

OBJECTS = main.o snoutlib/glfwapp.o snoutlib/settings.o snoutlib/loadingscreen.o snoutlib/procedural.o \
	  snoutlib/misc.o snoutlib/timer.o snoutlib/mfont.o snoutlib/svg.o snoutlib/tesselate.o \
	  snoutlib/menu.o snoutlib/shader.o snoutlib/staticmesh.o snoutlib/rtt_fbo.o snoutlib/gldefs.o\
	  snoutlib/particles.o snoutlib/perlin.o snoutlib/bbox.o snoutlib/fx.o\
	  gamemenu.o layout.o background.o boat.o pboat.o pe_smoke.o resources.o credits.o hiscore.o \
	  torpedo.o pe_bubbles.o pe_bubble_expl.o game.o missile.o fx_missile_expl.o pe_smoketrail.o \
	  fx_ship_expl.o fx_torpedo_expl.o pe_torpedo_expl.o fx_moving_text.o

SRCS = $(OBJECTS:.o=.cpp)

  
BINARY = sea

.SUFFIXES: .cpp .o

all: $(BINARY) 

resources: data/sha/seawaves.vs
	$(FILE2C) data/sha/seawaves.vs data/res/seawaves.vs.h text
	$(FILE2C) data/sha/clouds.vs data/res/clouds.vs.h text
	$(FILE2C) data/sha/clouds.fs data/res/clouds.fs.h text
	$(FILE2C) data/sha/font.fs data/res/font.fs.h text
	$(FILE2C) data/sha/staticmesh.fs data/res/staticmesh.fs.h text
	$(FILE2C) data/sha/particles.vs data/res/particles.vs.h text
	$(FILE2C) data/sha/particles.fs data/res/particles.fs.h text
	$(FILE2C) data/sha/line.fs data/res/line.fs.h text
	$(FILE2C) data/model/torpedo.m data/res/torpedo.m.h binary
	$(FILE2C) data/model/torpedo2.m data/res/torpedo2.m.h binary
	$(FILE2C) data/model/tanker.m data/res/tanker.m.h binary
	$(FILE2C) data/model/pboat.m data/res/pboat.m.h binary
	$(FILE2C) data/model/OpenGL_logo_flat.m data/res/OpenGL_logo_flat.m.h binary


$(BINARY): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(BINARY) $(OBJECTS) $(LIBS)

strip: $(BINARY)
	strip -s -R .comment $(BINARY)

pack: $(BINARY) strip
	upx --ultra-brute $(BINARY)

size: clean pack
      
clean: 
	rm -f $(OBJECTS) $(BINARY) core 

# dependencies

MAKEDEPEND = gcc -M $(CPPFLAGS) -o $(df).d $<
DEPDIR = .deps
df = $(DEPDIR)/$(*F)

%.o : %.cpp
	@test -d $(DEPDIR) || mkdir $(DEPDIR); \
	$(MAKEDEPEND); \
	  cp $(df).d $(df).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	      -e '/^$$/ d' -e 's/$$/ :/' < $(df).d >> $(df).P; \
	  rm -f $(df).d
	$(COMPILE.cpp) -o $@ $<

-include $(SRCS:%.cpp=$(DEPDIR)/%.P)
	
# DO NOT DELETE
