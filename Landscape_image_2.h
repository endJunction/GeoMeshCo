/*
 * Copyright (C) 2012
 *               Helmholtz Centre for Environmental Research (UFZ)
 *
 * This file is part of "GeoMeshCo." "GeoMeshCo" is free
 * software: you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * "GeoMeshCo" is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * "GeoMeshCo".  If not, see <http://www.gnu.org/licenses/>.
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

#ifndef CGAL_EXTENSION_LANDSCAPE_IMAGE_2
#define CGAL_EXTENSION_LANDSCAPE_IMAGE_2

#include <CGAL/basic.h>

#include "Image_2.h"

namespace CGAL_Extension
{

template <typename FT, typename Point_3>
class Landscape_image_2
{
    public:
    Landscape_image_2(
        const Image_2<short, FT>& image,
        const FT& bottom = -1,
        const FT& z_scale = 1,
        const FT& outside_value = 1)
        : _image(image), _bottom(bottom), _z_scale(z_scale),
        _out_value(outside_value)
    { }

    FT
    operator()(const Point_3& p) const
    {
        typedef typename Image_2<short, FT>::Value Value;
        const Value f = _image.interpolate(
            CGAL::to_double(p.x()), CGAL::to_double(p.y()));

        if (f)
        {
            if (p.z() <= _bottom)
                return -p.z();

            return p.z() - *f/_z_scale;
        }

        return _out_value;   // outside image;
    }

    private:
    const Image_2<short, FT>& _image;
    const FT& _bottom;
    const FT& _z_scale;
    const FT& _out_value;

};

}   // namespace CGAL_Extension

#endif  // CGAL_EXTENSION_LANDSCAPE_IMAGE_2
