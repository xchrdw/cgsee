/// Preprocessor directive
#pragma once

/// Include glm for matrices.
#include <glm/glm.hpp>

/// Include QImage and QSize for thumbnails.
#include <QImage>
#include <QSize>

/// Include QDateTime for timestamps.
#include <QDateTime>

#include <core/core_api.h>

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
class CORE_API NavigationHistoryElement
{

    /// Allow access to member variables for NavigationHistory.
    friend class NavigationHistory;

public:

    /// Constructor & destructor
    NavigationHistoryElement(NavigationHistoryElement * previous, glm::mat4 viewmatrix, float fovy, QImage thumbnail);
    ~NavigationHistoryElement();

    /// Setter
    void setPrevious(NavigationHistoryElement * previous);
    void setNext(NavigationHistoryElement * next);

    /// Getter
    NavigationHistoryElement * previous();
    NavigationHistoryElement * next();
    NavigationHistoryElement * first();
    NavigationHistoryElement * last();
    glm::mat4 viewMatrix();
    float fovy();
    QImage thumbnail();
    qint64 timestamp();
    int size();

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

    /// Member variables
    NavigationHistoryElement * m_previous;
    NavigationHistoryElement * m_next;
    glm::mat4 m_viewmatrix;
    float m_fovy;
    QImage m_thumbnail;
    qint64 m_timestamp;
    static int m_length;

};
