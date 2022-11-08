#include <string>
#include <vector>

#ifndef _KDTREE_KDPOINT
#define _KDTREE_KDPOINT

namespace kdtree
{
    class kdtree;

    class kdpoint
    {
        friend class kdtree;

        bool init;

        std::vector<long> values;    
        std::vector<long> min, max;

        long dimensions;

        kdpoint *left, *right;
        
    public:
        long identity;

    public:
        kdpoint(long dimensions) { makeNull(); reset(dimensions); }
        ~kdpoint() { cleanup(); }

        bool initalised() { return init; }
        void reset(long dimensions);

        void clear();
        
    public:
        void set(long *points, unsigned long length = 0L);
        void set(long value, unsigned long dimension);
        void set(long value);

        long get(unsigned long dimension);

    public:
        void copy(const kdpoint &source);
        bool equals(const kdpoint &source);
        bool inside(const kdpoint &min, const kdpoint &max);

        bool issame(long value);
        
    public:
        std::string output();

    protected:
        void makeNull();
        void cleanup();
    };
};

#endif