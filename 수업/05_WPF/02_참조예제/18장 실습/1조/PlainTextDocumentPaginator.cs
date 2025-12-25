//-----------------------------------------------------------
// PlainTextDocumentPaginator.cs (c) 2006 by Charles Petzold
//-----------------------------------------------------------

/*************************************************************************************
 클래스명 : PlainTextDocumentPaginator 
 필      드 : 
	char[] charsBreak
        string txt					// 인쇄할 문서 데이터
	string txtHeader				// 글꼴 관련된 패밀리(글꼴의 두께, 
	Typeface face				        // 글꼴 관련된 패밀리(글꼴의 두께,							//스타일 및 늘이기 기능을 정의합니.)

	double em	                                // 폰트 사이즈
        Size sizePage					// 인쇄 사이즈
        Size sizeMax					// 최대 사이즈
        Thickness margins			        // 인쇄 여백
        PrintTicket prntkt			        // 인쇄 작업의 설정 클래스
        TextWrapping txtwrap			        // 텍스트 줄 바꿈 방식

        
        List<DocumentPage> listPages;			// DocumentPage 객체로 각 페이지를 저장.

 설      명 : 인쇄에 필요한 요소 설정 및 지원.
**************************************************************************************/
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Printing;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Media;

namespace Petzold.NotepadClone
{
    public class PlainTextDocumentPaginator : DocumentPaginator
    {
        // Private fields, including those associated with public properties.
        char[] charsBreak = new char[] { ' ', '-' };
        string txt = "";                                // 인쇄할 문서 데이터
        string txtHeader = null;
        Typeface face = new Typeface("");               // 글꼴 관련된 패밀리(글꼴의 두께, 								//스타일 및 늘이기 기능을 정의합니.)

        double em = 11;                                 // 폰트 사이즈
        Size sizePage = new Size(8.5 * 96, 11 * 96);    // 인쇄 사이즈
        Size sizeMax = new Size(0, 0);
        Thickness margins = new Thickness(96);          // 인쇄 여백
        PrintTicket prntkt = new PrintTicket();         // 인쇄 작업의 설정 클래스
        TextWrapping txtwrap = TextWrapping.Wrap;       // 텍스트 줄 바꿈 방식

        // DocumentPage 객체로 각 페이지를 저장.
        List<DocumentPage> listPages;

        // Public properties.
        public string Text 
        {
            set { txt = value; }
            get { return txt; }
        }
        public TextWrapping TextWrapping
        {
            set { txtwrap = value; }
            get { return txtwrap; }
        }
        public Thickness Margins
        {
            set { margins = value; }
            get { return margins; }
        }
        public Typeface Typeface
        {
            set { face = value; }
            get { return face; }
        }
        public double FaceSize
        {
            set { em = value; }
            get { return em; }
        }
//      PrintTicket 개체는 PrintTicket 문서라는 특정 형식의 XML 문서를 사용하기 쉽게 표현한 것입니다. 
//      PrintTicket 문서는 양면 인쇄, 한 부씩 인쇄, 스테이플링 등 프린터의 다양한 기능을 설정하는 방법을 프린터에 지시하는 명령 집합입니다     
        public PrintTicket PrintTicket
        {
            set { prntkt = value; }
            get { return prntkt; }
        }
        public string Header
        {
            set { txtHeader = value; }
            get { return txtHeader; }
        }

        // Required overrides.
        //파생 클래스에서 재정의되는 경우 
        // PageCount가 총 페이지 수인지 여부를 나타내는 값을 가져옵니다
        public override bool IsPageCountValid
        {
            get 
            {
                if (listPages == null)
                    Format();

                return true; 
            }
        }
        //파생 클래스에서 재정의되는 경우 현재 서식이 지정된 페이지 수를 가져옵니다.
        public override int PageCount
        {
            get 
            {
                if (listPages == null)
                    return 0;

                return listPages.Count; 
            }
        }
        public override Size PageSize
        {
            set { sizePage = value; }
            get { return sizePage; }
        }

