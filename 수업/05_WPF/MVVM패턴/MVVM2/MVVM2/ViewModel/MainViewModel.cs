using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Input;

namespace MVVM2
{
    internal class MainViewModel
    {
        public ObservableCollection<TodoItem> Items { get; set; }

        public ICommand AddItemCommand { get; set; }    //Icommand잘봐

        public MainViewModel()
        {
            Items = new ObservableCollection<TodoItem>();
            AddItemCommand = new RelayCommand(AddItem);
        }

        private void AddItem()
        {
            Items.Add(new TodoItem { Title = "New Item" , IsCompleted = false});
        }
    }

    public class RelayCommand : ICommand //3
    {
        private Action _execute;//콜백함수를 저장 중

        public RelayCommand(Action execute)
        {
            _execute = execute;
        }

        public event EventHandler CanExecuteChanged;

        public bool CanExecute(object parameter)
        {//실행할수있는지 없는지 여부
            return true;
        }

        public void Execute(object parameter)
        {//
            _execute();
        }
    }
}
