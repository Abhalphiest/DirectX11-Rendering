using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace SceneTool
{
    class Scene
    {
        struct Mesh { public string filepath; } //in case we want to expand functionality in the future
        struct Object
        {
            public string name; public int meshindex, materialindex;
            public double[] position, orientation;
            public double scale;
        }
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
        public void buildObject(int meshindex, int materialindex, string name, double[] position, double[] orientation, double scale)
        {
            Object obj = new SceneTool.Scene.Object();
            obj.meshindex = meshindex;
            obj.materialindex = materialindex;
            obj.name = name;
            obj.position = position;
            obj.orientation = orientation;
            obj.scale = scale;
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

        public void editObject(int index, int meshindex, int materialindex, string name, double[] position, double[] orientation, double scale)
        {
            if (index < 0) return;
            Object obj;
            obj.meshindex = meshindex;
            obj.materialindex = materialindex;
            obj.name = name;
            obj.position = position;
            obj.orientation = orientation;
            obj.scale = scale;
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

        private void processLine(ref string line)
        {
            int i = 0;
            while (Char.IsWhiteSpace(line[0])) i++;

            line.Remove(0, i); //remove leading whitespace

            i = line.IndexOf('#');
            if (i != -1)
                line.Remove(i); //remove comments
        }
        //will be done later
        public bool loadScene(string filepath) {
            // File input object\
            StreamReader file;
            try
            {
                file = new StreamReader(filepath);
            }
            catch (Exception ex) //invalid filepath
            {
                return false;
            }

            string line = file.ReadLine();                     // String for line reading
            processLine(ref line);


            while (line != null  && !line.Contains("%")) //while in resources section of file
            {

                processLine(ref line);
                if(line.Length <= 0)
                {
                    line = file.ReadLine();
                    continue;
                }
                switch (line[0]) //first not whitespace character tells us how to interpret
                {
                    case 'o': //object file
                        {
                            line.Remove(0, 1);
                            processLine(ref line);
                            Mesh mesh;
                            mesh.filepath = line;
                            meshes.Add(mesh);
                            break;
                        }
                    case 't': //diffuse texture
                        {
                            line.Remove(0, 1);
                            processLine(ref line);
                            diffusetextures.Add(line);
                            break;
                        }
                    case 'n': //normal map
                        {
                            line.Remove(0, 1);
                            processLine(ref line);
                            normaltextures.Add(line);
                            break;
                        }
                    case 's': //spec map
                        {
                            line.Remove(0, 1);
                            processLine(ref line);
                            spectextures.Add(line);
                            break;
                        }
                    case 'm': //multiply map
                        {
                            line.Remove(0, 1);
                            processLine(ref line);
                            multiplytextures.Add(line);
                            break;
                        }
                    case 'p': //pixel shader
                        {
                            line.Remove(0, 1);
                            processLine(ref line);
                            pixelshaders.Add(line);
                            break;
                        }
                    case 'v': //vertex shader
                        {
                            line.Remove(0, 1);
                            processLine(ref line);
                            vertexshaders.Add(line);
                            break;
                        }
                }
                line = file.ReadLine();
            }

            line = file.ReadLine();
            //moving on to constructing materials
            while (line !=  null && !line.Contains('$'))
            {
                processLine(ref line);
                if(line.Length <= 0)
                {
                    line = file.ReadLine();
                    continue;
                }
                //get our indices
                string[] indices = line.Split('/');
                //create material
                Material mat = new Material();
                mat.diffuseindex = Int16.Parse(indices[0]);
                mat.normalindex = Int16.Parse(indices[1]);
                mat.specindex = Int16.Parse(indices[2]);
                mat.multiplyindex = Int16.Parse(indices[3]);
                materials.Add(mat);
                line = file.ReadLine();
            }

            line = file.ReadLine();
            //now we make objects
            while (line != null && !line.Contains('&'))
            {
                processLine(ref line);
                if (line.Length <= 0)
                {
                    line = file.ReadLine();
                    continue;
                }
                string[] indices = line.Split('/');
                Object obj = new Object();
                obj.meshindex = Int16.Parse(indices[0]);
                obj.materialindex = Int16.Parse(indices[1]);

                line = file.ReadLine();
                processLine(ref line);
                while(line.Length < 0)
                {
                    line = file.ReadLine();
                    processLine(ref line);
                }
                line.Remove(0, 3); //remove pos prefix
                processLine(ref line);
                string[] values = line.Split(' ');
                double[] position = new double[3];
                position[0] = Int16.Parse(values[0]);
                position[1] = Int16.Parse(values[1]);
                position[2] = Int16.Parse(values[2]);

                obj.position = position;

                //orientation
                line = file.ReadLine();
                processLine(ref line);
                while (line.Length < 0)
                {
                    line = file.ReadLine();
                    processLine(ref line);
                }
                line.Remove(0, 3); //remove orn prefix
                processLine(ref line);
                values = line.Split(' ');
                double[] orientation = new double[3];
                orientation[0] = Double.Parse(values[0]);
                orientation[1] = Double.Parse(values[1]);
                orientation[2] = Double.Parse(values[2]);

                obj.orientation = orientation;

                //scale
                line = file.ReadLine();
                processLine(ref line);
                while (line.Length < 0)
                {
                    line = file.ReadLine();
                    processLine(ref line);
                }

                line.Remove(0, 2); //remove sc prefix
                processLine(ref line);
                obj.scale = Double.Parse(line);

                objects.Add(obj);
                //done with this object
                line = file.ReadLine();
            }

            line = file.ReadLine();
            //now we make the lights
            while (line != null)
            {
                processLine(ref line);
                if (line.Length < 0)
                {
                    line = file.ReadLine();
                    continue;
                }

                switch (line[0])
                {
                    case 'd': //directional light
                        {
                            line = file.ReadLine();
                            processLine(ref line);
                            while (line.Length < 0)
                            {
                                line = file.ReadLine();
                                processLine(ref line);
                            }

                            string[] values = line.Split(' ');
                            double[] ambient = new double[4];
                            ambient[0] = Double.Parse(values[0]);
                            ambient[1] = Double.Parse(values[1]);
                            ambient[2] = Double.Parse(values[2]);
                            ambient[3] = Double.Parse(values[3]);


                            line = file.ReadLine();
                            processLine(ref line);
                            while (line.Length < 0)
                            {
                                line = file.ReadLine();
                                processLine(ref line);
                            }

                            values = line.Split(' ');
                            double[] diffuse = new double[4];
                            diffuse[0] = Double.Parse(values[0]);
                            diffuse[1] = Double.Parse(values[1]);
                            diffuse[2] = Double.Parse(values[2]);
                            diffuse[3] = Double.Parse(values[3]);

                            line = file.ReadLine();
                            processLine(ref line);
                            while (line.Length < 0)
                            {
                                line = file.ReadLine();
                                processLine(ref line);
                            }

                            values = line.Split(' ');
                            double[] direction = new double[3];
                            direction[0] = Double.Parse(values[0]);
                            direction[1] = Double.Parse(values[1]);
                            direction[2] = Double.Parse(values[2]);

                            DirectionalLight d = new DirectionalLight();
                            d.ambientColor = ambient;
                            d.diffuseColor = diffuse;
                            d.direction = direction;

                            dlights.Add(d);
                            break;
                        }
                    case 'p': //point light
                        {
                            line = file.ReadLine();
                            processLine(ref line);
                            while (line.Length < 0)
                            {
                                line = file.ReadLine();
                                processLine(ref line);
                            }

                            string[] values = line.Split(' ');
                            double[] ambient = new double[4];
                            ambient[0] = Double.Parse(values[0]);
                            ambient[1] = Double.Parse(values[1]);
                            ambient[2] = Double.Parse(values[2]);
                            ambient[3] = Double.Parse(values[3]);


                            line = file.ReadLine();
                            processLine(ref line);
                            while (line.Length < 0)
                            {
                                line = file.ReadLine();
                                processLine(ref line);
                            }

                            values = line.Split(' ');
                            double[] diffuse = new double[4];
                            diffuse[0] = Double.Parse(values[0]);
                            diffuse[1] = Double.Parse(values[1]);
                            diffuse[2] = Double.Parse(values[2]);
                            diffuse[3] = Double.Parse(values[3]);

                            line = file.ReadLine();
                            processLine(ref line);
                            while (line.Length < 0)
                            {
                                line = file.ReadLine();
                                processLine(ref line);
                            }

                            values = line.Split(' ');
                            double[] position = new double[3];
                            position[0] = Double.Parse(values[0]);
                            position[1] = Double.Parse(values[1]);
                            position[2] = Double.Parse(values[2]);

                            PointLight p = new PointLight();
                            p.ambientColor = ambient;
                            p.diffuseColor = diffuse;
                            p.position = position;

                            plights.Add(p);
                            break;
                        }
                    case 's': //spot light
                        {
                            line = file.ReadLine();
                            processLine(ref line);
                            while (line.Length < 0)
                            {
                                line = file.ReadLine();
                                processLine(ref line);
                            }

                            string[] values = line.Split(' ');
                            double[] ambient = new double[4];
                            ambient[0] = Double.Parse(values[0]);
                            ambient[1] = Double.Parse(values[1]);
                            ambient[2] = Double.Parse(values[2]);
                            ambient[3] = Double.Parse(values[3]);


                            line = file.ReadLine();
                            processLine(ref line);
                            while (line.Length < 0)
                            {
                                line = file.ReadLine();
                                processLine(ref line);
                            }

                            values = line.Split(' ');
                            double[] diffuse = new double[4];
                            diffuse[0] = Double.Parse(values[0]);
                            diffuse[1] = Double.Parse(values[1]);
                            diffuse[2] = Double.Parse(values[2]);
                            diffuse[3] = Double.Parse(values[3]);


                            line = file.ReadLine();
                            processLine(ref line);
                            while (line.Length < 0)
                            {
                                line = file.ReadLine();
                                processLine(ref line);
                            }

                            values = line.Split(' ');
                            double[] direction = new double[4];
                            direction[0] = Double.Parse(values[0]);
                            direction[1] = Double.Parse(values[1]);
                            direction[2] = Double.Parse(values[2]);
                            direction[3] = Double.Parse(values[3]);

                            line = file.ReadLine();
                            processLine(ref line);
                            while (line.Length < 0)
                            {
                                line = file.ReadLine();
                                processLine(ref line);
                            }

                            values = line.Split(' ');
                            double[] position = new double[3];
                            position[0] = Double.Parse(values[0]);
                            position[1] = Double.Parse(values[1]);
                            position[2] = Double.Parse(values[2]);

                            SpotLight s = new SpotLight();
                            s.ambientColor = ambient;
                            s.diffuseColor = diffuse;
                            s.direction = direction;
                            s.position = position;

                            slights.Add(s);
                            break;
                        }
                }
            }

            //close the file
            file.Close();
            return true;


        }
        public bool saveScene(string filepath)
        {
            StreamWriter file;
            try
            {
                file = File.AppendText(filepath);
            }
            catch (Exception ex) //invalid filepath
            {
                return false;
            }


            foreach (Mesh m in meshes)
            {
                file.WriteLine("m " + m.filepath);
            }
            foreach (string t in diffusetextures)
            {
                file.WriteLine("t " + t);
            }
            foreach (string n in normaltextures)
            {
                file.WriteLine("n " + n);
            }
            foreach (string s in spectextures)
            {
                file.WriteLine("s " + s);
            }
            foreach (string m in multiplytextures)
            {
                file.WriteLine("m " + m);
            }
            foreach (string p in pixelshaders)
            {
                file.WriteLine("p " + p);
            }
            foreach (string v in vertexshaders)
            {
                file.WriteLine("v " + v);
            }

            file.WriteLine("%");

            //now materials

            foreach (Material mat in materials)
            {
                file.WriteLine("#"+mat.name);
                file.WriteLine(mat.diffuseindex.ToString() + "/" + mat.normalindex.ToString()+"/"
                        +mat.specindex.ToString()+"/"+mat.multiplyindex.ToString()+"/"
                        +mat.vertexshader.ToString()+"/"+mat.pixelshader.ToString());
            }
            file.WriteLine("$");
            //now objects
            foreach (Object obj in objects)
            {
                file.WriteLine("#" + obj.name);
                file.WriteLine(obj.meshindex.ToString() + "/" +obj.materialindex.ToString());
                file.WriteLine("pos " + obj.position[0] + " " + obj.position[1] + " " + obj.position[2]);
                file.WriteLine("orn " + obj.orientation[0] + " " + obj.orientation[1] + " " + obj.orientation[2]);
                file.WriteLine("sc " + obj.scale);
                file.WriteLine();
            }
            file.WriteLine("&");
            //now lights
            foreach (DirectionalLight d in dlights)
            {
                file.WriteLine("#" + d.name);
                file.WriteLine("d");
                file.WriteLine(d.ambientColor[0] + " " + d.ambientColor[1] + " " + d.ambientColor[2] + " " + d.ambientColor[3]);
                file.WriteLine(d.diffuseColor[0] + " " + d.diffuseColor[1] + " " + d.diffuseColor[2] + " " + d.diffuseColor[3]);
                file.WriteLine(d.direction[0] + " " + d.direction[1] + " " + d.direction[2]);
                file.WriteLine();
            }
            foreach (PointLight p in plights)
            {
                file.WriteLine("#" + p.name);
                file.WriteLine("p");
                file.WriteLine(p.ambientColor[0] + " " + p.ambientColor[1] + " " + p.ambientColor[2] + " " + p.ambientColor[3]);
                file.WriteLine(p.diffuseColor[0] + " " + p.diffuseColor[1] + " " + p.diffuseColor[2] + " " + p.diffuseColor[3]);
                file.WriteLine(p.position[0] + " " + p.position[1] + " " + p.position[2]);
                file.WriteLine();
            }
            foreach(SpotLight s in slights)
            {
                file.WriteLine("#" + s.name);
                file.WriteLine("s");
                file.WriteLine(s.ambientColor[0] + " " + s.ambientColor[1] + " " + s.ambientColor[2] + " " + s.ambientColor[3]);
                file.WriteLine(s.diffuseColor[0] + " " + s.diffuseColor[1] + " " + s.diffuseColor[2] + " " + s.diffuseColor[3]);
                file.WriteLine(s.direction[0] + " " + s.direction[1] + " " + s.direction[2] + " " + s.direction[3]);
                file.WriteLine(s.position[0] + " " + s.position[1] + " " + s.position[2]);
                file.WriteLine();
            }
            file.Close();
            return true;
        }


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
            if (index >= dlights.Count)
            {
                if (index >= dlights.Count + plights.Count)
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

        public void deleteObject(int index)
        {
            if (index < 0) return;
            objects.RemoveAt(index);
        }

        public void getObjectData(int index, out string name, out int meshindex, out int materialindex, out double[] data)
        {
            name = null;
            meshindex = 0;
            materialindex = 0;
            data = null;

            if (index < 0) return;

            Object obj = objects[index];
            name = obj.name;
            meshindex = obj.meshindex;
            materialindex = obj.materialindex;
            data[0] = obj.position[0];
            data[1] = obj.position[1];
            data[2] = obj.position[2];
            data[3] = obj.orientation[0];
            data[4] = obj.orientation[1];
            data[5] = obj.orientation[2];
            data[6] = obj.scale;
        }

        public void Clear()
        {
            objects.Clear();
            meshes.Clear();
            materials.Clear();
            vertexshaders.Clear();
            pixelshaders.Clear();
            diffusetextures.Clear();
            spectextures.Clear();
            normaltextures.Clear();
            multiplytextures.Clear();
            dlights.Clear();
            slights.Clear();
            plights.Clear();
        }
    }
}
