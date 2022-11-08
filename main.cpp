#include <iostream>
#include <vector>
#include "kdtree.h"

int main(int argc, char *argv[])
{
    int points[][2] = { {97,23},{55,77},{34,76},{80,60},{99,04},{81,05},{05,81},{30,79},{15,80},{70,65},
                        {90,40},{40,30},{30,40},{20,60},{60,50},{20,20},{30,01},{60,40},{70,25},{44,62},
                        {55,55},{55,10},{15,45},{83,22},{76,46},{56,32},{45,55},{10,70},{10,30},{79,50}
                      };

    const int count = 30;
    bool is_front[count];

	kdtree::kdpoint origin(2);
	origin.set(0L);

	kdtree::kdtree *approximation = new kdtree::kdtree(2,count);
	if(approximation == NULL) return 0;

	for(int i = 0; i < count; ++i)
	{	
        is_front[i] = true;

		kdtree::kdpoint point1(2);
		point1.set(points[i][0],0);
		point1.set(points[i][1],1);

		approximation->insert(&point1);
	}

	for(int i = 0; i < count; ++i)
	{
		kdtree::kdpoint point1(2);
		point1.set(points[i][0],0);
		point1.set(points[i][1],1);

		for(int j = 0; j < count; ++j)
		{
			if(i != j)
			{
				kdtree::kdpoint point2(2);
				point2.set(points[j][0],0);
				point2.set(points[j][1],1);

				if(approximation->inside(point1, &origin, &point2))
				{
					is_front[i] = false;
					break;
				}
				
			}
		}
	}
    
    std::cout << "Pareto Front;\n";

	for(int i = 0; i < count; ++i)
	{
		if(is_front[i]) 
		{
            std::cout << points[i][0] << "," << points[i][1] << "\r\n";
		}
	}

    delete approximation;

    return 0;
}