#include "kdtree.h"
#include <string>
#include <stack>
#include <tuple>

void kdtree::kdtree::reset(long dimensions, long length)
{
    init = false; cleanup();

    root = NULL;

    this->dimensions = dimensions;
    this->length = length;

    unallocated.reserve(length);

    values = new kdpoint*[length];
    if(values == NULL) return;

    for (long i = 0L; i < length; ++i) values[i] = NULL;

	for (long i = 0L; i < length; ++i)
	{
        values[i] = new kdpoint(dimensions);
		if (values[i] == NULL)
		{
			return;
		}

		if (!values[i]->initalised())
		{
			return;
		}
	}

    for (long i = length - 1L; i >= 0L; i--)
    {
        unallocated.push_back(values[i]);
    }

    init = true;
}

void kdtree::kdtree::insert(kdpoint *point)
{
    std::stack<std::tuple<kdpoint**,kdpoint*,int>> stack;
    stack.push(std::tuple<kdpoint**,kdpoint*,int>(&root,NULL,0));

    while(stack.size() > 0)
    {
        std::tuple<kdpoint**,kdpoint*,int> current = stack.top();
        stack.pop();

        kdpoint **n = std::get<0>(current);
        kdpoint *parent = std::get<1>(current);
        int depth = std::get<2>(current);
        
        if(*n == NULL)
        {
            kdpoint *temp = pop();
            if(temp == NULL) return;

            temp->copy(*point);
            
            if(parent != NULL)
            {
                for(long i=0L; i < dimensions; ++i)
                {
                    temp->min[i] = parent->min[i];
                    temp->max[i] = parent->max[i];
                }

                unsigned long cd = depth % dimensions;

                for(long i = 0L; i < dimensions; ++i)
                {
                    if(i != cd)
                    {
                        if(temp->values[i] <= parent->values[i])
                        {
                            temp->max[i] = parent->values[i];
                        }
                        else temp->min[i] = parent->values[i];
                    }
                } 
            }

            *n = temp;
            return;
        }

        if((*n)->equals(*point))
        {
            return;
        }

        unsigned long cd = depth % dimensions;

        if (point->values[cd] < ((*n)->values[cd]))
        {
            stack.push(std::tuple<kdpoint**,kdpoint*,int>(&((*n)->left),*n,depth + 1UL));
        }
        else
        {
            stack.push(std::tuple<kdpoint**,kdpoint*,int>(&((*n)->right),*n,depth + 1UL));         
        }
    };
}

bool kdtree::kdtree::inside(const kdpoint &value, kdpoint *min, kdpoint *max)
{
    std::stack<std::tuple<kdpoint*,int>> stack;
    stack.push(std::tuple<kdpoint*,int>(root,0));

    while(stack.size() > 0)
    {
        std::tuple<kdpoint*,int> current = stack.top();
        stack.pop();

        kdpoint *n = std::get<0>(current);    

        if(n != NULL)
        {
            int depth = std::get<1>(current);

            if(n->inside(*min, *max)) 
            {    
                if(n->equals(value)) 
                {
                    return true;
                }
            }

            if(isintersect(n->left, min, max))
            {
                stack.push(std::tuple<kdpoint*,int>(n->left,depth+1));
            }

            if(isintersect(n->right, min, max))
            {
                stack.push(std::tuple<kdpoint*,int>(n->right,depth+1));
            }
        }
    };

    return false;
}

void kdtree::kdtree::remove(kdpoint &point)
{
   root = remove(point, root, 0);
}
  
bool kdtree::kdtree::exists(kdpoint &point)
{
    std::stack<std::tuple<kdpoint*,int>> stack;
    stack.push(std::tuple<kdpoint*,int>(root, 0));

    while(stack.size() > 0)
    {
        std::tuple<kdpoint*,int> current = stack.top();        
        stack.pop();

        kdpoint *n = std::get<0>(current);        

        if(n != NULL)
        {
            int depth = std::get<1>(current);
            int cd = depth % dimensions;

            if(n->equals(point))
            {  
                return true;
            }
        
            if (point.values[cd] < n->values[cd])
            {
                stack.push(std::tuple<kdpoint*,int>(n->left,depth+1));
            }
            else
            {
                stack.push(std::tuple<kdpoint*,int>(n->right,depth+1));
            }
        }
    };

    return false;
}

bool kdtree::kdtree::isintersect(kdpoint *root, kdpoint *min, kdpoint *max)
{
    if(root == NULL) return false;

    for(long i=0L; i < dimensions; ++i)
    {
        if (min->values[i] > root->max[i]) return false;
        if (max->values[i] < root->min[i]) return false;
    }

    return true;
}

kdtree::kdpoint *kdtree::kdtree::min(kdpoint *x, kdpoint *y, kdpoint *z, int dimension)
{
    kdpoint *result = x;

    if (y != NULL && y->values[dimension] < result->values[dimension]) result = y;
    if (z != NULL && z->values[dimension] < result->values[dimension]) result = z;
    
    return result;
}

kdtree::kdpoint *kdtree::kdtree::min(kdpoint *T, int dim, unsigned long cd)
{    
    if(T == NULL) return NULL;
        
    if(cd == dim)
    {
        if(T->left == NULL) return T;
        else return min(T->left, dim, (cd+1)%dimensions);
    }
    else
    {
        return min(T,
            min(T->left, dim, (cd+1)%dimensions),
            min(T->right, dim, (cd+1)%dimensions),
            dim);
    }
}

kdtree::kdpoint *kdtree::kdtree::remove(kdpoint &x, kdpoint *T,  int cd)
{
    if(T == NULL) return NULL;

    int next_cd = (cd+1)%dimensions;

    if(x.equals(*T))
    {
        if(T->right != NULL)
        {             
            kdpoint *m = min(T->right, cd, next_cd);
            T->copy(*m);
            T->right = remove(*T,T->right,next_cd);
        }
        else if(T->left != NULL)
        {            
            kdpoint *m = min(T->left, cd, next_cd);         
            T->copy(*m);
            T->right = remove(*T,T->left,next_cd);
            T->left = NULL;
        }
        else
        {
            push(T);

            return NULL;
        }
    }
    else if(x.values[cd] < T->values[cd])
    {
        T->left = remove(x, T->left, next_cd);
    }
    else
    {
        T->right = remove(x, T->right, next_cd);
    }

    return T;

}
kdtree::kdpoint *kdtree::kdtree::pop()
{
    if(unallocated.size() <= 0L)
    {
        return NULL;
    }

    kdpoint *temp = unallocated.back();
    unallocated.pop_back();
    
    temp->clear();

    return temp;
}

void kdtree::kdtree::push(kdpoint *source)
{
    unallocated.push_back(source);
}

void kdtree::kdtree::makeNull()
{
    values = NULL;
}

void kdtree::kdtree::cleanup()
{
    if(values != NULL)
    {
		for (long i = length - 1L; i >= 0L; i--)
		{
			if (values[i] != NULL) delete values[i];
		}

		delete[] values;
    }
}

