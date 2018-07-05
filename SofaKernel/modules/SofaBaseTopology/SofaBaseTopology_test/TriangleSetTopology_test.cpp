/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2018 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#include <SofaBaseTopology/SofaBaseTopology_test/fake_TopologyScene.h>
#include <sofa/helper/testing/BaseTest.h>
#include <SofaBaseTopology/TriangleSetTopologyContainer.h>
#include <sofa/helper/system/FileRepository.h>

using namespace sofa::component::topology;
using namespace sofa::helper::testing;


class TriangleSetTopology_test : public BaseTest
{
public:
    bool testEmptyContainer();
    bool testTriangleBuffers();
    bool testEdgeBuffers();
    bool testVertexBuffers();
    bool checkTopology();
};


bool TriangleSetTopology_test::testEmptyContainer()
{
    TriangleSetTopologyContainer::SPtr triangleContainer = sofa::core::objectmodel::New< TriangleSetTopologyContainer >();
    EXPECT_EQ(triangleContainer->getNbTriangles(), 0);
    EXPECT_EQ(triangleContainer->getNumberOfElements(), 0);
    EXPECT_EQ(triangleContainer->getNumberOfTriangles(), 0);
    EXPECT_EQ(triangleContainer->getTriangles().size(), 0);

    EXPECT_EQ(triangleContainer->getNumberOfEdges(), 0);
    EXPECT_EQ(triangleContainer->getNbEdges(), 0);
    EXPECT_EQ(triangleContainer->getEdges().size(), 0);

    EXPECT_EQ(triangleContainer->d_initPoints.getValue().size(), 0);
    EXPECT_EQ(triangleContainer->getNbPoints(), 0);
    EXPECT_EQ(triangleContainer->getPoints().size(), 0);

    return true;
}


bool TriangleSetTopology_test::testTriangleBuffers()
{
    fake_TopologyScene* scene = new fake_TopologyScene("C:/projects/sofa-dev/share/mesh/square1.obj", sofa::core::topology::TopologyObjectType::TRIANGLE);
    TriangleSetTopologyContainer* topoCon = dynamic_cast<TriangleSetTopologyContainer*>(scene->getNode().get()->getMeshTopology());

    if (topoCon == NULL)
    {
        if (scene != NULL)
            delete scene;
        return false;
    }

    // Check creation of the container
    EXPECT_EQ((topoCon->getName()), std::string("topoCon"));

    // Check triangle container buffers size
    EXPECT_EQ(topoCon->getNbTriangles(), 26);
    EXPECT_EQ(topoCon->getNumberOfElements(), 26);
    EXPECT_EQ(topoCon->getNumberOfTriangles(), 26);
    EXPECT_EQ(topoCon->getTriangles().size(), 26);

    // check edges should not be created
    EXPECT_EQ(topoCon->getNumberOfEdges(), 0);
    EXPECT_EQ(topoCon->getNbEdges(), 0);
    EXPECT_EQ(topoCon->getEdges().size(), 0);

    // The first 2 triangles in this file should be :
    sofa::helper::fixed_array<TriangleSetTopologyContainer::PointID, 3> triTruth0(0, 18, 11);
    sofa::helper::fixed_array<TriangleSetTopologyContainer::PointID, 3> triTruth1(0, 4, 18);


    // check triangle buffer
    const sofa::helper::vector<TriangleSetTopologyContainer::Triangle>& triangles = topoCon->getTriangleArray();
    if (triangles.empty())
        return false;
    
    // check triangle 
    const TriangleSetTopologyContainer::Triangle& tri0 = triangles[0];
    EXPECT_EQ(tri0.size(), 3u);
    
    for (int i = 0; i<3; ++i)
        EXPECT_EQ(tri0[i], triTruth0[i]);
    
    // check triangle indices
    int vertexID = topoCon->getVertexIndexInTriangle(tri0, triTruth0[1]);
    EXPECT_EQ(vertexID, 1);
    vertexID = topoCon->getVertexIndexInTriangle(tri0, triTruth0[2]);
    EXPECT_EQ(vertexID, 2);
    vertexID = topoCon->getVertexIndexInTriangle(tri0, 120);
    EXPECT_EQ(vertexID, -1);


    // Check triangle buffer access    
    const TriangleSetTopologyContainer::Triangle& tri1 = topoCon->getTriangle(1);
    for (int i = 0; i<3; ++i)
        EXPECT_EQ(tri1[i], triTruth1[i]);

    const TriangleSetTopologyContainer::Triangle& tri2 = topoCon->getTriangle(1000);
    for (int i = 0; i<3; ++i)
        EXPECT_EQ(tri2[i], -1);


    if(scene != NULL)
        delete scene;

    return true;
}


