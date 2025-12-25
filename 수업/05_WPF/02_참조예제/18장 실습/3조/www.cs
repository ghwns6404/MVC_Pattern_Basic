using Petzold.ChooseFont;
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Media;
//*****************************************************
//클래스명 : NotepadClone
//필    드 :
//설    명 :
//*****************************************************
namespace Petzold.NotepadClone
{
    public partial class NotepadClone
    {
        void AddFormatMenu(Menu menu)
        {
            //탑 레벨 Format 항목 생성
            MenuItem itemFormat = new MenuItem();
            itemFormat.Header = "F_ormat";
            menu.Items.Add(itemFormat);

            //자동 줄바꿈 메뉴 항목 생성
            WordWrapMenuItem itemWrap = new WordWrapMenuItem();
            itemFormat.Items.Add(itemWrap);

            //TextWrapping 프로퍼티 항목과 텍스트 박스를 바인딩
            
            //TextWrapping 프로퍼티와 WordWramenuItem의 WordWrap프로퍼티를 바인딩하기 위해 Binding 객체 생성.
            Binding bind = new Binding();                                      
            bind.Path = new PropertyPath(TextBox.TextWrappingProperty);
            // 데이터 소스 설정.
            bind.Source = txtbox;                                              
            //바인팅 모드 : 타킷도 소스에 반영할수 있도록 TwoWay 설정.
            bind.Mode = BindingMode.TwoWay;
            //TextBox의 SetBinding 메소드를 호출.
            itemWrap.SetBinding(WordWrapMenuItem.WordWrapProperty, bind);

            //Font 메뉴 항목생성
            MenuItem itemFont = new MenuItem();
            itemFont.Header = "_Font...";
            itemFont.Click +=FontOnClick;
            itemFormat.Items.Add(itemFont);
        }

        void FontOnclick(object sender, RoutedEventArgs args)
        {
            //Fontdialog 생성...
            FontDialog dlg = new FontDialog();
            dlg.Owner = this;

            //폰트 대화상자의 텍스트 박스 프로퍼티를 설정
            dlg.TypeFace = new Typeface(txtbox.FontFamily, txtbox.Fontstyle, txtbox.FontWeight, txtbox.FontStretch);
            dlg.FaceSize = txtbox.Fontsize;

            if (dlg.showdialog().GetValueOrDefault())
            {
                //폰트 대화상자 프로퍼티를 텍스트 박스에 설정
                txtbox.FontFamily = dlg.TypeFace.FontFamily;
                txtbox.FontSize = dlg.FaceSize;
                txtbox.FontStyle = dlg.Typeface.Style;
                txtbox.FontWeight = dlg.Typeface.Weight;
                txtbox.FontStretch = dlg.Typeface.stretch;

            }
        
        }
    
    }

}