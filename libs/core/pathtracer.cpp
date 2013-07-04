#include <random>
#include <time.h>

#include "pathtracer.h"

#include "program.h"
#include "bufferobject.h"
#include "framebufferobject.h"
//#include "gpuquery.h"

static const QString TRANSFORM_UNIFORM ("transform");
static const QString TRANSFORMINVERSE_UNIFORM ("transformInverse");
static const QString RANDOM_INT_UNIFORM("randomInt");

std::mt19937 rng;

namespace {
    QMap<QString, GLuint> initTextureSlots() {
        QMap<QString, GLuint> textureSlots;
        textureSlots["indexBuffer"] = 0;
        textureSlots["vertexBuffer"] = 1;
        textureSlots["normalBuffer"] = 2;
        textureSlots["geometryBuffer"] = 3;
        textureSlots["randomVectors"] = 4;
        textureSlots["accumulation"] = 5;
        textureSlots["testTex"] = 6;
        return textureSlots;
    }
}

const QMap<QString, GLuint> PathTracer::textureSlots(initTextureSlots());


PathTracer::PathTracer(const QString & name)
:   Camera(name)
,   m_invalidatedGeometry(true)
,   m_vao(-1)
,   m_vertexBO(nullptr)
,   m_randomVectorTexture(-1)
,   m_randomVectors(nullptr)
,   m_accuFramebuffer(-1)
{
    m_accuTexture[0] = m_accuTexture[1] = -1;
}

PathTracer::~PathTracer()
{
}

void PathTracer::initialize(const Program & program)
{
    if(-1 == m_vao)
        initVertexBuffer(program);
    if (-1 == m_randomVectorTexture)
        initRandomVectorBuffer(program);
    if (m_accuFramebuffer == -1) {

        glGenTextures(2, m_accuTexture);

        float *red = new float[4*m_viewport.x*m_viewport.y];
        float *blue = new float[4*m_viewport.x*m_viewport.y];
        float *green = new float[4];
        for (int i=0; i < 4*m_viewport.x*m_viewport.y; i+=4){
            red[i+0] = 1.0f; red[i+1] = 0.0f; red[i+2] = 0.0f; red[i+3] = 1.0f;
            blue[i+0] = 0.0f; blue[i+1] = 0.0f; blue[i+2] = 1.0f; blue[i+3] = 1.0f;
            //green[i+0] = 0.0f; green[i+1] = 1.0f; green[i+2] = 0.0f; green[i+3] = 1.0f;
        }
        green[0] = 0.0f; green[1] = 1.0f; green[2] = 0.0f; green[3] = 1.0f;

        glGenTextures(1, &m_testTex);
        glBindTexture(GL_TEXTURE_2D, m_testTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1, 1, 0, GL_RGBA, GL_FLOAT, green);
        glError();

        glBindTexture(GL_TEXTURE_2D, m_accuTexture[0]);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_viewport.x, m_viewport.y, 0, GL_RGBA, GL_FLOAT, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_viewport.x, m_viewport.y, 0, GL_RGBA, GL_FLOAT, red); // <-- test attached color filled array
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
        glBindTexture(GL_TEXTURE_2D, m_accuTexture[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_viewport.x, m_viewport.y, 0, GL_RGBA, GL_FLOAT, blue);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
        glBindTexture(GL_TEXTURE_2D, 0);
        glError();
   
        glGenFramebuffers(1, &m_accuFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_accuFramebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_accuTexture[0], 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_accuTexture[1], 0);
        // use only one framebuffer -> color attachment 0, 1, two textures
        // 1test: fill with blue/read -> test switching
        // 2test: ++green, accu
        // -!-
        glError();

        const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        glError();
        if(GL_FRAMEBUFFER_COMPLETE != status)
            qDebug("Path Tracing Frame Buffer Object incomplete.");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glError();

        for (auto it = PathTracer::textureSlots.cbegin(); it != PathTracer::textureSlots.cend(); ++it)
            program.setUniform(it.key(), it.value());
    }
}

