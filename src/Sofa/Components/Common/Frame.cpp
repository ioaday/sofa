/* Francois Faure, INRIA-UJF, 2006
 */
#include "Frame.h"
#include <iostream>
using std::cout;
using std::endl;

namespace Sofa
{
namespace Components
{
namespace Common
{



Frame::Frame (const Vec3 &origin, const Mat33& matrix )
    : origin_(origin)
    , basis_(matrix)
{}
Frame::Frame (const Vec3 &origin, const Quat &orientation, const Vec3& scale )
{
    setTransform( origin, orientation, scale );
}

Frame::Frame (const Vec3 &origin )
    : origin_(origin)
{
    basis_.identity();
}

Frame::Frame ()
{
    clear();
}

void Frame::clear()
{
    basis_.identity();
    origin_.clear();
}

//void clear() { origin_.clear(); basis__.clear(); scale_=Vec3(1,1,1); }

Frame::Vec3& Frame::getOrigin ()
{
    return origin_;
}
const Frame::Vec3& Frame::getOrigin () const
{
    return origin_;
}
void Frame::setOrigin( const Frame::Vec3& origin )
{
    origin_=origin;
}

Frame::Mat33& Frame::getBasis ()
{
    return basis_;
}
const Frame::Mat33& Frame::getBasis () const
{
    return basis_;
}
void Frame::setBasis( const Frame::Mat33& m )
{
    basis_=m;
}

void Frame::setTransform( const Frame::Vec3& origin, const Frame::Quat& orientation, const Frame::Vec3& scale )
{
    setOrigin(origin);
    orientation.toMatrix(basis_);
    for( int i=0; i<3; i++ )
    {
        basis_[i][0] *= scale[0];
        basis_[i][1] *= scale[1];
        basis_[i][2] *= scale[2];
    }
}



Frame Frame::identity()
{
    Frame c;
    c.clear();
    return c;
}

/// Apply a transformation defined in the child frame (mult. right)
void Frame::multRight( const Frame& c )
{
    origin_ += basis_ * c.getOrigin();
    basis_ = basis_ * c.getBasis();
}

/// compute the product with another frame on the right
Frame Frame::mult( const Frame& c ) const
{
    Frame r = (*this);
    r.multRight(c);
    return r;
}

/// Write the OpenGL transformation matrix
void Frame::writeOpenGlMatrix( float m[16] ) const
{
    m[0] = basis_[0][0];
    m[1] = basis_[1][0];
    m[2] = basis_[2][0];
    m[3] = 0;
    m[4] = basis_[0][1];
    m[5] = basis_[1][1];
    m[6] = basis_[2][1];
    m[7] = 0;
    m[8] = basis_[0][2];
    m[9] = basis_[1][2];
    m[10] = basis_[2][2];
    m[11] = 0;
    m[12] = origin_[0];
    m[13] = origin_[1];
    m[14] = origin_[2];
    m[15] = 1;
}

/// Compute the transformation from the parent to the child
Frame Frame::inversed() const
{
    Mat33 inv;
#ifdef NDEBUG
    inv.invert( basis_ );
#else
    bool invertible = inv.invert( basis_ );
    assert( invertible );
#endif
    return Frame( -(inv*origin_) , inv );
}

std::ostream& operator << (std::ostream& out, const Sofa::Components::Common::Frame& c )
{
    out<<"origin = "<<c.getOrigin()<<", basis matrix="<<endl;
    for( int i=0; i<3; i++ )
    {
        for( int j=0; j<3; j++ )
            out<<c.getBasis()[i][j]<<" ";
        out<<endl;
    }
    return out;
}

}//Common
}//Components
}//Sofa



