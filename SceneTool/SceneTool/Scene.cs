using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SceneTool
{
    class Scene
    {
        struct Mesh { public string filepath; }
        struct Object { public int meshindex, materialindex; } //needs more data - position, orientation, etc
        struct Material { public int diffuseindex, multiplyindex, normalindex, specindex, pixelshader, vertexshader; }
        struct DirectionalLight { public double[] ambientColor, diffuseColor, direction; }
        struct PointLight { public double[] ambientColor, diffuseColor, position; }
        struct SpotLight { public double[] ambientColor, diffuseColor, position, direction; }

        List<string> pixelshaders;
        List<string> vertexshaders;
        List<string> diffusetextures;
        List<string> multiplytextures;
        List<string> normaltextures;
        List<string> spectextures;
        List<Mesh> meshes;
        List<Material> materials;
        List<Object> objects;
        List<DirectionalLight> dlights;
        List<PointLight> plights;
        List<SpotLight> slights;

        //need scene properties here


        public Scene()
        {
            pixelshaders = new List<string>();
            vertexshaders = new List<string>();
            meshes = new List<Mesh>();
            materials = new List<Material>();
            objects = new List<Object>();
            dlights = new List<DirectionalLight>();
            plights = new List<PointLight>();
            slights = new List<SpotLight>();
            diffusetextures = new List<string>();
            normaltextures = new List<string>();
            multiplytextures = new List<string>();
            spectextures = new List<string>();

        }
        public void buildDirectionalLight(double [] ambientColor, double[] diffuseColor,  double[] direction) {
            DirectionalLight dlight = new DirectionalLight();
            dlight.ambientColor = ambientColor;
            dlight.diffuseColor = diffuseColor;
            dlight.direction = direction;
            dlights.Add(dlight);
        }
        public void buildPointLight(double[]ambientColor, double[] diffuseColor, double[] position) {
            PointLight plight = new PointLight();
            plight.ambientColor = ambientColor;
            plight.diffuseColor = diffuseColor;
            plight.position = position;
            plights.Add(plight);
        }
        public void buildSpotLight(double[]ambientColor, double[] diffuseColor, double[] position, double[] direction) {
            SpotLight slight = new SpotLight();
            slight.ambientColor = ambientColor;
            slight.diffuseColor = diffuseColor;
            slight.position = position;
            slight.direction = direction;
            slights.Add(slight);
        }
        public void buildObject(int meshindex, int materialindex) {
            Object obj = new SceneTool.Scene.Object();
            obj.meshindex = meshindex;
            obj.materialindex = materialindex;
            objects.Add(obj);
        }
        public void buildMaterial(int diffuseindex, int normalindex, int specularindex, int multiplyindex, int pshader, int vshader) {
            Material mat = new SceneTool.Scene.Material();
            mat.diffuseindex = diffuseindex;
            mat.normalindex = normalindex;
            mat.specindex = specularindex;
            mat.multiplyindex = multiplyindex;
            mat.pixelshader = pshader;
            mat.vertexshader = vshader;
            materials.Add(mat);
        }
        public void loadMesh(string filepath) {
            //more complicated implementation
            //need to load mesh, calculate bounding box
            //build and hookup rectangle
            //etc
        }
        public void deleteMesh(int index) {
            meshes.RemoveAt(index);
        }
        public void loadDiffuse(string filepath) {
            diffusetextures.Add(filepath);
        }
        public void loadSpecular(string filepath) { spectextures.Add(filepath); }
        public void loadMultiply(string filepath) { multiplytextures.Add(filepath); }
        public void loadNormal(string filepath) { normaltextures.Add(filepath); }
        public void deleteTexture(int index) {
            //more complicated implementation
            //need to calculate which list to go off of
            //based on size of index relative to lengths of lists
            //and order of display in the list box
        }
        public void loadPixelShader(string filepath) { pixelshaders.Add(filepath); }
        public void loadVertexShader(string filepath) { vertexshaders.Add(filepath); }
        public void deleteShader(int index) { //see deleteTexture above
        }

        //will be done later
        public bool loadScene(string filepath) { return true; }
        public bool saveScene(string filepath) { return true; }
    }
}
