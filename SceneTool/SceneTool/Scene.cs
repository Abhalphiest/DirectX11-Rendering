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
            foreach (Material m in materials)
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
            foreach (DirectionalLight l in dlights)
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
            foreach (Mesh m in meshes)
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
        public void buildDirectionalLight(double[] ambientColor, double[] diffuseColor, double[] direction, string name)
        {
            DirectionalLight dlight = new DirectionalLight();
            dlight.ambientColor = ambientColor;
            dlight.diffuseColor = diffuseColor;
            dlight.direction = direction;
            dlight.name = name;
            dlights.Add(dlight);
        }
        public void buildPointLight(double[] ambientColor, double[] diffuseColor, double[] position, string name)
        {
            PointLight plight = new PointLight();
            plight.ambientColor = ambientColor;
            plight.diffuseColor = diffuseColor;
            plight.position = position;
            plight.name = name;
            plights.Add(plight);
        }
        public void buildSpotLight(double[] ambientColor, double[] diffuseColor, double[] position, double[] direction, string name)
        {
            SpotLight slight = new SpotLight();
            slight.ambientColor = ambientColor;
            slight.diffuseColor = diffuseColor;
            slight.position = position;
            slight.direction = direction;
            slight.name = name;
            slights.Add(slight);
        }
        public void buildObject(int meshindex, int materialindex, string name)
        {
            Object obj = new SceneTool.Scene.Object();
            obj.meshindex = meshindex;
            obj.materialindex = materialindex;
            obj.name = name;
            objects.Add(obj);
        }
        public void buildMaterial(int diffuseindex, int normalindex, int specularindex, int multiplyindex, int pshader, int vshader, string name)
        {
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
        public void deleteMaterial(int index)
        {
            if (index < 0) return;
            materials.RemoveAt(index);
        }

        public void editObject(int index, int meshindex, int materialindex, string name)
        {
            if (index < 0) return;
            Object obj;
            obj.meshindex = meshindex;
            obj.materialindex = materialindex;
            obj.name = name;
            objects[index] = obj;
        }
        public void loadMesh(string filepath)
        {
            Mesh mesh = new SceneTool.Scene.Mesh();
            mesh.filepath = filepath;
            meshes.Add(mesh);
        }
        public void deleteMesh(int index)
        {
            if (index < 0) return;
            meshes.RemoveAt(index);
        }
        public void loadDiffuse(string filepath)
        {
            diffusetextures.Add(filepath);
        }
        public void loadSpecular(string filepath) { spectextures.Add(filepath); }
        public void loadMultiply(string filepath) { multiplytextures.Add(filepath); }
        public void loadNormal(string filepath) { normaltextures.Add(filepath); }
        public void deleteTexture(int index)
        {
            //more complicated implementation
            //need to calculate which list to go off of
            //based on size of index relative to lengths of lists
            //and order of display in the list box
            if (index < 0) return;
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
        public void deleteShader(int index)
        { //see delete texture above for comments
            if (index < 0) return;
            if (index >= vertexshaders.Count)//must be a pixel shader
            {
                pixelshaders.RemoveAt(index - vertexshaders.Count);
            }
            else //its a vertex shader
                vertexshaders.RemoveAt(index);

        }

        public void deleteLight(int index)
        {
            if (index < 0)
                return;
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

        public void editLight(int index, double[] ambient, double[] diffuse, double[] position, double[] direction, string name)
        {
            if (index < 0)
                return;
            if (index >= dlights.Count)
            {
                index -= dlights.Count;
                if (index >= plights.Count)
                {
                    SpotLight newslight = new SpotLight();
                    newslight.ambientColor = ambient;
                    newslight.diffuseColor = diffuse;
                    newslight.direction = direction;
                    newslight.position = position;
                    newslight.name = name;
                    slights[index] = newslight;
                }
                else
                {
                    PointLight newplight = new PointLight();
                    newplight.ambientColor = ambient;
                    newplight.diffuseColor = diffuse;
                    newplight.position = position;
                    newplight.name = name;
                    plights[index] = newplight;
                }
            }
            else
            {
                DirectionalLight newdlight = new DirectionalLight();
                newdlight.ambientColor = ambient;
                newdlight.diffuseColor = diffuse;
                newdlight.direction = direction;
                newdlight.name = name;
                dlights[index] = newdlight;
            }
        }
        //will be done later
        public bool loadScene(string filepath) { return true; }
        public bool saveScene(string filepath) { return true; }


        public bool isDirectionalLight(int index)
        {
            return index >= 0 && index < dlights.Count;
        }
        public bool isSpotLight(int index)
        {
            return index >= dlights.Count + plights.Count && index < dlights.Count + plights.Count + slights.Count;
        }
        public bool isPointLight(int index)
        {
            return index >= dlights.Count && index < dlights.Count + plights.Count;
        }

        public string[] getLightData(int index)
        {
            if (index < 0) return null;

            string[] lightData = new string[16];
            if(index >= dlights.Count)
            {
                if(index>= dlights.Count+plights.Count)
                {
                    SpotLight light = slights[index];
                    lightData[0] = light.ambientColor[0].ToString();
                    lightData[1] = light.ambientColor[1].ToString();
                    lightData[2] = light.ambientColor[2].ToString();
                    lightData[3] = light.ambientColor[3].ToString();
                    lightData[4] = light.diffuseColor[0].ToString();
                    lightData[5] = light.diffuseColor[1].ToString();
                    lightData[6] = light.diffuseColor[2].ToString();
                    lightData[7] = light.diffuseColor[3].ToString();
                    lightData[8] = light.position[0].ToString();
                    lightData[9] = light.position[1].ToString();
                    lightData[10] = light.position[2].ToString();
                    lightData[11] = light.direction[0].ToString();
                    lightData[12] = light.direction[1].ToString();
                    lightData[13] = light.direction[2].ToString();
                    lightData[14] = light.direction[3].ToString();
                    lightData[15] = light.name;
                }
                else
                {
                    PointLight light = plights[index];
                    lightData[0] = light.ambientColor[0].ToString();
                    lightData[1] = light.ambientColor[1].ToString();
                    lightData[2] = light.ambientColor[2].ToString();
                    lightData[3] = light.ambientColor[3].ToString();
                    lightData[4] = light.diffuseColor[0].ToString();
                    lightData[5] = light.diffuseColor[1].ToString();
                    lightData[6] = light.diffuseColor[2].ToString();
                    lightData[7] = light.diffuseColor[3].ToString();
                    lightData[8] = light.position[0].ToString();
                    lightData[9] = light.position[1].ToString();
                    lightData[10] = light.position[2].ToString();
                    lightData[11] = "";
                    lightData[12] = "";
                    lightData[13] = "";
                    lightData[14] = "";
                    lightData[15] = light.name;
                }
            }
            else
            {
                DirectionalLight light = dlights[index];
                lightData[0] = light.ambientColor[0].ToString();
                lightData[1] = light.ambientColor[1].ToString();
                lightData[2] = light.ambientColor[2].ToString();
                lightData[3] = light.ambientColor[3].ToString();
                lightData[4] = light.diffuseColor[0].ToString();
                lightData[5] = light.diffuseColor[1].ToString();
                lightData[6] = light.diffuseColor[2].ToString();
                lightData[7] = light.diffuseColor[3].ToString();
                lightData[8] = "";
                lightData[9] = "";
                lightData[10] = "";
                lightData[11] = light.direction[0].ToString();
                lightData[12] = light.direction[1].ToString();
                lightData[13] = light.direction[2].ToString();
                lightData[14] = "";
                lightData[15] = light.name;
            }
            return lightData;
        }

        public int[] getMaterialData(int index, out string name)
        {
            name = "";
            if (index < 0) return null;
            int[] matData = new int[6];
            Material mat = materials[index];
            matData[0] = mat.diffuseindex;
            matData[1] = mat.specindex;
            matData[2] = mat.multiplyindex;
            matData[3] = mat.normalindex;
            matData[4] = mat.vertexshader;
            matData[5] = mat.pixelshader;
            name = mat.name;
            return matData;
        }
    

    public void editMaterial(int index, int diffuseindex, int specularindex, int multiplyindex,
        int normalindex, int pixelshader, int vertexshader, string name)
    {
            if (index < 0) return;
            Material mat;
            mat.diffuseindex = diffuseindex;
            mat.specindex = specularindex;
            mat.multiplyindex = multiplyindex;
            mat.normalindex = normalindex;
            mat.pixelshader = pixelshader;
            mat.vertexshader = vertexshader;
            mat.name = name;
            materials[index] = mat;

    }
    }
}
