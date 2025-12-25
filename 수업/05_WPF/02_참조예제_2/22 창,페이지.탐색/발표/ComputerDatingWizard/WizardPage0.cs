//--------------------------------------------
// WizardPage0.cs (c) 2006 by Charles Petzold
//--------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;

namespace Petzold.ComputerDatingWizard
{
    public partial class WizardPage0
    {
        public WizardPage0()
        {
            InitializeComponent();
        }
        void BeginButtonOnClick(object sender, RoutedEventArgs args)//다음페이지를 실행(페이지에서 입력했던 정보가 저장됨)
        {
            if (NavigationService.CanGoForward)     
                NavigationService.GoForward();
            else
            {
                Vitals vitals = new Vitals();
                WizardPage1 page = new WizardPage1(vitals);      //페이지 다시생성
                NavigationService.Navigate(page);
            }
        }
    }
}
