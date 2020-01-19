#include "GLFramebuffer.h"
#include "GLDebug.h"

#include "Exceptions/EngineRuntimeException.h"

#include <spdlog/spdlog.h>

GLFramebuffer::GLFramebuffer(int width, int height,
                             size_t colorComponentsCount,
                             GLTextureInternalFormat colorComponentsFormat,
                             bool createDepthStencilComponent)
    : m_width(width), m_height(height),
      m_colorComponentsCount(colorComponentsCount),
      m_colorComponentsFormat(colorComponentsFormat)
{
    SW_ASSERT(colorComponentsCount <= 4);

    GL_CALL(glCreateFramebuffers(1, &m_framebuffer));

    for (size_t colorComponentIndex = 0; colorComponentIndex < m_colorComponentsCount; colorComponentIndex++) {
        m_colorComponents[colorComponentIndex] = std::make_unique<GLTexture>(GLTextureType::Texture2D,
                                                                             m_width, m_height, m_colorComponentsFormat);

        GLenum colorAttachmentIndex = GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(colorComponentIndex);

        GL_CALL(glNamedFramebufferTexture(m_framebuffer, colorAttachmentIndex,
                                  m_colorComponents[colorComponentIndex]->getGLHandle(), 0));
    }

    if (createDepthStencilComponent) {
        GLTextureInternalFormat depthInternalFormat = GLTextureInternalFormat::Depth24Stencil8;
        m_depthComponent = std::make_unique<GLTexture>(GLTextureType::Texture2D, m_width, m_height, depthInternalFormat);

        GLenum attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;

        GL_CALL(glNamedFramebufferTexture(m_framebuffer, attachmentType, m_depthComponent->getGLHandle(), 0));
    }

    GL_CALL_BLOCK_BEGIN();

    GLenum readFramebufferStatus = glCheckNamedFramebufferStatus(m_framebuffer, GL_READ_FRAMEBUFFER);
    GLenum drawFramebufferStatus = glCheckNamedFramebufferStatus(m_framebuffer, GL_DRAW_FRAMEBUFFER);

    GL_CALL_BLOCK_END();

    if (readFramebufferStatus != GL_FRAMEBUFFER_COMPLETE ||
        drawFramebufferStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        ENGINE_RUNTIME_ERROR("Failed to create framebuffer, statuses are " + std::to_string(readFramebufferStatus) +
                             " " + std::to_string(drawFramebufferStatus));
    }

    enableWritingToAllBuffers();
}

GLFramebuffer::GLFramebuffer(int width, int height)
    : m_framebuffer(0),
      m_width(width), m_height(height),
      m_colorComponentsCount(1)
{

}

GLFramebuffer::~GLFramebuffer()
{
    if (m_framebuffer != 0) {
        glDeleteFramebuffers(1, &m_framebuffer);
    }
}

Rect GLFramebuffer::getBounds() const
{
    return Rect(0, 0, m_width, m_height);
}

float GLFramebuffer::getAspectRatio() const
{
    return static_cast<float>(m_width) / m_height;
}

int GLFramebuffer::getWidth() const
{
    return m_width;
}

int GLFramebuffer::getHeight() const
{
    return m_height;
}

void GLFramebuffer::clearColor(const glm::vec4& color, size_t componentIndex)
{
    glClearNamedFramebufferfv(m_framebuffer, GL_COLOR, static_cast<GLint>(componentIndex), reinterpret_cast<const float*>(&color));
}

void GLFramebuffer::clearDepthStencil(float depthValue, int stencilValue)
{
    glClearNamedFramebufferfi(m_framebuffer, GL_DEPTH_STENCIL, 0, depthValue, stencilValue);
}

void GLFramebuffer::copyColor(GLFramebuffer& target, size_t sourceComponentIndex, size_t targetComponentIndex)
{
    copyColor(target, getBounds(), target.getBounds(), FramebufferCopyFilter::Nearest,
              sourceComponentIndex, targetComponentIndex);
}

