using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _0422_Form
{
    public partial class Form2_Color2_Sub : Form
    {
        public string TextLabel { get; set; }

        public Form2_Color2_Sub(string msg)
        {            
            InitializeComponent();
            this.Text = msg;
            SetStyle(ControlStyles.SupportsTransparentBackColor, true);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            Graphics g =  e.Graphics;
            g.DrawString(TextLabel, this.Font, new SolidBrush(Color.Black), 10, 7);
        }
    }
}
