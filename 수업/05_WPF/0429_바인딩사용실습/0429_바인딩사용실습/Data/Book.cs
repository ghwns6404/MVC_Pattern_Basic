using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Data;
using System.Xml.Linq;

namespace _0429_바인딩사용실습
{
    public class Book : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        
        private void OnPropertyChanged(string title) //데이터가 바뀔때마다 UI한테 통지
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(title));
        }
        //----------------------------
        private string title;
        public string Title
        {
            get { return title; }   //get => title;
            set
            {
                title = value;
                OnPropertyChanged("Title");
            }
        }
        //----------------------------
        private int? price;
        public int? Price
        {
            get { return price; }   
            set
            {
                price = value;
                OnPropertyChanged("Price");
            }
        }
        //----------------------------
        private string publisher;
        public string Publisher
        {
            get { return publisher; }
            set
            {
                publisher = value;
                OnPropertyChanged("Publisher");
            }
        }
        //----------------------------
        private string image;
        public string Image
        {
            get { return image; }
            set
            {
                image = value;
                OnPropertyChanged("Image");
            }
        }
        //----------------------------
        private bool? stock;
        public bool? Stock
        {
            get { return stock; }
            set
            {
                stock = value;
                OnPropertyChanged("Stock");
            }
        }
        //----------------------------
    }



    public class StockConverter : IValueConverter
    {
        //데이터 -> UI
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (targetType != typeof(bool?))
                return null;

            bool? stock = (bool?)value;

            if (stock == null)
            {
                return null;
            }
            else
            {
                return !stock;
            }
        }

        //UI -> 데이터 
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (targetType != typeof(bool?))
                return null;

            bool? stock = (bool?)value;

            if (stock == null)
            {
                return null;
            }
            else
            {
                return !stock;
            }
        }
    }
    public class StockToStringConverter : IValueConverter
    {
        //date -> ui
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            bool? stock = (bool?)value;

            if (stock == null)
                return "";
            else if (stock == true)
                return "재고있음";
            else if (stock == false)
                return "재고없음";
            else
                return "";
        }

        //UI -> DATA
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            string stock = (string)value;

            if (stock == null)
                return "";
            else if (stock == "재고있음")
                return true;
            else if (stock == "재고없음")
                return false;
            else
                return "";
        }
    }

    public class AgeValidationRule : ValidationRule
    {
        public int Min { get; set; }
        public int Max { get; set; }

        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            //타입 불일치 
            int number;
            if (int.TryParse((string)value, out number) == false)
            {
                return new ValidationResult(false, "정수를 입력하세요");
            }

            if (Min <= number && Max >= number)
            {
                return ValidationResult.ValidResult;  //정상 코드
                //return new ValidationResult(true, null); 
            }
            else
            {
                return new ValidationResult(false, $"나이는 {Min}에서 {Max}만 가능");
            }
        }
    }
    public class MaleValidationRule : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            string msg = (string)value;

            if (msg == "재고있음" || msg == "재고없음")
            {
                return ValidationResult.ValidResult;  //정상 코드
            }
            else
            {
                return new ValidationResult(false, "[남성] [여성]만 입력 가능합니다.");
            }
        }
    }
}
