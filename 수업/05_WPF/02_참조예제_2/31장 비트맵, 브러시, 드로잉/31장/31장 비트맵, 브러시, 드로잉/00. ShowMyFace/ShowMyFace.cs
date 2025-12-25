//-------------------------------------------
// ShowMyFace.cs (c) 2006 by Charles Petzold
//-------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace Petzold.ShowMyFace
{
    class ShowMyFace : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new ShowMyFace());
        }
        public ShowMyFace()
        {
            Title = "Show My Face";

            ///******************************************************************
            //  3장의 ShowMyFace 예제
            //******************************************************************/
            Uri uri = new Uri("http://www.charlespetzold.com/PetzoldTattoo.jpg");
            //BitmapImage bitmap = new BitmapImage(uri);
            //Image img = new Image();
            //img.Source = bitmap;
            //Content = img;

            ///******************************************************************
            //  p1245 BitmapImage 코드
            //******************************************************************/
            Image rotated90 = new Image();
            TransformedBitmap tb = new TransformedBitmap();
            FormatConvertedBitmap fb = new FormatConvertedBitmap();
            CroppedBitmap cb = new CroppedBitmap();


            // Create the source to use as the tb source.
            BitmapImage bi = new BitmapImage();
            bi.BeginInit();
            bi.UriSource = uri;
            bi.EndInit();

            //cb.BeginInit();
            //cb.Source = bi;
            //Int32Rect rect = new Int32Rect();
            //rect.X = 220;
            //rect.Y = 200;
            //rect.Width = 120;
            //rect.Height = 80;
            //cb.SourceRect = rect;

            //cb.EndInit();


            fb.BeginInit();
            fb.Source = bi;
            fb.DestinationFormat = PixelFormats.Gray2;
            fb.EndInit();

            // Properties must be set between BeginInit and EndInit calls.
            tb.BeginInit();
            tb.Source = fb;
            // Set image rotation.
            tb.Transform = new RotateTransform(90);
            tb.EndInit();
            // Set the Image source.
            rotated90.Source = tb;
            Content = rotated90;
        }
    }
}
