using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SceneTool
{
    
    public partial class mainscreen : Form
    {
        Scene scene;
        const int NUM_CELLS = 80;
        const int CELL_SIZE = 5;
        public mainscreen()
        {
            scene = new SceneTool.Scene();
            InitializeComponent();
        }

        private void scenewindow_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            Pen p = new Pen(Color.Gray);

            for(int i = 0; i <= NUM_CELLS; i++)
            {
                // Vertical
                g.DrawLine(p, i * CELL_SIZE, 0, i * CELL_SIZE, NUM_CELLS * CELL_SIZE);
                // Horizontal
                g.DrawLine(p, 0, i * CELL_SIZE, NUM_CELLS * CELL_SIZE, i * CELL_SIZE);
            }
        }

        private void newscene_Click(object sender, EventArgs e)
        {
            startpanel.Visible = false;
            scenewindow.Visible = true;
            tabControl.Visible = true;
            clearbutton.Visible = true;
            loadbutton.Visible = true;
            savescenebutton.Visible = true;
        }

        private void createlightbutton_Click(object sender, EventArgs e)
        {
            buildlightpanel.Visible = true;
            lightpanel.Visible = false;
        }

        private void buildlightbackbutton_Click(object sender, EventArgs e)
        {
            lightname.Text = "";
            buildlightbutton.Visible = false;
            savelightchangesbutton.Visible = true;
            spotradio.Enabled = true;
            directionalradio.Enabled = true;
            pointradio.Enabled = true;
            lightdirectionX.Clear();
            lightdirectionY.Clear();
            lightdirectionZ.Clear();
            lightdirectionW.Clear();
            lightpositionX.Clear();
            lightpositionY.Clear();
            lightpositionZ.Clear();
            ambientR.Clear();
            ambientG.Clear();
            ambientB.Clear();
            ambientA.Clear();
            diffuseR.Clear();
            diffuseG.Clear();
            diffuseB.Clear();
            diffuseA.Clear();
            directionalradio.Checked = true;

            buildlightpanel.Visible = false;
            lightpanel.Visible = true;
        }

        private void buildlightbutton_Click(object sender, EventArgs e)
        {
            string name = lightname.Text;
            if(directionalradio.Checked)
            {
                double[] direction = new double[3];
                double[] ambient = new double[4];
                double[] diffuse = new double[4];

                //get direction
                bool result = Double.TryParse(lightdirectionX.Text, out direction[0]);
                if(!result)
                {
                    //error handling
                }
                result= Double.TryParse(lightdirectionY.Text, out direction[1]);
                if(!result)
                {
                    //error handling
                }
                result = Double.TryParse(lightdirectionZ.Text, out direction[2]);
                if(!result)
                {
                    //error handling
                }

                //get ambient
                result = Double.TryParse(ambientR.Text, out ambient[0]);
                if(! result)
                {
                    //error handling
                }
                result = Double.TryParse(ambientG.Text, out ambient[1]);
                if(!result)
                {
                    //error handling
                }
                result = Double.TryParse(ambientB.Text, out ambient[2]);
                if(!result)
                {
                    //error handling
                }
                result = Double.TryParse(ambientA.Text, out ambient[3]);
                if (!result)
                {
                    //error handling
                }



                //get diffuse

                result = Double.TryParse(diffuseR.Text, out diffuse[0]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(diffuseG.Text, out diffuse[1]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(diffuseB.Text, out diffuse[2]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(diffuseA.Text, out diffuse[3]);
                if (!result)
                {
                    //error handling
                }

                //actually build the thing
                scene.buildDirectionalLight(ambient,diffuse,direction,name);
            }
            else if(pointradio.Checked)
            {
                double[] position = new double[3];
                double[] ambient = new double[4];
                double[] diffuse = new double[4];

                //get position
                bool result = Double.TryParse(lightpositionX.Text, out position[0]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(lightpositionY.Text, out position[1]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(lightpositionZ.Text, out position[2]);
                if (!result)
                {
                    //error handling
                }

                //get ambient
                result = Double.TryParse(ambientR.Text, out ambient[0]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(ambientG.Text, out ambient[1]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(ambientB.Text, out ambient[2]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(ambientA.Text, out ambient[3]);
                if (!result)
                {
                    //error handling
                }



                //get diffuse

                result = Double.TryParse(diffuseR.Text, out diffuse[0]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(diffuseG.Text, out diffuse[1]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(diffuseB.Text, out diffuse[2]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(diffuseA.Text, out diffuse[3]);
                if (!result)
                {
                    //error handling
                }
                scene.buildPointLight(ambient,diffuse,position,name);
            }
            else if(spotradio.Checked)
            {
                double[] direction = new double[3];
                double[] position = new double[3];
                double[] ambient = new double[4];
                double[] diffuse = new double[4];

                //get direction
                bool result = Double.TryParse(lightdirectionX.Text, out direction[0]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(lightdirectionY.Text, out direction[1]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(lightdirectionZ.Text, out direction[2]);
                if (!result)
                {
                    //error handling
                }

                //get position
                result = Double.TryParse(lightpositionX.Text, out position[0]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(lightpositionY.Text, out position[1]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(lightpositionZ.Text, out position[2]);
                if (!result)
                {
                    //error handling
                }

                //get ambient
                result = Double.TryParse(ambientR.Text, out ambient[0]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(ambientG.Text, out ambient[1]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(ambientB.Text, out ambient[2]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(ambientA.Text, out ambient[3]);
                if (!result)
                {
                    //error handling
                }



                //get diffuse

                result = Double.TryParse(diffuseR.Text, out diffuse[0]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(diffuseG.Text, out diffuse[1]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(diffuseB.Text, out diffuse[2]);
                if (!result)
                {
                    //error handling
                }
                result = Double.TryParse(diffuseA.Text, out ambient[3]);
                if (!result)
                {
                    //error handling
                }
                scene.buildSpotLight(ambient,diffuse,position,direction,name);
            }
            updateLightLists();
            buildlightbackbutton_Click(sender, e);
        }

        private void loadmeshbutton_Click(object sender, EventArgs e)
        {
            scene.loadMesh(meshfilepath.Text);
            updateMeshLists();
        }

        private void deletemeshbutton_Click(object sender, EventArgs e)
        {
            
            scene.deleteMesh(currentmeshlist.SelectedIndex);
            updateMeshLists();
        }

        private void loadtexturebutton_Click(object sender, EventArgs e)
        {
            if (diffuseradio.Checked)
                scene.loadDiffuse(texturefilepath.Text);
            else if (normalradio.Checked)
                scene.loadNormal(texturefilepath.Text);
            else if (specularradio.Checked)
                scene.loadSpecular(texturefilepath.Text);
            else if (multiplyradio.Checked)
                scene.loadMultiply(texturefilepath.Text);
            updateTextureLists();
        }

        private void deletetexturebutton_Click(object sender, EventArgs e)
        {
            scene.deleteTexture(currenttextureslist.SelectedIndex);
            updateTextureLists();
        }

        private void loadshaderbutton_Click(object sender, EventArgs e)
        {
            if (pixelshaderradio.Checked)
                scene.loadPixelShader(shaderfilepath.Text);
            else if (vertexshaderradio.Checked)
                scene.loadVertexShader(shaderfilepath.Text);
            updateShaderLists();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            scene.deleteShader(currentshaderlist.SelectedIndex);
            updateShaderLists();
        }


        private void mainscreen_Load(object sender, EventArgs e)
        {

        }

        private void directionalradio_CheckedChanged(object sender, EventArgs e)
        {
            if(directionalradio.Checked) //prevent what we can't use right now
            {
                lightpositionX.Text = "";
                lightpositionX.ReadOnly = true;
                lightpositionY.Text = "";
                lightpositionY.ReadOnly = true;
                lightpositionZ.Text = "";
                lightpositionZ.ReadOnly = true;
                lightdirectionW.Text = "";
                lightdirectionW.ReadOnly = true;
                lightdirectionW.Visible = false;
                lightpositionX.BackColor = Color.DarkGray;
                lightpositionY.BackColor = Color.DarkGray;
                lightpositionZ.BackColor = Color.DarkGray;

            }
            else //reset everything to normal
            {
                lightpositionX.ReadOnly = false;
                lightpositionY.ReadOnly = false;
                lightpositionZ.ReadOnly = false;
                lightdirectionW.ReadOnly = false;
                lightpositionX.BackColor = Color.White;
                lightpositionY.BackColor = Color.White;
                lightpositionZ.BackColor = Color.White;
                lightdirectionW.Visible = true;

            }
        }

        private void pointradio_CheckedChanged(object sender, EventArgs e)
        {
            if (pointradio.Checked) //prevent what we can't use right now
            {
                lightdirectionX.Text = "";
                lightdirectionX.ReadOnly = true;
                lightdirectionY.Text = "";
                lightdirectionY.ReadOnly = true;
                lightdirectionZ.Text = "";
                lightdirectionZ.ReadOnly = true;
                lightdirectionW.Text = "";
                lightdirectionW.ReadOnly = true;
                lightdirectionW.Visible = false;
                lightdirectionX.BackColor = Color.DarkGray;
                lightdirectionY.BackColor = Color.DarkGray;
                lightdirectionZ.BackColor = Color.DarkGray;

            }
            else //reset everything to normal
            {
                lightdirectionX.ReadOnly = false;
                lightdirectionY.ReadOnly = false;
                lightdirectionZ.ReadOnly = false;
                lightdirectionW.ReadOnly = false;
                lightdirectionX.BackColor = Color.White;
                lightdirectionY.BackColor = Color.White;
                lightdirectionZ.BackColor = Color.White;
                lightdirectionW.Visible = true;

            }
        }

        private void materialbackbutton_Click(object sender, EventArgs e)
        {
            materialpanel.Visible = true;
            creatematerialpanel.Visible = false;
        }

        private void updateMaterialLists()
        {
            materialslist.Items.Clear();
            materialslist.Items.AddRange(scene.getMaterialNameArray());
            objectmateriallist.Items.Clear();
            objectmateriallist.Items.AddRange(scene.getMaterialNameArray());
        }
        private void updateObjectLists()
        {
            objectlist.Items.Clear();
            objectlist.Items.AddRange(scene.getObjectNameArray());
        }
        private void updateShaderLists()
        {
            materialpshaderlist.Items.Clear();
            materialpshaderlist.Items.AddRange(scene.getPixelShaderArray());

            materialvshaderlist.Items.Clear();
            materialvshaderlist.Items.AddRange(scene.getVertexShaderArray());

            currentshaderlist.Items.Clear();
            currentshaderlist.Items.AddRange(scene.getShaderArray());
        }
        private void updateTextureLists()
        {
            materialdiffuselist.Items.Clear();
            materialspecularlist.Items.Clear();
            materialnormallist.Items.Clear();
            materialmultiplylist.Items.Clear();
            materialmultiplylist.Items.AddRange(scene.getMultiplyTextureArray());
            materialdiffuselist.Items.AddRange(scene.getDiffuseTextureArray());
            materialspecularlist.Items.AddRange(scene.getSpecularTextureArray());
            materialnormallist.Items.AddRange(scene.getNormalTextureArray());

            currenttextureslist.Items.Clear();
            currenttextureslist.Items.AddRange(scene.getTextureArray());
        }
        private void updateMeshLists()
        {
            currentmeshlist.Items.Clear();
            currentmeshlist.Items.AddRange(scene.getMeshArray());
            objectmeshlist.Items.Clear();
            objectmeshlist.Items.AddRange(scene.getMeshArray());
        }
        private void updateLightLists()
        {
            currentlightslist.Items.Clear();
            currentlightslist.Items.AddRange(scene.getLightArray());
        }

        private void buildmaterialbutton_Click(object sender, EventArgs e)
        {
            int diffuseindex, multindex, specindex, normalindex, vshaderindex, pshaderindex;
            string name;
            name = materialname.Text;
            diffuseindex = materialdiffuselist.SelectedIndex;
            multindex = materialmultiplylist.SelectedIndex;
            specindex = materialspecularlist.SelectedIndex;
            normalindex = materialnormallist.SelectedIndex;
            vshaderindex = materialvshaderlist.SelectedIndex;
            pshaderindex = materialpshaderlist.SelectedIndex;

            scene.buildMaterial(diffuseindex, normalindex, specindex, multindex, pshaderindex, vshaderindex, name);
            updateMaterialLists();
            materialbackbutton_Click(sender, e);
        }

        private void loadscenebutton_Click(object sender, EventArgs e)
        {
            
        }

        private void deletelightbutton_Click(object sender, EventArgs e)
        {
            scene.deleteLight(currentlightslist.SelectedIndex);
            updateLightLists();
        }

        private void editlightbutton_Click(object sender, EventArgs e)
        {
            lightpanel.Visible = false;
            buildlightpanel.Visible = true;
            buildlightbutton.Visible = false;
            savelightchangesbutton.Visible = true;
            if (scene.isDirectionalLight(currentlightslist.SelectedIndex))
            {
                directionalradio.Checked = true;
                spotradio.Enabled = false;
                pointradio.Enabled = false;
            }
            else if (scene.isPointLight(currentlightslist.SelectedIndex))
            {
                pointradio.Checked = true;
                directionalradio.Enabled = false;
                spotradio.Enabled = false;
            }
            else
            {
                spotradio.Checked = true;
                pointradio.Enabled = false;
                directionalradio.Enabled = false;
            }

            string[] lightData = scene.getLightData(currentlightslist.SelectedIndex);
            ambientR.Text = lightData[0];
            ambientG.Text = lightData[1];
            ambientB.Text = lightData[2];
            ambientA.Text = lightData[3];
            diffuseR.Text = lightData[4];
            diffuseG.Text = lightData[5];
            diffuseB.Text = lightData[6];
            diffuseA.Text = lightData[7];
            lightpositionX.Text = lightData[8];
            lightpositionY.Text = lightData[9];
            lightpositionZ.Text = lightData[10];
            lightdirectionX.Text = lightData[11];
            lightdirectionY.Text = lightData[12];
            lightdirectionZ.Text = lightData[13];
            lightdirectionW.Text = lightData[14];
            lightname.Text = lightData[15];

        }

        private void savelightchangesbutton_Click(object sender, EventArgs e)
        {
            double[] direction = new double[3];
            double[] position = new double[3];
            double[] ambient = new double[4];
            double[] diffuse = new double[4];

            //get direction
            bool result = Double.TryParse(lightdirectionX.Text, out direction[0]);
            if (!result)
            {
                //error handling
            }
            result = Double.TryParse(lightdirectionY.Text, out direction[1]);
            if (!result)
            {
                //error handling
            }
            result = Double.TryParse(lightdirectionZ.Text, out direction[2]);
            if (!result)
            {
                //error handling
            }

            //get position
            result = Double.TryParse(lightpositionX.Text, out position[0]);
            if (!result)
            {
                //error handling
            }
            result = Double.TryParse(lightpositionY.Text, out position[1]);
            if (!result)
            {
                //error handling
            }
            result = Double.TryParse(lightpositionZ.Text, out position[2]);
            if (!result)
            {
                //error handling
            }

            //get ambient
            result = Double.TryParse(ambientR.Text, out ambient[0]);
            if (!result)
            {
                //error handling
            }
            result = Double.TryParse(ambientG.Text, out ambient[1]);
            if (!result)
            {
                //error handling
            }
            result = Double.TryParse(ambientB.Text, out ambient[2]);
            if (!result)
            {
                //error handling
            }
            result = Double.TryParse(ambientA.Text, out ambient[3]);
            if (!result)
            {
                //error handling
            }



            //get diffuse

            result = Double.TryParse(diffuseR.Text, out diffuse[0]);
            if (!result)
            {
                //error handling
            }
            result = Double.TryParse(diffuseG.Text, out diffuse[1]);
            if (!result)
            {
                //error handling
            }
            result = Double.TryParse(diffuseB.Text, out diffuse[2]);
            if (!result)
            {
                //error handling
            }
            result = Double.TryParse(diffuseA.Text, out diffuse[3]);
            if (!result)
            {
                //error handling
            }
            scene.editLight(currentlightslist.SelectedIndex,ambient, diffuse, position, direction, lightname.Text);

            //clean up the mess we made
            
            updateLightLists();
            buildlightbackbutton_Click(sender, e); //reset all our values
        }

        private void addmaterialbutton_Click(object sender, EventArgs e)
        {
            materialpanel.Visible = false;
            creatematerialpanel.Visible = true;
        }

        private void deletematerialbutton_Click(object sender, EventArgs e)
        {
            scene.deleteMaterial(materialslist.SelectedIndex);
            updateMaterialLists();
        }

        private void editmaterialbutton_Click(object sender, EventArgs e)
        {
            materialpanel.Visible = false;
            creatematerialpanel.Visible = true;
            buildmaterialbutton.Visible = false;
            materialsavechangesbutton.Visible = true;
            string name;
            int[] matData = scene.getMaterialData(materialslist.SelectedIndex, out name);
            materialname.Text = name;
            
        }

       

        private void objectbackbutton_Click(object sender, EventArgs e)
        {
            objectpanel.Visible = true;
            createobjectpanel.Visible = false;
            objectmeshlist.ClearSelected();
            objectmateriallist.ClearSelected();
            objectname.Text = "";
            
        }

        private void objectsavechangesbutton_Click(object sender, EventArgs e)
        {
            scene.editObject(objectlist.SelectedIndex, objectmeshlist.SelectedIndex,
                objectmateriallist.SelectedIndex, objectname.Text);
            objectsavechangesbutton.Visible = false;
            buildobjectbutton.Visible = true;
            objectbackbutton_Click(sender, e);
            updateObjectLists();
        }

        private void buildobjectbutton_Click(object sender, EventArgs e)
        {
            scene.buildObject(objectmeshlist.SelectedIndex, objectmateriallist.SelectedIndex, objectname.Text);
            updateObjectLists();
            objectbackbutton_Click(sender, e);
        }

        private void materialsavechangesbutton_Click(object sender, EventArgs e)
        {
            scene.editMaterial(materialslist.SelectedIndex, materialdiffuselist.SelectedIndex,
                  materialspecularlist.SelectedIndex, materialmultiplylist.SelectedIndex,
                  materialnormallist.SelectedIndex,materialpshaderlist.SelectedIndex,
                  materialvshaderlist.SelectedIndex, materialname.Text);
            updateMaterialLists();
            materialsavechangesbutton.Visible = false;
            buildmaterialbutton.Visible = true;
            materialbackbutton_Click(sender, e);
        }

        private void createobjectbutton_Click(object sender, EventArgs e)
        {
            createobjectpanel.Visible = true;
            objectpanel.Visible = false;
        }
    }
}
