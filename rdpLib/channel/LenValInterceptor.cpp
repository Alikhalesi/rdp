#include "LenValInterceptor.hpp"
//===========================================================================================================
void LenValInterceptor::dataInput(QVariant data)
{
    int endSentinel = 1211364875;
Accept(data);

QByteArray dt=data.toByteArray();
//qDebug() << dt.toHex();
    qDebug()<<"all msg data: "<<dt.length();
if(dt.length()<4)
{
    qDebug()<<"daddddddddddddddddddddddddddddd";

}
 qDebug()<<"dtsize "<<dataSize_;
while(true)
{
    qDebug()<<"dtsize "<<dataSize_;
    if(dataSize_==0)
    {
    readedData_.clear();

    QByteArray array2;
   // qDebug() << dt.toHex();
            array2.resize(4);
            array2[0] = dt.at(0);
            array2[1] = dt.at(1);
            array2[2] = dt.at(2);
            array2[3] = dt.at(3);
            
            memcpy(&dataSize_, array2, sizeof(unsigned int));
         if(dt.length() < 5)
         {
             return;
         }
            
            
qDebug()<<"bef : "<<dt.length();

   dt= dt.right(dt.length()-sizeof(unsigned int));
   qDebug()<<"after : "<<dt.length();

    qDebug()<<"msg len: "<<dataSize_;
    }

    if(dt.isEmpty())
    {
        qDebug()<<"emptyyyyyyyyyyyyyy";
        break;
    }
     qDebug()<<"dt lenght: "<<dt.length();
    if(dt.length()<=dataSize_)
    {
        readedData_+=dt;
        dataSize_-=dt.length();
     
        break;
    }
    else{
      //  qDebug() << dt.toHex();
        readedData_+=dt.left(dataSize_);
        dt=dt.right(dt.length()-dataSize_);
        dataSize_=0;
      //  qDebug() << dt.toHex();
        emit  dataOutput(readedData_);
        continue;
    }


}

        if(dataSize_==0)
        {

        emit dataOutput(readedData_);
        }
}
//==============================================================================================================
void LenValInterceptor::Accept(QVariant data)
{
    if(!data.canConvert(QMetaType::QByteArray))
    {
        throw std::runtime_error{"Type Invalid: LenValInterceptor"};
    }
}

