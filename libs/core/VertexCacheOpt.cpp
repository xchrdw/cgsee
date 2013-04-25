#include <cstdlib>     
#include <ctime>       

#include "VertexCacheOpt.h"


void VertexCacheOpt::applyOptimization(t_uints &indices) {
	srand (time(NULL));
	int n, m;
	for (int i=0; i<100000; ++i)
	{
		n = (rand()%indices.size()/3)*3;
		m = (rand()%indices.size()/3)*3;
		
		if (n==m)
			continue;

		indices[n] ^= indices[m] ^= indices[n] ^= indices[m];
		indices[n+1] ^= indices[m+1] ^= indices[n+1] ^= indices[m+1];
		indices[n+2] ^= indices[m+2] ^= indices[n+2] ^= indices[m+2];
	}
}