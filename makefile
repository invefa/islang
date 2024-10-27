# this is a makfile common edition
# writing by invefa

###########################
#### constant settings ####
###########################

#compiler relevants
compiler_name         := gcc
compiler_flags        := -fdiagnostics-color=always -std=gnu11
release_version_flags := -s -O3
todebug_version_flags := -g -DISL_DEBUG
enable_warnings       := all shadow no-pointer-sign inline unreachable-code
optional_macros       := ISL_BWMF #islang build with makefile


#debugger relevants
debugger_name   := gdb
debugger_flags  :=

#source files relevants
source_dir      := src
source_subdirs  := \
	utils memgr string report lexer parser compiler vm generic

#depend files relevants
depend_flags    :=

#build relevants
build_base_dir   := builds
build_target_dir := target
build_depend_dir := depend

#run relevants
run_args        :=

#symbols
symbol_empty    :=
symbol_space    := $(symbol_empty) $(symbol_empty)
symbol_comma    := ,

#options for Compatibility (different from OS, capable for Windows / Linux / MacOS)
ifeq ($(OS),Windows_NT)
    build_target    := is.exe
    dir_slash       := $(symbol_empty)\$(symbol_empty)
    rmfile_cmd      := del /Q
    rmdir_cmd       := rmdir /Q /S
else
    build_target    := is
    rmfile_cmd      := rm -rf
    rmdir_cmd       := rm -rf
    dir_slash       := $(symbol_empty)/$(symbol_empty)
endif
mkdir_cmd       := mkdir
echo_cmd        := echo


###################################
#### expanding and redirecting ####
###################################

#full expanding and redirecting for variables

build_depend_dir:= $(build_base_dir)/$(build_depend_dir)

ifeq ($(mode),debug)
    build_version_flags := $(todebug_version_flags)
    build_dir           := $(build_base_dir)/debug
else
    build_version_flags := $(release_version_flags)
    build_dir           := $(build_base_dir)/release
endif

build_target_dir:= $(build_dir)/$(build_target_dir)

source_subdirs  := $(foreach _dir,$(source_subdirs),$(source_dir)/$(_dir))
source_dirs     := $(source_dir) $(source_subdirs)

compile_header  := $(compiler_name) $(compiler_flags) $(build_version_flags)
compile_header  += $(foreach _waring,$(enable_warnings),-W$(_waring))
compile_header  += $(foreach _dir,$(source_dirs),-I$(_dir))
compile_header  += $(foreach _macro,$(optional_macros),-D$(_macro))

build_dirs	    := $(build_base_dir) $(build_depend_dir) $(build_dir) $(build_target_dir)
build_dirs      += $(foreach _dir,$(source_dirs),$(build_depend_dir)/$(_dir))
build_dirs      += $(foreach _dir,$(source_dirs),$(build_dir)/$(_dir))
build_target    := $(build_target_dir)/$(build_target)