bool TriangleSetTopology_test::testEdgeBuffers()
{
    fake_TopologyScene* scene = new fake_TopologyScene("C:/projects/sofa-dev/share/mesh/square1.obj", sofa::core::topology::TopologyObjectType::TRIANGLE);
    TriangleSetTopologyContainer* topoCon = dynamic_cast<TriangleSetTopologyContainer*>(scene->getNode().get()->getMeshTopology());

    if (topoCon == NULL)
    {
        if (scene != NULL)
            delete scene;
        return false;
    }

    // create and check edges
    const sofa::helper::vector< TriangleSetTopologyContainer::TrianglesAroundEdge >& triAroundEdges = topoCon->getTrianglesAroundEdgeArray();
        
    // check only the edge buffer size: Full test on edges are done in EdgeSetTopology_test
    EXPECT_EQ(topoCon->getNumberOfEdges(), 45);
    EXPECT_EQ(topoCon->getNbEdges(), 45);
    EXPECT_EQ(topoCon->getEdges().size(), 45);

    // check edge created element
    TriangleSetTopologyContainer::Edge edge = topoCon->getEdge(0);
    EXPECT_EQ(edge[0], 18);
    EXPECT_EQ(edge[1], 11);


    // check TriangleAroundEdge buffer access
    EXPECT_EQ(triAroundEdges.size(), 45);
    const TriangleSetTopologyContainer::TrianglesAroundEdge& triAEdge = triAroundEdges[0];
    const TriangleSetTopologyContainer::TrianglesAroundEdge& triAEdgeM = topoCon->getTrianglesAroundEdge(0);

    EXPECT_EQ(triAEdge.size(), triAEdgeM.size());
    for (int i = 0; i < triAEdge.size(); i++)
        EXPECT_EQ(triAEdge[i], triAEdgeM[i]);

    // check TriangleAroundEdge buffer element for this file
    EXPECT_EQ(triAEdge[0], 0);
    EXPECT_EQ(triAEdge[1], 15);


    // check EdgesInTriangle buffer acces
    const sofa::helper::vector< TriangleSetTopologyContainer::EdgesInTriangle > & edgeInTriangles = topoCon->getEdgesInTriangleArray();
    EXPECT_EQ(edgeInTriangles.size(), 26);

    const TriangleSetTopologyContainer::EdgesInTriangle& edgeInTri = edgeInTriangles[2];
    const TriangleSetTopologyContainer::EdgesInTriangle& edgeInTriM = topoCon->getEdgesInTriangle(2);

    EXPECT_EQ(edgeInTri.size(), edgeInTriM.size());
    for (int i = 0; i < edgeInTri.size(); i++)
        EXPECT_EQ(edgeInTri[i], edgeInTriM[i]);

    sofa::helper::fixed_array<int, 3> edgeInTriTruth(5, 6, 3);
    for (int i = 0; i<3; ++i)
        EXPECT_EQ(edgeInTri[i], edgeInTriTruth[i]);
    
    
    // Check Edge Index in Triangle
    for (int i = 0; i<3; ++i)
        EXPECT_EQ(topoCon->getEdgeIndexInTriangle(edgeInTri, edgeInTriTruth[i]), i);

    int edgeId = topoCon->getEdgeIndexInTriangle(edgeInTri, 20000);
    EXPECT_EQ(edgeId, -1);

    // check link between TrianglesAroundEdge and EdgesInTriangle
    for (int i = 0; i < 4; ++i)
    {
        TriangleSetTopologyContainer::EdgeID edgeId = i;
        const TriangleSetTopologyContainer::TrianglesAroundEdge& _triAEdge = triAroundEdges[edgeId];
        for (int j = 0; j < _triAEdge.size(); ++j)
        {
            TriangleSetTopologyContainer::TriangleID triId = _triAEdge[j];
            const TriangleSetTopologyContainer::EdgesInTriangle& _edgeInTri = edgeInTriangles[triId];
            bool found = false;
            for (int k = 0; k < _edgeInTri.size(); ++k)
            {
                if (_edgeInTri[k] == edgeId)
                {
                    found = true;
                    break;
                }
            }

            if (found == false)
            {
                if (scene != NULL)
                    delete scene;
                return false;
            }
        }
    }
    
    return true;
}


