/// Preprocessor directive
#pragma once

/// Include header file.
#include <core/navigation/navigationhistoryelement.h>

/// Include QImage and QSize for thumbnails.
#include <QImage>
#include <QSize>

/// Include QDateTime for timestamps.
#include <QDateTime>

/// Include glm for matrices.
#include <glm/glm.hpp>

/**
 * @brief The NavigationHistoryElement class.
 * @details NavigationHistoryElement is the class for holding all data and
 *          properties for the single navigation history objects. Each element
 *          has pointers to a previous and a next element, creating a linked
 *          list of history items.
 *
 * @param previous The previous NavigationHistoryElement in history.
 * @param viewmatrix The current view matrix.
 * @param fovy The current field of view.
 * @param thumbnail The current snapshot of the canvas.
 */
class NavigationHistoryElement
{

public:

    /// Constructor & destructor
    NavigationHistoryElement(NavigationHistoryElement * previous, glm::mat4 viewmatrix, float fovy, QImage thumbnail);
    ~NavigationHistoryElement();


    /// Setter
    void setPrevious(NavigationHistoryElement * previous);
    void setNext(NavigationHistoryElement * next);

    /// Getter
    NavigationHistoryElement * getPrevious();
    NavigationHistoryElement * getNext();
    NavigationHistoryElement * getFirst();
    NavigationHistoryElement * getLast();
    glm::mat4 getViewMatrix();
    float getFovy();
    QImage getThumbnail();
    qint64 getTimestamp();
    int getSize();

    /// Public methods
    bool isFirst();
    bool isLast();
    bool isEqualViewMatrix(const glm::mat4 & viewmatrix);
    bool isEqualFovy(const float & fovy);

protected:

    /// Protected methods
    void deleteOrphaned();
    void deleteFirst();

private:

    /// Properties
    static const int MAX_LENGTH;
    static const int THUMBNAIL_SIZE;

    /// Member variables
    NavigationHistoryElement * m_previous;
    NavigationHistoryElement * m_next;
    glm::mat4 m_viewmatrix;
    float m_fovy;
    QImage m_thumbnail;
    qint64 m_timestamp;
    static int m_length;

};
