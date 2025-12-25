//---------------------------------------------------
// NotepadClone.Print.cs (c) 2006 by Charles Petzold
//---------------------------------------------------
using Petzold.PrintWithMargins;     // for PageMarginsDialog.
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Printing;

namespace Petzold.NotepadClone
{
    public partial class NotepadClone : Window
    {
        // Fields for printing.
        PrintQueue prnqueue;                        //전에 사용하였던 print자동선택관련
        PrintTicket prntkt;                         //사용자가 설정하였던 정보관련
        //사용자가 이전에 선택했던 print Dialog의 옵션들을 저장하는 객체
        Thickness marginPage = new Thickness(96);   //여백을 설정


        //NotepadClone.cs 에 호출됨
        void AddPrintMenuItems(MenuItem itemFile)   //프린트 관련 추가 하는 함수
        {
            // Page Setup menu item.
            MenuItem itemSetup = new MenuItem();    //Page Set_up 메뉴에 대한 설정
            itemSetup.Header = "Page Set_up...";    //Page Set_up 메뉴 이름 나타내기
            itemSetup.Click += PageSetupOnClick;    //클릭시 PageSetupOnClick 이벤트 호출
            itemFile.Items.Add(itemSetup);          //메뉴 목록에 추가 해 준다.
            //인자로 받은 itemFile 매뉴아이템의 하위 매뉴가 된다.

            // Print menu item.
            MenuItem itemPrint = new MenuItem();             //Print 메뉴에 대한 설정
            itemPrint.Header = "_Print...";                  //_Print 메뉴 이름 나타내기
            itemPrint.Command = ApplicationCommands.Print;   //메뉴 항목에 연결된 명령 가져오기
            itemFile.Items.Add(itemPrint);                   //메뉴 목록에 추가 해 준다.
            CommandBindings.Add(                             //프린트 관련 공통 다이얼로그를 가져온다.
                new CommandBinding(ApplicationCommands.Print, PrintOnExecuted));
        }
        void PageSetupOnClick(object sender, RoutedEventArgs args)
        {       //PageSetupOnClick 이벤트 호출시 발생
           
            PageMarginsDialog dlg = new PageMarginsDialog();                    
            //PageMarginsDialog 객체생성.

            dlg.Owner = this;                                                   
            //현재 지정되어 있는 메모장의 정보를 담는다.(크기, 파일 이름, 위치 등)

            dlg.PageMargins = marginPage;
            //기존에 가지고 있었던 margin 정보를 열 다이얼로그에 보낸다.                           
            //Left ,Right ,Top, Bottom에 대한 정보를 가져온다.

            if (dlg.ShowDialog().GetValueOrDefault())                           
            {       //ok를 누르면...

                marginPage = dlg.PageMargins;
                //Page Setup에서 설정한 정보를 입력받아 가져온다.
                //Left, Right, Top, Bottom의 여백 값을 전역으로 둔 marginPage에 저장
            }
        }
        void PrintOnExecuted(object sender, ExecutedRoutedEventArgs args)       
            //PrintOnExecuted 이벤트 호출시 발생
        {
            PrintDialog dlg = new PrintDialog();      
            //PrintDialog 객체생성.
           
            if (prnqueue != null)         
                dlg.PrintQueue = prnqueue;
            //만약 전에 저장했던 정보가 없다면..(처음켰다면) 넘어가고 있다면
            //현재 printdlg에 넣어준다.

            if (prntkt != null) 
                dlg.PrintTicket = prntkt;
            //위와 동일...  (이것은 가로나 세로로 프린팅하는것과 같은 옵션)

            if (dlg.ShowDialog().GetValueOrDefault())                           
            {   //ok를 누르면...
                //PrintDialog에서 설정한 정보를 입력받아 가져온다.              
                prnqueue = dlg.PrintQueue; 
                prntkt = dlg.PrintTicket;
                //dlg에서 변경된 각종 옵션을 나중에 다시 쓸 수 있도록
                //각 객체에 저장한다.

                PlainTextDocumentPaginator paginator =                         
                    new PlainTextDocumentPaginator();
                //PlainTextDocumentPaginator 객체 생성.               
                paginator.PrintTicket = prntkt;                                 
                //페이지 설정 정보를 가져온다.
                paginator.Text = txtbox.Text;                                   
                //본문의 정보를 가져온다.
                paginator.Header = strLoadedFile;                               
                //문서에 저장되있는 제목을 가져온다.
                paginator.Typeface =
                    new Typeface(txtbox.FontFamily, txtbox.FontStyle,
                                 txtbox.FontWeight, txtbox.FontStretch);
                //문서 글꼴에 대한 스타일들을 보내준다.  (여기서는 다중스타일 적용 안됨)
                paginator.FaceSize = txtbox.FontSize;                           
                //글꼴 사이즈
                paginator.TextWrapping = txtbox.TextWrapping;                   
                //선택된 Wrapping 정보를 보내준다.
                //사용자가 선택한 모든 text와 관련된 부가적인 정보를 paginator객체에 보내준다.
                paginator.Margins = marginPage;                                 
                //여백 정보를 보내준다.
                paginator.PageSize = new Size(dlg.PrintableAreaWidth,           
                                              dlg.PrintableAreaHeight);
                //페이지 전체 크기를 보내준다.
                dlg.PrintDocument(paginator, Title);
                //paginator 객체를 이용하여 최종 프린트를 한다.
                //각 페이지마다 PlainTextDocumentPaginator.cs에 있는 GetPage에게 리턴받은
                //페이지를 프린팅해주는 것 같다.
            }
        }
    }
}
