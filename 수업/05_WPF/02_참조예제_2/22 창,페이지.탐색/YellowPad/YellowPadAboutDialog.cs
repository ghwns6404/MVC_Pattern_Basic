//-----------------------------------------------------
// YellowPadAboutDialog.cs (c) 2006 by Charles Petzold
//-----------------------------------------------------
using System;
using System.Diagnostics;           // Process class를 위한 NameSpace.
using System.IO;
using System.Windows;
using System.Windows.Media;
using System.Windows.Markup;
using System.Windows.Navigation;    //RequestNavigateEventArgs를 위한 NameSpace.

namespace Petzold.YellowPad
{
    public partial class YellowPadAboutDialog
    {
        public YellowPadAboutDialog()
        {
            InitializeComponent();

            // 저작관/서명 관련 Drawing 객체를 읽은후 Image 엘리먼트에 설정.
            Uri uri = new Uri("pack://application:,,,/Images/Signature.xaml");
            Stream stream = Application.GetResourceStream(uri).Stream;
            Drawing drawing = (Drawing)XamlReader.Load(stream);
            stream.Close();

            imgSignature.Source = new DrawingImage(drawing);
        }
        // 하이퍼 링크가 클릭되면 필자의 웹사이트로 이동.
        void LinkOnRequestNavigate(object sender, RequestNavigateEventArgs args)
        {
            Process.Start(args.Uri.OriginalString);
            args.Handled = true;
        }
    }
}
