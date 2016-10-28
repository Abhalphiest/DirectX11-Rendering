namespace SceneTool
{
    partial class startscreen
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
            this.newscene = new System.Windows.Forms.Button();
            this.editscene = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // newscene
            // 
            this.newscene.Location = new System.Drawing.Point(178, 82);
            this.newscene.Name = "newscene";
            this.newscene.Size = new System.Drawing.Size(169, 23);
            this.newscene.TabIndex = 0;
            this.newscene.Text = "New Scene";
            this.newscene.UseVisualStyleBackColor = true;
            // 
            // editscene
            // 
            this.editscene.Location = new System.Drawing.Point(178, 143);
            this.editscene.Name = "editscene";
            this.editscene.Size = new System.Drawing.Size(169, 23);
            this.editscene.TabIndex = 1;
            this.editscene.Text = "Edit Existing Scene";
            this.editscene.UseVisualStyleBackColor = true;
            // 
            // startscreen
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(509, 272);
            this.Controls.Add(this.editscene);
            this.Controls.Add(this.newscene);
            this.Name = "startscreen";
            this.Text = "Scene Tool";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button newscene;
        private System.Windows.Forms.Button editscene;
    }
}

