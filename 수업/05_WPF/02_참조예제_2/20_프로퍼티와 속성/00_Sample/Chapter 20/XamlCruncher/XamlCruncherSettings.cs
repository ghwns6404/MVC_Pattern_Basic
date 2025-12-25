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
        // 사용자 기호에 관한 기본 설정
        public Dock Orientation = Dock.Left;    // Dock위치를 Left로 지정
        public int TabSpaces = 4;               // 탭을 눌렀을 때의 여백을 4칸으로 지정
        public string StartupDocument =         // 프로그램 실행하자마자 텍스트박스에 써져있을 내용
            "<Button xmlns=\"http://schemas.microsoft.com/winfx" + 
                        "/2006/xaml/presentation\"\r\n" +
            "        xmlns:x=\"http://schemas.microsoft.com/winfx" + 
                        "/2006/xaml\">\r\n" +
            "    Hello, XAML!\r\n" +
            "</Button>\n";

        // NotepadCloneSettings 내의 기본 설정을 초기화하는 생성자
        public XamlCruncherSettings()
        {
            FontFamily = "Lucida Console";  // 폰트 설정
            FontSize = 10 / 0.75;           // 폰트 사이즈 설정
        }
    }
}