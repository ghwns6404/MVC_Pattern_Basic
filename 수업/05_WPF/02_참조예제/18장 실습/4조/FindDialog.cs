//-------------------------------------------
// FindDialog.cs (c) 2006 by Charles Petzold
//-------------------------------------------

/*************************************************************************************
 클래스명 : FindDialog
 필      드 : 없음
 설      명 : FindReplaceDialog를 Find대화 상자로 만들기 위해
                 Find에서는 쓰지 않는 Label, TextBox, Replace 버튼, Replace All 버튼을 숨김
**************************************************************************************/

using System;
using System.Windows;
using System.Windows.Controls;

namespace Petzold.NotepadClone
{
    class FindDialog : FindReplaceDialog
    {
        public FindDialog(Window owner): base(owner)
        {
            Title = "Find";

            // 컨트롤 몇 개를 숨김
			lblReplace.Visibility = Visibility.Collapsed;		// FindReplaceDialog에서 선언한 Label
            txtboxReplace.Visibility = Visibility.Collapsed;	// 텍스트 박스
            btnReplace.Visibility = Visibility.Collapsed;		// Replace 버튼
            btnAll.Visibility = Visibility.Collapsed;			// Replace All 버튼을 숨김
        }
    }
}
