//-----------------------------------------------------
// XamlCruncherSettings.cs (c) 2006 by Charles Petzold
//-----------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace Petzold.XamlCruncher
{
    public class XamlCruncherSettings : Petzold.NotepadClone.NotepadCloneSettings
    {
        // Default settings of user preferences.
        public Dock Orientation = Dock.Left;//Orientation : TextBox와 Frame 의 위치 제어를 한다 
        public int TabSpaces = 4;   //텝키를 누를때 몇개의 공백을 삽입하는가를 지정한다 
        public string StartupDocument = //처음 프로그램을 실행했을때 File매뉴의 New를 선택했을시 XAML을 설정한다 
            "<Button xmlns=\"http://schemas.microsoft.com/winfx" + 
                        "/2006/xaml/presentation\"\r\n" +
            "        xmlns:x=\"http://schemas.microsoft.com/winfx" + 
                        "/2006/xaml\">\r\n" +
                        
            "    Hello, XAML!\r\n" +
            "</Button>\n";

        // Constructor to initialize default settings in NotepadCloneSettings.
        public XamlCruncherSettings()
        {
            FontFamily = "Lucida Console";//기본폰트를 10포인트의 Lucida Console 고정간격 폰트로 바꾼다  (기본폰트 설정)
            FontSize = 10 / 0.75;
        }
    }
}