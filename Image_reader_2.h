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

#ifndef IMAGE_READER_2
#define IMAGE_READER_2

#include <algorithm>
#include <string>
#include <vector>

#include <tiffio.h>

template <typename T, typename I> class Image_2;

class Image_reader_2
{

    public:
    Image_reader_2(const std::string& filename)
        : _filename(filename)
    { }

    virtual ~Image_reader_2()
    { }

    protected:
    const std::string _filename;
};

class TIFF_image_reader_2
    : public Image_reader_2
{

    public:
    TIFF_image_reader_2(const std::string& filename)
        : Image_reader_2(filename),
        tiff_file(0), _bpp(0), _spp(0), _width(0), _height(0)
    {
        tiff_file = TIFFOpen(_filename.c_str(), "r");
        if (tiff_file == nullptr)
            throw;

        TIFFGetField(tiff_file, TIFFTAG_BITSPERSAMPLE, &_bpp);
        TIFFGetField(tiff_file, TIFFTAG_SAMPLESPERPIXEL, &_spp);

        TIFFGetField(tiff_file, TIFFTAG_IMAGELENGTH, &_height);
        TIFFGetField(tiff_file, TIFFTAG_IMAGEWIDTH, &_width);
    }

    virtual ~TIFF_image_reader_2()
    {
        _TIFFfree(tiff_file);
    }

    size_t
    bpp() const
    {
        return _bpp;
    }

    template <typename PixelT, typename InterpolationT>
    Image_2<PixelT, InterpolationT>*
    get_image() const
    {
        const uint32 linesize = TIFFScanlineSize(tiff_file);

        std::vector<PixelT> data(_width * _height);

        // Linesize must be equal to width, otherwise an image will be
        // incomplete.
        if (linesize/(_bpp/8*_spp) != _width)
            throw;

        for (uint32 i = 0; i < _height; i++)
            TIFFReadScanline(tiff_file, &data[_width*i], i, 0);

        return new Image_2<PixelT, InterpolationT>(_width, _height, &data[0]);
    }

    private:
    TIFF* tiff_file;
    uint32 _bpp, _spp;
    uint32 _width, _height;

    private:
    TIFF_image_reader_2(const TIFF_image_reader_2&);
    TIFF_image_reader_2 operator=(const TIFF_image_reader_2&);
};

#endif  // IMAGE_READER_2