bool TriangleSetTopology_test::testVertexBuffers()
{
    fake_TopologyScene* scene = new fake_TopologyScene("C:/projects/sofa-dev/share/mesh/square1.obj", sofa::core::topology::TopologyObjectType::TRIANGLE);
    TriangleSetTopologyContainer* topoCon = dynamic_cast<TriangleSetTopologyContainer*>(scene->getNode().get()->getMeshTopology());

    if (topoCon == NULL)
    {
        if (scene != NULL)
            delete scene;
        return false;
    }

    // create and check vertex buffer
    const sofa::helper::vector< TriangleSetTopologyContainer::TrianglesAroundVertex >& triAroundVertices = topoCon->getTrianglesAroundVertexArray();

    //// check only the vertex buffer size: Full test on vertics are done in PointSetTopology_test
    EXPECT_EQ(topoCon->d_initPoints.getValue().size(), 20);
    EXPECT_EQ(topoCon->getNbPoints(), 20); //TODO: check why 0 and not 20
    EXPECT_EQ(topoCon->getPoints().size(), 20);


    // check TrianglesAroundVertex buffer access
    EXPECT_EQ(triAroundVertices.size(), 20);
    const TriangleSetTopologyContainer::TrianglesAroundVertex& triAVertex = triAroundVertices[0];
    const TriangleSetTopologyContainer::TrianglesAroundVertex& triAVertexM = topoCon->getTrianglesAroundVertex(0);

    EXPECT_EQ(triAVertex.size(), triAVertexM.size());
    for (int i = 0; i < triAVertex.size(); i++)
        EXPECT_EQ(triAVertex[i], triAVertexM[i]);

    // check TrianglesAroundVertex buffer element for this file    
    EXPECT_EQ(triAVertex[0], 0);
    EXPECT_EQ(triAVertex[1], 1);


    const TriangleSetTopologyContainer::Triangle &tri = topoCon->getTriangle(1);
    int vId = topoCon->getVertexIndexInTriangle(tri, 0);
    EXPECT_NE(vId, -1);
    vId = topoCon->getVertexIndexInTriangle(tri, 20000);
    EXPECT_EQ(vId, -1);

    return true;
}



bool TriangleSetTopology_test::checkTopology()
{
    fake_TopologyScene* scene = new fake_TopologyScene("C:/projects/sofa-dev/share/mesh/square1.obj", sofa::core::topology::TopologyObjectType::TRIANGLE);
    TriangleSetTopologyContainer* topoCon = dynamic_cast<TriangleSetTopologyContainer*>(scene->getNode().get()->getMeshTopology());

    if (topoCon == NULL)
    {
        if (scene != NULL)
            delete scene;
        return false;
    }

    bool res = topoCon->checkTopology();
    
    if (scene != NULL)
        delete scene;
    
    return res;
}



TEST_F(TriangleSetTopology_test, testEmptyContainer)
{
    ASSERT_TRUE(testEmptyContainer());
}

TEST_F(TriangleSetTopology_test, testTriangleBuffers)
{
    ASSERT_TRUE(testTriangleBuffers());
}

TEST_F(TriangleSetTopology_test, testEdgeBuffers)
{
    ASSERT_TRUE(testEdgeBuffers());
}

TEST_F(TriangleSetTopology_test, testVertexBuffers)
{
    ASSERT_TRUE(testVertexBuffers());
}


TEST_F(TriangleSetTopology_test, checkTopology)
{
    ASSERT_TRUE(checkTopology());
}


// TODO: test element on Border
// TODO: test triangle add/remove
// TODO: test check connectivity
