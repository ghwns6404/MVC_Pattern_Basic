using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.ControlXCV
{
    class ControlXCV : Petzold.ControlXCV1.CutCopyPaste
    {
        KeyGesture gestCut = new KeyGesture(Key.X, ModifierKeys.Control);       // 키 설정. 이벤트
        KeyGesture gestCopy = new KeyGesture(Key.C, ModifierKeys.Control);
        KeyGesture gestPaste = new KeyGesture(Key.V, ModifierKeys.Control);
        KeyGesture gestDelete = new KeyGesture(Key.Delete);
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new ControlXCV());
        }
        public ControlXCV()
        {
            Title = "Control X,C AND V";
            itemCut.InputGestureText = "Ctrl+X";
            itemCopy.InputGestureText = "Ctrl+C";
            itemPaste.InputGestureText = "Ctrl+V";
            itemDelete.InputGestureText = "Delete";
        }
        protected override void OnPreviewKeyDown(KeyEventArgs e)                // 키다운 이벤트 오버라이드.
        {
            base.OnPreviewKeyDown(e);
            e.Handled = true;                   // 이벤트 인수를 Ture 를 설정하는것으로 시작 -> 처음 검사를 수행하기 위해서. 
            if (gestCut.Matches(null, e))
                CutOnClick(this, e);

            else if (gestCopy.Matches(null, e))
                CopyOnClick(this, e);

            else if (gestPaste.Matches(null, e))
                PasteOnClick(this, e);

            else if (gestDelete.Matches(null, e))
                DeleteOnClick(this, e);

            else
                e.Handled = false;              // 아니라면 false 지정.
               
        }
    }
}
