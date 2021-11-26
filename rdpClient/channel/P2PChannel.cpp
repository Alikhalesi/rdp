#include "P2PChannel.hpp"
//==============================================================================
void P2PChannel::Connect()
{
        connect(client_, SIGNAL(connected()),this, SLOT(connectedSlot()));
        connect(client_, SIGNAL(disconnected()),this, SLOT(disconnectedSlot()));
        connect(client_, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWrittenSlot(qint64)));
        connect(client_, SIGNAL(readyRead()),this, SLOT(readyReadSlot()));

        qDebug() << "connecting...";

        // this is not blocking call
        client_->connectToHost("google.com", 80);

        // we need to wait...
        if(!client_->waitForConnected(5000))
        {
            qDebug() << "Error: " << client_->errorString();
            emit disconnected();
        }
}
//==============================================================================
void P2PChannel::Disconnect()
{

}
//==============================================================================

void P2PChannel::connectedSlot()
{
    qDebug() << "connected...";

    // Hey server, tell me about you.
    client_->write("HEAD / HTTP/1.0\r\n\r\n\r\n\r\n");
}
//==============================================================================
void P2PChannel::disconnectedSlot()
{
    qDebug() << "disconnected...";
}
//==============================================================================
void P2PChannel::bytesWrittenSlot(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}
//==============================================================================
void P2PChannel::readyReadSlot()
{
    qDebug() << "reading...";

    // read the data from the socket
    qDebug() << client_->readAll();
}
