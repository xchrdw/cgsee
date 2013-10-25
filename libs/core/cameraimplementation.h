#pragma once

#include <glm/glm.hpp>

#include <functional>
#include <QString>
#include <QMap>
#include <QVector>

#include "declspec.h"

class Program;
class Camera;


class CGSEE_API CameraImplementation
{
public:
    CameraImplementation(Camera & abstraction);
    virtual ~CameraImplementation();

    virtual void drawScene( const Program & program, const glm::mat4 & transform) = 0;

    // returns a new instance of the specified type, if the camera type is registered
    static CameraImplementation * newCameraByName(Camera & abstraction, QString name);
    // create an instance of each registered camera type
    static QVector<CameraImplementation*> newImplementations(Camera & abstraction);

    virtual const QString implementationName() const = 0;

protected:
    // call abstraction to render the scene using its current render technique
    void abstractionRenderScene(const Program & program) const;

protected:
    typedef std::function<CameraImplementation*(Camera * abstraction)> CamConstructor;

    static bool registerImplementation(
        QString name, 
        CamConstructor constructor);
    static QMap<QString, CamConstructor> implementations;

    template <typename T>
    static T* createInstace(Camera * abstraction)
    {
        return new T(*abstraction);
    }

protected:
    Camera & m_abstraction;

    virtual void onInvalidatedView();
    virtual void onInvalidatedViewport(const int width, const int height);
    virtual void onInvalidatedChildren();

friend class Camera;
};