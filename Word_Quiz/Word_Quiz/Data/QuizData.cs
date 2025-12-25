using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Word_Quiz.Data
{
    public class QuizData
    {
        public string Word { get; set; }
        public string Meaning { get; set; }


        public QuizData()
        {

        }

        public QuizData(string word, string meaning)
        {
            Word = word;
            Meaning = meaning;
        }
    }
}
