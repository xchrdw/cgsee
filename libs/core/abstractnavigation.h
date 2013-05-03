#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <QApplication>
#include <QFileInfo>
#include <QStringList>
#include <QSettings>
#include <QKeyEvent>

enum MouseButton {
    LEFT, RIGHT, MIDDLE
};


class AbstractNavigation {

public:

	AbstractNavigation();
	virtual ~ AbstractNavigation();

	virtual void reset() = 0;

	//void assignCoordinateProvider(AbstractCoordinateProvider *provider);

	//void setSoftwareMapBounds(const Bounds& bounds);

	// interaction mapping

	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);

	virtual void mouseMoveEvent(const glm::vec2 point);
	virtual void mousePressEvent(const glm::vec2 point, MouseButton button);
	virtual void mouseReleaseEvent(const glm::vec2 point, MouseButton button);
	virtual void mouseDoubleClickEvent(const glm::vec2 point, MouseButton button);

	virtual void wheelEvent(QWheelEvent *event);

	//virtual void setCameraConfiguration(const glm::vec3 camera, const glm::vec3 center);

	// lazy matrices getters

	virtual const glm::mat4 viewMatrix() = 0;

	virtual const glm::mat4 viewMatrixInverted() = 0;

};