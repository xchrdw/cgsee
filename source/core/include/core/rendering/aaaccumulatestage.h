#pragma once

#include <core/core_api.h>

#include <QString>

#include <globjects/Texture.h>
#include <globjects/base/ref_ptr.h>

#include <core/rendering/abstractpostprocessingstage.h>


// under construction
class CORE_API AAAccumulateStage : public AbstractPostProcessingStage
{
public:
    AAAccumulateStage(PipelinePainter & painter, const QString & inputBufferName, const QString & outputBufferName);
    virtual ~AAAccumulateStage(void);

    virtual void reloadShaders() override;
    virtual void resize(const int width, const int height) override;
    virtual void render() override;

protected:
    QString m_outputBufferName;
	globjects::ref_ptr<globjects::Texture> m_accumulateBuffer[2];
    bool m_targetBuffer;
    // ownership lies somewhere else
    globjects::ref_ptr<globjects::Texture> m_inputBuffer;
};
