#include <random>
#include <ctime>

#include "pathtracer.h"

#include <core/camera.h>
#include <core/program.h>
#include <core/fileassociatedshader.h>
#include <core/bufferobject.h>
#include <core/framebufferobject.h>
#include <core/scenegraph/pathtracingbvh.h>
#include <core/datacore/datablock.h>


static const QString RANDOM_INT_UNIFORM0("randomInt0");
static const QString RANDOM_INT_UNIFORM1("randomInt1");
static const QString FRAMECOUNTER_UNIFORM("frameCounter");
static const QString ANTIALIASING_OFFSET_UNIFORM("antialiasingOffset");

std::mt19937 rng;
std::uniform_real_distribution<float> aaOffsetDistribution(-1.0f, 1.0f);

namespace {
    // GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS is 16 on Intel HD3000
    static const int slotOffset = 9;
    QMap<QString, GLint> initTextureSlots() {
        QMap<QString, GLint> textureSlots;
        textureSlots["indexBuffer"] = slotOffset + 0;
        textureSlots["vertexBuffer"] = slotOffset + 1;
        textureSlots["normalBuffer"] = slotOffset + 2;
        textureSlots["geometryBuffer"] = slotOffset + 3;
        textureSlots["randomVectors"] = slotOffset + 4;
        textureSlots["accumulation"] = slotOffset + 5;
        textureSlots["skyboxTexture"] = slotOffset + 6;
        return textureSlots;
    }
}

const QMap<QString, GLint> PathTracer::textureSlots(initTextureSlots());


PathTracer::PathTracer(Camera & camera)
:   RenderTechnique(camera)
,   m_invalidatedGeometry(true)
,   m_invalidatedAccu(true)
,   m_program(nullptr)
,   m_bvh(nullptr)
,   m_vao(-1)
,   m_vertexBO(nullptr)
,   m_randomVectorTexture(-1)
,   m_randomVectors(nullptr)
,   m_frameCounter(-1)
,   m_accuFramebuffer(-1)
,   m_staticCubeMap(-1)
{
    m_accuTexture[0] = m_accuTexture[1] = -1;
    /*this->setZFar(300.0);
    this->setZNear(1.0);
    this->setFovy(45.0f);*/
}

PathTracer::~PathTracer()
{
    delete m_bvh;
}

