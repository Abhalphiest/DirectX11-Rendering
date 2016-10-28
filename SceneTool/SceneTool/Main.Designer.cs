namespace SceneTool
{
    partial class mainscreen
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.scenewindow = new System.Windows.Forms.Panel();
            this.mainpanel = new System.Windows.Forms.Panel();
            this.objectlistlabel = new System.Windows.Forms.Label();
            this.objectlist = new System.Windows.Forms.ListBox();
            this.startpanel = new System.Windows.Forms.Panel();
            this.newscene = new System.Windows.Forms.Button();
            this.meshfilepathlabel = new System.Windows.Forms.Label();
            this.meshnamelabel = new System.Windows.Forms.Label();
            this.meshname = new System.Windows.Forms.TextBox();
            this.meshfilepath = new System.Windows.Forms.TextBox();
            this.loadmeshbutton = new System.Windows.Forms.Button();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.objecttab = new System.Windows.Forms.TabPage();
            this.meshtab = new System.Windows.Forms.TabPage();
            this.texturetab = new System.Windows.Forms.TabPage();
            this.shadertab = new System.Windows.Forms.TabPage();
            this.lighttab = new System.Windows.Forms.TabPage();
            this.loadscenebutton = new System.Windows.Forms.Button();
            this.createobjectbutton = new System.Windows.Forms.Button();
            this.propertiestab = new System.Windows.Forms.TabPage();
            this.button1 = new System.Windows.Forms.Button();
            this.editobjectbutton = new System.Windows.Forms.Button();
            this.addobjectbutton = new System.Windows.Forms.Button();
            this.deleteobjectbutton = new System.Windows.Forms.Button();
            this.materialstab = new System.Windows.Forms.TabPage();
            this.button2 = new System.Windows.Forms.Button();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.label1 = new System.Windows.Forms.Label();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.texturename = new System.Windows.Forms.TextBox();
            this.texturenamelabel = new System.Windows.Forms.Label();
            this.loadtexturebutton = new System.Windows.Forms.Button();
            this.texturefilepathlabel = new System.Windows.Forms.Label();
            this.texturefilepath = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.button5 = new System.Windows.Forms.Button();
            this.listBox2 = new System.Windows.Forms.ListBox();
            this.label3 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.button6 = new System.Windows.Forms.Button();
            this.button7 = new System.Windows.Forms.Button();
            this.button8 = new System.Windows.Forms.Button();
            this.mainpanel.SuspendLayout();
            this.startpanel.SuspendLayout();
            this.tabControl.SuspendLayout();
            this.objecttab.SuspendLayout();
            this.meshtab.SuspendLayout();
            this.texturetab.SuspendLayout();
            this.shadertab.SuspendLayout();
            this.lighttab.SuspendLayout();
            this.materialstab.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // scenewindow
            // 
            this.scenewindow.Location = new System.Drawing.Point(25, 21);
            this.scenewindow.Name = "scenewindow";
            this.scenewindow.Size = new System.Drawing.Size(519, 335);
            this.scenewindow.TabIndex = 0;
            this.scenewindow.Visible = false;
            this.scenewindow.Paint += new System.Windows.Forms.PaintEventHandler(this.scenewindow_Paint);
            // 
            // mainpanel
            // 
            this.mainpanel.Controls.Add(this.deleteobjectbutton);
            this.mainpanel.Controls.Add(this.addobjectbutton);
            this.mainpanel.Controls.Add(this.editobjectbutton);
            this.mainpanel.Controls.Add(this.button1);
            this.mainpanel.Controls.Add(this.createobjectbutton);
            this.mainpanel.Controls.Add(this.objectlistlabel);
            this.mainpanel.Controls.Add(this.objectlist);
            this.mainpanel.Location = new System.Drawing.Point(6, 6);
            this.mainpanel.Name = "mainpanel";
            this.mainpanel.Size = new System.Drawing.Size(215, 298);
            this.mainpanel.TabIndex = 1;
            this.mainpanel.Visible = false;
            // 
            // objectlistlabel
            // 
            this.objectlistlabel.AutoSize = true;
            this.objectlistlabel.Location = new System.Drawing.Point(14, 58);
            this.objectlistlabel.Name = "objectlistlabel";
            this.objectlistlabel.Size = new System.Drawing.Size(80, 13);
            this.objectlistlabel.TabIndex = 7;
            this.objectlistlabel.Text = "Current Objects";
            // 
            // objectlist
            // 
            this.objectlist.FormattingEnabled = true;
            this.objectlist.Location = new System.Drawing.Point(30, 74);
            this.objectlist.Name = "objectlist";
            this.objectlist.Size = new System.Drawing.Size(132, 108);
            this.objectlist.TabIndex = 4;
            // 
            // startpanel
            // 
            this.startpanel.Controls.Add(this.loadscenebutton);
            this.startpanel.Controls.Add(this.newscene);
            this.startpanel.Location = new System.Drawing.Point(206, 38);
            this.startpanel.Name = "startpanel";
            this.startpanel.Size = new System.Drawing.Size(423, 283);
            this.startpanel.TabIndex = 0;
            // 
            // newscene
            // 
            this.newscene.Location = new System.Drawing.Point(148, 72);
            this.newscene.Name = "newscene";
            this.newscene.Size = new System.Drawing.Size(135, 23);
            this.newscene.TabIndex = 0;
            this.newscene.Text = "New Scene";
            this.newscene.UseVisualStyleBackColor = true;
            this.newscene.Click += new System.EventHandler(this.newscene_Click);
            // 
            // meshfilepathlabel
            // 
            this.meshfilepathlabel.AutoSize = true;
            this.meshfilepathlabel.Location = new System.Drawing.Point(23, 46);
            this.meshfilepathlabel.Name = "meshfilepathlabel";
            this.meshfilepathlabel.Size = new System.Drawing.Size(73, 13);
            this.meshfilepathlabel.TabIndex = 4;
            this.meshfilepathlabel.Text = "Mesh Filepath";
            // 
            // meshnamelabel
            // 
            this.meshnamelabel.AutoSize = true;
            this.meshnamelabel.Location = new System.Drawing.Point(20, 104);
            this.meshnamelabel.Name = "meshnamelabel";
            this.meshnamelabel.Size = new System.Drawing.Size(64, 13);
            this.meshnamelabel.TabIndex = 3;
            this.meshnamelabel.Text = "Mesh Name";
            // 
            // meshname
            // 
            this.meshname.Location = new System.Drawing.Point(23, 125);
            this.meshname.Name = "meshname";
            this.meshname.Size = new System.Drawing.Size(100, 20);
            this.meshname.TabIndex = 2;
            // 
            // meshfilepath
            // 
            this.meshfilepath.Location = new System.Drawing.Point(23, 66);
            this.meshfilepath.Name = "meshfilepath";
            this.meshfilepath.Size = new System.Drawing.Size(100, 20);
            this.meshfilepath.TabIndex = 1;
            // 
            // loadmeshbutton
            // 
            this.loadmeshbutton.Location = new System.Drawing.Point(23, 209);
            this.loadmeshbutton.Name = "loadmeshbutton";
            this.loadmeshbutton.Size = new System.Drawing.Size(75, 23);
            this.loadmeshbutton.TabIndex = 0;
            this.loadmeshbutton.Text = "Load Mesh";
            this.loadmeshbutton.UseVisualStyleBackColor = true;
            this.loadmeshbutton.Click += new System.EventHandler(this.loadmeshbutton_Click);
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.objecttab);
            this.tabControl.Controls.Add(this.materialstab);
            this.tabControl.Controls.Add(this.meshtab);
            this.tabControl.Controls.Add(this.texturetab);
            this.tabControl.Controls.Add(this.shadertab);
            this.tabControl.Controls.Add(this.lighttab);
            this.tabControl.Controls.Add(this.propertiestab);
            this.tabControl.Location = new System.Drawing.Point(546, 12);
            this.tabControl.Multiline = true;
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(253, 357);
            this.tabControl.TabIndex = 8;
            // 
            // objecttab
            // 
            this.objecttab.Controls.Add(this.mainpanel);
            this.objecttab.Location = new System.Drawing.Point(4, 40);
            this.objecttab.Name = "objecttab";
            this.objecttab.Padding = new System.Windows.Forms.Padding(3);
            this.objecttab.Size = new System.Drawing.Size(245, 313);
            this.objecttab.TabIndex = 0;
            this.objecttab.Text = "Objects";
            this.objecttab.UseVisualStyleBackColor = true;
            // 
            // meshtab
            // 
            this.meshtab.Controls.Add(this.meshfilepathlabel);
            this.meshtab.Controls.Add(this.meshnamelabel);
            this.meshtab.Controls.Add(this.meshname);
            this.meshtab.Controls.Add(this.meshfilepath);
            this.meshtab.Controls.Add(this.loadmeshbutton);
            this.meshtab.Location = new System.Drawing.Point(4, 40);
            this.meshtab.Name = "meshtab";
            this.meshtab.Padding = new System.Windows.Forms.Padding(3);
            this.meshtab.Size = new System.Drawing.Size(245, 313);
            this.meshtab.TabIndex = 1;
            this.meshtab.Text = "Add Mesh";
            this.meshtab.UseVisualStyleBackColor = true;
            // 
            // texturetab
            // 
            this.texturetab.Controls.Add(this.loadtexturebutton);
            this.texturetab.Controls.Add(this.texturenamelabel);
            this.texturetab.Controls.Add(this.texturename);
            this.texturetab.Controls.Add(this.texturefilepath);
            this.texturetab.Controls.Add(this.texturefilepathlabel);
            this.texturetab.Location = new System.Drawing.Point(4, 40);
            this.texturetab.Name = "texturetab";
            this.texturetab.Size = new System.Drawing.Size(245, 313);
            this.texturetab.TabIndex = 2;
            this.texturetab.Text = "Add Texture";
            this.texturetab.UseVisualStyleBackColor = true;
            // 
            // shadertab
            // 
            this.shadertab.Controls.Add(this.button5);
            this.shadertab.Controls.Add(this.radioButton1);
            this.shadertab.Controls.Add(this.textBox1);
            this.shadertab.Controls.Add(this.label2);
            this.shadertab.Location = new System.Drawing.Point(4, 40);
            this.shadertab.Name = "shadertab";
            this.shadertab.Size = new System.Drawing.Size(245, 313);
            this.shadertab.TabIndex = 3;
            this.shadertab.Text = "Add Shader";
            this.shadertab.UseVisualStyleBackColor = true;
            // 
            // lighttab
            // 
            this.lighttab.Controls.Add(this.panel1);
            this.lighttab.Location = new System.Drawing.Point(4, 40);
            this.lighttab.Name = "lighttab";
            this.lighttab.Size = new System.Drawing.Size(245, 313);
            this.lighttab.TabIndex = 4;
            this.lighttab.Text = "Lights";
            this.lighttab.UseVisualStyleBackColor = true;
            // 
            // loadscenebutton
            // 
            this.loadscenebutton.Location = new System.Drawing.Point(148, 130);
            this.loadscenebutton.Name = "loadscenebutton";
            this.loadscenebutton.Size = new System.Drawing.Size(135, 23);
            this.loadscenebutton.TabIndex = 1;
            this.loadscenebutton.Text = "Edit Existing Scene";
            this.loadscenebutton.UseVisualStyleBackColor = true;
            // 
            // createobjectbutton
            // 
            this.createobjectbutton.Location = new System.Drawing.Point(30, 18);
            this.createobjectbutton.Name = "createobjectbutton";
            this.createobjectbutton.Size = new System.Drawing.Size(106, 23);
            this.createobjectbutton.TabIndex = 8;
            this.createobjectbutton.Text = "Create Object";
            this.createobjectbutton.UseVisualStyleBackColor = true;
            // 
            // propertiestab
            // 
            this.propertiestab.Location = new System.Drawing.Point(4, 58);
            this.propertiestab.Name = "propertiestab";
            this.propertiestab.Padding = new System.Windows.Forms.Padding(3);
            this.propertiestab.Size = new System.Drawing.Size(222, 295);
            this.propertiestab.TabIndex = 5;
            this.propertiestab.Text = "Scene Properties";
            this.propertiestab.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(148, 306);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 9;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // editobjectbutton
            // 
            this.editobjectbutton.Location = new System.Drawing.Point(30, 189);
            this.editobjectbutton.Name = "editobjectbutton";
            this.editobjectbutton.Size = new System.Drawing.Size(132, 23);
            this.editobjectbutton.TabIndex = 10;
            this.editobjectbutton.Text = "Edit Object";
            this.editobjectbutton.UseVisualStyleBackColor = true;
            // 
            // addobjectbutton
            // 
            this.addobjectbutton.Location = new System.Drawing.Point(30, 219);
            this.addobjectbutton.Name = "addobjectbutton";
            this.addobjectbutton.Size = new System.Drawing.Size(132, 23);
            this.addobjectbutton.TabIndex = 11;
            this.addobjectbutton.Text = "Add Object to Scene";
            this.addobjectbutton.UseVisualStyleBackColor = true;
            // 
            // deleteobjectbutton
            // 
            this.deleteobjectbutton.Location = new System.Drawing.Point(30, 249);
            this.deleteobjectbutton.Name = "deleteobjectbutton";
            this.deleteobjectbutton.Size = new System.Drawing.Size(132, 23);
            this.deleteobjectbutton.TabIndex = 12;
            this.deleteobjectbutton.Text = "Delete Object";
            this.deleteobjectbutton.UseVisualStyleBackColor = true;
            // 
            // materialstab
            // 
            this.materialstab.Controls.Add(this.button4);
            this.materialstab.Controls.Add(this.button3);
            this.materialstab.Controls.Add(this.label1);
            this.materialstab.Controls.Add(this.listBox1);
            this.materialstab.Controls.Add(this.button2);
            this.materialstab.Location = new System.Drawing.Point(4, 40);
            this.materialstab.Name = "materialstab";
            this.materialstab.Padding = new System.Windows.Forms.Padding(3);
            this.materialstab.Size = new System.Drawing.Size(245, 313);
            this.materialstab.TabIndex = 6;
            this.materialstab.Text = "Materials";
            this.materialstab.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(7, 168);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 0;
            this.button2.Text = "button2";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // listBox1
            // 
            this.listBox1.FormattingEnabled = true;
            this.listBox1.Location = new System.Drawing.Point(7, 25);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(162, 121);
            this.listBox1.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 7);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "label1";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(6, 197);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 3;
            this.button3.Text = "button3";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(6, 226);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(75, 23);
            this.button4.TabIndex = 4;
            this.button4.Text = "button4";
            this.button4.UseVisualStyleBackColor = true;
            // 
            // texturename
            // 
            this.texturename.Location = new System.Drawing.Point(20, 142);
            this.texturename.Name = "texturename";
            this.texturename.Size = new System.Drawing.Size(100, 20);
            this.texturename.TabIndex = 1;
            // 
            // texturenamelabel
            // 
            this.texturenamelabel.AutoSize = true;
            this.texturenamelabel.Location = new System.Drawing.Point(17, 126);
            this.texturenamelabel.Name = "texturenamelabel";
            this.texturenamelabel.Size = new System.Drawing.Size(74, 13);
            this.texturenamelabel.TabIndex = 2;
            this.texturenamelabel.Text = "Texture Name";
            // 
            // loadtexturebutton
            // 
            this.loadtexturebutton.Location = new System.Drawing.Point(20, 183);
            this.loadtexturebutton.Name = "loadtexturebutton";
            this.loadtexturebutton.Size = new System.Drawing.Size(97, 23);
            this.loadtexturebutton.TabIndex = 4;
            this.loadtexturebutton.Text = "Load Texture";
            this.loadtexturebutton.UseVisualStyleBackColor = true;
            // 
            // texturefilepathlabel
            // 
            this.texturefilepathlabel.AutoSize = true;
            this.texturefilepathlabel.Location = new System.Drawing.Point(17, 63);
            this.texturefilepathlabel.Name = "texturefilepathlabel";
            this.texturefilepathlabel.Size = new System.Drawing.Size(83, 13);
            this.texturefilepathlabel.TabIndex = 3;
            this.texturefilepathlabel.Text = "Texture Filepath";
            // 
            // texturefilepath
            // 
            this.texturefilepath.Location = new System.Drawing.Point(20, 84);
            this.texturefilepath.Name = "texturefilepath";
            this.texturefilepath.Size = new System.Drawing.Size(100, 20);
            this.texturefilepath.TabIndex = 0;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(18, 28);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "label2";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(24, 45);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 20);
            this.textBox1.TabIndex = 2;
            // 
            // radioButton1
            // 
            this.radioButton1.AutoSize = true;
            this.radioButton1.Location = new System.Drawing.Point(21, 90);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(85, 17);
            this.radioButton1.TabIndex = 3;
            this.radioButton1.TabStop = true;
            this.radioButton1.Text = "radioButton1";
            this.radioButton1.UseVisualStyleBackColor = true;
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(24, 138);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(75, 23);
            this.button5.TabIndex = 4;
            this.button5.Text = "button5";
            this.button5.UseVisualStyleBackColor = true;
            // 
            // listBox2
            // 
            this.listBox2.FormattingEnabled = true;
            this.listBox2.Location = new System.Drawing.Point(19, 27);
            this.listBox2.Name = "listBox2";
            this.listBox2.Size = new System.Drawing.Size(156, 95);
            this.listBox2.TabIndex = 0;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(19, 8);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 13);
            this.label3.TabIndex = 1;
            this.label3.Text = "label3";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.button8);
            this.panel1.Controls.Add(this.button7);
            this.panel1.Controls.Add(this.button6);
            this.panel1.Controls.Add(this.listBox2);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Location = new System.Drawing.Point(3, 3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(237, 307);
            this.panel1.TabIndex = 2;
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(19, 143);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(75, 23);
            this.button6.TabIndex = 2;
            this.button6.Text = "button6";
            this.button6.UseVisualStyleBackColor = true;
            // 
            // button7
            // 
            this.button7.Location = new System.Drawing.Point(19, 182);
            this.button7.Name = "button7";
            this.button7.Size = new System.Drawing.Size(75, 23);
            this.button7.TabIndex = 3;
            this.button7.Text = "button7";
            this.button7.UseVisualStyleBackColor = true;
            // 
            // button8
            // 
            this.button8.Location = new System.Drawing.Point(19, 226);
            this.button8.Name = "button8";
            this.button8.Size = new System.Drawing.Size(75, 23);
            this.button8.TabIndex = 4;
            this.button8.Text = "button8";
            this.button8.UseVisualStyleBackColor = true;
            // 
            // mainscreen
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(802, 384);
            this.Controls.Add(this.tabControl);
            this.Controls.Add(this.startpanel);
            this.Controls.Add(this.scenewindow);
            this.Name = "mainscreen";
            this.Text = "Scene Tool";
            this.mainpanel.ResumeLayout(false);
            this.mainpanel.PerformLayout();
            this.startpanel.ResumeLayout(false);
            this.tabControl.ResumeLayout(false);
            this.objecttab.ResumeLayout(false);
            this.meshtab.ResumeLayout(false);
            this.meshtab.PerformLayout();
            this.texturetab.ResumeLayout(false);
            this.texturetab.PerformLayout();
            this.shadertab.ResumeLayout(false);
            this.shadertab.PerformLayout();
            this.lighttab.ResumeLayout(false);
            this.materialstab.ResumeLayout(false);
            this.materialstab.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel scenewindow;
        private System.Windows.Forms.Panel mainpanel;
        private System.Windows.Forms.ListBox objectlist;
        private System.Windows.Forms.Label objectlistlabel;
        private System.Windows.Forms.Panel startpanel;
        private System.Windows.Forms.Button newscene;
        private System.Windows.Forms.Label meshfilepathlabel;
        private System.Windows.Forms.Label meshnamelabel;
        private System.Windows.Forms.TextBox meshname;
        private System.Windows.Forms.TextBox meshfilepath;
        private System.Windows.Forms.Button loadmeshbutton;
        private System.Windows.Forms.Button loadscenebutton;
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage objecttab;
        private System.Windows.Forms.TabPage meshtab;
        private System.Windows.Forms.TabPage texturetab;
        private System.Windows.Forms.TabPage shadertab;
        private System.Windows.Forms.TabPage lighttab;
        private System.Windows.Forms.Button deleteobjectbutton;
        private System.Windows.Forms.Button addobjectbutton;
        private System.Windows.Forms.Button editobjectbutton;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button createobjectbutton;
        private System.Windows.Forms.TabPage propertiestab;
        private System.Windows.Forms.TabPage materialstab;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button loadtexturebutton;
        private System.Windows.Forms.Label texturenamelabel;
        private System.Windows.Forms.TextBox texturename;
        private System.Windows.Forms.TextBox texturefilepath;
        private System.Windows.Forms.Label texturefilepathlabel;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button button8;
        private System.Windows.Forms.Button button7;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.ListBox listBox2;
        private System.Windows.Forms.Label label3;
    }
}