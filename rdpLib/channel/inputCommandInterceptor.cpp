#include "inputCommandInterceptor.hpp"
void InputCommandInterceptor::dataInput(QVariant data)
{
    Accept(data);


QByteArray dt=data.toByteArray();
  inputCommand cmd;
 inputCommand* sended= (inputCommand*) dt.data();
 cmd.type=sended->type;
 cmd.cmd=sended->cmd;

 qDebug()<<(int)cmd.type;
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
