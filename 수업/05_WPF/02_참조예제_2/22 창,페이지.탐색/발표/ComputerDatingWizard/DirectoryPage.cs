//----------------------------------------------
// DirectoryPage.cs (c) 2006 by Charles Petzold
//----------------------------------------------
using Petzold.RecurseDirectoriesIncrementally;
using System;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;

namespace Petzold.ComputerDatingWizard
{
    public partial class DirectoryPage : PageFunction<DirectoryInfo>
    {
        // 생성자.
        public DirectoryPage()
        {
            InitializeComponent();
            treevue.SelectedItemChanged += TreeViewOnSelectedItemChanged;
        }
        // ok버튼을 활성화시키는 이벤트 핸들러.
        void TreeViewOnSelectedItemChanged(object sender,       
                            RoutedPropertyChangedEventArgs<object> args)
        {
            btnOk.IsEnabled = args.NewValue != null;        //버튼이 선택되었을때만 활성화
        }
        //  Cancel 과 OK 버튼의 이벤트핸들러
        void CancelButtonOnClick(object sender, RoutedEventArgs args)
        {
            OnReturn(new ReturnEventArgs<DirectoryInfo>());         
        }
        void OkButtonOnClick(object sender, RoutedEventArgs args)
        {
            DirectoryInfo dirinfo =                 
                (treevue.SelectedItem as DirectoryTreeViewItem).DirectoryInfo;      //해당 디렉토리로 ㄱㄱ

            OnReturn(new ReturnEventArgs<DirectoryInfo>(dirinfo));      //dirinfo인자를 넘김
        }
    }
}
