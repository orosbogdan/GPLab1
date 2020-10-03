#include "GPSLab1.hpp"

namespace gps {

    glm::vec4 TransformPoint(const glm::vec4& point)
    {
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 finalMatrix= glm::translate(rotationMatrix, glm::vec3(2.0f, 0.0f, 1.0f));

        return finalMatrix * point;
    }

    float ComputeAngle(const glm::vec3& v1, const glm::vec3& v2)
    {
        float angle = (glm::dot(v1, v2)) / (glm::length(v1) * glm::length(v2));

        return glm::degrees(glm::acos(angle));
    }

    bool IsConvex(const std::vector<glm::vec2>& vertices)
    {
        /*
        For each consecutive pair of edges of the polygon (each triplet of points), compute the z-component 
        of the cross product of the vectors defined by the edges pointing towards
        the points in increasing order. Take the cross product of these vectors:
        The polygon is convex if the z-components of the cross products 
        are either all positive or all negative.
        */

        std::vector<glm::vec3> normalsList;
        std::vector<glm::vec3> edges;

        for (int i = 0; i < vertices.size(); i++)
        {
            glm::vec3 pointOne;
            glm::vec3 pointTwo;

            if (i == vertices.size() - 1) 
            {
                pointOne = glm::vec3(vertices[0].x, vertices[0].y, 0);
                pointTwo = glm::vec3(vertices[i].x, vertices[i].y, 0);
            }
            else {
                pointOne = glm::vec3(vertices[i + 1].x, vertices[i + 1].y, 0);
                pointTwo = glm::vec3(vertices[i].x, vertices[i].y, 0);
            }
            
            edges.push_back( pointOne-pointTwo );
        }

        //compute normals of consecutive edges
        for (int i = 0; i < edges.size(); i++)
        {

            if (i == edges.size() - 1)
            {
                normalsList.push_back(glm::normalize(glm::cross(edges[0], edges[i])));

            }
            else {
                normalsList.push_back(glm::normalize(glm::cross(edges[i+1], edges[i])));
            }

        }


        // Check if the z component of ALL edges are either positive or negative
        int positives = 0;
        int negatives = 0;
        for (int i = 0; i < normalsList.size(); i++)
        {
            if (normalsList.at(i).z >= 0) {
                positives++;
            }
            else
            {
                negatives++;
            }
        }

        return positives == 0 || negatives ==0;
    }

    std::vector<glm::vec2> ComputeNormals(const std::vector<glm::vec2>& vertices)
    {
        //Credits to Adam Godra
 
        std::vector<glm::vec2> normalsList;
        glm::vec2 edge1 = vertices.at(1) - vertices.at(0);
        glm::vec2 edge2 = vertices.at(2) - vertices.at(1);

        glm::vec3 edge1_3d = glm::vec3(edge1.x, edge1.y, 0.0f);
        glm::vec3 edge2_3d = glm::vec3(edge2.x, edge2.y, 0.0f);

        glm::vec3 plane_normal = glm::normalize(glm::cross(edge1_3d, edge2_3d));

        std::vector<glm::vec2> edges;

        for (int i = 0; i < vertices.size(); ++i)
        {
            // the same procedure, but between the current normal and the normal for the plane
            glm::vec3 currentEdge;

            if (i == vertices.size() - 1) // if you're at the last vertex, you want to form the vector determied by the  
            {
                currentEdge = glm::vec3(vertices[0].x - vertices[i].x, vertices[0].y - vertices[i].y, 0);
            }
            else {
                currentEdge = glm::vec3(vertices[i + 1].x - vertices[i].x, vertices[i + 1].y - vertices[i].y, 0);
            }

            edges.emplace_back(currentEdge);
        }

        for (auto edge : edges) {
            glm::vec3 edge_3d = glm::vec3(edge.x, edge.y, 0.0f);
            glm::vec3 normal = glm::normalize(glm::cross(edge_3d, plane_normal));
            normalsList.emplace_back(glm::vec2(normal.x, normal.y));
        }

        return normalsList;
    }
}