void PathTracer::initVertexBuffer(const Program & program)
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

    m_vertexBO->bind(program.attributeLocation("a_vertex"));

    glBindVertexArray(0);
    glError();
}

void PathTracer::initRandomVectorBuffer(const Program & program)
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

void PathTracer::setUniforms(const Program & program)
{
    program.setUniform(VIEWPORT_UNIFORM, m_viewport);
    program.setUniform(VIEW_UNIFORM, m_view);
    program.setUniform(PROJECTION_UNIFORM, m_projection);
    program.setUniform(TRANSFORM_UNIFORM, m_transform);
    program.setUniform(TRANSFORMINVERSE_UNIFORM, m_transformInverse);
    program.setUniform(CAMERAPOSITION_UNIFORM, getEye());
}

void PathTracer::draw(
    const Program & program
    ,   const glm::mat4 & transform)
{
    return draw(program);
}

void PathTracer::draw(
    const Program & program
,   FrameBufferObject * target)
{
    // call group draw with initOnly, to initialize all needed buffer objects
    Group::draw(program, glm::mat4(), true); // includes call "program.use()"

    initialize(program);

    if(m_invalidatedGeometry)
        buildBoundingVolumeHierarchy();


    //update m_transform
    update();

    program.setUniform(RANDOM_INT_UNIFORM, rng());
    setUniforms(program);

    // switch the rendering buffers for each pass
    m_whichBuffer = !m_whichBuffer;
    unsigned short readIndex = m_whichBuffer ? 0 : 1;
    unsigned short writeIndex = m_whichBuffer ? 1 : 0;

    glActiveTexture(GL_TEXTURE0 + textureSlots["testTex"]);
    glBindTexture(GL_TEXTURE_2D, m_testTex);
    glError();

    glActiveTexture(GL_TEXTURE0 + textureSlots["accumulation"]);
    glBindTexture(GL_TEXTURE_2D, m_accuTexture[readIndex]);
    glError();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_accuFramebuffer);
    GLenum writebuffers[] = { GL_COLOR_ATTACHMENT0 + writeIndex };
    glDrawBuffers(1, writebuffers);


    //glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(m_vao);                                                                  
    glError();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);


    glViewport(0, 0, m_viewport.x, m_viewport.y);
    
    // TODO: fetch geometry data from m_children
    //       atm geometry data is attached to the shader by PolygonalDrawable
    //     -> requieres new SceneGraph + Iterators

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
    glError();

    //if(target)
    //    target->bind();
    glBlitFramebuffer(0, 0, m_viewport.x, m_viewport.y, 0, 0, m_viewport.x, m_viewport.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    //if(target)
    //    target->release();

    program.release();
}

void PathTracer::buildBoundingVolumeHierarchy()
{

}

void PathTracer::invalidateGeometry()
{
    m_invalidatedGeometry = true;
}

void PathTracer::append(Group * group)
{
    Group::append(group);
}

void PathTracer::setViewport(
        const int width
    ,   const int height)
{
    Camera::setViewport(width, height);
    if (m_accuTexture[0] == -1)
        return;
    for (int i=0; i<2; ++i) {
        glBindTexture(GL_TEXTURE_2D, m_accuTexture[i]);
        glError();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_viewport.x, m_viewport.y, 0, GL_RGBA, GL_FLOAT, 0);
        glError();

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

        glBindTexture(GL_TEXTURE_2D, 0);
        glError();
    }
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

    std::hash_map<glm::highp_uint, glm::uint> cache;

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
,   std::hash_map<glm::highp_uint, glm::uint> & cache)
{
    const bool aSmaller(a < b);
    const glm::highp_uint smaller(aSmaller ? a : b);
    const glm::highp_uint greater(aSmaller ? b : a);
    const glm::highp_uint hash((smaller << 32) + greater);

    std::hash_map<glm::highp_uint, glm::uint>::const_iterator h(cache.find(hash));
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
