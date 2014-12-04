# Makefile générique
#
# Options du Makefile
#
# Les options sont désactivées par les valeurs 0,n ou no; toute autre valeur
# les active.
# DEBUG   : lorsque cette option est activée, les options debug sont utilisées
#           (suffixée par _DBG), le répertoire de construction est nommé Debug
#           Si elle est désactivée, une version release sera construite en
#           utilisant les options sufficées par _RLS, le répertoire de
#           construction sera nommé Release
# PROFILE : activée ajoute les options de profilage de code (_PRF)
# PTHREAD : activée ajoute les options pour le multi threading (_THR)
#           defini les macros pour pouvoir utiliser la glibc en mode
#           thread safe
# SILENT  : supprime l'affichage des commandes au profit d'un message plus court
#           les eereurs sont mises en gras pour une meilleure lecture
DEBUG     ?= 1
PROFILE   ?= 0
PTHREAD   ?= 0
SILENT    ?= 1
#
# Description de la cible 
#
# EXENAME : nom de l'exécutable, s'il est non renseigné la valeur par défaut
#           (le nom du répertoire où se trouve le Makefile) sera utilisé.
#           Par exemple, si ce template est dans le répertoire ~/Projets/test
#           le nom par défaut de l'exécutable sera test.
# VERSION : une chaine libre décrivant la version
# les valeurs EXENAME et VERSION sont données au compilateur via les macros
# PROGRAM_EXENAME et PROGRAM_VERSION
EXENAME   ?=
VERSION   ?= 1.0
#
# Paquets pkg-config
#
# Liste des paquets configurés par pkg-config nécessaires à la construction de
# l'exécutable. Par exemple pour intégrer les options pour utiliser GLib :
# PKGLIST ?= glib-2.0
PKGLIST   ?=
#
# libutil
#
# LIBUTIL : si cette option est activée alors toutes les sources présentes dans
#           le répertoire libutil seront compilée et liées dans une bibliothèque
#           statique libutil.a qui sera utilisé lors de la construction de 
#           l'exécutable
LIBUTIL   ?= 1
LOG       ?= 1
#
# Compilateur C utilisé
#
CC         = gcc
# Options de base
# Par défaut on utilise les extension GNU et on compile en C99 en émettant 
# les warnings les plus utiles.
CPPFLAGS_DEF   := -D_GNU_SOURCE
CFLAGS_DEF     := -std=c99 -Wall -Wextra
LDFLAGS_DEF    :=
LDLIBS_DEF     := -lm
# Options pour le mode DEBUG=1
CPPFLAGS_DBG   := -DDEBUG
CFLAGS_DBG     := -ggdb3
LDFLAGS_DBG    := -ggdb3
LDLIBS_DBG     :=
# Options pour le mode DEBUG=0 (=mode Release)
CPPFLAGS_RLS   := -DRELEASE -DNDEBUG
CFLAGS_RLS     := -O3
LDFLAGS_RLS    :=
LDLIBS_RLS     :=
# Options pour le mode PROFILE=1
CPPFLAGS_PRF   := -DPROFILE
CFLAGS_PRF     := -pg
LDFLAGS_PRF    := -pg
LDLIBS_PRF     :=
# Options pour le mode PTHREAD=1
CPPFLAGS_THR   := -D_REENTRANT -D_THREAD_SAFE -DPTHREAD
CFLAGS_THR     :=
LDFLAGS_THR    :=
LDLIBS_THR     := -pthread
#
# Commandes utilisées dans les recettes
ECHO            = echo
ECHO_N          = echo -n
RM              = rm -f
RMDIR           = rm -rf
AR              = ar
PKGCONFIG       = pkg-config
TPUT            = tput
FIND            = find
#
# Ici se termine la liste des variables pilotant le comportement du Makefile
###############################################################################
###############################################################################
###############################################################################
###############################################################################
#
#
#
define set-options =
  ifndef $(1)
    override $(1) := 0
  else
    ifneq (,$(findstring "$(strip $($(1)))","0" "no" "n"))
      override $(1) := 0
    else 
      override $(1) := 1
    endif
  endif
endef
$(foreach option,                                   \
          DEBUG PROFILE PTHREAD SILENT LIBUTIL LOG, \
          $(eval                                    \
             $(call set-options,$(option))          \
           )                                        \
 )

EMPTY =
SPACE = $(EMPTY) $(EMPTY)

MAKE_ABSOLUTE_PATH  := $(abspath $(lastword $(MAKEFILE_LIST)))
SOURCE_DIRNAME      := src/
LIBUTIL_DIRNAME     := libutil/
LIBUTIL_NAME        := libutil.a
DEPS_DIRNAME        := .deps/
TOPLEVEL_DIR        := $(dir $(MAKE_ABSOLUTE_PATH))
SOURCE_DIR          := $(TOPLEVEL_DIR)$(SOURCE_DIRNAME)
LIBUTIL_DIR         := $(TOPLEVEL_DIR)$(LIBUTIL_DIRNAME)

