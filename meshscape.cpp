/* "GeoMeshCo" is a collection of mesh creation utilities for geophysical
 * applications.
 * 
 * Copyright (C) 2012
 *               Helmholtz Centre for Environmental Research (UFZ)
 * 
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * As a special exception, you have permission to link this program
 * with the Computational Geometry Algorithms Library (CGAL,
 * <http://www.cgal.org>) and distribute executables, as long as you
 * follow the requirements of the GNU GPL in regard to all of
 * the software in the executable aside from CGAL.
 *
 *
 * Author: Dmitrij Yu. Naumov
 *
 */

#include <fstream>
#include "cli.h"
#include "Landscape_image_2.h"
#include "Image_reader_2.h"
#include "Image_2.h"

#include <CGAL/Surface_mesh_default_triangulation_3.h>
#include <CGAL/Surface_mesh_default_criteria_3.h>
#include <CGAL/Complex_2_in_triangulation_3.h>
#include <CGAL/IO/Complex_2_in_triangulation_3_file_writer.h>

#include <CGAL/make_surface_mesh.h>
#include <CGAL/Implicit_surface_3.h>

typedef CGAL::Surface_mesh_default_triangulation_3 Tr;
typedef CGAL::Complex_2_in_triangulation_3<Tr> C2t3;

typedef Tr::Geom_traits K;
typedef Image_2<short, K::FT> Image;
typedef CGAL_Extension::Landscape_image_2<K::FT, K::Point_3> LandscapeImage;
typedef CGAL::Implicit_surface_3<K, LandscapeImage> Surface;

#define POW2(x) ((x)*(x))

int
main(int argc, char* argv[])
{
    // Process command line arguments
    CLI cli(argc, argv);

    // Load image
    std::cout << "Load image." << std::endl;
    const TIFF_image_reader_2 reader(cli.input);
    if (reader.bpp() != 16)
    {
        std::cerr << "Non 16 bit images are not supported.\n";
        return EXIT_FAILURE;
    }

    const Image* const image = reader.get_image<short, K::FT>();

    const LandscapeImage landscape(*image, cli.bottom, cli.z_scale);

    //
    // Surface
    //
    std::cout << "Create mesh surface." << std::endl;
    // Find bounding sphere.
    K::Point_3 bounding_sphere_center(  // Center of the image
        image->x_size() / 2.,
        image->y_size() / 2.,
        -1);
    K::FT bounding_sphere_squared_radius =  // (diagonal/2)^2 = diagonal^2 / 4;
        (POW2(image->x_size()) + POW2(image->y_size()))/4.
            + POW2(std::numeric_limits<short>::max()/cli.z_scale);
    K::Sphere_3 bounding_sphere(bounding_sphere_center,
        bounding_sphere_squared_radius);
    std::cout << "center " << bounding_sphere_center << " r " << bounding_sphere_squared_radius << std::endl;

    Surface surface(landscape, bounding_sphere, 1e-3/CGAL::sqrt(bounding_sphere_squared_radius));

    CGAL::Surface_mesh_default_criteria_3<Tr> mesh_criteria(
        cli.angular_bound,
        cli.facet_size,
        cli.approximation);


    //
    // Mesh generation
    //
    std::cout << "Start mesh generation." << std::endl;
    Tr tr;
    C2t3 c2t3(tr);
    CGAL::make_surface_mesh(c2t3, surface, mesh_criteria, CGAL::Manifold_with_boundary_tag());

    delete image;

    std::ofstream out(cli.output);
    out << std::setprecision(17);
    CGAL::output_surface_facets_to_off(out, c2t3);
    return 0;
}
