//--------------------------------------------
// WizardPage3.cs (c) 2006 by Charles Petzold
//--------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;

namespace Petzold.ComputerDatingWizard
{
    public partial class WizardPage3: Page
    {
        Vitals vitals;

        // 생성자.
        public WizardPage3(Vitals vitals)
        {
            InitializeComponent();
            this.vitals = vitals;
        }
        // Previous 와 Finish 버튼 이벤트 핸들러.
        void PreviousButtonOnClick(object sender, RoutedEventArgs args)
        {
            NavigationService.GoBack();
        }
        void FinishButtonOnClick(object sender, RoutedEventArgs args)
        {
            // 이 페이지의 정보 저장.
            vitals.MomsMaidenName = txtboxMom.Text;
            vitals.Pet = 
                Vitals.GetCheckedRadioButton(grpboxPet).Content as string;
            vitals.Income = 
                Vitals.GetCheckedRadioButton(grpboxIncome).Content as string;

            // 항상 마지막 페이지는 다시생성.
            WizardPage4 page = new WizardPage4(vitals);
            NavigationService.Navigate(page);
        }
    }
}
