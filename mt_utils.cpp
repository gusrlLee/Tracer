#include "mt_utils.h"

std::string readShaderFile(std::string shaderFilePath) 
{
    std::string shaderCode;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try 
    {
        shaderFile.open(shaderFilePath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "Shader File : " << shaderFilePath << std::endl;
        std::runtime_error("Failed to open shader file !");
    }
    
    return shaderCode;
}

bool loadGltfModel(tinygltf::Model &model, const char *modelDataFilePath)
{
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool res = loader.LoadASCIIFromFile(&model, &err, &warn, modelDataFilePath);
    
    if (!warn.empty()) 
    {
        std::cout << "[TINYGLTF] WARN : " << warn << std::endl;
    }
    
    if (!err.empty()) 
    {
        std::cout << "[TINYGLTF] ERROR : " << err << std::endl;
    }

    if (!res)
    {
        std::cout << "Failed to load glTF model : " << modelDataFilePath << std::endl;
    }
    else 
    {
        std::cout << "Success to laod glTF model : " << modelDataFilePath << std::endl;
    }
    
    return res;
}

void debugModel(tinygltf::Model &model) 
{
  for (auto &mesh : model.meshes) {
    std::cout << "mesh : " << mesh.name << std::endl;
    for (auto &primitive : mesh.primitives) {
      const tinygltf::Accessor &indexAccessor =
          model.accessors[primitive.indices];

      std::cout << "indexaccessor: count " << indexAccessor.count << ", type "
                << indexAccessor.componentType << std::endl;

      tinygltf::Material &mat = model.materials[primitive.material];
      for (auto &mats : mat.values) {
        std::cout << "mat : " << mats.first.c_str() << std::endl;
      }

      for (auto &image : model.images) {
        std::cout << "image name : " << image.uri << std::endl;
        std::cout << "  size : " << image.image.size() << std::endl;
        std::cout << "  w/h : " << image.width << "/" << image.height
                  << std::endl;
      }

      std::cout << "indices : " << primitive.indices << std::endl;
      std::cout << "mode     : "
                << "(" << primitive.mode << ")" << std::endl;

      for (auto &attrib : primitive.attributes) {
        std::cout << "attribute : " << attrib.first.c_str() << std::endl;
      }
    }
  }
}
