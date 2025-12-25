//IPicture.cs

using System.ServiceModel;


namespace _0507_PictureService
{
    [ServiceContract]
    internal interface IPicture
    {
        [OperationContract]
        byte[] GetPicture(string strFileName);

        [OperationContract]
        string[] GetPictureList();

        [OperationContract]
        bool UploadPicture(string strFileName, byte[] bytePic);
    }
}