        //페이지 지정자에 의해 생성된 문서 페이지를 나타냅니다
        public override DocumentPage GetPage(int numPage)
        {
            return listPages[numPage];
        }

        //파생 클래스에서 재정의되는 경우 콘텐츠 페이지 매기기를 수행하는 개체를 가져옵니다.
        public override IDocumentPaginatorSource Source
        {
            get { return null; }
        }

        // 인쇄할 때 텍스트 한 줄이 끝났음을 알려주는 
	// 화살표를 표시하는 내부 클래스
        class PrintLine
        {
            public string String;
            public bool Flag;

            public PrintLine(string str, bool flag)
            {
                String = str;
                Flag = flag;
            }
        }

        // 전체 문서를 페이지로 포맷팅.
        void Format()
        {
            // LinewWithFlag 객체로 문서의 각 줄을 저장
            List<PrintLine> listLines = new List<PrintLine>();

            // 몇 가지 기본적인 계산에 이를 이용
            FormattedText formtxtSample = GetFormattedText("W");

            // 인쇄되는 줄의 폭
            double width = PageSize.Width - Margins.Left - Margins.Right;

            // 심각한 위험 : 작업중지 -> 인쇄문서가 실제사이즈보다 클때
            if (width < formtxtSample.Width)
                return;

            string strLine;
            Pen pn = new Pen(Brushes.Black, 2);
            StringReader reader = new StringReader(txt);

            // listLines에 각 줄을 저장하기 위해 ProcessLine을 호출.
            while (null != (strLine = reader.ReadLine()))
                ProcessLine(strLine, width, listLines);

            reader.Close();

            // 페이지를 인쇄할 준비 시작.
            double heightLine = formtxtSample.LineHeight + formtxtSample.Height;
            double height = PageSize.Height - Margins.Top - Margins.Bottom;
            int linesPerPage = (int)(height / heightLine);

            // Serious problem: Abandon ship.   // 인쇄할 페이지 라인이 없을때
            if (linesPerPage < 1)
                return;

            int numPages = (listLines.Count + linesPerPage - 1) / linesPerPage;// 인쇄할 페이지 수
            double xStart = Margins.Left;
            double yStart = Margins.Top;

            // DocumentPage.
            listPages = new List<DocumentPage>();
            
            for (int iPage = 0, iLine = 0; iPage < numPages; iPage++)
            {
                // DrawingVisual 객체를 생성해 Drawing Context를 오픈.
                // DrawingVisual class
                // DrawingVisual은 화면에서 벡터 그래픽을 렌더링하는 데 
                //  사용할 수 있는 시각적 개체입니다. 내용은 시스템에서 유지됩니다. 

                DrawingVisual vis = new DrawingVisual();

                //DrawingContext class
                //그리기, 푸시 및 팝 명령을 사용하여 표시 내용을 설명합니다
                DrawingContext dc = vis.RenderOpen();

                // 페이지 상단에 헤더를 출력.
                if (Header != null && Header.Length > 0)
                {
                    FormattedText formtxt = GetFormattedText(Header);
                    formtxt.SetFontWeight(FontWeights.Bold);
                    Point ptText = new Point(xStart, yStart - 2 * formtxt.Height);
                    dc.DrawText(formtxt, ptText);
                } 

                // 페이지 하단에 꼬리말 출력.
                if (numPages > 1)
                {
                    FormattedText formtxt = 
                        GetFormattedText("Page " + (iPage+1) + " of " + numPages);
                    formtxt.SetFontWeight(FontWeights.Bold);
                    Point ptText = new Point(
                        (PageSize.Width + Margins.Left - 
                                            Margins.Right - formtxt.Width) / 2,
                        PageSize.Height - Margins.Bottom + formtxt.Height);
                    dc.DrawText(formtxt, ptText);
                }

                // 페이지상의 각 줄에 대해 처리.
                for (int i = 0; i < linesPerPage; i++, iLine++)
                {
                    if (iLine == listLines.Count)
                        break;

                    // 줄의 텍스트를 보여주기 위해 정보를 설정.
                    string str = listLines[iLine].String;
                    FormattedText formtxt = GetFormattedText(str);
                    Point ptText = new Point(xStart, yStart + i * heightLine); 
                    dc.DrawText(formtxt, ptText);

                    // 작은 화살표 플래그 출력.
                    if (listLines[iLine].Flag)
                    {
                        double x = xStart + width + 6;
                        double y = yStart + i * heightLine + formtxt.Baseline;
                        double len = face.CapsHeight * em;
                        dc.DrawLine(pn, new Point(x, y), 
                                        new Point(x + len, y - len));
                        dc.DrawLine(pn, new Point(x, y), 
                                        new Point(x, y - len / 2));
                        dc.DrawLine(pn, new Point(x, y), 
                                        new Point(x + len / 2, y));
                    }
                }
                dc.Close();

                // Visual 기반의 Document Page 객체를 생성.
                //페이지 지정자에 의해 생성된 문서 페이지를 나타냅니다. 
                DocumentPage page = new DocumentPage(vis);
                listPages.Add(page);
            }
            reader.Close();
        }

