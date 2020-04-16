.PRECIOUS: %/.

ASM = nasm
CC 	= clang
CXX = clang++
LD	= clang++

ASM_TO_COMPILE 	=		# define sources you wish
C_TO_COMPILE 	= 		# to build as these when
CPP_TO_COMPILE	= 		# calling the makefile_builder
EXE_TARGET		= 		# also define the EXE_TARGET


ifeq ($(OS), Windows_NT)
# Windows + minGW
ECHO 			= echo -e
ASM_FLAGS 		= -fwin32
LINKER_FLAGS 	= -lpsapi -static -static-libgcc -static-libstdc++
else
# Linux
ECHO 			= echo
ASM_FLAGS 		= -felf
endif


build_dir 		:= build
obj_dir			:= $(build_dir)/objs
dep_dir			:= $(build_dir)/deps
orig_o_dir 		:= $(obj_dir)/orig
orig_d_dir 		:= $(dep_dir)/orig

TARGET			= $(build_dir)/$(EXE_TARGET)

all: $(TARGET)

clean:
	@$(ECHO) ------------- Cleaning --------------
	@rm -rf $(build_dir)

%/.:
	@$(ECHO) ----------- Creating Directory : $(@D) -----------------
	@mkdir -p $(@D)

ORIG_C_SOURCES		:= $(C_TO_COMPILE)
ORIG_CPP_SOURCES	:= $(CPP_TO_COMPILE)

ORIG_C_DEPS			:= $(ORIG_C_SOURCES:%.c=$(orig_d_dir)/c/%.d)
ORIG_C_OBJS			:= $(ORIG_C_SOURCES:%.c=$(orig_o_dir)/c/%.o)
ORIG_CPP_DEPS		:= $(ORIG_CPP_SOURCES:%.cpp=$(orig_d_dir)/cpp/%.d)
ORIG_CPP_OBJS		:= $(ORIG_CPP_SOURCES:%.cpp=$(orig_o_dir)/cpp/%.o)

ORIG_ASM_OBJS		:= $(ASM_TO_COMPILE:%.asm=$(orig_o_dir)/asm/%.o)

INCLUDE_DIRS 		:= $(DIR_TO_INCLUDE)

C_FLAGS 	:= -std=c11	  -Wall -O3
CXX_FLAGS 	:= -std=c++11 -Wall -O3 -Weffc++
LD_FLAGS    := $(LINKER_FLAGS)

ifeq ($(BUILD_AS_DLL), yes)
LD_FLAGS 	+= -shared -lws2_32
endif


DEPFLAGS_DEFAULT	 = -MT $@ -MMD -MP
DEPFLAGS_C_ORIG 	 = $(DEPFLAGS_DEFAULT) -MF $(orig_d_dir)/c/$*.d
DEPFLAGS_CPP_ORIG 	 = $(DEPFLAGS_DEFAULT) -MF $(orig_d_dir)/cpp/$*.d


DEPFILES := $(ORIG_CPP_SOURCES:%.cpp=$(orig_d_dir)/cpp/%.d) $(ORIG_C_SOURCES:%.c=$(orig_d_dir)/c/%.d)
$(DEPFILES):
include $(wildcard $(DEPFILES))


.SECONDEXPANSION:

#  ------------------- Compile objects -------------------
$(orig_o_dir)/asm/%.o : %.asm makefile_builder.mk | $$(@D)/.
	@$(ECHO) ----------- Assembling : $@ -----------------
	$(ASM) -w+all $(ASM_FLAGS) $< -o $@

$(orig_o_dir)/c/%.o : %.c makefile_builder.mk | $$(@D)/. $(orig_d_dir)/c/$$(*D)/.
	@$(ECHO) ----------- Compiling : $@ -----------------
	$(CC) $(C_FLAGS) $(DEPFLAGS_C_ORIG) $(INCLUDE_DIRS) -c $< -o $@

$(orig_o_dir)/cpp/%.o : %.cpp makefile_builder.mk | $$(@D)/. $(orig_d_dir)/cpp/$$(*D)/.
	@$(ECHO) ----------- Compiling : $@ -----------------
	$(CXX) $(CXX_FLAGS) $(DEPFLAGS_CPP_ORIG) $(INCLUDE_DIRS) -c $< -o $@

#  ------------------- link final target -------------------
$(TARGET): $(ORIG_ASM_OBJS) $(ORIG_C_OBJS) $(ORIG_CPP_OBJS) makefile | $$(@D)/.
	@$(ECHO) ----------- Linking : $@ -----------------
	$(LD) $(ORIG_ASM_OBJS) $(ORIG_C_OBJS) $(ORIG_CPP_OBJS) $(LD_FLAGS) -o $@ -s
	@$(ECHO) ----------- DONE: Target $@ -----------