void PathTracer::initialize()
{
    if (m_program == nullptr) {
        m_program = new Program();
        m_program->attach(
            new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/pathTracing.frag"));
        m_program->attach(
            new FileAssociatedShader(GL_VERTEX_SHADER, "data/pathTracing.vert"));
    }

    if (m_bvh == nullptr) {
        m_bvh = new PathTracingBVH();
    }
    if(-1 == m_vao)
        initVertexBuffer();
    if (-1 == m_randomVectorTexture)
        initRandomVectorBuffer();
    if (m_accuFramebuffer == -1) {

        glGenTextures(2, m_accuTexture);

        resizeTextures(m_camera.viewport());
   
        glGenFramebuffers(1, &m_accuFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_accuFramebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_accuTexture[0], 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_accuTexture[1], 0);
        glError();

        const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        glError();
        if(GL_FRAMEBUFFER_COMPLETE != status)
            qDebug("Path Tracing Frame Buffer Object incomplete.");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glError();
    }
    if (m_staticCubeMap == -1) {
        initSkybox();
    }
}

void PathTracer::initVertexBuffer()
{
    // By default, counterclockwise polygons are taken to be front-facing.
    // http://www.opengl.org/sdk/docs/man/xhtml/glFrontFace.xml

    static const GLfloat vertices[] =
    {
        +1.f, -1.f
    ,   +1.f, +1.f
    ,   -1.f, -1.f
    ,   -1.f, +1.f
    };

    glGenVertexArrays(1, &m_vao);
    glError();
    glBindVertexArray(m_vao);                                                                  
    glError();

    // setup array buffer

    if(!m_vertexBO)
    {
        m_vertexBO = new BufferObject(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	    m_vertexBO->data<GLfloat>(vertices, 8, GL_FLOAT, 2);
    }

    // bind all buffers to their attributes

    m_vertexBO->bind(m_program->attributeLocation("a_vertex"));

    glBindVertexArray(0);
    glError();
}

void PathTracer::initRandomVectorBuffer()
{
    std::vector<glm::vec3> rndVecData;

    pointsOnSphere(rndVecData, 1000);

    m_randomVectors = new BufferObject(GL_TEXTURE_BUFFER, GL_STATIC_READ);
    m_randomVectors->data<glm::vec3>(rndVecData.data(), rndVecData.size(),  GL_RGB32F, sizeof(glm::vec3));
    
    glActiveTexture(GL_TEXTURE0 + PathTracer::textureSlots["randomVectors"]);
    glGenTextures(1, &m_randomVectorTexture);
    glBindTexture(GL_TEXTURE_BUFFER, m_randomVectorTexture);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, m_randomVectors->buffer());
    glError();
}

char * loadTextureRAW(const char * filename, unsigned int width, unsigned int height, unsigned int depth)
{
    char * data;
    FILE * file;

    file = fopen( filename, "rb" );
    if ( file == NULL ) return nullptr;

    data = (char *)malloc( width * height * depth );

    fread( data, width * height * depth, 1, file );
    fclose( file );

    return data;
}

void PathTracer::initSkybox()
{
    GLubyte *faces[6];
    
    glGenTextures( 1, &m_staticCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_staticCubeMap);
    faces[0] = (GLubyte*) loadTextureRAW("./data/skybox/skybox_left.raw", 512, 512, 3);
    faces[1] = (GLubyte*) loadTextureRAW("./data/skybox/skybox_right.raw", 512, 512, 3);
    faces[2] = (GLubyte*) loadTextureRAW("./data/skybox/skybox_top.raw", 512, 512, 3);
    faces[3] = (GLubyte*) loadTextureRAW("./data/skybox/skybox_bottom.raw", 512, 512, 3);
    faces[4] = (GLubyte*) loadTextureRAW("./data/skybox/skybox_back.raw", 512, 512, 3);
    faces[5] = (GLubyte*) loadTextureRAW("./data/skybox/skybox_front.raw", 512, 512, 3);
    for (int i = 0; i < 6; ++i) {
        if (faces[i] == nullptr) {
            qDebug("error while loading skybox textures.");
            return;
        }
    }
    
    for (int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB8,
                512,
                512,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                faces[i]);
    }
    glError();
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glError();
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glActiveTexture(GL_TEXTURE0 + textureSlots["skyboxTexture"]);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_staticCubeMap);
    glError();
    return;
}

void PathTracer::setUniforms()
{
    m_program->setUniform(FRAMECOUNTER_UNIFORM, m_frameCounter);
    m_program->setUniform(RANDOM_INT_UNIFORM0, (unsigned int)(rng()));
    m_program->setUniform(RANDOM_INT_UNIFORM1, (unsigned int)(rng()));

    glm::vec2 offset(aaOffsetDistribution(rng), aaOffsetDistribution(rng));
    offset /= m_camera.viewport();
    m_program->setUniform(ANTIALIASING_OFFSET_UNIFORM, offset);

    for (auto it = PathTracer::textureSlots.cbegin(); it != PathTracer::textureSlots.cend(); ++it)
    {
        m_program->setUniform(it.key(), it.value());
    }
}

void PathTracer::resizeTextures(const glm::ivec2 & viewport) const
{
    if (m_accuTexture[0] == -1)
        return;

    for (int i=0; i<2; ++i) {
        glBindTexture(GL_TEXTURE_2D, m_accuTexture[i]);
        glError();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, viewport.x, viewport.y, 0, GL_RGBA, GL_FLOAT, 0);
        glError();

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

        glBindTexture(GL_TEXTURE_2D, 0);
        glError();
    }
}

