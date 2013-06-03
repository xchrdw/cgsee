
#include "assimploader.h"
#include "group.h"
#include "polygonalgeometry.h"
#include "polygonaldrawable.h"

AssimpLoader::AssimpLoader()
: AbstractModelLoader()
, m_importer(new Assimp::Importer())
{
}

AssimpLoader::~AssimpLoader()
{
    delete m_importer;
}

QStringList AssimpLoader::loadableTypes() const
{
    QStringList types = {
        "Collada (*.dae *.xml)",
        "Blender (*.blend)",
        "Biovision BVH (*.bvh)",
        "3D Studio Max 3DS (*.3ds)",
        "3D Studio Max ASE (*.ase)",
        "Wavefront Object (*.obj)",
        "Stanford Polygon Library (*.ply)",
        "AutoCAD DXF (*.dxf)",
        "IFC-STEP, Industry Foundation Classes (*.ifc)",
        "Neutral File Format (*.nff)",
        "Sense8 WorldToolkit (*.wtk)",
        "Valve Model (*.smd *.vta)",
        "Quake I (*.mdl)",
        "Quake II (*.md2)",
        "Quake III (*.md3)",
        "Quake 3 BSP (*.pk3)",
        "RtCW (*.mdc)",
        "Doom 3 (*.md5mesh *.md5anim *.md5camera)",
        "DirectX X (*.x)",
        "Quick3D (*.q3o *.q3s)",
        "Raw Triangles (.raw)",
        "AC3D (*.ac)",
        "Stereolithography (*.stl)",
        "Autodesk DXF (*.dxf)",
        "Irrlicht Mesh (*.irrmesh *.xml)",
        "Irrlicht Scene (*.irr *.xml)",
        "Object File Format (*.off)",
        "Terragen Terrain (*.ter)",
        "3D GameStudio Model (*.mdl)",
        "3D GameStudio Terrain (*.hmp)",
        "Ogre (*.mesh.xml, *.skeleton.xml, *.material)",
        "Milkshape 3D (*.ms3d)",
        "LightWave Model (*.lwo)",
        "LightWave Scene (*.lws)",
        "Modo Model (*.lxo)",
        "CharacterStudio Motion (*.csm)",
        "Stanford Ply (*.ply)",
        "TrueSpace (*.cob *.scn)",
        "XGL (*.xgl *.zgl)"
    };

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

    const aiScene * scene = m_importer->ReadFile(filePath.toStdString(),
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
        mat.a1, mat.a2, mat.a3, mat.a4,
        mat.b1, mat.b2, mat.b3, mat.b4,
        mat.c1, mat.c2, mat.c3, mat.c4,
        mat.d1, mat.d2, mat.d3, mat.d4
    );
    group->setTransform(transform);

    for (int i = 0; i < node.mNumChildren; i++)
        group->append(parseNode(scene, drawables, *(node.mChildren[i])));

    for (int i = 0; i < node.mNumMeshes; i++)
        group->append(drawables[node.mMeshes[i]]);

    return group;
}

void AssimpLoader::parseMeshes(aiMesh **meshes,
    const unsigned int numMeshes, QList<PolygonalDrawable *> &drawables) const
{
    for (int i = 0; i < numMeshes; i++)
        drawables.insert(i, parseMesh(*meshes[i]));
}

PolygonalDrawable * AssimpLoader::parseMesh(const aiMesh & mesh) const
{
    PolygonalGeometry * geometry = new PolygonalGeometry(QString(mesh.mName.C_Str()) + " geometry");

    for (int i = 0; i < mesh.mNumVertices; i++) {
        glm::vec3 vector(
            mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z
        );
        geometry->vertices().push_back(vector);
    }

    for (int i = 0; i < mesh.mNumFaces; i++) {
        if (mesh.mFaces[i].mNumIndices != 3)
            qCritical("Ignore polygon with num vertices != 3 (only triangles are supported).");
        else
            for (int j = 0; j < mesh.mFaces[i].mNumIndices; j++)
                geometry->indices().push_back(mesh.mFaces[i].mIndices[j]);
    }

    geometry->setMode(GL_TRIANGLES);
    geometry->retrieveNormals();

    PolygonalDrawable * drawable = new PolygonalDrawable(mesh.mName.C_Str());
    drawable->setGeometry(geometry);
    return drawable;
}