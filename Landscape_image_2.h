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
#include <iostream>
#include <vector>
#include <cmath>
#include <CGAL/basic.h>


namespace CGAL_Extension
{

template <typename FT, typename Point_3>
class Landscape_image_2
{
    public:
    Landscape_image_2(const std::string& file)
    {
        tiff_file = TIFFOpen(file.c_str(), "r");
        TIFFGetField(tiff_file, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tiff_file, TIFFTAG_IMAGEWIDTH, &width);

        uint32 bpp, spp;
        TIFFGetField(tiff_file, TIFFTAG_BITSPERSAMPLE, &bpp);
        TIFFGetField(tiff_file, TIFFTAG_SAMPLESPERPIXEL, &spp);

        std::cerr << "bpp/spp " << bpp << "/" << spp << std::endl;

        const uint32 linesize = TIFFScanlineSize(tiff_file);
        std::cerr << "linesize " << linesize << std::endl;

        unsigned char* buf = (unsigned char*) malloc(linesize * height);
        for (uint32 i = 0; i < height; i++)
            TIFFReadScanline(tiff_file, &buf[i * linesize], i, 0);

        data = std::vector<FT>(width * height);
        // Scale image to [0, 1].
        const auto min_max = std::minmax_element((unsigned char*)buf,(unsigned char*)(buf + width * height));
        const double scale = 1;// *(min_max.second) - *(min_max.first);
        const double shift = *(min_max.first);
        std::cerr << "min/max " << *(min_max.first) << "/" << *(min_max.second);
        std::cerr << " scale/shift " << scale << "/" << shift << std::endl;
        //const double scale = 1;
        //const double shift = 0;
        for (uint32 i = 0; i < height; i++)
        {
            for (uint32 j = 0; j < width; j++)
            {
                data[i * width + j] =
                    (buf[i*height + j] - shift) / scale;
            }
        }
        //_TIFFfree(raster);
    }

    ~Landscape_image_2()
    {
        /*
        if (raster != nullptr)
            _TIFFfree(raster);
        TIFFClose(tiff_file);
        */
    }

    size_t xdim() const { return width; }
    size_t ydim() const { return height; }

#define LINEAR_INTERPOLATION 1
    FT
    operator()(const Point_3& p) const
    {
        const int x0 = std::floor(CGAL::to_double(p.x()));
        const int y0 = std::floor(CGAL::to_double(p.y()));
        const FT* p0 = &data[0] + y0 * height + x0;

        if (x0 < 0 || y0 < 0)
            return -1;

        FT f;
#ifdef CONSTANT_INTERPOLATION
        if (x0 >= width || y0 >= height)
            return -1;

        f = *p0;
#endif  // CONSTANT_INTERPOLATION

#ifdef LINEAR_INTERPOLATION

        if (x0 + 1 >= width || y0 + 1 >= height)
            return -1;

        const FT& f00 = p0[0 * height + 0];
        const FT& f01 = p0[0 * height + 1];
        const FT& f10 = p0[1 * height + 0];
        const FT& f11 = p0[1 * height + 1];

        const FT dx = p.x() - x0;
        const FT dy = p.y() - y0;

        f = f00 * (1 - dx) * (1 - dy) +
            f01 *      dx  * (1 - dy) +
            f10 * (1 - dx) *      dy  +
            f11 *      dx  *      dy;
#endif  // LINEAR_INTERPOLATION

        if (f <= 0)
            return -1;
        return p.z() - f;
    }

    private:
    TIFF* tiff_file;
    uint32 width, height;
    std::vector<FT> data;

    private:
    //Landscape_image_2(const Landscape_image_2&);
    //Landscape_image_2 operator=(const Landscape_image_2&);
};

}   // namespace CGAL_Extension

#endif  // CGAL_EXTENSION_LANDSCAPE_IMAGE_2
