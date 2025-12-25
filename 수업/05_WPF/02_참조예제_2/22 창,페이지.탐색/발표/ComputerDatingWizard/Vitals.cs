//---------------------------------------
// Vitals.cs (c) 2006 by Charles Petzold
//---------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;

namespace Petzold.ComputerDatingWizard              
{
    public class Vitals                     //축적된 정보를 저장하는 중요한 장소
    {
        public string Name;             //프로그램 내에서 사용되어질 변수 
        public string Home;
        public string Gender;
        public string FavoriteOS;
        public string Directory;
        public string MomsMaidenName;
        public string Pet;
        public string Income;

        public static RadioButton GetCheckedRadioButton(GroupBox grpbox)    //그룹박스로 둘러싸인 stackpanel 내부에 그룹지어진 RadioButton
        {                                                                   //컨트롤을 직접 상속받음
            Panel pnl = grpbox.Content as Panel;                            //RadioButton을 가져오기 쉽게 정적 메소드로 구현 

            if (pnl != null)
            {
                foreach (UIElement el in pnl.Children)
                {
                    RadioButton radio = el as RadioButton;

                    if (radio != null && (bool)radio.IsChecked)
                        return radio;
                }
            }
            return null;
        }
    }
}
