voronoi_for_kbspj
=================

This is a part of codes for operating system project by institue for
kommunikations-und Betriebssysteme in TU-Berlin.

The topic of this project is "Multiroboter-scheduling in Raum und Zeit", and
there is the implementation of generating the voronoi diagrams according to
Fortune's Algorithm. I was in charge of "path planning in dynamic environment"
in this porject, and voronoi diagram is the basic approach for our solution.

As for different progress of the project, i implemented two different
version. "voronoi_kbs" is the version without dynamic memory management
function, i.e. without "malloc" function, simily function is realized by using
memory pools, which is detailed impleneted by necessity. "voronoi_mm" is the
version with malloc.
