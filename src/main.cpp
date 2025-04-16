#include "app/app.h"

int main(int argc, char *argv[])
{
    return matchmaker::app::App(argc, argv).exec();
}
