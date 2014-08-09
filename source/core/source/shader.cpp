
#include <core/shader.h>

#include <map>
#include <cassert>
#include <iostream>

#include <glbinding/gl/types.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/boolean.h>

#include <core/gpuquery.h>
#include <core/program.h>



const Shader::t_typeStrings Shader::typeStrings = initializeTypeStrings();

const Shader::t_typeStrings Shader::initializeTypeStrings()
{
    t_typeStrings typeStrings;

    typeStrings[gl::GL_TESS_EVALUATION_SHADER] = "GL_TESS_EVALUATION_SHADER";
    typeStrings[gl::GL_TESS_CONTROL_SHADER]    = "GL_TESS_CONTROL_SHADER";
#ifdef GL_COMPUTE_SHADER
    typeStrings[gl::GL_COMPUTE_SHADER]         = "GL_COMPUTE_SHADER";
#endif
    typeStrings[gl::GL_FRAGMENT_SHADER]        = "GL_FRAGMENT_SHADER";
    typeStrings[gl::GL_GEOMETRY_SHADER]        = "GL_GEOMETRY_SHADER";
    typeStrings[gl::GL_VERTEX_SHADER]          = "GL_VERTEX_SHADER";

    return typeStrings;
}

Shader::Shader(const gl::GLenum type)
:   m_type(type)
,   m_shader(-1)
,   m_compiled(false)
{
    if(typeStrings.end() == typeStrings.find(type))
        qCritical("Shader type %i is not supported.", type);

    m_shader = gl::glCreateShader(type);
    glError();
}
    
Shader::~Shader()
{
    if(isShader())
    {
        gl::glDeleteShader(m_shader);
        glError();

        m_shader = 0;
    }
}

inline const bool Shader::isShader() const
{
    return m_shader != -1;
}

const bool Shader::setSource(
    const QString & source
,   const bool update)
{
    m_source = source;

    const QByteArray bytes(m_source.toLocal8Bit());
    const gl::GLchar * chr(bytes.constData());

    gl::glShaderSource(m_shader, 1, &chr, nullptr);
    glError();
    gl::glCompileShader(m_shader);
    glError();

    gl::GLint status(static_cast<gl::GLint>(gl::GL_FALSE));
    gl::glGetShaderiv(m_shader, gl::GL_COMPILE_STATUS, &status);

    m_compiled = (static_cast<gl::GLint>(gl::GL_TRUE) == status);
    m_log = "";

    if(!m_compiled)
    {
        gl::GLint maxLength(0);
        gl::GLint logLength(0);

        gl::glGetShaderiv(m_shader, gl::GL_INFO_LOG_LENGTH, &maxLength);
        glError();

        gl::GLchar *log = new gl::GLchar[maxLength];
        gl::glGetShaderInfoLog(m_shader, maxLength, &logLength, log);
        glError();

        m_log = log;

        t_typeStrings::const_iterator i(typeStrings.find(type()));
        if(typeStrings.end() != i)
            qCritical("Compiling shader of type %s failed.", qPrintable(i.value()));
        else
            qCritical("Compiling shader of type %i failed.", type());

        if(!m_log.isEmpty())
            qCritical("%s", qPrintable(m_log));
    }

    if(update)
        this->update();

    return isCompiled();
}

const QString & Shader::source() const
{
    return m_source;
}

void Shader::update()
{
    t_programs::const_iterator i(m_programs.begin());
    const t_programs::const_iterator iEnd(m_programs.end());

    for(; i != iEnd; ++i)
        (*i)->invalidate();
}

const gl::GLenum Shader::type() const
{
    return m_type;
}

const gl::GLuint Shader::shader() const
{
    return m_shader;
}

const bool Shader::isCompiled() const
{
    return m_compiled;
}

const Shader::t_programs & Shader::programs() const
{
    return m_programs;
}

Shader::t_programs & Shader::programs()
{
    return m_programs;
}