        // 텍스트의 각 줄에 대해 여러 줄로 이루어진 텍스트를 처리
        void ProcessLine(string str, double width, List<PrintLine> list)
        {
            str = str.TrimEnd(' ');

            // TextWrapping == TextWrapping.NoWrap.
            // ------------------------------------
            if (TextWrapping == TextWrapping.NoWrap)
            {
                do
                {
                    int length = str.Length;

                    while (GetFormattedText(str.Substring(0, length)).Width > 
                                                                        width)
                        length--;

                    list.Add(new PrintLine(str.Substring(0, length), 
                                                    length < str.Length));
                    // 부문 문자열을 검색합니다.
                    str = str.Substring(length);
                }
                while (str.Length > 0);
            }
            // TextWrapping == TextWrapping.Wrap or TextWrapping.WrapWithOverflow.
            // -------------------------------------------------------------------
            else 
            {
                do
                {
                    int length = str.Length;
                    bool flag = false;

                    while (GetFormattedText(str.Substring(0, length)).Width > width)
                    {
                        int index = str.LastIndexOfAny(charsBreak, length - 2);

                        if (index != -1)
                            length = index + 1; // Include trailing space or dash.
                        else
                        {
                            // At this point, we know that the next possible 
                            //  space or dash break is beyond the allowable 
                            //  width. Check if there's *any* space or dash break.
                            index = str.IndexOfAny(charsBreak);

                            if (index != -1)
                                length = index + 1;

                            // If TextWrapping.WrapWithOverflow, just display the
                            //  line. If TextWrapping.Wrap, break it with a flag.
                            if (TextWrapping == TextWrapping.Wrap)
                            {
                                while (GetFormattedText(str.Substring(0, length)).
                                                                    Width > width)
                                    length--;

                                flag = true;
                            }
                            break;  // out of while loop.
                        }
                    }

                    list.Add(new PrintLine(str.Substring(0, length), flag));
                    str = str.Substring(length);
                }
                while (str.Length > 0);
            }
        }
        // FormattedText 객체를 생성
        FormattedText GetFormattedText(string str)
        {
            // FormattedText class
            //Windows Presentation Foundation (WPF)
            //응용 프로그램에서 텍스트를 그리기 위한 하위 수준 컨트롤을 제공합니다
            return new FormattedText(str, CultureInfo.CurrentCulture,
                            FlowDirection.LeftToRight, face, em, Brushes.Black);
            // 파라미터
            // 1. 지정된 텍스트
            // 2. CultureInfo ==>
            //              culture 이름, 쓰기 시스템, 사용된 달력, 날짜 형식 지정 및 문자열 정렬 방법 등 특정 culture에 대한 정보를 제공합니다.
            // 3. 서체 ==> 텍스트 및 UI 요소의 콘텐츠 흐름 방향을 지정하는 상수
            // 4. 글꼴
            // 5. 글꼴 크기
            // 6. 브러시 색상
        }
    }
}
