using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Media;
using System.Windows.Input;
using System.Windows.Controls;

namespace Petzold.CommandTheMenu
{
    class CommandTheMenu:Window
    {
        TextBlock text;
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new CommandTheMenu());
        }
        public CommandTheMenu()
        {
            Title = "Command the Menu";

            //DockPanel   생성
            DockPanel dock = new DockPanel();
            Content = dock;

            //탑 메뉴가 될 Menu 생성
            Menu menu = new Menu();
            dock.Children.Add(menu);
            DockPanel.SetDock(menu, Dock.Top);

            //나머지 영역을 채울 TextBlock을 생성
            text = new TextBlock();
            text.Text = "Sample clipboard text";
            text.HorizontalAlignment = HorizontalAlignment.Center;
            text.VerticalAlignment = VerticalAlignment.Center;
            text.FontSize = 32;
            text.TextWrapping = TextWrapping.Wrap;
            dock.Children.Add(text);

            MenuItem itemEdit = new MenuItem();
            itemEdit.Header = "_Edit";
            menu.Items.Add(itemEdit);

            //Edit 메뉴 항목 생성
            MenuItem itemCut = new MenuItem();
            itemCut.Header = "Cu_t";
            itemCut.Command = ApplicationCommands.Cut;
            itemEdit.Items.Add(itemCut);


            MenuItem itemCopy = new MenuItem();
            itemCopy.Header = "_Copy";
            itemCopy.Command = ApplicationCommands.Copy;
            itemEdit.Items.Add(itemCopy);


            MenuItem itemPaste = new MenuItem();
            itemPaste.Header = "_Paste";
            itemPaste.Command = ApplicationCommands.Paste;
            itemEdit.Items.Add(itemPaste);


            MenuItem itemDelete = new MenuItem();
            itemDelete.Header = "_Delete";
            itemDelete.Command = ApplicationCommands.Delete;
            itemEdit.Items.Add(itemDelete);

            //Window 컬렉션에 바인딩될 커맨드를 추가
            CommandBindings.Add(new CommandBinding(ApplicationCommands.Cut, CutOnExecute, CutCanExecute));
            CommandBindings.Add(new CommandBinding(ApplicationCommands.Copy, CopyOnExecute, CutCanExecute));
            CommandBindings.Add(new CommandBinding(ApplicationCommands.Paste, PasteOnExecute, PasteCanExecute));
            CommandBindings.Add(new CommandBinding(ApplicationCommands.Delete, DeleteOnExecute, CutCanExecute));

            InputGestureCollection CollGestures = new InputGestureCollection();
            CollGestures.Add(new KeyGesture(Key.R, ModifierKeys.Control));

            RoutedUICommand commrestore = new RoutedUICommand("_Restore", "Restore", GetType(), CollGestures);
            
        }
        void CutCanExecute(object sender, CanExecuteRoutedEventArgs args)
        {
            args.CanExecute = text.Text != null && text.Text.Length > 0;
        }
        void PasteCanExecute(object sender, CanExecuteRoutedEventArgs args)
        {
            args.CanExecute = Clipboard.ContainsText();
        }
        void CutOnExecute(object sender, ExecutedRoutedEventArgs args)
        {
            ApplicationCommands.Copy.Execute(null, this);
            ApplicationCommands.Delete.Execute(null, this);
        }
        void CopyOnExecute(object sender, ExecutedRoutedEventArgs args)
        {
            Clipboard.SetText(text.Text);
        }
        void PasteOnExecute(object sender, ExecutedRoutedEventArgs args)
        {
            text.Text = Clipboard.GetText();
        }
        void DeleteOnExecute(object sender, ExecutedRoutedEventArgs args)
        {
            text.Text = null;
        }
    }
}
