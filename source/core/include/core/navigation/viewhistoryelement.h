#pragma once

#include <QDateTime>
#include <QImage>
#include <glm/glm.hpp>

class ViewHistoryElement
{

public:
    ViewHistoryElement(ViewHistoryElement * previous, glm::mat4 viewmatrix, float fovy, QImage thumbnail);
    ~ViewHistoryElement();

    void reset();

    void setNext(ViewHistoryElement * next);
    void setPrevious(ViewHistoryElement * previous);

    ViewHistoryElement * getPrevious();
    ViewHistoryElement * getNext();
    ViewHistoryElement * getLast();
    ViewHistoryElement * getFirst();
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
    static int m_maxLength;
    static int m_thumbnailSize;

    ViewHistoryElement * m_previous;
    ViewHistoryElement * m_next;
    qint64 m_timestamp;
    glm::mat4 m_viewmatrix;
    float m_fovy;
    QImage m_thumbnail;
};
