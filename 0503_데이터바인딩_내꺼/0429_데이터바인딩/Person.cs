using System;
using System.ComponentModel;
using System.Globalization;
using System.Windows.Controls;
using System.Windows.Data;

namespace _0429_데이터바인딩
{
    internal class Person : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged = null;

        private string name;
        public string Name
        {
            get { return name; }
            set
            {
                name = value;
                if (PropertyChanged != null)
                    PropertyChanged(this, new PropertyChangedEventArgs("Name"));
            }
        }

        private string phone;
        public string Phone
        {
            get { return phone; }
            set
            {
                phone = value;
                if (PropertyChanged != null)//까먹으면 금붕어새끼 !!! from.찬호
                    PropertyChanged(this, new PropertyChangedEventArgs("Phone"));
            }
        }

        public int? age;
        public int? Age
        {
            get { return age; }
            set
            {
                age = value;
                if (PropertyChanged != null)
                    PropertyChanged(this, new PropertyChangedEventArgs("Age"));
            }
        }

        private bool? male;
        public bool? Male
        {
            get { return male; }
            set
            {
                male = value;
                if (PropertyChanged != null)
                    PropertyChanged(this, new PropertyChangedEventArgs("Male"));
            }
        }

        public override string ToString()
        {
            return Name + ", " + Phone + ", " + age + ", " + male;
        }
    }

    public class MaleToFemaleConverter : IValueConverter
    {
        //데이터 -> UI
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (targetType != typeof(bool?))
                return null;

            bool? male = (bool?)value; 

            if (male == null)
            {
                return null;
            }
            else
            {
                return !male; //리턴될때 true 하지만 ! 로 반전을 시켜서 false가 넘어가게됨 
            }
        }

        //UI -> 데이터 
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (targetType != typeof(bool?))
                return null;

            bool? male = (bool?)value;

            if (male == null)
            {
                return null;
            }
            else
            {
                return !male; //리턴될때 true 하지만 ! 로 반전을 시켜서 false가 넘어가게됨
            }
        }
    }
    public class MaleToStringConverter : IValueConverter
    {
        //date -> ui
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            bool? male = (bool?)value;

            if (male == null)
                return "";
            else if (male == true)
                return "남성";
            else if (male == false)
                return "여성";
            else
                return "";
        }

        //UI -> DATA
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            string male = (string)value;

            if (male == null)
                return "";
            else if (male == "남성")
                return true;
            else if (male == "여성")
                return false;
            else
                return "";
        }
    }

    public class AgeValidationRule : ValidationRule
    {
        public int Min {  get; set; }
        public int Max { get; set;  }

        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            //타입 불일치 
            int number;
            if(int.TryParse((string)value, out number) == false)
            {
                return new ValidationResult(false, "정수를 입력하세요");
            }
            
            if(Min <= number && Max >= number)
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

            if(msg == "남성" || msg == "여성")
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
