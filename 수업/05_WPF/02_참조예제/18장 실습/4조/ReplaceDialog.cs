//----------------------------------------------
// ReplaceDialog.cs (c) 2006 by Charles Petzold
//----------------------------------------------

/*************************************************************************************
 클래스명 : ReplaceDialog
 필      드 : 없음
 설      명 : FindReplaceDialog를 Replace대화 상자로 만들기 위해
                 Replace에서는 쓰지 않는 라디오 버튼이 2개 들어간
                 그룹박스를 숨김.
**************************************************************************************/

using System;
using System.Windows;
using System.Windows.Controls;

namespace Petzold.NotepadClone
{
    class ReplaceDialog : FindReplaceDialog
    {
        public ReplaceDialog(Window owner): base(owner)
        {
            Title = "Replace";

            // 그룹 박스를 숨김
			groupDirection.Visibility = Visibility.Hidden;	// FindReplaceDialog에서 선언한 그룹 박스를 숨김
        }
    }
}
