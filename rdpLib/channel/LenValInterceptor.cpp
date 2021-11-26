#include "LenValInterceptor.hpp"
//===========================================================================================================
void LenValInterceptor::dataInput(QVariant data)
{
Accept(data);
QByteArray dt=data.toByteArray();
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
            array2.reserve(4);
            array2[0] = dt[0];
            array2[1] = dt[1];
            array2[2] = dt[2];
            array2[3] = dt[3];

            memcpy(&dataSize_, array2, sizeof(unsigned int));

            if(dt.length()<5)
            {
             break;


            }
qDebug()<<"bef : "<<dt.length();

   dt= dt.left(dt.length()-sizeof(unsigned int));
   qDebug()<<"after : "<<dt.length();
    //socket_->read((char*)&dataSize_,sizeof( unsigned int));
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
        readedData_+=dt.left(dataSize_);
        dt=dt.right(dt.length()-dataSize_);
        dataSize_=0;
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

