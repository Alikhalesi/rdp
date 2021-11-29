#include "inputCommandInterceptor.hpp"
void InputCommandInterceptor::dataInput(QVariant data)
{
    Accept(data);


QByteArray dt=data.toByteArray();

  inputCommand cmd;
  memcpy(&cmd,dt.data(),sizeof (inputCommand));
// inputCommand* sended= (inputCommand*) dt.data();
// cmd.type=sended->type;
// cmd.occurance=sended->occurance;
// cmd.cmd=sended->cmd;

 qDebug()<<(int)cmd.type<<dt.size();
        emit dataOutput(QVariant::fromValue(cmd));
}
//===========================================================================================
void InputCommandInterceptor::Accept(QVariant data)
{
    if(!data.canConvert(QMetaType::QByteArray))
    {
        throw std::runtime_error{"Type Invalid: LenValInterceptor"};
    }
}
