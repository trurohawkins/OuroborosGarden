TARGET = OuroborosGarden

ENGINEDIR = FormEngine/

LIBDIR = lib/
INCDIR = include/

RENDERER = TUI
RENDERDIR = $(ENGINEDIR)$(RENDERER)/
RENDERLIB = $(RENDERDIR)lib/
RENDERINC = $(RENDERDIR)include/

AUDIO = AudioMan
AUDIODIR = $(ENGINEDIR)$(AUDIO)/
AUDIOLIB = $(AUDIODIR)lib/
AUDIOINC = $(AUDIODIR)include/
AUDIOFLAGS = -lportaudio -lsndfile

HELPERDIR = $(ENGINEDIR)HelperFuncs/
HELPERINC = $(HELPERDIR)include/
HELPERLIB = $(HELPERDIR)lib/

MOLTNDIR = $(ENGINEDIR)MOLTN/
MOLTNINC = $(MOLTNDIR)include/
MOLTNLIB = $(MOLTNDIR)lib/

OIBDIR = $(ENGINEDIR)OIB/
OIBINC = $(OIBDIR)include/
OIBLIB = $(OIBDIR)lib/

GAMEDIR = $(ENGINEDIR)GameCore/
GAMEINC = $(GAMEDIR)include/
GAMELIB = $(GAMEDIR)lib/

FORMDIR = $(ENGINEDIR)Form/
FORMINC = $(FORMDIR)include/
FORMLIB = $(FORMDIR)lib/

DEV_CFLAGS = -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer
DEV_LDFLAGS = -fsanitize=address,undefined

TSAN_CFLAGS = -g -O1 -fsanitize=thread -fno-omit-frame-pointer
TSAN_LDFLAGS = -fsanitize=thread

PROD_CFLAGS = -O2
PROD_LDFLAGS =

CFLAGS = -MMD -MP -I$(HELPERINC) -I$(MOLTNINC) -I$(OIBINC) -I$(GAMEINC) -I$(FORMINC) -I$(AUDIOINC) -I$(RENDERINC) -I$(INCDIR)
LDFLAGS =

dev: CFLAGS += $(DEV_CFLAGS)
dev: LDFLAGS += $(DEV_LDFLAGS)
dev: $(TARGET)

tsan: CFLAGS += $(TSAN_CFLAGS)
tsan: LDFLAGS += $(TSAN_LDFLAGS)
tsan: $(TARGET)

prod: CFLAGS += $(PROD_CFLAGS)
prod: LDFLAGS += $(PROD_LDFLAGS)
prod: $(TARGET)

# Linking
$(TARGET): $(FORMINC)FormEngine.h $(FORMLIB)libFormEngine.a $(RENDERLIB)lib$(RENDERER).a $(RENDERINC)$(RENDERER).h $(AUDIOLIB)lib$(AUDIO).a $(AUDIOINC)$(AUDIO).h $(GAMEINC)GameCore.h $(GAMELIB)libGameCore.a $(OIBINC)OIB.h $(OIBLIB)libOIB.a $(MOLTNLIB)libMoltnCore.a $(MOLTNINC)MoltnCore.h $(HELPERLIB)libHelper.a  $(HELPERINC)helper.h main.o  
	gcc main.o -o $@ $(LDFLAGS) -L$(FORMLIB) -lFormEngine -L$(AUDIOLIB) -l$(AUDIO) -L$(RENDERLIB) -l$(RENDERER) -L$(GAMELIB) -lGameCore -L$(OIBLIB) -lOIB -L$(MOLTNLIB) -lMoltnCore -L$(HELPERLIB) -lHelper $(AUDIOFLAGS) -lm

main.o: main.c snake.c snake.h constants.h dirt.c dirt.h stat.c stat.h plant.c plant.h grass.c grass.h flower.c flower.h
	cp $(AUDIODIR)lsan.supp .
	gcc $(CFLAGS) -c main.c -o $@

$(HELPERLIB)libHelper.a:
	$(MAKE) -C $(HELPERDIR)

$(MOLTNLIB)libMoltnCore.a:
	$(MAKE) -C $(MOLTNDIR)

$(OIBLIB)libOIB.a:
	$(MAKE) -C $(OIBDIR)

$(GAMELIB)libGameCore.a:
	$(MAKE) -C $(GAMEDIR)

$(FORMLIB)libFormEngine.a:
	$(MAKE) -C $(FORMDIR)

$(RENDERLIB)lib$(RENDERER).a:
	$(MAKE) -C $(RENDERDIR)

$(AUDIOLIB)lib$(AUDIO).a:
	$(MAKE) -C $(AUDIODIR)

# tools
clean:
	rm -f *.o *.d
	$(MAKE) clean -C $(ENGINEDIR)

fclean:
	rm -rf $(TARGET) *.o *.d lsan.supp
	$(MAKE) fclean -C $(ENGINEDIR)
