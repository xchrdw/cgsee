#pragma once

#include <QDateTime>
#include <QPixmap>
#include <glm/glm.hpp>

class ViewHistory
{

public:
    ViewHistory(ViewHistory* previous, glm::mat4 viewmatrix, float fovy);
    ~ViewHistory();

    void reset();

    void setNext(ViewHistory* next);
    void setPrevious(ViewHistory* previous);

    ViewHistory* getPrevious();
    ViewHistory* getNext();
    ViewHistory* getLast();
    ViewHistory* getFirst();
    qint64 getTimestamp();
    int getSize();
    glm::mat4 getViewMatrix();
    float getFovy();
    QPixmap getThumbnail();

    bool isLast();
    bool isFirst();

    bool isEqualViewMatrix(const glm::mat4 & viewmatrix);
    bool isEqualFovy(const float & fovy);

private:

    void deleteOrphaned();
    void deleteFirst();

    static int m_size;
    ViewHistory* m_previous;
    ViewHistory* m_next;
    qint64 m_timestamp;
    glm::mat4 m_viewmatrix;
    float m_fovy;
    QPixmap m_thumbnail;
};
