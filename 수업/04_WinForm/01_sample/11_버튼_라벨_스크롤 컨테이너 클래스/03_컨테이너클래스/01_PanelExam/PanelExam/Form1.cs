using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace PanelExam
{
    public partial class Form1 : Form
    {
        int index_color = 0;
        int index_border = 0;
        public Form1()
        {
            InitializeComponent();
        }

        private void button11_Click(object sender, EventArgs e)
        {
            // Panel 바탕색 바꾸기
            if(index_color == 0)
            {
                this.panel.BackColor = Color.Red;
                this.panel.BackgroundImage = null;
            }
            else if(index_color == 1)
            {
                this.panel.BackColor = Color.Yellow;
            }
            else if (index_color == 2)
            {
                this.panel.BackColor = Color.Blue;
            }
            else if (index_color == 3)
            {
                this.panel.BackgroundImage = Image.FromFile("회벽.bmp");
            }

            if(index_color > 3)
            {
                index_color = 0;                
            }
            else
            {
                index_color++;
            }            
        }

        private void button12_Click(object sender, EventArgs e)
        {
            // Panel BoarderStyle 바꾸기
            if(index_border == 0)
            {
                this.panel.BorderStyle = BorderStyle.None;
            }
            else if(index_border == 1)
            {
                this.panel.BorderStyle = BorderStyle.FixedSingle;
            }
            else if(index_border == 2)
            {
                this.panel.BorderStyle = BorderStyle.Fixed3D;
            }

            if (index_border > 2)
            {
                index_border = 0;
            }
            else
            {
                index_border++;
            }     
        }
    }
}