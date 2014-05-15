
// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include <QApplication>
#include <iostream>
#include <boost/filesystem.hpp>


#include "src/findmf/qtwidgets/TwoPanelImageWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ralab::TwoPaneledImageWidget tpi;

    //ralab::MultiArrayVisLog<float> adapt2( mp2.getImageMap().getMap() );
    //ralab::MultiArrayVisAsinh<float> adapt1( mp2.getImageMap().getMap() );
    //ralab::MultiArrayVisLog<float> adapt0( mp.getImageMap().getMap() );

    ralab::MapVisMock mvm(1000,1000);
    tpi.setTopMap(&mvm);
    //tpi.setBottomMap(&adapt0);
    tpi.setBottomMap(&mvm);
    tpi.show();
    int res;
    res = app.exec();
    return res;
}