void PathTracer::renderScene(
    const Program &
,   FrameBufferObject * target)
{
    initialize();

    m_program->use();

    ++m_frameCounter;
    
    if (m_invalidatedAccu) {
        m_frameCounter = 1;
        m_invalidatedAccu = false;
    }

    if (m_invalidatedGeometry) {
        // rebuild bounding volume hierarchy :
        m_bvh->buildBVHFromObjectsHierarchy(&m_camera);
        m_bvh->geometryToTexture(GL_TEXTURE0 + PathTracer::textureSlots["geometryBuffer"]);
        m_invalidatedGeometry = false;
    }

    setUniforms();

    // switch the rendering buffers for each pass
    m_whichBuffer = !m_whichBuffer;
    GLenum readIndex = m_whichBuffer ? 0 : 1;
    GLenum writeIndex = m_whichBuffer ? 1 : 0;

    glActiveTexture(GL_TEXTURE0 + textureSlots["accumulation"]);
    glBindTexture(GL_TEXTURE_2D, m_accuTexture[readIndex]);
    glError();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_accuFramebuffer);
    GLenum writebuffers[] = { GL_COLOR_ATTACHMENT0 + writeIndex };
    glDrawBuffers(1, writebuffers);
    glError();


    glBindVertexArray(m_vao);                                                                  
    glError();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);

    glViewport(0, 0, m_camera.viewport().x, m_camera.viewport().y);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glError();

    glDisable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);

    glBindVertexArray(0);
    glError();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    // copy written buffer to screen/output framebuffer

    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_accuFramebuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + writeIndex );

    //if (target != nullptr)
    //    target->bind();
    glBlitFramebuffer(0, 0, m_camera.viewport().x, m_camera.viewport().y, 0, 0, m_camera.viewport().x, m_camera.viewport().y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    //if (target != nullptr)
    //    target->release();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    
    m_program->release();
}

void PathTracer::onInvalidatedView()
{
    invalidateAccumulator();
}

void PathTracer::onInvalidatedViewport(
        const int width
    ,   const int height)
{
    if (m_accuFramebuffer == -1)
        return;
    resizeTextures(glm::ivec2(width, height));
    invalidateAccumulator();
}

void PathTracer::onInvalidatedChildren()
{
    invalidateGeometry();
}

void PathTracer::invalidateGeometry()
{
    m_invalidatedGeometry = true;
    invalidateAccumulator();
}

void PathTracer::invalidateAccumulator()
{
    m_invalidatedAccu = true;
}




// https://code.google.com/p/pathgl/source/browse/pathgl.cpp thanks Daniel :-D
// creates at least minN points on a unitsphere by creating a hemi-icosphere:
// approach to create evenly distributed points on a sphere
//   1. create points of icosphere
//   2. cutout hemisphere
//   3. randomize point list

