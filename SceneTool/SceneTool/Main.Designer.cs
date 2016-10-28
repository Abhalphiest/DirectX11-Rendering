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
            this.addmesh = new System.Windows.Forms.Button();
            this.addtexture = new System.Windows.Forms.Button();
            this.creatematerial = new System.Windows.Forms.Button();
            this.createobject = new System.Windows.Forms.Button();
            this.objectlist = new System.Windows.Forms.ListBox();
            this.createlight = new System.Windows.Forms.Button();
            this.addshader = new System.Windows.Forms.Button();
            this.addlabel = new System.Windows.Forms.Label();
            this.startpanel = new System.Windows.Forms.Panel();
            this.newscene = new System.Windows.Forms.Button();
            this.mainpanel.SuspendLayout();
            this.startpanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // scenewindow
            // 
            this.scenewindow.Location = new System.Drawing.Point(12, 12);
            this.scenewindow.Name = "scenewindow";
            this.scenewindow.Size = new System.Drawing.Size(400, 335);
            this.scenewindow.TabIndex = 0;
            this.scenewindow.Visible = false;
            this.scenewindow.Paint += new System.Windows.Forms.PaintEventHandler(this.scenewindow_Paint);
            // 
            // mainpanel
            // 
            this.mainpanel.Controls.Add(this.addlabel);
            this.mainpanel.Controls.Add(this.addshader);
            this.mainpanel.Controls.Add(this.createlight);
            this.mainpanel.Controls.Add(this.objectlist);
            this.mainpanel.Controls.Add(this.createobject);
            this.mainpanel.Controls.Add(this.creatematerial);
            this.mainpanel.Controls.Add(this.addtexture);
            this.mainpanel.Controls.Add(this.addmesh);
            this.mainpanel.Location = new System.Drawing.Point(433, 12);
            this.mainpanel.Name = "mainpanel";
            this.mainpanel.Size = new System.Drawing.Size(196, 336);
            this.mainpanel.TabIndex = 1;
            this.mainpanel.Visible = false;
            // 
            // addmesh
            // 
            this.addmesh.Location = new System.Drawing.Point(44, 8);
            this.addmesh.Name = "addmesh";
            this.addmesh.Size = new System.Drawing.Size(110, 23);
            this.addmesh.TabIndex = 0;
            this.addmesh.Text = "Add Mesh";
            this.addmesh.UseVisualStyleBackColor = true;
            // 
            // addtexture
            // 
            this.addtexture.Location = new System.Drawing.Point(44, 66);
            this.addtexture.Name = "addtexture";
            this.addtexture.Size = new System.Drawing.Size(110, 23);
            this.addtexture.TabIndex = 1;
            this.addtexture.Text = "Add Texture";
            this.addtexture.UseVisualStyleBackColor = true;
            // 
            // creatematerial
            // 
            this.creatematerial.Location = new System.Drawing.Point(44, 95);
            this.creatematerial.Name = "creatematerial";
            this.creatematerial.Size = new System.Drawing.Size(110, 22);
            this.creatematerial.TabIndex = 2;
            this.creatematerial.Text = "Create Material";
            this.creatematerial.UseVisualStyleBackColor = true;
            // 
            // createobject
            // 
            this.createobject.Location = new System.Drawing.Point(44, 123);
            this.createobject.Name = "createobject";
            this.createobject.Size = new System.Drawing.Size(110, 23);
            this.createobject.TabIndex = 3;
            this.createobject.Text = "Create Object";
            this.createobject.UseVisualStyleBackColor = true;
            // 
            // objectlist
            // 
            this.objectlist.FormattingEnabled = true;
            this.objectlist.Location = new System.Drawing.Point(32, 211);
            this.objectlist.Name = "objectlist";
            this.objectlist.Size = new System.Drawing.Size(132, 108);
            this.objectlist.TabIndex = 4;
            // 
            // createlight
            // 
            this.createlight.Location = new System.Drawing.Point(44, 152);
            this.createlight.Name = "createlight";
            this.createlight.Size = new System.Drawing.Size(110, 23);
            this.createlight.TabIndex = 5;
            this.createlight.Text = "Create Light";
            this.createlight.UseVisualStyleBackColor = true;
            // 
            // addshader
            // 
            this.addshader.Location = new System.Drawing.Point(44, 37);
            this.addshader.Name = "addshader";
            this.addshader.Size = new System.Drawing.Size(110, 23);
            this.addshader.TabIndex = 6;
            this.addshader.Text = "Add Shader";
            this.addshader.UseVisualStyleBackColor = true;
            // 
            // addlabel
            // 
            this.addlabel.AutoSize = true;
            this.addlabel.Location = new System.Drawing.Point(32, 192);
            this.addlabel.Name = "addlabel";
            this.addlabel.Size = new System.Drawing.Size(72, 13);
            this.addlabel.TabIndex = 7;
            this.addlabel.Text = "Add to Scene";
            // 
            // startpanel
            // 
            this.startpanel.Controls.Add(this.newscene);
            this.startpanel.Location = new System.Drawing.Point(102, 23);
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
            // mainscreen
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(641, 360);
            this.Controls.Add(this.startpanel);
            this.Controls.Add(this.scenewindow);
            this.Controls.Add(this.mainpanel);
            this.Name = "mainscreen";
            this.Text = "Scene Tool";
            this.mainpanel.ResumeLayout(false);
            this.mainpanel.PerformLayout();
            this.startpanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel scenewindow;
        private System.Windows.Forms.Panel mainpanel;
        private System.Windows.Forms.Button addshader;
        private System.Windows.Forms.Button createlight;
        private System.Windows.Forms.ListBox objectlist;
        private System.Windows.Forms.Button createobject;
        private System.Windows.Forms.Button creatematerial;
        private System.Windows.Forms.Button addtexture;
        private System.Windows.Forms.Button addmesh;
        private System.Windows.Forms.Label addlabel;
        private System.Windows.Forms.Panel startpanel;
        private System.Windows.Forms.Button newscene;
    }
}