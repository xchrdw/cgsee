#pragma once
#include <QList>
#include <glm/glm.hpp>
#include "declspec.h"

class CGSEE_CORE_API StPatch
{
public:
    //StPatch();

    glm::ivec2 addLine(glm::vec2 start, glm::vec2 end);
    void addLine(int start, int end);
    glm::ivec3 addTriangle(glm::vec2 a, glm::vec2 b, glm::vec2 c);
    void addTriangle(int a, int b, int c);

    void calculateConvexHull(QList<glm::vec2> &out)const;
    void calculateAabb(glm::vec2 &min, glm::vec2 &max)const;
    void apply(glm::mat2 rotScale, glm::vec2 transl);

    int countLines() const;
    glm::vec4 getLine(int i) const;
    int countPoints() const;
    glm::vec2 getPoint(int i) const;

    // Additional stuff
    void optimizeRotation();

    bool pointIntersects(glm::vec2 p) const;
    static bool linesIntersect(glm::vec4 l1, glm::vec4 l2);
protected:
    QList<glm::vec2> m_points;
    QList<glm::ivec2> m_lines;
    QList<glm::ivec3> m_triangles;
};