void PathTracer::pointsOnSphere(
    std::vector<glm::vec3> & points
,   const unsigned int minN)
{
    // 1. create an icosphere

    static const float t = (1.f + sqrtf(5.f)) * 0.5f;

    std::vector<glm::vec3> icopoints;
    std::vector<glm::uvec3> icofaces;

    // basic icosahedron

    icopoints.push_back(glm::vec3(-1, t, 0));
    icopoints.push_back(glm::vec3( 1, t, 0));
    icopoints.push_back(glm::vec3(-1,-t, 0));
    icopoints.push_back(glm::vec3( 1,-t, 0));

    icopoints.push_back(glm::vec3( 0,-1, t));
    icopoints.push_back(glm::vec3( 0, 1, t));
    icopoints.push_back(glm::vec3( 0,-1,-t));
    icopoints.push_back(glm::vec3( 0, 1,-t));

    icopoints.push_back(glm::vec3( t, 0,-1));
    icopoints.push_back(glm::vec3( t, 0, 1));
    icopoints.push_back(glm::vec3(-t, 0,-1));
    icopoints.push_back(glm::vec3(-t, 0, 1));

    // normalize
    for(int i = 0; i < 12; ++i)
        icopoints[i] = glm::normalize(icopoints[i]);

    icofaces.push_back(glm::uvec3(  0, 11,  5));
    icofaces.push_back(glm::uvec3(  0,  5,  1));
    icofaces.push_back(glm::uvec3(  0,  1,  7));
    icofaces.push_back(glm::uvec3(  0,  7, 10));
    icofaces.push_back(glm::uvec3(  0, 10, 11));

    icofaces.push_back(glm::uvec3(  1,  5,  9));
    icofaces.push_back(glm::uvec3(  5, 11,  4));
    icofaces.push_back(glm::uvec3( 11, 10,  2));
    icofaces.push_back(glm::uvec3( 10,  7,  6));
    icofaces.push_back(glm::uvec3(  7,  1,  8));

    icofaces.push_back(glm::uvec3(  3,  9,  4));
    icofaces.push_back(glm::uvec3(  3,  4,  2));
    icofaces.push_back(glm::uvec3(  3,  2,  6));
    icofaces.push_back(glm::uvec3(  3,  6,  8));
    icofaces.push_back(glm::uvec3(  3,  8,  9));

    icofaces.push_back(glm::uvec3(  4,  9,  5));
    icofaces.push_back(glm::uvec3(  2,  4, 11));
    icofaces.push_back(glm::uvec3(  6,  2, 10));
    icofaces.push_back(glm::uvec3(  8,  6,  7));
    icofaces.push_back(glm::uvec3(  9,  8,  1));

    // iterative triangle refinement - split each triangle
    // into 4 new ones and create points appropriately.

    const int r = static_cast<int>(ceil(log(static_cast<float>(minN * 2 / 12.f)) / log(4.f))); // N = 12 * 4 ^ r

    std::unordered_map<glm::highp_uint, glm::uint> cache;

    for(int i = 0; i < r; ++i)
    {
        const int size(static_cast<int>(icofaces.size()));

        for(int f = 0; f < size; ++f)
        {
            glm::uvec3 & face(icofaces[f]);

            const glm::uint  a(face.x);
            const glm::uint  b(face.y);
            const glm::uint  c(face.z);

            const glm::uint ab(splitEdge(a, b, icopoints, cache));
            const glm::uint bc(splitEdge(b, c, icopoints, cache));
            const glm::uint ca(splitEdge(c, a, icopoints, cache));

            face = glm::uvec3(ab, bc, ca);

            icofaces.push_back(glm::uvec3(a, ab, ca));
            icofaces.push_back(glm::uvec3(b, bc, ab));
            icofaces.push_back(glm::uvec3(c, ca, bc));
        }
    }

    // 2. remove lower hemisphere

    const int size(static_cast<int>(icopoints.size()));
    for(int i = 0; i < size; ++i)
        if(icopoints[i].y > 0.f)
            points.push_back(icopoints[i]);

    // 3. shuffle all points of hemisphere

    std::shuffle(points.begin(), points.end(), rng);
}


// splits a triangle edge by adding an appropriate new point (normalized on sphere)
// to the points list (if not already cached) and returns the index to this point.
const glm::uint PathTracer::splitEdge(
    const glm::uint a
,   const glm::uint b
,   std::vector<glm::vec3> & points
,   std::unordered_map<glm::highp_uint, glm::uint> & cache)
{
    const bool aSmaller(a < b);
    const glm::highp_uint smaller(aSmaller ? a : b);
    const glm::highp_uint greater(aSmaller ? b : a);
    const glm::highp_uint hash((smaller << 32) + greater);

    std::unordered_map<glm::highp_uint, glm::uint>::const_iterator h(cache.find(hash));
    if(cache.end() != h)
        return h->second;

    const glm::vec3 & a3(points[a]);
    const glm::vec3 & b3(points[b]);

    const glm::vec3 s(glm::normalize((a3 + b3) * 0.5f));

    points.push_back(s);
    const glm::uint i(static_cast<glm::uint>(points.size()) - 1);

    cache[hash] = i;

    return i;
}
