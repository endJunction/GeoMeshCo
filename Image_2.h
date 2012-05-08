#ifndef IMAGE_2
#define IMAGE_2

#include <boost/optional.hpp>

template <typename PixelT, typename InterpolationT = double>
class Image_2
{
    public:
    typedef typename boost::optional<InterpolationT> Value;

    public:
    Image_2(const size_t x, const size_t y, const PixelT* d)
        : _x_size(x), _y_size(y), _data(new PixelT[x*y])
    {
        for (size_t i = 0; i < x*y; i++)
            _data[i] = d[i];
        //std::copy(d, d+x*y, &_data);
    }

    virtual ~Image_2()
    {
        delete[] _data;
    }

    inline size_t x_size() const { return _x_size; }
    inline size_t y_size() const { return _y_size; }

    inline
    PixelT
    operator()(const size_t x, const size_t y) const
    {
        return this->get(x, y);
    }

    inline
    PixelT
    get(const size_t x, const size_t y) const
    {
        return _data[_x_size*y + x];
    }

#define LINEAR_INTERPOLATION 1
    inline
    Value
    interpolate(const double x, const double y) const
    {
        if (x < 0 || y < 0)
            return Value();

        const size_t x0 = std::floor(x);
        const size_t y0 = std::floor(y);

#ifdef CONSTANT_INTERPOLATION
        if (x0 >= _x_size || y0 >= _y_size)
            return Value();

        return Value(this->get(x0, y0));
#endif  // CONSTANT_INTERPOLATION

#ifdef LINEAR_INTERPOLATION

        if (x0 + 1 >= _x_size || y0 + 1 >= _y_size)
            return Value();

        static PixelT fs[4];
        this->getPatch(x0, y0, 1, fs);

        const InterpolationT dx  = x - x0;
        const InterpolationT dy  = y - y0;
        const InterpolationT dx1 = 1 - dx;
        const InterpolationT dy1 = 1 - dy;

        const InterpolationT weights[4] = { dx1 * dy1, dx * dy1, dx1 * dy, dx * dy };

        return Value(fs[0] * weights[0] +
            fs[1] * weights[1] +
            fs[2] * weights[2] +
            fs[3] * weights[3]);

#endif  // LINEAR_INTERPOLATION
    }

    private:
    /// Writes pixel values of a patch to given pointer.
    ///
    /// A patch is a square subsection of image of size \f$(s+1)^2\f$.
    /// The values are written in following order:\n
    /// {(x,y), ..., (x+s,y), ... , (x,y+s), ..., (x+s,y+s)}.
    /// \pre    Allocated \c patch memory size \f$\ge (s+1)^2\f$.
    inline
    void
    getPatch(const size_t x, const size_t y, const unsigned short s,
        PixelT* const patch) const
    {
        PixelT* p0 = _data + y * _x_size + x;   // pointer to BL corner.
        for (unsigned short i = 0; i <= s; i++)
            for (unsigned short j = 0; j <= s; j++)
                patch[i*(s+1) + j] = *(p0 + i * _x_size + j);
    }


    private:
    size_t _x_size;
    size_t _y_size;

    protected:
    PixelT* _data;

    private:
    Image_2<PixelT, InterpolationT>(const Image_2<PixelT, InterpolationT>&);
    Image_2<PixelT, InterpolationT>& operator=(const Image_2<PixelT, InterpolationT>&);
};

#endif  // IMAGE_2
