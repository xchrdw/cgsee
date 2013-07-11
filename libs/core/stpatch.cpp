#include "stpatch.h"
namespace
{
    struct SortByPolarAngleWith
    {
        SortByPolarAngleWith(glm::vec2 point):
            m_point(point)
        {

        }

        bool operator ()(glm::vec2 a, glm::vec2 b)
        {
            glm::vec2 ma = a - m_point;
            glm::vec2 mb = b - m_point;

            return glm::normalize(ma).x > glm::normalize(mb).x;
        }

    protected:
        glm::vec2 m_point;
    };

    //from http://en.wikipedia.org/wiki/Graham_scan#Pseudocode
    bool isCcw(glm::vec2 a, glm::vec2 b, glm::vec2 c)
    {
        return (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x) > 0;
    }
}

void StPatch::addLine(int start, int end)
{
    glm::ivec2 toAdd(qMin(start, end), qMax(start, end));
    if (!m_lines.contains(toAdd))
        m_lines.append(toAdd);
}

glm::ivec2 StPatch::addLine(glm::vec2 start, glm::vec2 end)
{
    glm::vec2 points [2] = {start, end};
    glm::ivec2 indices;

    for (int i = 0; i < 2; ++i)
    {
        bool found = false;
        int mpointsSize = m_points.size(); 
        for (int j = 0; j < mpointsSize; ++j)
        {
            if (glm::dot(m_points[j] - points[i], m_points[j] - points[i]) < 0.0001)
            {
                found = true;
                indices[i] = j;
                break;
            }
        }

        if (!found)
        {
            m_points.append(points[i]);
            indices[i] = m_points.size() - 1;
        }
    }
    addLine(indices.x, indices.y);
    return indices;
}

void StPatch::addTriangle(int a, int b, int c)
{
    m_triangles.append(glm::ivec3(a,b,c));
    addLine(a, b);
    addLine(b, c);
    addLine(a, c);
}

glm::ivec3 StPatch::addTriangle(glm::vec2 a, glm::vec2 b, glm::vec2 c)
{
    glm::ivec2 ab = addLine(a, b);
    glm::ivec2 bc = addLine(b, c);
    glm::ivec2 ac = addLine(a, c);
    assert(ab.x == ac.x);
    assert(ab.y == bc.x);
    assert(bc.y == ac.y);

    glm::ivec3 result(ab.xy, bc.y);
    m_triangles.append(result);
    return result;
}

// an implementation of http://en.wikipedia.org/wiki/Graham_scan#Pseudocode
void StPatch::calculateConvexHull(QList<glm::vec2> &out) const
{
    out.clear();
    // find the point with the lowest y coord
    out = m_points;
    int startPoint = 0;
    for (int i = 0; i < m_points.size(); ++i)
    {
        if (m_points[i].y < m_points[startPoint].y)
            startPoint = i;
    }
    out.swap(0, startPoint);
    qSort(out.begin() + 1, out.end(), SortByPolarAngleWith(out[0]));
    out.append(out[0]); // add start point to the end of the list

    for (int i = 2; i < out.size()-1;)
    {
        bool throwIt = false;
        for (int j = i+1; j < out.size(); ++j)
        {
            if (!isCcw(out[i-1], out[i], out[j]))
            {
                throwIt = true;
                break;
            }
        }

        if (throwIt)
        {
            out.removeAt(i);
        }
        else
            ++i;
    }

    assert(out.last() == out.first());
}

void StPatch::calculateAabb(glm::vec2 &min, glm::vec2 &max) const
{
    min = glm::vec2(std::numeric_limits<float>::infinity());
    max = glm::vec2(- std::numeric_limits<float>::infinity());

    for (glm::vec2 const &point: m_points)
    {
        min = glm::min(min, point);
        max = glm::max(max, point);
    }
}

void StPatch::apply(glm::mat2 rotScale, glm::vec2 transl)
{
    for (glm::vec2 &point: m_points)
    {
        point = rotScale * point + transl;
    }
}

int StPatch::countLines() const
{
    return m_lines.size();
}

glm::vec4 StPatch::getLine(int i) const
{
    return glm::vec4(m_points[m_lines[i].x], m_points[m_lines[i].y]);
}

int StPatch::countPoints() const
{
    return m_points.size();
}

glm::vec2 StPatch::getPoint(int i) const
{
    return m_points[i];
}

bool StPatch::pointIntersects(glm::vec2 point) const
{
    for (glm::ivec3 const& tri: m_triangles)
    {
        glm::vec2 a = m_points[tri.x], b = m_points[tri.y], c = m_points[tri.z];
        glm::vec2 ab = b - a; glm::vec2 ac = c - a;
        glm::vec2 p = point - a;

        glm::vec2 v1(-ac.y, ac.x); v1 /= glm::dot(v1, ab);
        glm::vec2 v2(-ab.y, ab.x); v2 /= glm::dot(v2, ac);

        glm::vec2 coeff(glm::dot(v1, p), glm::dot(v2, p));
        if (coeff.x > 0 && coeff.y > 0 && glm::dot(coeff, glm::vec2(1,1)) < 1)
            return true;
    }
    return false;
}

bool StPatch::linesIntersect(glm::vec4 l1, glm::vec4 l2)
{
    glm::mat2 equationMatrix (l1.z - l1.x, l2.x - l2.z, 
        l1.w - l1.y, l2.y - l2.w);
    if (std::abs(glm::determinant(equationMatrix)) < 0.003)
        return false; // for our purposes, parallel lines are as good as not intersecting lines
    glm::mat2 solutionMatrix = glm::inverse(equationMatrix);
    glm::vec2 solution = solutionMatrix * (l2.xy - l1.xy);
    
    if (solution.x > 0.003 && solution.x < 0.997 && solution.y > 0.003 && solution.y < 0.997)
    {
        return true;
    }
    return false;
}