void GLFramebuffer::copyColor(GLFramebuffer& target, const Rect& sourceRect, const Rect& targetRect,
                              FramebufferCopyFilter filter, size_t sourceComponentIndex, size_t targetComponentIndex)
{
    SW_ASSERT(sourceComponentIndex < m_colorComponentsCount &&
              targetComponentIndex < target.m_colorComponentsCount);

    GLenum sourceAttachment = GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(sourceComponentIndex);
    GLenum targetAttachment = GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(targetComponentIndex);

    copyTo(target, sourceRect, targetRect,
           GL_COLOR_BUFFER_BIT, filter, sourceAttachment, targetAttachment);

}

void GLFramebuffer::copyDepthStencil(GLFramebuffer& target)
{
    copyDepthStencil(target, getBounds(), target.getBounds(), FramebufferCopyFilter::Nearest);
}

void GLFramebuffer::copyDepthStencil(GLFramebuffer& target, const Rect& sourceRect, const Rect& targetRect,
                                     FramebufferCopyFilter filter)
{
    copyTo(target, sourceRect, targetRect,
           GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, filter, GL_NONE, GL_NONE);
}

void GLFramebuffer::copyTo(GLFramebuffer& target,
                           const Rect& sourceRect,
                           const Rect& targetRect,
                           GLbitfield copyMask,
                           FramebufferCopyFilter filter,
                           GLenum sourceAttachment,
                           GLenum targetAttachment)
{
    GLbitfield blitMask = copyMask;
    GLenum blitFilter = GL_NONE;

    switch (filter) {
    case FramebufferCopyFilter::Linear:
        blitFilter = GL_LINEAR;
        break;

    case FramebufferCopyFilter::Nearest:
        blitFilter = GL_NEAREST;
        break;

    default:
        SW_ASSERT(false);
        break;
    }

    GL_CALL_BLOCK_BEGIN();

    if (sourceAttachment != GL_NONE && m_framebuffer != 0) {
        glNamedFramebufferReadBuffer(m_framebuffer, sourceAttachment);
    }

    if (targetAttachment != GL_NONE && target.m_framebuffer != 0) {
        glNamedFramebufferDrawBuffer(target.m_framebuffer, targetAttachment);
    }

    glBlitNamedFramebuffer(m_framebuffer, target.getGLHandle(),
                           sourceRect.getOriginX(), sourceRect.getOriginY(),
                           sourceRect.getOriginX() + sourceRect.getWidth(),
                           sourceRect.getOriginY() + sourceRect.getHeight(),
                           targetRect.getOriginX(), targetRect.getOriginY(),
                           targetRect.getOriginX() + targetRect.getWidth(),
                           targetRect.getOriginY() + targetRect.getHeight(),
                           blitMask, blitFilter);

    if (targetAttachment != GL_NONE && target.m_framebuffer != 0) {
        target.enableWritingToAllBuffers();
    }

    GL_CALL_BLOCK_END();
}

void GLFramebuffer::enableWritingToAllBuffers()
{
    std::vector<GLuint> drawBuffers(m_colorComponentsCount);

    for (size_t colorComponentIndex = 0; colorComponentIndex < m_colorComponentsCount; colorComponentIndex++) {
        GLenum colorAttachmentIndex = GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(colorComponentIndex);
        drawBuffers[colorComponentIndex] = colorAttachmentIndex;
    }

    GL_CALL(glNamedFramebufferDrawBuffers(m_framebuffer, static_cast<GLsizei>(m_colorComponentsCount), drawBuffers.data()));
}

GLuint GLFramebuffer::getGLHandle() const
{
    return m_framebuffer;
}

GLTexture* GLFramebuffer::getDepthComponent() const
{
    return m_depthComponent.get();
}

GLTexture* GLFramebuffer::getColorComponent(size_t index) const
{
    SW_ASSERT(index <= 4);

    return m_colorComponents[index].get();
}

