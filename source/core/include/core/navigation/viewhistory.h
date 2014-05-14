#pragma once

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
    int getId();
    int getSize();
    glm::mat4 getViewMatrix();
    float getFovy();

    bool isLast();
    bool isFirst();

    bool isEqualTo(const ViewHistory & viewhistory);

private:

    void deleteOrphaned();
    void deleteFirst();

    static int m_size;
    ViewHistory* m_previous;
    ViewHistory* m_next;
    int m_id;
    glm::mat4 m_viewmatrix;
    float m_fovy;

    // @TODO small view screenshot m_thumbnail
    // @TODO replace ID with m_timestamp
};
