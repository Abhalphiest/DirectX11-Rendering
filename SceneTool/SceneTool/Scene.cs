using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SceneTool
{
    class Scene
    {
        struct Mesh { public string filepath; } //in case we want to expand functionality in the future
        struct Object { public string name; public int meshindex, materialindex; } //needs more data - position, orientation, etc
        struct Material { public string name; public int diffuseindex, multiplyindex, normalindex, specindex, pixelshader, vertexshader; }
        struct DirectionalLight { public double[] ambientColor, diffuseColor, direction; public string name; }
        struct PointLight { public double[] ambientColor, diffuseColor, position; public string name; }
        struct SpotLight { public double[] ambientColor, diffuseColor, position, direction; public string name; }

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

        public string[] getMaterialNameArray()
        {
            string[] names = new string[materials.Count];
            int i = 0;
            foreach(Material m in materials)
            {
                names[i] = m.name;
                i++;
            }
            return names;
        }
        public string[] getObjectNameArray()
        {
            string[] names = new string[objects.Count];
            int i = 0;
            foreach (Object o in objects)
            {
                names[i] = o.name;
                i++;
            }
            return names;
        }
        public string[] getDiffuseTextureArray()
        {
            return diffusetextures.ToArray();
        }
        public string[] getSpecularTextureArray()
        {
            return spectextures.ToArray();
        }
        public string[] getMultiplyTextureArray()
        {
            return multiplytextures.ToArray();
        }
        public string[] getNormalTextureArray()
        {
            return normaltextures.ToArray();
        }
        public string[] getTextureArray()
        {
            //diffuse,specular,multiply,normal
            List<string> texturelist = new List<string>();
            texturelist.AddRange(diffusetextures);
            texturelist.AddRange(spectextures);
            texturelist.AddRange(multiplytextures);
            texturelist.AddRange(normaltextures);
            return texturelist.ToArray();

        }

        public string[] getLightArray()
        {
            string[] lights = new string[dlights.Count + plights.Count + slights.Count];
            int i = 0;
            foreach(DirectionalLight l in dlights)
            {
                lights[i] = l.name + " (Directional)";
                i++;
            }
            foreach (PointLight p in plights)
            {
                lights[i] = p.name + " (Point)";
                i++;
            }
            foreach (SpotLight s in slights)
            {
                lights[i] = s.name + " (Spot)";
                i++;
            }
            return lights;
        }
        public string[] getMeshArray()
        {
            string[] filepaths = new string[meshes.Count];
            int i = 0;
            foreach(Mesh m in meshes)
            {
                filepaths[i] = m.filepath;
                i++;
            }
            return filepaths;
        }
        public string[] getPixelShaderArray()
        {
            return pixelshaders.ToArray();
        }
        public string[] getVertexShaderArray()
        {
            return vertexshaders.ToArray();
        }
        public string[] getShaderArray()
        {
            //vertex then pixel
            List<string> shaders = new List<string>();
            shaders.AddRange(vertexshaders);
            shaders.AddRange(pixelshaders);
            return shaders.ToArray();
        }
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
        public void buildDirectionalLight(double [] ambientColor, double[] diffuseColor,  double[] direction,string name) {
            DirectionalLight dlight = new DirectionalLight();
            dlight.ambientColor = ambientColor;
            dlight.diffuseColor = diffuseColor;
            dlight.direction = direction;
            dlight.name = name;
            dlights.Add(dlight);
        }
        public void buildPointLight(double[]ambientColor, double[] diffuseColor, double[] position,string name) {
            PointLight plight = new PointLight();
            plight.ambientColor = ambientColor;
            plight.diffuseColor = diffuseColor;
            plight.position = position;
            plight.name = name;
            plights.Add(plight);
        }
        public void buildSpotLight(double[]ambientColor, double[] diffuseColor, double[] position, double[] direction, string name) {
            SpotLight slight = new SpotLight();
            slight.ambientColor = ambientColor;
            slight.diffuseColor = diffuseColor;
            slight.position = position;
            slight.direction = direction;
            slight.name = name;
            slights.Add(slight);
        }
        public void buildObject(int meshindex, int materialindex) {
            Object obj = new SceneTool.Scene.Object();
            obj.meshindex = meshindex;
            obj.materialindex = materialindex;
            objects.Add(obj);
        }
        public void buildMaterial(int diffuseindex, int normalindex, int specularindex, int multiplyindex, int pshader, int vshader,string name) {
            Material mat = new SceneTool.Scene.Material();
            mat.diffuseindex = diffuseindex;
            mat.normalindex = normalindex;
            mat.specindex = specularindex;
            mat.multiplyindex = multiplyindex;
            mat.pixelshader = pshader;
            mat.vertexshader = vshader;
            mat.name = name;
            materials.Add(mat);
        }
        public void loadMesh(string filepath) {
            Mesh mesh = new SceneTool.Scene.Mesh();
            mesh.filepath = filepath;
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

            //diffuse,spec,multiply,normal
            if (index >= diffusetextures.Count)
            {
                index -= diffusetextures.Count;
                if (index >= spectextures.Count)
                {
                    index -= spectextures.Count;
                    if (index >= multiplytextures.Count)
                        normaltextures.RemoveAt(index - multiplytextures.Count);
                    else
                        multiplytextures.RemoveAt(index);
                }
                else
                    spectextures.RemoveAt(index);
            }
            else
                diffusetextures.RemoveAt(index);
        }
        public void loadPixelShader(string filepath) { pixelshaders.Add(filepath); }
        public void loadVertexShader(string filepath) { vertexshaders.Add(filepath); }
        public void deleteShader(int index) { //see delete texture above for comments
            if (index >= vertexshaders.Count)//must be a pixel shader
            {
                pixelshaders.RemoveAt(index - vertexshaders.Count);
            }
            else //its a vertex shader
                vertexshaders.RemoveAt(index);

        }

        public void deleteLight(int index)
        {
            if (index >= dlights.Count)
            {
                index -= dlights.Count;
                if (index >= plights.Count)
                    slights.RemoveAt(index - plights.Count);
                else
                    plights.RemoveAt(index);
            }
            else
                dlights.RemoveAt(index);
        }
        //will be done later
        public bool loadScene(string filepath) { return true; }
        public bool saveScene(string filepath) { return true; }
    }
}
