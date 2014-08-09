#pragma once

#include <core/core_api.h>

#include <QString>
#include <QMap>
#include <QSet>

#include <glbinding/gl/types.h>


class Program;

class CORE_API Shader
{
public:
    typedef QSet<Program *> t_programs;

public:
    Shader(const gl::GLenum type);
    virtual ~Shader();

    const bool setSource(
        const QString & source
    ,   const bool update = true);
    const QString & source() const;

    void update();

    const gl::GLenum type() const;
    const gl::GLuint shader() const;

    const bool isCompiled() const;

    const t_programs & programs() const;
    t_programs & programs();

protected:
    inline const bool isShader() const;

protected:
    const gl::GLenum m_type;
    gl::GLuint m_shader;

    QString m_source;
    QString m_log;
    
    bool m_compiled;

    t_programs m_programs;

protected:
    typedef QMap<gl::GLenum, QString> t_typeStrings;
    static const t_typeStrings typeStrings;

    static const t_typeStrings initializeTypeStrings();
};
