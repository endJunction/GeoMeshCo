# GeoMeshCo

GeoMeshCo is a collection of meshing utilites for geophysical applications. The
collection's intend is to provide utilities for mesh creation and manipulation
for finite element methods appearing in geophysical context: *e.g.* landscape
surface generation from DEM data, surface meshes of geological layers, insertion
of fault-systems in meshed volumes.

The first utility **meshscape** generates surface meshes from grayscale
landscape DEM images. The surface is generated from a tiff image and will be
saved in the Geomview Object File Format ([.off] [geomview07]). To convert GIS data
from *e.g.* SRTM data available via the [SRTM home
page](http://www2.jpl.nasa.gov/srtm/) to tiff image format we can use [GDAL
translator library][gdal12].


## Requirements

This code is written in C++ conforming to the C++11 standard. Further it uses [CGAL
library][cgal12] for mesh generation, which depends on [Boost][boost12], [GMP][gmp12]
and [MPFR][mpfr12]. Compilation is done using [CMake][cmake12] build system.
Develeper documentation is generated with [Doxygen][doxygen12].


## Feature development

Possible new features are:
 - 3D mesh generation of layered geological strata.
 - Different interpolation methods of DEM data.
 - Correct handling of coordinate systems.
 - ...

Any suggestions regarding existing code and wishes for new functionality are
welcome.

## References

1. "Boost C++ Libraries, 1.49.0, 2012" http://www.boost.org
[boost12]: http://www.boost.org "Boost C++ Libraries, 1.49.0, 2012"
2. "CMake, 2012" http://cmake.org
[cmake12]: http://cmake.org "CMake, 2012"
3. "Doxygen Manual, 1.8.0, 2012" http://www.stack.nl/~dimitri/doxygen
[doxygen12]: http://www.stack.nl/~dimitri/doxygen "Doxygen Manual, 1.8.0, 2012"
4. "Geomview 1.9.4 Manual, 2008.  Section 4.2 Object File Formats" http://www.geomview.org/docs/html/OFF.html
[geomview07]: http://www.geomview.org/docs/html/OFF.html "Geomview 1.9.4 Manual, 2008.  Section 4.2 Object File Formats"
5. "CGAL, Computational Geometry Algorithms Library" http://www.cgal.org
[cgal12]: http://www.cgal.org "CGAL, Computational Geometry Algorithms Library"
6. "Laurent Rineau and Mariette Yvinec. 3D Surface Mesh Generation. In CGAL User and Reference Manual. CGAL Editorial Board, 4.0 edition, 2012." http://www.cgal.org/Manual/4.0/doc_html/cgal_manual/packages.html#Pkg:SurfaceMesher3
[cgal12_a]: http://www.cgal.org/Manual/4.0/doc_html/cgal_manual/packages.html#Pkg:SurfaceMesher3 "Laurent Rineau and Mariette Yvinec. 3D Surface Mesh Generation. In CGAL User and Reference Manual. CGAL Editorial Board, 4.0 edition, 2012."
7. "GDAL - Geospatial Data Abstraction Library: Version 1.9.0, Open Source Geospatial Foundation" http://gdal.osgeo.org
[gdal12]: http://gdal.osgeo.org "GDAL - Geospatial Data Abstraction Library: Version 1.9.0, Open Source Geospatial Foundation"
8. "The GNU Multiple Precision Arithmetic Library" http://gmplib.org
[gmp12]: http://gmplib.org "The GNU Multiple Precision Arithmetic Library"
9. "The GNU MPFR Library" http://www.mpfr.org
[mpfr12]: http://www.mpfr.org "The GNU MPFR Library"


## License and Copying

Author:

 - Dmitrij Yu. Naumov <GeoMeshCo@naumov.de>.

Copyright:

 - 2012 - Helmholtz Centre for Environmental Research (UFZ)

-----------------------------

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
