
#include <core/program.h>

#include <algorithm>
#include <cassert>

#include <glm/gtc/type_ptr.hpp>

#include <core/shader.h>
#include <globjects/globjects.h>
#include <glbinding/glbinding.h>


Program::Program()
:   m_program(-1)
,   m_linked(false)
,   m_dirty(true)
{
    m_program = gl::glCreateProgram();

}

Program::~Program()
{
    if(isProgram())
    {
        while(m_shaders.cend() != m_shaders.cbegin())
            detach(*(m_shaders.begin()));

        gl::glDeleteProgram(m_program);
    

        m_program = 0;
    }
}

inline const bool Program::isProgram() const
{
    return m_program != -1;
}

const bool Program::isUsed() const
{
    gl::GLint program(-1);
    gl::glGetIntegerv(gl::GLenum::GL_CURRENT_PROGRAM, &program);

    if(-1 == program)
        return false;

    return program == m_program;
}

const bool Program::use() const
{
    if(m_dirty)
        link();

    if(!m_linked)
        return false;

    if(isUsed())
        return true;

    gl::glUseProgram(m_program);

    return !globjects::Error::get().isError();
}

const bool Program::release() const
{
    assert(isUsed());

    gl::glUseProgram(0);
    return !globjects::Error::get().isError();
}

const bool Program::link() const
{
    if(!m_dirty)
        return isLinked();

    for(const Shader * shader : m_shaders)
        if(!shader->isCompiled())
            return false;

    m_dirty = false;

    gl::glLinkProgram(m_program);


    gl::GLint status(GL_FALSE);
    gl::glGetProgramiv(m_program, gl::GLenum::GL_LINK_STATUS, &status);


    m_linked = (gl::GLboolean::GL_TRUE == status);
    m_log = "";

    // check for compile errors

    gl::GLint maxLength(0);
    gl::GLint logLength(0);

    gl::glGetProgramiv(m_program, gl::GLenum::GL_INFO_LOG_LENGTH, &maxLength);


    if(!maxLength)
        return isLinked();

    gl::GLchar *log = new gl::GLchar[maxLength];
    gl::glGetProgramInfoLog(m_program, maxLength, &logLength, log);


    m_log = log;

    if(!m_log.isEmpty() && m_log != "No errors.\n")
        qWarning("%s", log);

    return isLinked();
}

const bool Program::attach(Shader * shader)
{
    if(!shader)
        return false;

    if(m_shaders.end() != m_shaders.find(shader))
        return true;

    m_shaders.insert(shader);
    shader->programs().insert(this);

    gl::glAttachShader(m_program, shader->shader());
    m_dirty = true;

    return !globjects::Error::get().isError();
}

const bool Program::detach(Shader * shader)
{
    if(!shader)
        return false;
    if(m_shaders.end() == m_shaders.find(shader))
        return true;

    gl::glDetachShader(m_program, shader->shader());
    const bool result(!globjects::Error::get().isError());

    m_dirty = true;

    shader->programs().remove(this);
    m_shaders.remove(shader);

    if(shader->programs().empty())
        delete shader;

    return !result;
}

const Program::t_shaders & Program::shaders()
{
    return m_shaders;
}

const gl::GLuint Program::program() const
{
    return m_program;
}

void Program::invalidate()
{
    m_dirty = true;
    link();
}

const bool Program::isLinked() const
{
    return m_linked;
}

const gl::GLint Program::attributeLocation(const QString & name) const
{
    if(m_dirty || !m_linked)
        link();

    const QByteArray bytes(name.toLocal8Bit());
    const gl::GLchar * chr(bytes.constData());

    const gl::GLint location = gl::glGetAttribLocation(m_program, chr);


    return location;
}

const gl::GLint Program::uniformLocation(const QString & name) const
{
    if(m_dirty || !m_linked)
        link();

    use();

    const QByteArray bytes(name.toLocal8Bit());
    const gl::GLchar * chr(bytes.constData());

    const gl::GLint location = gl::glGetUniformLocation(m_program, chr);


    return location;
}

void Program::setUniform(
    const QString & name
,   const float value) const
{
    const gl::GLint location(uniformLocation(name));

    gl::glUniform1f(location, value);

}

void Program::setUniform(
    const QString & name
,   const int value) const
{
    const gl::GLint location(uniformLocation(name));

    gl::glUniform1i(location, value);

}

void Program::setUniform(
    const QString & name
,   const unsigned int value) const
{
    const gl::GLint location(uniformLocation(name));

    gl::glUniform1ui(location, value);

}

void Program::setUniform(
    const QString & name
,   const glm::vec2 & vec) const
{
    const gl::GLint location(uniformLocation(name));

    gl::glUniform2fv(location, 1, glm::value_ptr(vec));

}

void Program::setUniform(
    const QString & name
    ,   const glm::vec2 * vec_array
    ,   const unsigned int & count) const
{
    const gl::GLint location(uniformLocation(name));

    gl::glUniform2fv(location, count, glm::value_ptr(*vec_array));

}

void Program::setUniform(
    const QString & name
,   const glm::ivec2 & vec) const
{
    const gl::GLint location(uniformLocation(name));

    gl::glUniform2iv(location, 1, glm::value_ptr(vec));

}

void Program::setUniform(
    const QString & name
,   const glm::vec3 & vec) const
{
    const gl::GLint location(uniformLocation(name));

    gl::glUniform3fv(location, 1, glm::value_ptr(vec));

}

void Program::setUniform(
    const QString & name
,   const glm::vec3 * vec_array
,   const unsigned int & count) const
{
    const gl::GLint location(uniformLocation(name));

    gl::glUniform3fv(location, count, glm::value_ptr(*vec_array));

}

void Program::setUniform(
    const QString & name
,   const glm::ivec3 & vec) const
{
    const gl::GLint location(uniformLocation(name));

    gl::glUniform3iv(location, 1, glm::value_ptr(vec));

}

void Program::setUniform(
    const QString & name
,   const glm::vec4 & vec) const
{
    const gl::GLint location(uniformLocation(name));

    gl::glUniform4fv(location, 1, glm::value_ptr(vec));

}

void Program::setUniform(
    const QString & name
,   const glm::ivec4 & vec) const
{
    const gl::GLint location(uniformLocation(name));

    gl::glUniform4iv(location, 1, glm::value_ptr(vec));

}

void Program::setUniform(
    const QString & name
,   const glm::uvec4 & vec) const
{
    const gl::GLint location(uniformLocation(name));

    gl::glUniform4uiv(location, 1, glm::value_ptr(vec));

}

void Program::setUniform(
    const QString & name
,   const glm::mat3 & mat
,   const bool transpose) const
{
    const gl::GLint location(uniformLocation(name));

    gl::glUniformMatrix3fv(location, 1, transpose,  glm::value_ptr(mat));

}

void Program::setUniform(
    const QString & name
,   const glm::mat4 & mat
,   const bool transpose) const
{
    const gl::GLint location(uniformLocation(name));

    gl::glUniformMatrix4fv(location, 1, transpose,  glm::value_ptr(mat));

}
