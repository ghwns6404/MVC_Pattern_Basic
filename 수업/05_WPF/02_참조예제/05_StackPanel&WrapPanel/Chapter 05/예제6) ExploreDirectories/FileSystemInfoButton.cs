//-----------------------------------------------------
// FileSystemInfoButton.cs (c) 2006 by Charles Petzold
//-----------------------------------------------------
using System;
using System.Diagnostics;       // For the Process class
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.ExploreDirectories
{
    public class FileSystemInfoButton : Button
    {
        FileSystemInfo info;

        // Parameterless constructor make "My Documents" button.
        public FileSystemInfoButton()
            :
            this(new DirectoryInfo(
                Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments)))
        {
        }

        // Single-argument constructor makes directory or file button.
        public FileSystemInfoButton(FileSystemInfo info)
        {
            this.info = info;               //첨에는 Document의 경로를 가져와서
            Content = info.Name;            //Content안에 넣어줌
            if (info is DirectoryInfo)
                FontWeight = FontWeights.Bold;
            Margin = new Thickness(10);
        }

        // Two-argument constructor makes "Parent Directory" button.
        public FileSystemInfoButton(FileSystemInfo info, string str)
            :
            this(info)
        {
            Content = str;
        }

        // OnClick override does everything else.
        protected override void OnClick()
        {
            if (info is FileInfo)               //파일 선택시 동작
            {
                Process.Start(info.FullName);
            }
            else if (info is DirectoryInfo)     //디렉토리 선택시, 디렉토리 경로를 메인으로 가져온다.
            {
                DirectoryInfo dir = info as DirectoryInfo;
                Application.Current.MainWindow.Title = dir.FullName;

                Panel pnl = Parent as Panel;    //부모 판넬을 가져온 후, 자식의 정보는 삭제
                pnl.Children.Clear();

                if (dir.Parent != null)
                    pnl.Children.Add(new FileSystemInfoButton(dir.Parent, ".."));           //상위가 널이 아니면 ..출력

                foreach (FileSystemInfo inf in dir.GetFileSystemInfos())                    //foreach로 현재 디렉토리 모든ㄷ 파일 출력
                    pnl.Children.Add(new FileSystemInfoButton(inf));
            }
            base.OnClick();
        }
    }
}