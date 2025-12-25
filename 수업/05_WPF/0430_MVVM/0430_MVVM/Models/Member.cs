//Member.cs
using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace _0430_MVVM
{
    internal class Member : INotifyPropertyChanged
    {
        public int Id { get; set; }

        private string name;
        public string Name 
        {
            get => name;
            set { name = value; OnPropertyChanged("Name"); }
        }

        private string phone;
        public string Phone 
        {
            get => phone;
            set { phone = value; OnPropertyChanged("Phone"); }
        }

        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged(string name)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }
    }
}
