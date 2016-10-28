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
        const int NUM_CELLS = 80;
        const int CELL_SIZE = 5;
        public mainscreen()
        {
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

    }
}
