#include "IDeamon.hpp"
//=============================================================
void IDeamon::Start()
{
    stopped_.test_and_set();
    while(true)
    {
        QCoreApplication::processEvents();
        if(!stopped_.test_and_set())
        {
            break;
        }
        Run();
    }

}
//=============================================================
void IDeamon::Stop()
{
stopped_.clear();
}
//=============================================================
