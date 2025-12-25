//--------------------------------------------
// WizardPage2.cs (c) 2006 by Charles Petzold
//--------------------------------------------
using System;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;

namespace Petzold.ComputerDatingWizard
{
    public partial class WizardPage2
    {
        Vitals vitals;

        // 생성자.
        public WizardPage2(Vitals vitals)
        {
            InitializeComponent();
            this.vitals = vitals;
        }

        // Browse 버튼 이벤트 핸들러.
        void BrowseButtonOnClick(object sender, RoutedEventArgs args)
        {
            DirectoryPage page = new DirectoryPage();
            page.Return += DirPageOnReturn;
            NavigationService.Navigate(page);
        }
        void DirPageOnReturn(object sender, ReturnEventArgs<DirectoryInfo> args)
        {
            if (args.Result != null)
                txtboxFavoriteDir.Text = args.Result.FullName;
        }
        // 하단 버튼 이벤트 핸들러.
        void PreviousButtonOnClick(object sender, RoutedEventArgs args)
        {
            NavigationService.GoBack();
        }
        void NextButtonOnClick(object sender, RoutedEventArgs args)
        {
            vitals.FavoriteOS = txtboxFavoriteOS.Text;      //마찬가지로 입력정보를 생성자로 전달
            vitals.Directory = txtboxFavoriteDir.Text;

            if (NavigationService.CanGoForward)
                NavigationService.GoForward();
            else
            {
                WizardPage3 page = new WizardPage3(vitals);
                NavigationService.Navigate(page);
            }
        }
    }
}
