#include "kdpoint.h"
#include <limits.h>

void kdtree::kdpoint::reset(long dimensions)
{
    init = false; cleanup();

    left = NULL; right = NULL;

    this->dimensions = dimensions;

    identity = 0L;

    values.reserve(dimensions);
    min.reserve(dimensions);
    max.reserve(dimensions);

    for(long i=0L; i < dimensions; ++i)
    {
        values[i] = 0L;
        min[i] = LONG_MIN;
        max[i] = LONG_MAX;
    }

    init = true;
}

void kdtree::kdpoint::clear()
{
    left = NULL; right = NULL;

    for(long i=0L; i < dimensions; ++i)
    {
        values[i] = 0L;
        min[i] = LONG_MIN;
        max[i] = LONG_MAX;
    }
}

void kdtree::kdpoint::set(long *points, unsigned long length)
{
    long min = length;
    if(min == 0L) min = dimensions;
    if(min > dimensions) min = dimensions;

    for(long i = 0L; i < min; ++i)
    {
        values[i] = points[i];
    }
}

void kdtree::kdpoint::set(long value, unsigned long dimension)
{
    if(dimension >= dimensions) return;
    if(dimension < 0L) return;

    values[dimension] = value;
}

void kdtree::kdpoint::set(long value)
{
    for(long i = 0L; i < dimensions; ++i)
    {
        values[i] = 0L;
        min[i] = LONG_MIN;
        max[i] = LONG_MAX;
    }
}

long kdtree::kdpoint::get(unsigned long dimension)
{
    if(dimension >= dimensions) return -1L;
    if(dimension < 0L) return -1L;

    return values[dimension];
}

void kdtree::kdpoint::copy(const kdpoint &source)
{
    if (dimensions <= source.dimensions)
    {
        long _min = dimensions;
        if(source.dimensions < _min) _min = source.dimensions;

        for(long i=0L; i < _min; ++i)
        {
            values[i] = source.values[i];
            min[i] = source.min[i];
            max[i] = source.max[i];
        }
    
        dimensions = source.dimensions;
        identity = source.identity;
    }
}

bool kdtree::kdpoint::equals(const kdpoint &source)
{
    if (dimensions < source.dimensions) return false;
    
    long min = dimensions;
    if(source.dimensions < min) min = source.dimensions;

    for(long i=0L; i < min; ++i)
    {
        if(values[i] != source.values[i]) return false;
    }
    
    return true;
}

bool kdtree::kdpoint::inside(const kdpoint &min, const kdpoint &max)
{
    if(min.dimensions != dimensions) return false;
    if(max.dimensions != dimensions) return false;

    for(long i=0L; i < dimensions; ++i)
    {
        if(values[i] < min.values[i]) return false;
        if(values[i] > max.values[i]) return false;
    }

    return true;
}

bool kdtree::kdpoint::issame(long value)
{
    for(long i=0L; i < dimensions; ++i)
    {
        if(values[i] != value) return false;
    }

    return true;
}

std::string kdtree::kdpoint::output()
{
    std::string result;

    for(long i=0L;i<dimensions;++i)
    {

        result += std::to_string(values[i]);
        if(i < dimensions - 1L) result += std::string(",");
    }

    return result;
}

void kdtree::kdpoint::makeNull()
{
    
}

void kdtree::kdpoint::cleanup()
{

}
