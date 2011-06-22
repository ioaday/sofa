#ifndef RIGIDTOQUATENGINE_INL
#define RIGIDTOQUATENGINE_INL

#include <sofa/component/engine/RigidToQuatEngine.h>

namespace sofa
{

namespace component
{

namespace engine
{
template <class DataTypes>
RigidToQuatEngine<DataTypes>::RigidToQuatEngine()
    : f_positions( initData (&f_positions, "positions", "Positions (Vector of 3)") )
    , f_orientations( initData (&f_orientations, "orientations", "Orientations (Quaternion)") )
    , f_rigids( initData (&f_rigids, "rigids", "Rigid (Position + Orientation)") )
{
    //
    addAlias(&f_positions,"position");
    addAlias(&f_orientations,"orientation");
    addAlias(&f_rigids,"rigid");
}

template <class DataTypes>
RigidToQuatEngine<DataTypes>::~RigidToQuatEngine()
{

}

template <class DataTypes>
void RigidToQuatEngine<DataTypes>::init()
{
    addInput(&f_positions);
    addInput(&f_orientations);

    addOutput(&f_rigids);

    setDirtyValue();
}

template <class DataTypes>
void RigidToQuatEngine<DataTypes>::reinit()
{
    update();
}

template <class DataTypes>
void RigidToQuatEngine<DataTypes>::update()
{
    cleanDirty();

    const helper::vector<Vec3>& positions = f_positions.getValue();
    const helper::vector<Quat>& orientations = f_orientations.getValue();

    helper::vector<RigidVec3>& rigids = *(f_rigids.beginEdit());

    unsigned int sizeRigids = positions.size();

    if(positions.size() != orientations.size())
    {
        serr << "Warnings : size of positions and orientations are not equal" << sendl;
        sizeRigids = ( positions.size() > orientations.size() ) ?  orientations.size() :  positions.size() ;
    }

    rigids.clear();
    for (unsigned int i=0 ; i< sizeRigids ; i++)
    {
        RigidVec3 r(positions[i], orientations[i]);
        rigids.push_back(r);
    }

    f_rigids.endEdit();
}

} // namespace engine

} // namespace component

} // namespace sofa

#endif // RIGIDTOQUATENGINE_INL