ifndef CC
  override CC = gcc
endif

ifeq (1,$(SILENT))
  override CC       := @$(CC)
  RM                := @$(RM)
  RMDIR             := @$(RMDIR)
  AR                := @$(AR)
  FIND              := @$(FIND)
  MSG               := @$(ECHO)
  BOLD_ON           := @$(ECHO_N) `$(TPUT) bold`
  BOLD_OFF          := @$(ECHO_N) `$(TPUT) sgr0`
  LINEFEED          := @$(ECHO_N) \#
else
  MSG               := @$(ECHO_N) \#
  BOLD_ON           := @$(ECHO_N) \#
  BOLD_OFF          := @$(ECHO_N) \#
  LINEFEED          := @$(ECHO) \#
endif

CPPFLAGS            := $(CPPFLAGS_DEF)
CFLAGS              := $(CFLAGS_DEF)
LDFLAGS             := $(LDFLAGS_DEF)
LDLIBS              := $(LDLIBS_DEF)

ifeq (1,$(PROFILE))
  EXT               := .prf
  TYPE              += profiled
  CPPFLAGS          += $(CPPFLAGS_PRF)
  CFLAGS            += $(CFLAGS_PRF)
  LDFLAGS           += $(LDFLAGS_PRF)
  LDLIBS            += $(LDLIBS_PRF)
endif

ifeq (1,$(DEBUG))
  TYPE              += debug
  BUILDDIR_DIRNAME  := Debug
  CPPFLAGS          += $(CPPFLAGS_DBG)
  CFLAGS            += $(CFLAGS_DBG)
  LDFLAGS           += $(LDFLAGS_DBG)
  LDLIBS            += $(LDLIBS_DBG)
else
  TYPE              += release
  BUILDDIR_DIRNAME  := Release
  CPPFLAGS          += $(CPPFLAGS_RLS)
  CFLAGS            += $(CFLAGS_RLS)
  LDFLAGS           += $(LDFLAGS_RLS)
  LDLIBS            += $(LDLIBS_RLS)
endif
ifndef EXENAME
  override EXENAME  := $(shell basename $(TOPLEVEL_DIR))
endif	
BUILDDIR            := $(TOPLEVEL_DIR)$(BUILDDIR_DIRNAME)$(EXT)/
SOURCE_BUILDDIR     := $(BUILDDIR)$(SOURCE_DIRNAME)
SOURCE_DEPSDIR      := $(SOURCE_BUILDDIR)$(DEPS_DIRNAME)
LIBUTIL_BUILDDIR    := $(BUILDDIR)$(LIBUTIL_DIRNAME)
LIBUTIL_DEPSDIR     := $(LIBUTIL_BUILDDIR)$(DEPS_DIRNAME)

ifeq (1,$(PTHREAD))
  TYPE              += multi thread enabled
  CPPFLAGS          += $(CPPFLAGS_THR)
  CFLAGS            += $(CFLAGS_THR)
  LDFLAGS           += $(LDFLAGS_THR)
  LDLIBS            += $(LDLIBS_THR)
endif
TYPE                += version

#
#
#
CPPFLAGS            += -DPROGRAM_NAME="\"$(EXENAME)\"" -DPROGRAM_VERSION="\"$(VERSION)\"" -DPROGRAM_BUILD="\"$(TYPE)\""
CPPFLAGS            += -iquote $(SOURCE_DIR)
ifeq (1,$(LIBUTIL))
  CPPFLAGS          += -iquote $(LIBUTIL_DIR) 
  ifeq (1,$(LOG))
    CPPFLAGS        += -DXLOG
    PROGRAM_DOMAIN  := -D__DOMAIN__=\"$(EXENAME)\"
    SOURCE_BASENAME  = -D__FILE_BASENAME__=\"$(SOURCE_DIRNAME)$(<F)\"
    LIBUTIL_DOMAIN  := -D__DOMAIN__=\"$(LIBUTIL_NAME)\"
    LIBUTIL_BASENAME = -D__FILE_BASENAME__=\"$(LIBUTIL_DIRNAME)$(<F)\"
  endif
endif

ifdef PKGLIST
  ifeq (0,$(shell $(PKGCONFIG) --exists $(PKGLIST) && echo 1 || echo 0))
    $(error Missing pkg-config or invalid package in '$(PKGLIST)')
  endif
  CFLAGS            += $(shell $(PKGCONFIG) --cflags $(PKGLIST))
  LDFLAGS           += $(shell $(PKGCONFIG) --libs $(PKGLIST))
endif

#
#
#
#
DIRLIST := $(BUILDDIR) $(SOURCE_BUILDDIR) $(SOURCE_DEPSDIR)
ifeq (1,$(LIBUTIL))
  DIRLIST += $(LIBUTIL_BUILDDIR) $(LIBUTIL_DEPSDIR)
  LIBLNK  := $(LIBUTIL_BUILDDIR)$(LIBUTIL_NAME)
  LDFLAGS += $(LIBLNK)