source_files    := $(foreach _dir,$(source_dirs),$(wildcard $(_dir)/*.c))
object_files    := $(foreach _file,$(source_files),$(build_dir)/$(_file:.c=.o))
depend_files    := $(foreach _file,$(source_files),$(build_depend_dir)/$(_file:.c=.d))

phony_depend_files  := $(foreach _file,$(depend_files),phony$(_file))
existed_depend_files:= $(wildcard $(depend_files))

###############
#### rules ####
###############

#######################
# build & run methods #
#######################

#default target for this makefile
build-run:   build run
build-debug: build debug

run:
	$(build_target) $(run_args)

debug:
	$(debugger_name) $(build_target) $(debugger_flags)

build: $(build_target)
$(build_target): $(object_files)
	@$(echo_cmd) building target: $@
	@$(compile_header) $^ -o $@

rebuild: clean-all depend build

#some trick to get the actual depend file path
@convert-to-depend = $(patsubst $(build_dir)/%.o,$(build_depend_dir)/%.d,$@)

#pre-define rule for recipes of depend files
$(build_dir)/%.o: %.c | mkdirs
	@$(echo_cmd) building object: $@
	@$(compile_header) -MQ $@ -MF $(@convert-to-depend) -MP -MMD -c $< -o $@
#make the phony targets for depend files
$(depend_files):
#include recipes of depend files
include $(existed_depend_files)

#################
# clean methods #
#################

clean: clean-object
clean-all:
	-$(rmfile_cmd) $(wildcard $(subst /,$(dir_slash),$(build_target) $(object_files) $(depend_files)))
clean-target:
	-$(rmfile_cmd) $(wildcard $(subst /,$(dir_slash),$(build_target)))
clean-object:
	-$(rmfile_cmd) $(wildcard $(subst /,$(dir_slash),$(object_files)))
clean-depend:
	-$(rmfile_cmd) $(wildcard $(subst /,$(dir_slash),$(depend_files)))
clean-dirs:
	-$(rmdir_cmd) $(wildcard $(subst /,$(dir_slash),$(build_base_dir)))

#################
# other methods #
#################

depend: $(phony_depend_files)

#de-phony-prefix for target to get actual depend file path
@deprefix-phony = $(patsubst phony%,%,$@)

#phony targets for depend method to make depend files
#different with upper phony target form 'notdir'
#it never be find out at this project.
$(phony_depend_files): phony$(build_depend_dir)/%.d : %.c | mkdirs
	@$(echo_cmd) refreashing depend file: $(@deprefix-phony)
	@$(compile_header) $< -MP -MM -MF $(@deprefix-phony) -MQ $(@deprefix-phony:.d=.o)

mkdirs: $(build_dirs)
$(build_dirs): #mk-build-dirs
	$(mkdir_cmd) $(subst /,$(dir_slash),$@)
mk-build-dir:
	$(mkdir_cmd) $(subst /,$(dir_slash),$(build_base_dir))

#test codes
test_escapes = -o $@ $< -MM -MT $(@:.d=.o)

echo:
	@$(echo_cmd) source_subdirs: $(source_subdirs)
	@$(echo_cmd) ---------------------------------------------
	@$(echo_cmd) source_dirs: $(source_dirs)
	@$(echo_cmd) ---------------------------------------------
	@$(echo_cmd) build_dirs: $(build_dirs)
	@$(echo_cmd) ---------------------------------------------
	@$(echo_cmd) build_target: $(build_target)
	@$(echo_cmd) ---------------------------------------------
	@$(echo_cmd) source_files: $(source_files)
	@$(echo_cmd) ---------------------------------------------
	@$(echo_cmd) object_files: $(object_files)
	@$(echo_cmd) ---------------------------------------------
	@$(echo_cmd) depend_files: $(depend_files)
	@$(echo_cmd) ---------------------------------------------
	@$(echo_cmd) phony_depend_files: $(phony_depend_files)
	@$(echo_cmd) ---------------------------------------------
	@$(echo_cmd) existed_depend_files: $(existed_depend_files)
	@$(echo_cmd) ---------------------------------------------
	@$(echo_cmd) dir_slash: "$(dir_slash)"
	@$(echo_cmd) ---------------------------------------------
	@$(echo_cmd) compile_header: $(compile_header)
	@$(echo_cmd) ---------------------------------------------
	@$(echo_cmd) subst_with_dir_slash: $(subst /,$(dir_slash), build/src/utils)
	@$(echo_cmd) ---------------------------------------------
	@$(echo_cmd) test_escapes: $(test_escapes)

help:
	@$(echo_cmd) usage: make [option]
	@$(echo_cmd)   option:
	@$(echo_cmd)      build-run    : build and run project build-target (default)
	@$(echo_cmd)      build-debug  : build and run project build-target with debugger
	@$(echo_cmd)      run          : run     project build-target
	@$(echo_cmd)      debug        : run     project build-target with debugger
	@$(echo_cmd)      build        : build   project build-target
	@$(echo_cmd)      rebuild      : rebuild project build-target
	@$(echo_cmd)      depend       : check project depending relevants and freash *.d files
	@$(echo_cmd)      clean        : clean object files
	@$(echo_cmd)      clean-x      : clean files (x can be 'all','target','object','depend','dirs')
	@$(echo_cmd)      echo         : echo infomations to debug for this makfile
	@$(echo_cmd) write by invefa.

#set phony targets
.PHONY:                                 \
build rebuild                           \
run debug build-run build-debug         \
clean clean-all clean-target            \
clean-object clean-depend clean-dirs    \
depend mkdirs mk-build-dir echo help
