#
#    Automatically generated by configure - modify only under penalty of death
#
FESTIVALDIR = /home/paul/Build/festival/festival/
SPEECHTOOLSDIR = /home/paul/Build/festival/speech_tools/
INCLUDEPATH += $${FESTIVALDIR}/src/include
INCLUDEPATH += $${SPEECHTOOLSDIR}/include
DEFINES += FESTIVAL_HOME=\"$${FESTIVALDIR}\"
LIBS += -L$${FESTIVALDIR}/src/lib
LIBS += -L$${SPEECHTOOLSDIR}/lib
LIBS += -lFestival -lestools -lestbase -leststring -ltermcap
