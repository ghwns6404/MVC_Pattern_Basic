////IPicture.cs

namespace _0507_PictureService
{
    internal class CPicture : IPicture
    {
        private  MyDB mydb = MyDB.Singleton;
        public byte[] GetPicture(string name)
        {
            return mydb.Get_Picture(name);
        }

        public string[] GetPictureList()
        {
            return mydb.Select_Picture_List();
        }

        public bool UploadPicture(string strFileName, byte[] bytePic)
        {
            return mydb.Upload_Picture(strFileName, bytePic);
        }
    }
}
