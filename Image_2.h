#ifndef IMAGE_2
#define IMAGE_2

template <typename PixelValue>
class Image_2
{
    public:
    Image_2(const size_t x, const size_t y, const PixelValue* d)
        : _x_size(x), _y_size(y), _data(0)
    {
        _data = new PixelValue[x*y];
        for (size_t i = 0; i < x*y; i++)
            _data[i] = d[i];
        //std::copy(d, d+x*y, &_data);
    }

    template <typename InputIterator>
    Image_2(const size_t stride, InputIterator first, InputIterator last)
        : _x_size(stride), _y_size(std::distance(first, last)/stride)
    {
        _data = new PixelValue[_x_size*_y_size];
        std::copy(first, last, _data);
    }

    virtual ~Image_2()
    {
        delete[] _data;
    }

    inline size_t x_size() const { return _x_size; }
    inline size_t y_size() const { return _y_size; }

    inline
    PixelValue
    operator()(const size_t x, const size_t y) const
    {
        return this->get(x, y);
    }

    inline
    PixelValue
    get(const size_t x, const size_t y) const
    {
        return _data[_x_size*y + x];
    }

    inline
    void
    getPatch(const size_t x, const size_t y, const unsigned short s,
        PixelValue* const patch) const
    {
        PixelValue* p0 = _data + y * _x_size + x;   // pointer to BL corner.
        for (unsigned short i = 0; i <= s; i++)
            for (unsigned short j = 0; j <= s; j++)
                patch[i*(s+1) + j] = *(p0 + i * _x_size + j);
    }

#define LINEAR_INTERPOLATION 1
    inline
    PixelValue
    interpolate(const double x, const double y) const
    {
        if (x < 0 || y < 0)
            return 0;

        const size_t x0 = std::floor(x);
        const size_t y0 = std::floor(y);

#ifdef CONSTANT_INTERPOLATION
        if (x0 >= _x_size || y0 >= _y_size)
            return 0;

        return this->get(x0, y0);
#endif  // CONSTANT_INTERPOLATION

#ifdef LINEAR_INTERPOLATION

        if (x0 + 1 >= _x_size || y0 + 1 >= _y_size)
            return 0;

        static double fs[4];
        this->getPatch(x0, y0, 1, fs);

        const double dx  = x - x0;
        const double dy  = y - y0;
        const double dx1 = 1 - dx;
        const double dy1 = 1 - dy;

        const double weights[4] = { dx1 * dy1, dx * dy1, dx1 * dy, dx * dy };

        return fs[0] * weights[0] +
            fs[1] * weights[1] +
            fs[2] * weights[2] +
            fs[3] * weights[3];

#endif  // LINEAR_INTERPOLATION
    }

    private:
    size_t _x_size;
    size_t _y_size;

    protected:
    PixelValue* _data;

    private:
    Image_2<PixelValue>(const Image_2<PixelValue>&);
    Image_2<PixelValue>& operator=(const Image_2<PixelValue>&);
};

#endif  // IMAGE_2
