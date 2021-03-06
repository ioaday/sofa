#include "HolonomicConstraintValue.h"

#include <sofa/core/ObjectFactory.h>
#include "../utils/map.h"

namespace sofa {
namespace component {
namespace odesolver {


int HolonomicConstraintValueClass = core::RegisterObject("Holonomic constraint").add< HolonomicConstraintValue >();


using namespace utils;

HolonomicConstraintValue::HolonomicConstraintValue( mstate_type* mstate )
    : Stabilization( mstate )
{}


void HolonomicConstraintValue::dynamics(SReal* dst, unsigned n, unsigned dim, bool /*stabilization*/, const core::MultiVecCoordId& posId, const core::MultiVecDerivId&) const {
    assert( mstate );

    const unsigned size = n*dim;

    // warning only cancelling relative velocities of violated constraints (given by mask)

    assert( mask.getValue().empty() || mask.getValue().size() == n );
    const mask_type& mask = this->mask.getValue();

    if( mask.empty() ){
        memset( dst, 0, size*sizeof(SReal) );
    }
    else {
        // for possible elastic constraint
        mstate->copyToBuffer(dst, posId.getId(mstate.get()), size);

        unsigned i = 0;
        for(SReal *it=dst, *last = dst + size; it < last; it+=dim, ++i) {
            if( mask[i] ) memset( it, 0, dim*sizeof(SReal) ); // already violated
            else map(it, dim) *= -1.0 / this->getContext()->getDt(); // not violated
        }
    }


}




}
}
}
