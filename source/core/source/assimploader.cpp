
#include <core/assimploader.h>

#include <glbinding/gl/enum.h>

#include <core/datacore/datablock.h>
#include <core/scenegraph/group.h>
#include <core/scenegraph/polygonalgeometry.h>
#include <core/scenegraph/polygonaldrawable.h>

AssimpLoader::AssimpLoader(std::shared_ptr<DataBlockRegistry> registry)
: AbstractModelLoader(registry)
, m_importer(new Assimp::Importer())
{

}

AssimpLoader::~AssimpLoader()
{
    delete m_importer;
}

QStringList AssimpLoader::namedLoadableTypes() const
{
    QStringList types = AbstractModelLoader::namedLoadableTypes();
    types
        << "Collada (*.dae *.xml)"
        << "Blender (*.blend)"
        << "Biovision BVH (*.bvh)"
        << "3D Studio Max 3DS (*.3ds)"
        << "3D Studio Max ASE (*.ase)"
        << "Wavefront Object (*.obj)"
        << "Stanford Polygon Library (*.ply)"
        << "AutoCAD DXF (*.dxf)"
        << "IFC-STEP, Industry Foundation Classes (*.ifc)"
        << "Neutral File Format (*.nff)"
        << "Sense8 WorldToolkit (*.wtk)"
        << "Valve Model (*.smd *.vta)"
        << "Quake I (*.mdl)"
        << "Quake II (*.md2)"
        << "Quake III (*.md3)"
        << "Quake 3 BSP (*.pk3)"
        << "RtCW (*.mdc)"
        << "Doom 3 (*.md5mesh *.md5anim *.md5camera)"
        << "DirectX X (*.x)"
        << "Quick3D (*.q3o *.q3s)"
        << "Raw Triangles (.raw)"
        << "AC3D (*.ac)"
        << "Stereolithography (*.stl)"
        << "Autodesk DXF (*.dxf)"
        << "Irrlicht Mesh (*.irrmesh *.xml)"
        << "Irrlicht Scene (*.irr *.xml)"
        << "Object File Format (*.off)"
        << "Terragen Terrain (*.ter)"
        << "3D GameStudio Model (*.mdl)"
        << "3D GameStudio Terrain (*.hmp)"
        << "Ogre (*.mesh.xml, *.skeleton.xml, *.material)"
        << "Milkshape 3D (*.ms3d)"
        << "LightWave Model (*.lwo)"
        << "LightWave Scene (*.lws)"
        << "Modo Model (*.lxo)"
        << "CharacterStudio Motion (*.csm)"
        << "Stanford Ply (*.ply)"
        << "TrueSpace (*.cob *.scn)"
        << "XGL (*.xgl *.zgl)";

    return types;
}

QStringList AssimpLoader::loadableExtensions() const
{
    aiString assimp_extensions;
    m_importer->GetExtensionList(assimp_extensions);
    QStringList extensions = QString(assimp_extensions.C_Str()).split(";");
    for (int i = 0; i < extensions.length(); i++)
        extensions[i].remove(0, 2);

    return extensions;
}

Group * AssimpLoader::importFromFile(const QString & filePath) const
{
    qDebug("Reading geometry with Assimp from \"%s\".", qPrintable(filePath));

    const std::string filePath_asStd = (filePath.toStdString());
    const aiScene * scene = m_importer->ReadFile(filePath_asStd,
                                              aiProcess_Triangulate);

    if (!scene) {
        qCritical("Assimp couldn't load %s", qPrintable(filePath));
        return nullptr;
    }

    QList<PolygonalDrawable *> drawables;
    drawables.reserve(scene->mNumMeshes);
    this->parseMeshes(scene->mMeshes, scene->mNumMeshes, drawables);
    
    Group * group = parseNode(*scene, drawables, *(scene->mRootNode));
    
    m_importer->FreeScene();
    
    return group;
}

Group * AssimpLoader::parseNode(const aiScene & scene,
    const QList<PolygonalDrawable *> &drawables, const aiNode & node) const
{
    Group * group = new Group(node.mName.C_Str());

    const aiMatrix4x4 & mat = node.mTransformation;
    glm::mat4 transform(
        mat.a1, mat.b1, mat.c1, mat.d1,
        mat.a2, mat.b2, mat.c2, mat.d2,
        mat.a3, mat.b3, mat.c3, mat.d3,
        mat.a4, mat.b4, mat.c4, mat.d4
    );
    /*
    translation vector is marked with []
    assimp's aiMatrix4x4 : row major (as usually written down in maths)
     a1  a2  a3 [a4]  <1. row>
     b1  b2  b3 [b4]  <2. row>
     c1  c2  c3 [c4]  <3. row>
     d1  d2  d3  d4   <4. row>
    glm/openGL mat4 : column major (not as stated in http://assimp.sourceforge.net/lib_html/data.html , search for "All matrices")
     a1  b1  c1  d1   <1. column>
     a2  b2  c2  d2   <2. column>
     a3  b3  c3  d3   <3. column>
    [a4][b4][c4] d4   <4. column>
    */
    group->setTransform(transform);

    for (unsigned int i = 0; i < node.mNumChildren; i++)
        group->append(parseNode(scene, drawables, *(node.mChildren[i])));

    for (unsigned int i = 0; i < node.mNumMeshes; i++)
        group->append(drawables[node.mMeshes[i]]);

    return group;
}

void AssimpLoader::parseMeshes(aiMesh **meshes,
    const unsigned int numMeshes, QList<PolygonalDrawable *> &drawables) const
{
    for (unsigned int i = 0; i < numMeshes; i++)
        drawables.insert(i, parseMesh(*meshes[i]));
}

PolygonalDrawable * AssimpLoader::parseMesh(const aiMesh & mesh) const
{
    auto geometry = std::make_shared<PolygonalGeometry>(m_registry);
    
    const bool usesNormalIndices(mesh.mNormals != NULL);
    
    for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
        glm::vec3 vector(
                         mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z
                         );
        geometry->setVertex(i, vector);
    }
    
    if (usesNormalIndices) {
        for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
            glm::vec3 vector(
                             mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z
                             );
            geometry->setNormal(i, vector);
        }
    }
    
    unsigned int currentIndex = 0;
    for (unsigned int i = 0; i < mesh.mNumFaces; i++) {
        if (mesh.mFaces[i].mNumIndices != 3)
            qCritical("Ignore polygon with num vertices != 3 (only triangles are supported).");
        else
            for (unsigned int j = 0; j < mesh.mFaces[i].mNumIndices; j++)
                geometry->setIndex(currentIndex++, mesh.mFaces[i].mIndices[j]);
    }
    
    if (!usesNormalIndices)
        geometry->retrieveNormals();
    
    PolygonalDrawable * drawable = new PolygonalDrawable(mesh.mName.C_Str());
    drawable->setGeometry(geometry);
    drawable->setMode(gl::GL_TRIANGLES);
    return drawable;
}