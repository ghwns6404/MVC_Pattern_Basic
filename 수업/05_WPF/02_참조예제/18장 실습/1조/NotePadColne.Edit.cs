using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace NotePadClone
{
    public partial class NotePadClone
    {
        void AddEditMenu(Menu SetMenu)
        {
            //메뉴 최상당
            MenuItem ItemEdit = new MenuItem();
            ItemEdit.Header = "_Edit";
            SetMenu.Items.Add(ItemEdit);
            
            //메뉴 Undo
            MenuItem ItemUndo = new MenuItem();
            ItemUndo.Header = " _Undo";
            ItemUndo.Command = ApplicationCommands.Undo;
            ItemEdit.Items.Add(ItemUndo);
            CommandBindings.Add(new CommandBinding(ApplicationCommands.Undo, UndonExecute,UndoCanExecute));

            //메뉴 Redo
            MenuItem ItemRedo = new MenuItem();
            ItemRedo.Header = "_Redo";
            ItemRedo.Command = ApplicationCommands.Redo;
            ItemEdit.Items.Add(ItemRedo);
            CommandBindings.Add(new CommandBinding(ApplicationCommands.Redo, RedoOnExecute, RedoCanExecute));

            ItemEdit.Items.Add(new Separator());

            //자르기 메뉴;
            MenuItem ItemCut = new MenuItem();
            ItemCut.Header = "Cu_t";
            ItemCut.Command = ApplicationCommands.Cut;
            ItemEdit.Items.Add(ItemCut);
            CommandBindings.Add(new CommandBinding(ApplicationCommands.Cut, CutOnExecute, CutCanExecute));

            //복사 메뉴
            MenuItem ItemCopy = new MenuItem();
            ItemCopy.Header = "_Copy";
            ItemCopy.Command = ApplicationCommands.Copy;
            ItemEdit.Items.Add(ItemCopy);
            CommandBindings.Add(new CommandBinding(ApplicationCommands.Delete, CopyOnExecute, CutCanExecute));

            //삭제 메뉴
            MenuItem ItemDelete = new MenuItem();
            ItemDelete.Header = "De_lete";
            ItemDelete.Command = ApplicationCommands.Delete;
            ItemEdit.Items.Add(ItemDelete);
            CommandBindings.Add(new CommandBinding(ApplicationCommands.Delete, DeleteOnExecute, CutCanExecute));

            //삭제 메뉴
            MenuItem ItemPaste = new MenuItem();
            ItemPaste.Header = "_Paste";
            ItemPaste.Command = ApplicationCommands.Paste;
            ItemEdit.Items.Add(ItemPaste);
            CommandBindings.Add(new CommandBinding(ApplicationCommands.Paste, PasteOnExecute, PasteCanExecute));

            ItemEdit.Items.Add(new Separator());

            AddFindMenuItems(ItemEdit);

            ItemEdit.Items.Add(new Separator());

            //전체 선택 메뉴

            MenuItem ItemAll = new MenuItem();
            ItemAll.Header = "Selete_All";
            ItemAll.Command = ApplicationCommands.SelectAll;
            ItemEdit.Items.Add(ItemAll);
            CommandBindings.Add(new CommandBinding(ApplicationCommands.SelectAll, SelectAllOnExecute));

            InputGestureCollection Coll = new InputGestureCollection();
            Coll.Add(new KeyGesture(Key.F5));
            RoutedUICommand CommTimeDate = new RoutedUICommand("Time/_Date", "TimeDate", GetType(), Coll);

            MenuItem ItemDate = new MenuItem();
            ItemDate.Command = CommTimeDate;
            ItemEdit.Items.Add(ItemDate);
            CommandBindings.Add(new CommandBinding(CommTimeDate, TimeDateOnExecute));


        }
        void UndoCanExecute(object sender, CanExecuteRoutedEventArgs args)
        {
            args.CanExecute = txtbox.CanUndo;
        }
        void UndonExecute(object sender, ExecutedRoutedEventArgs args)
        {
            txtbox.Undo();
        }
        void RedoCanExecute(object sender, CanExecuteRoutedEventArgs args)
        {
            args.CanExecute = txtbox.CanRedo;
        }
        void RedoOnEexcute(object sender, ExecutedRoutedEventArgs args)
        {
            txtbox.Redo();
        }
        void CutCanExecute(object sender, CanExecuteRoutedEventArgs args)
        {
            args.CanExecute = txtbox.SelectedText.Length > 0;
        }
        void CutOnExecute(object sender, ExecutedRoutedEventArgs args)
        {
            txtbox.Cut();
        }
        void CopyOnExecute(object sender, ExecutedRoutedEventArgs args)
        {
            txtbox.Copy();
        }
        void DeleteOnExecute(object sender, ExecutedRoutedEventArgs args)
        {
            txtbox.SelectedText = "";
        }
        void PasteCanExecute(object sender, CanExecuteRoutedEventArgs args)
        {
            args.CanExecute = Clipboard.ContainsText();
        }
        void PasteOnExecute(object sender, ExecutedRoutedEventArgs args)
        {
            txtbox.Paste();
        }
        void SelectAllOnExecute(object sender, ExecutedRoutedEventArgs args)
        {
            txtbox.SelectAll();
        }
        void TimeDateOnExecute(object sender, ExecutedRoutedEventArgs args)
        {
            txtbox.SelectedText = DateTime.Now.ToString();
        }
     }
}
