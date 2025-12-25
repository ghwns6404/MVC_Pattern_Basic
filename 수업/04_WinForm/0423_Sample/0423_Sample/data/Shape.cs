using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0423_Sample
{
    public class Shape
    {
        public Point ShapePoint { get; set; }
        public Color ShapeColor { get; set; }
        public int ShapeIndex   { get; set; } 
        public int ShapeWidth   { get; set; }

        public Shape(Point pt, Color color, int idx, int width)
        {
            ShapePoint = pt;
            ShapeColor = color;
            ShapeIndex = idx;
            ShapeWidth = width;
        }

        public override string ToString()
        {
            return string.Format($"{ShapePoint}, {ShapeColor}, {ShapeIndex}, {ShapeWidth}");
        }
    }
}