endif

$(foreach dirent, $(DIRLIST),                     \
  $(shell [ -d $(dirent) ] || mkdir -p $(dirent)) \
)

PROGRAM_SOURCE      := $(wildcard $(SOURCE_DIR)*.c)
PROGRAM_OBJECT      := $(addprefix $(SOURCE_BUILDDIR), $(notdir $(PROGRAM_SOURCE:.c=.o)))
PROGRAM_DEPS        := $(addprefix $(SOURCE_DEPSDIR), $(notdir $(PROGRAM_SOURCE:.c=.d)))
LIBUTIL_SOURCE      := $(wildcard $(LIBUTIL_DIR)*.c)
LIBUTIL_OBJECT      := $(addprefix $(LIBUTIL_BUILDDIR), $(notdir $(LIBUTIL_SOURCE:.c=.o)))
LIBUTIL_DEPS        := $(addprefix $(LIBUTIL_DEPSDIR), $(notdir $(LIBUTIL_SOURCE:.c=.d)))
TARGET              := $(BUILDDIR)$(EXENAME)

#
#
#
.SUFFIXES:

$(TARGET): $(PROGRAM_OBJECT) $(LIBLNK)
	$(MSG) "CCLD    " $(BUILDDIR_DIRNAME)$(EXT)/$(EXENAME)
	$(BOLD_ON)
	$(CC) -o $(TARGET) $(LDFLAGS) $^ $(LDLIBS)
	$(BOLD_OFF)
	$(MSG) $(TYPE) built
	$(LINEFEED)

$(SOURCE_BUILDDIR)%.o: $(SOURCE_DIR)%.c
	$(MSG) "CC      " $(SOURCE_DIRNAME)$(<F)
	$(BOLD_ON)
	$(CC) -o $@ -c $(CPPFLAGS) $(SOURCE_BASENAME) $(PROGRAM_DOMAIN) $(CFLAGS) $<
	$(BOLD_OFF)
	$(LINEFEED)

$(SOURCE_DEPSDIR)%.d: $(SOURCE_DIR)%.c
	$(MSG) "GENDEP  " $(SOURCE_DIRNAME)$(<F)
	$(BOLD_ON)
	$(CC) -o $@ -E -MM -MT '$(SOURCE_BUILDDIR)$(notdir $(<:.c=.o))' $(CPPFLAGS) $(CFLAGS) $<
	$(BOLD_OFF)
	$(LINEFEED)

$(LIBLNK): $(LIBUTIL_OBJECT)
	$(MSG) "AR      " $(LIBUTIL_NAME)
	$(BOLD_ON)
	$(AR) rcs $@ $^
	$(BOLD_OFF)
	$(LINEFEED)

$(LIBUTIL_BUILDDIR)%.o: $(LIBUTIL_DIR)%.c
	$(MSG) "CC      " $(LIBUTIL_DIRNAME)$(<F)
	$(BOLD_ON)
	$(CC) -o $@ -c $(CPPFLAGS) $(LIBUTIL_BASENAME) $(LIBUTIL_DOMAIN) $(CFLAGS) $<
	$(BOLD_OFF)
	$(LINEFEED)

$(LIBUTIL_DEPSDIR)%.d:  $(LIBUTIL_DIR)%.c
	$(MSG) "GENDEP  " $(LIBUTIL_DIRNAME)$(<F)
	$(BOLD_ON)
	$(CC) -o $@ -E -MM -MT '$(LIBUTIL_BUILDDIR)$(notdir $(<:.c=.o))' $(CPPFLAGS) $(CFLAGS) $<
	$(BOLD_OFF)
	$(LINEFEED)

ifeq (,$(findstring clean,$(MAKECMDGOALS)))
  ifeq (,$(findstring backup,$(MAKECMDGOALS)))
    -include $(PROGRAM_DEPS)
    ifeq (1,$(LIBUTIL))
      -include $(LIBUTIL_DEPS)
    endif
  endif
endif


#
#
#
.PHONY: clean clean-temp backup

clean: clean-temp
	$(MSG) Deleteting $(BUILDDIR_DIRNAME)$(EXT)
	$(RMDIR) $(BUILDDIR)

clean-temp:
	$(MSG) Deleting temp files
	$(FIND) $(TOPLEVEL_DIR) -type f -a \( -name '*~' -o -name '\#*\#' -o -name '.*.sw?' \) -exec rm -rf \{\} +

backup:
	$(MSG) Backing up project
	@cd $(TOPLEVEL_DIR) && \
	name=$$(basename `pwd`) && \
        cd $(TOPLEVEL_DIR).. && \
	fname=$$name.`date +"%Y%m%d-%H%M%S"`.tgz && \
	tar czvf $$name/$$fname $$name/Makefile $$name/libutil $$name/src && \
	cd $(TOPLEVEL_DIR) && \
	echo -n "$$fname : " && \
	ls -lh $$fname
