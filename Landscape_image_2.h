/*
 * Copyright (C) 2012
 *               Helmholtz Centre for Environmental Research (UFZ)
 *
 * This file is part of "MeshScape." "MeshScape" is free
 * software: you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * "MeshScape" is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * "MeshScape".  If not, see <http://www.gnu.org/licenses/>.
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

#include <tiffio.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <CGAL/basic.h>

#include "Image_2.h"

namespace CGAL_Extension
{

template <typename FT, typename Point_3>
class Landscape_image_2
{
    public:
    Landscape_image_2(const std::string& file, const double z_scale)
    {
        TIFF* tiff_file = TIFFOpen(file.c_str(), "r");  // TODO check result
        uint32 width, height;
        TIFFGetField(tiff_file, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tiff_file, TIFFTAG_IMAGEWIDTH, &width);

        uint32 bpp = 0, spp = 0;
        TIFFGetField(tiff_file, TIFFTAG_BITSPERSAMPLE, &bpp);
        TIFFGetField(tiff_file, TIFFTAG_SAMPLESPERPIXEL, &spp);

        std::cout << "bpp/spp " << bpp << "/" << spp << std::endl;

        const uint32 linesize = TIFFScanlineSize(tiff_file);
        std::cout << "linesize " << linesize << std::endl;

        unsigned char* buf = (unsigned char*) malloc(linesize * height);
        for (uint32 i = 0; i < height; i++)
            TIFFReadScanline(tiff_file, &buf[i * linesize], i, 0);

        std::vector<FT> data;
        data = std::vector<FT>(width * height);

        // Scale image to [0, 1].
        const auto min_max = std::minmax_element((unsigned char*)buf,(unsigned char*)(buf + width * height));
        const double scale = (*(min_max.second) - *(min_max.first))/z_scale;
        const double shift = *(min_max.first);
        std::cout << "min/max " << (short)*(min_max.first) << "/" << (short)*(min_max.second);
        std::cout << " scale/shift " << scale << "/" << shift << std::endl;
        for (uint32 i = 0; i < height; i++)
        {
            for (uint32 j = 0; j < width; j++)
            {
                data[i * height + j] =
                    (buf[i*height + j] - shift) / scale;
            }
        }
        //_TIFFfree(raster);

        image = new Image_2<FT>(width, height, &data[0]);
    }

    ~Landscape_image_2()
    {
        /*
        if (raster != nullptr)
            _TIFFfree(raster);
        TIFFClose(tiff_file);
        */
    }

    size_t xdim() const { return image->x_size(); }
    size_t ydim() const { return image->y_size(); }

    FT
    operator()(const Point_3& p) const
    {
        const FT f = image->interpolate(
            CGAL::to_double(p.x()), CGAL::to_double(p.y()));
        //std::cerr << p << " " << f << " " << p.z() - f << std::endl;

        //std::cerr << p.x() << " " << p.y() << " " << p.z() << " -> " << x0 << " " << y0
            //<< ": " << f << std::endl;
        return p.z() - f;
    }

    private:
    Image_2<FT>* image;

    private:
    //Landscape_image_2(const Landscape_image_2&);
    //Landscape_image_2 operator=(const Landscape_image_2&);
};

}   // namespace CGAL_Extension

#endif  // CGAL_EXTENSION_LANDSCAPE_IMAGE_2
