//================================================
// @title        mx_wht.cpp
// @author       Jonathan Hadida
// @contact      Jonathan.hadida [at] dtc.ox.ac.uk
//================================================

#include "wht_util.h"
#include "mexArray.h"
#include <algorithm>

#define SEQUENCY_ORDER true



        /********************     **********     ********************/
        /********************     **********     ********************/



/**
 * Dependency to MexArray (see https://github.com/Sheljohn/MexArray).
 * Use the script start_matlab.sh to start Matlab, and compile with:
 * mex CXXFLAGS="\$CXXFLAGS -std=c++0x -Wall" mx_wht.cpp
 */
void mexFunction(	int nargout, mxArray *out[],
					int nargin, const mxArray *in[] )
{
	if ( nargin != 1 )
	{		
		mexErrMsgTxt("Usage: w = mx_wht(sequence);");
		return;
	}

	ndArray<const double,2> input(in[0]);
	ndArray<double,2> output( out[0] );

	std::vector<double> d( input.begin(), input.end() );
	fwht( d, SEQUENCY_ORDER );

	int size[2];
		size[0] = 1;
		size[1] = d.size();
	out[0] = mxCreateNumericArray( 2, size, mx_type<double>::id, mxREAL );
	std::copy( d.begin(), d.end(), output.begin() );
}