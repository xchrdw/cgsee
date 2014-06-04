#pragma once

#include <QDateTime>
#include <QImage>
#include <glm/glm.hpp>

class NavigationHistoryElement
{

public:
    NavigationHistoryElement(NavigationHistoryElement * previous, glm::mat4 viewmatrix, float fovy, QImage thumbnail);
    ~NavigationHistoryElement();

    void reset();

    void setNext(NavigationHistoryElement * next);
    void setPrevious(NavigationHistoryElement * previous);

    NavigationHistoryElement * getPrevious();
    NavigationHistoryElement * getNext();
    NavigationHistoryElement * getLast();
    NavigationHistoryElement * getFirst();
    qint64 getTimestamp();
    int getSize();
    glm::mat4 getViewMatrix();
    float getFovy();
    QImage getThumbnail();

    bool isLast();
    bool isFirst();

    bool isEqualViewMatrix(const glm::mat4 & viewmatrix);
    bool isEqualFovy(const float & fovy);

private:
    void deleteOrphaned();
    void deleteFirst();

    static int m_length;
    static const int MAX_LENGTH;
    static const int THUMBNAIL_SIZE;

    NavigationHistoryElement * m_previous;
    NavigationHistoryElement * m_next;
    qint64 m_timestamp;
    glm::mat4 m_viewmatrix;
    float m_fovy;
    QImage m_thumbnail;
};
