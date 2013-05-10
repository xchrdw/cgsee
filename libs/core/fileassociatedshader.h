
#pragma once

#include <QSet>
#include <QMap>

#include "declspec.h"
#include "shader.h"


// TODO: QFileSystemWatcher could be added for automated update.

class CGSEE_API FileAssociatedShader : public Shader
{
public:
    FileAssociatedShader(
        const GLenum type
    ,   const QString & filePath);

    virtual ~FileAssociatedShader();

    void reload();

    static void reloadAll();

protected:
    const bool setSourceFromFile(const bool update = true);

    static void registerShader(
        const QString & filePath
    ,   FileAssociatedShader * shader);

    static void unregisterShader(
        const QString & filePath
    ,   FileAssociatedShader * shader);

protected:
    const QString m_filePath;

protected:
    typedef QSet<FileAssociatedShader *> t_shaders;
    typedef QMap<QString, t_shaders *> t_shadersByFilePath;

    static t_shadersByFilePath shadersByFilePath;
};