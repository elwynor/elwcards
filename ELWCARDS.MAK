###############################################################################
#####               This makefile generates ELWCARDS.DLL                  #####
###############################################################################
!include $(WGDEV)\src\gcrules.mak

VIRDIR = .\GCVIRDAT

!if $(OS) == WNT
DLLLIB = $(DLLLIB) wgserver.lib galgsbl.lib
MATHDLL =
DEFILE =
PARSEOPTS = $(PARSEOPTS) -I. -DIS30 
!endif

!if $(OS) == WG2
DLLLIB = $(DLLLIB) wgserver.lib galgsbl.lib $(BORLAND)\lib\mathh.lib ${DOSEXT}\bc4\lib\phapi.lib
MATHDLL = $(DOSEXT)\bc4\lib\fpdmy
DEFILE = $(WGLIBDIR)\mathdef.def
PARSEOPTS = $(PARSEOPTS) -I. 
!endif

VIRVILES =          

OBJFILES =          \
     ${TEMPDIR}\elwcards.obj   

start:              \
     $(GCBUILD)     \
     elwcards.dll

elwcards.dll:       \
     $(OBJFILES)    \
     $(DLLLIB)      \
     $(DEFILE)      \
     $(VIRFILES)
     $(LINK) $(LIBCMD) @&&|
$(PRODLL) $(OBJFILES)
$(RUNDIR)\$&
$(RUNDIR)\$&
$(DLLLIB) $(LINKDLL)
$(DEFILE)
|
