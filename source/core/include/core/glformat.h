#pragma once

#include <core/core_api.h>

#include <QStringList>
#include <QMap>
#include <QVector>


class QGLFormat;

class CORE_API GLFormat
{
public:
    enum e_Profile
    {
        NoProfile            = 0
    ,   CoreProfile          = 1
    ,   CompatibilityProfile = 2
    };

public:
    GLFormat();
    GLFormat(const QGLFormat & format);

    virtual ~GLFormat();

    const QGLFormat asQGLFormat() const;

    const bool verify(const QGLFormat & format) const;


    const unsigned int majorVersion() const;
    const unsigned int minorVersion() const;
    void setVersion(
        const unsigned int major
    ,   const unsigned int minor);

    const e_Profile profile() const;
    void setProfile(const e_Profile profile);

    const QStringList extensions() const;
    void setExtensions(const QStringList & extensions);

    const unsigned int redBufferSize() const;
    void setRedBufferSize(const unsigned int size);
    const unsigned int greenBufferSize() const;
    void setGreenBufferSize(const unsigned int size);
    const unsigned int blueBufferSize() const;
    void setBlueBufferSize(const unsigned int size);
    const unsigned int alphaBufferSize() const;
    void setAlphaBufferSize(const unsigned int size);

    const bool depth() const;
    const unsigned int  depthBufferSize() const;
    void setDepthBufferSize(const unsigned int size);

    const bool stencil() const;
    const unsigned int  stencilBufferSize() const;
    void setStencilBufferSize(const unsigned int size);

    const bool doubleBuffer() const;
    void setDoubleBuffer(const bool enable);
    const bool stereo() const; // quadBuffer
    void setStereo(const bool enable);

    const bool sampleBuffers() const;
    void setSampleBuffers(const bool enable);
    const unsigned int  samples() const;
    void setSamples(const unsigned int samples);

    const int swapInterval() const;
    void setSwapInterval(const int interval);

    const bool vsync() const;
    void setVSync(const bool enable);

protected:
    static const bool verifyVersionAndProfile(
        const GLFormat & requested
    ,   const GLFormat & current);

    static const bool verifyExtensions(
        const GLFormat & requested);

    static const bool verifySwapInterval(
        const GLFormat & requested
    ,   const GLFormat & current);

    static void verifyBufferSize(
        const unsigned int sizeRequested
    ,   const unsigned int sizeInitialized
    ,   const QString & warning
    ,   QStringList & issues);

    static const bool verifyPixelFormat(
        const GLFormat & requested
    ,   const GLFormat & current);

protected:
    typedef QMap<unsigned int, unsigned int > t_minorsByMajor;
    static const t_minorsByMajor validGLVersions();

    typedef QMap<e_Profile, QString> t_profileIdentifier;
    static const t_profileIdentifier validProfileIdentifier();
    
protected:
    static const t_minorsByMajor m_validGLVersions;
    static const t_profileIdentifier m_validProfileIdentifier;

protected:

    unsigned int  m_majorVersion;
    unsigned int  m_minorVersion;

    e_Profile m_profile;

    QStringList m_extensions;

    unsigned int  m_redBufferSize;
    unsigned int  m_greenBufferSize;
    unsigned int  m_blueBufferSize;
    unsigned int  m_alphaBufferSize;

    unsigned int  m_depthBufferSize;
    unsigned int  m_stencilBufferSize;

    bool m_doubleBuffer;
    bool m_stereo;

    bool m_sampleBuffers;
    unsigned int  m_samples;

    int m_swapInterval;
};
